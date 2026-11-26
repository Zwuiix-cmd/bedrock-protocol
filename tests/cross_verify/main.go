package main

import (
	"bufio"
	"bytes"
	"encoding/hex"
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

func safeMarshal(pk packet.Packet) (data []byte, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("panic: %v", r)
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
			err = fmt.Errorf("panic: %v", r)
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

	var okCount, mismatchCount, overflowCount, exceptionCount, unsupportedCount, goPanicCount int

	for _, id := range ids {
		pk := pool[id]()
		name := reflect.TypeOf(pk).Elem().Name()
		raw, err := safeMarshal(pk)
		if err != nil {
			goPanicCount++
			continue
		}
		fmt.Fprintf(stdin, "%d %s\n", id, hex.EncodeToString(raw))
		if !scanner.Scan() {
			break
		}
		resp := scanner.Text()
		if strings.HasPrefix(resp, "OK ") {
			outHex := strings.TrimPrefix(resp, "OK ")
			outBytes, _ := hex.DecodeString(outHex)
			// Verify Go can decode what C++ encoded!
			pk2 := pool[id]()
			if err := safeUnmarshal(pk2, outBytes); err != nil {
				mismatchCount++
				fmt.Printf("[GO_DECODE_FAIL] %d %s: %v\n", id, name, err)
				continue
			}
			if outHex == hex.EncodeToString(raw) {
				okCount++
			} else {
				mismatchCount++
			}
		} else if resp == "OVERFLOW" {
			overflowCount++
		} else if strings.HasPrefix(resp, "EXCEPTION ") {
			exceptionCount++
		} else if strings.HasPrefix(resp, "UNSUPPORTED ") {
			unsupportedCount++
		}
	}

	stdin.Close()
	cmd.Wait()

	fmt.Printf("Cross-verify summary: Total=%d | ExactOK=%d | Mismatch=%d | Overflow=%d | Exception=%d | Unsupported=%d | GoPanic=%d\n",
		len(ids), okCount, mismatchCount, overflowCount, exceptionCount, unsupportedCount, goPanicCount)
}
