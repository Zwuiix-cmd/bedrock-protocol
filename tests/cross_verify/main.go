package main

import (
	"bufio"
	"bytes"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"reflect"
	"sort"
	"strings"

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

func populatePacket(pk packet.Packet) {
	switch p := pk.(type) {
	case *packet.Text:
		p.TextType = packet.TextTypeRaw
		p.Message = "hello"
	case *packet.InventoryTransaction:
		p.TransactionData = &protocol.NormalTransactionData{}
	case *packet.RequestAbility:
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
	case *packet.CommandOutput:
		p.OutputType = 3
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
	cppExe := filepath.Join("..", "..", "build", "Release", "bedrock-protocol-cross-verify.exe")
	if len(os.Args) > 1 {
		cppExe = os.Args[1]
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

	results := make([]PacketResult, 0, len(ids))

	for _, id := range ids {
		pk := pool[id]()
		populatePacket(pk)
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

	stdin.Close()
	cmd.Wait()

	enc := json.NewEncoder(os.Stdout)
	enc.SetIndent("", "  ")
	_ = enc.Encode(results)
}
