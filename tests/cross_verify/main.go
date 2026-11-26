package main

import (
	"bufio"
	"bytes"
	"encoding/hex"
	"encoding/json"
	"flag"
	"fmt"
	"math/rand"
	"os"
	"os/exec"
	"path/filepath"
	"reflect"
	"sort"
	"strings"

	"github.com/go-gl/mathgl/mgl32"
	"github.com/google/uuid"
	"github.com/sandertv/gophertunnel/minecraft/protocol"
	"github.com/sandertv/gophertunnel/minecraft/protocol/packet"
)

type PacketResult struct {
	ID     uint32 `json:"id"`
	Name   string `json:"name"`
	Status string `json:"status"`
	InHex  string `json:"in_hex,omitempty"`
	OutHex string `json:"out_hex,omitempty"`
	Error  string `json:"error,omitempty"`
}

func fuzzValue(v reflect.Value, rng *rand.Rand, depth int) {
	if !v.CanSet() || depth > 3 {
		return
	}
	t := v.Type()
	switch t {
	case reflect.TypeOf(uuid.UUID{}):
		var u uuid.UUID
		rng.Read(u[:])
		v.Set(reflect.ValueOf(u))
		return
	case reflect.TypeOf(mgl32.Vec2{}):
		v.Set(reflect.ValueOf(mgl32.Vec2{float32(rng.Intn(100)), float32(rng.Intn(100))}))
		return
	case reflect.TypeOf(mgl32.Vec3{}):
		v.Set(reflect.ValueOf(mgl32.Vec3{float32(rng.Intn(100)), float32(rng.Intn(100)), float32(rng.Intn(100))}))
		return
	case reflect.TypeOf(protocol.BlockPos{}):
		v.Set(reflect.ValueOf(protocol.BlockPos{int32(rng.Intn(50)), int32(rng.Intn(50)), int32(rng.Intn(50))}))
		return
	}

	switch v.Kind() {
	case reflect.Bool:
		v.SetBool(rng.Intn(2) == 1)
	case reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64:
		v.SetInt(int64(rng.Intn(2)))
	case reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64:
		v.SetUint(uint64(rng.Intn(2)))
	case reflect.Float32, reflect.Float64:
		v.SetFloat(float64(rng.Intn(50)) + 0.5)
	case reflect.String:
		v.SetString(fmt.Sprintf("s%d", rng.Intn(100)))
	case reflect.Array:
		for i := 0; i < v.Len(); i++ {
			fuzzValue(v.Index(i), rng, depth+1)
		}
	case reflect.Slice:
		if depth < 2 {
			n := 1
			sl := reflect.MakeSlice(t, n, n)
			for i := 0; i < n; i++ {
				fuzzValue(sl.Index(i), rng, depth+1)
			}
			v.Set(sl)
		}
	case reflect.Map:
		if t == reflect.TypeOf(map[string]any{}) {
			v.Set(reflect.ValueOf(map[string]any{"k": int32(rng.Intn(10))}))
		} else if t == reflect.TypeOf(map[uint32]any{}) {
			v.Set(reflect.ValueOf(map[uint32]any{0: int64(1), 4: "kodiak"}))
		}
	case reflect.Struct:
		for i := 0; i < v.NumField(); i++ {
			fuzzValue(v.Field(i), rng, depth+1)
		}
	}
}

func populatePacket(pk packet.Packet, rng *rand.Rand) {
	fuzzValue(reflect.ValueOf(pk).Elem(), rng, 0)
	switch p := pk.(type) {
	case *packet.Text:
		p.TextType = packet.TextTypeRaw
		p.Message = "hello"
	case *packet.InventoryTransaction:
		p.TransactionData = &protocol.NormalTransactionData{}
	case *packet.RequestAbility:
		p.Ability = 18
		p.Value = false
	case *packet.ClientMovementPredictionSync:
		p.ActorFlags = protocol.NewBitset(131)
	case *packet.Interact:
		p.ActionType = packet.InteractActionLeaveVehicle
	case *packet.MobEffect:
		p.Operation = packet.MobEffectAdd
	case *packet.Animate:
		p.ActionType = packet.AnimateActionSwingArm
	case *packet.SimpleEvent:
		p.EventType = packet.SimpleEventCommandsEnabled
	case *packet.CreativeContent:
		p.Groups = nil
		p.Items = nil
	case *packet.CommandRequest:
		p.CommandOrigin = protocol.CommandOrigin{Origin: protocol.CommandOriginPlayer}
	case *packet.CommandOutput:
		p.CommandOrigin = protocol.CommandOrigin{Origin: protocol.CommandOriginPlayer}
		p.OutputType = 3
		p.OutputMessages = nil
	case *packet.StructureBlockUpdate:
		p.Settings.Integrity = 100.0
	case *packet.PhotoTransfer:
		p.PhotoName = ".jpeg"
	case *packet.SetDisplayObjective:
		p.SortOrder = 0
		p.DisplaySlot = "list"
	case *packet.AgentAction:
		p.Action = 1
	case *packet.UpdateTrade:
		p.SerialisedOffers = []byte{0x0a, 0x00, 0x00}
	case *packet.UpdateEquip:
		p.SerialisedInventoryData = []byte{0x0a, 0x00, 0x00}
	}
}

func safeMarshal(pk packet.Packet) (data []byte, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("%v", r)
		}
	}()
	var buf bytes.Buffer
	w := protocol.NewWriter(&buf, 0)
	pk.Marshal(w)
	return buf.Bytes(), nil
}

func safeUnmarshal(pk packet.Packet, data []byte) (err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("%v", r)
		}
	}()
	r := protocol.NewReader(bytes.NewReader(data), 0, false)
	pk.Marshal(r)
	return nil
}

func main() {
	seed := flag.Int64("seed", 42, "RNG seed")
	iterations := flag.Int("iterations", 1, "Fuzz iterations per packet")
	flag.Parse()

	cppExe := filepath.Join("..", "..", "build", "Release", "bedrock-protocol-cross-verify.exe")
	if flag.NArg() > 0 {
		cppExe = flag.Arg(0)
	}

	cmd := exec.Command(cppExe)
	stdin, err := cmd.StdinPipe()
	if err != nil {
		panic(err)
	}
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		panic(err)
	}
	if err := cmd.Start(); err != nil {
		panic(err)
	}

	pool := packet.NewClientPool()
	for id, fn := range packet.NewServerPool() {
		if _, exists := pool[id]; !exists {
			pool[id] = fn
		}
	}

	ids := make([]uint32, 0, len(pool))
	for id := range pool {
		ids = append(ids, id)
	}
	sort.Slice(ids, func(i, j int) bool { return ids[i] < ids[j] })

	scanner := bufio.NewScanner(stdout)
	scanner.Buffer(make([]byte, 1024*1024), 1024*1024)

	rng := rand.New(rand.NewSource(*seed))
	results := make([]PacketResult, 0, len(ids)*(*iterations))

	for _, id := range ids {
		for it := 0; it < *iterations; it++ {
			pk := pool[id]()
			populatePacket(pk, rng)
			name := reflect.TypeOf(pk).Elem().Name()
			raw, err := safeMarshal(pk)
			if err != nil {
				results = append(results, PacketResult{ID: id, Name: name, Status: "go_panic", Error: err.Error()})
				continue
			}
			inHex := hex.EncodeToString(raw)
			fmt.Fprintf(stdin, "%d %s\n", id, inHex)
			if !scanner.Scan() {
				results = append(results, PacketResult{ID: id, Name: name, Status: "error", Error: "eof from cpp"})
				break
			}
			resp := scanner.Text()
			if strings.HasPrefix(resp, "OK ") {
				outHex := strings.TrimPrefix(resp, "OK ")
				outBytes, _ := hex.DecodeString(outHex)
				pk2 := pool[id]()
				if err := safeUnmarshal(pk2, outBytes); err != nil {
					results = append(results, PacketResult{ID: id, Name: name, Status: "go_decode_fail", InHex: inHex, OutHex: outHex, Error: err.Error()})
					continue
				}
				if outHex == inHex {
					results = append(results, PacketResult{ID: id, Name: name, Status: "ok"})
				} else {
					results = append(results, PacketResult{ID: id, Name: name, Status: "mismatch", InHex: inHex, OutHex: outHex})
				}
			} else if resp == "OVERFLOW" {
				results = append(results, PacketResult{ID: id, Name: name, Status: "overflow", InHex: inHex})
			} else if strings.HasPrefix(resp, "EXCEPTION ") {
				results = append(results, PacketResult{ID: id, Name: name, Status: "exception", Error: strings.TrimPrefix(resp, "EXCEPTION ")})
			} else if strings.HasPrefix(resp, "UNSUPPORTED ") {
				results = append(results, PacketResult{ID: id, Name: name, Status: "unsupported"})
			} else {
				results = append(results, PacketResult{ID: id, Name: name, Status: "error", Error: resp})
			}
		}
	}

	stdin.Close()
	cmd.Wait()

	enc := json.NewEncoder(os.Stdout)
	enc.SetIndent("", "  ")
	_ = enc.Encode(results)
}
