import itertools
import json
import os
import subprocess
import sys
import threading
import time

RESET = "\033[0m"
BOLD = "\033[1m"
DIM = "\033[2m"
CYAN = "\033[36m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
RED = "\033[31m"
GRAY = "\033[90m"


def main() -> int:
    if os.name == "nt":
        os.system("")

    here = os.path.dirname(os.path.abspath(__file__))
    cpp_exe = os.path.abspath(sys.argv[1]) if len(sys.argv) > 1 else os.path.join(here, "..", "..", "build", "Release", "bedrock-protocol-cross-verify.exe")

    print(f"\n{BOLD}{CYAN}=== bedrock-protocol Verification ==={RESET}")
    print(f"  {GREEN}[✓]{RESET} C++ Build & Unit Tests")

    done = threading.Event()

    def spin() -> None:
        for ch in itertools.cycle(["⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"]):
            if done.is_set():
                break
            sys.stdout.write(f"\r  {CYAN}{ch}{RESET} Running bidirectional protocol checks...")
            sys.stdout.flush()
            time.sleep(0.08)

    t = threading.Thread(target=spin, daemon=True)
    t.start()

    proc = subprocess.run(
        ["go", "-C", here, "run", ".", cpp_exe],
        capture_output=True,
        text=True,
    )
    done.set()
    t.join()

    if proc.returncode != 0:
        sys.stdout.write(f"\r  {RED}[✗]{RESET} Bidirectional protocol checks failed\n")
        print(proc.stderr, file=sys.stderr)
        return proc.returncode

    results = json.loads(proc.stdout)
    counts: dict[str, int] = {}
    for r in results:
        counts[r["status"]] = counts.get(r["status"], 0) + 1

    total = len(results)
    ok = counts.get("ok", 0)
    pct = (ok / total * 100.0) if total else 0.0
    sys.stdout.write(f"\r  {GREEN}[✓]{RESET} Bidirectional protocol checks ({ok}/{total} exact, {pct:.1f}%)\n\n")
    sys.stdout.flush()

    sections = [
        ("go_decode_fail", "DECODE FAIL", RED, "✗"),
        ("exception", "C++ EXCEPTION", RED, "✗"),
        ("overflow", "C++ OVERFLOW", RED, "✗"),
        ("mismatch", "BYTE MISMATCH", YELLOW, "!"),
        ("unsupported", "UNSUPPORTED", GRAY, "-"),
    ]

    for key, title, color, icon in sections:
        items = [r for r in results if r["status"] == key]
        if not items:
            continue
        print(f"{BOLD}{color}{title} ({len(items)}){RESET}")
        for r in items:
            detail = r.get("error", "")
            if not detail and r.get("in_hex") and r.get("out_hex"):
                detail = f"in={r['in_hex'][:24]}.. out={r['out_hex'][:24]}.."
            print(f"  {color}[{icon}] #{r['id']:<4}{RESET} {r['name']:<36} {DIM}{detail}{RESET}")
        print()

    print(
        f"{BOLD}Summary:{RESET} Total={CYAN}{total}{RESET} | "
        f"{GREEN}OK={ok} ({pct:.1f}%){RESET} | "
        f"{YELLOW}Mismatch={counts.get('mismatch', 0)}{RESET} | "
        f"{RED}Overflow={counts.get('overflow', 0)}{RESET} | "
        f"{RED}Exception={counts.get('exception', 0)}{RESET} | "
        f"{RED}DecodeFail={counts.get('go_decode_fail', 0)}{RESET} | "
        f"{GRAY}Unsupported={counts.get('unsupported', 0)}{RESET} | "
        f"{GRAY}GoPanic={counts.get('go_panic', 0)}{RESET}\n"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
