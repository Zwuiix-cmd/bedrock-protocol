import json
import os
import subprocess
import sys

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
        os.system("")  # enable ANSI escapes on Windows

    here = os.path.dirname(os.path.abspath(__file__))
    cpp_exe = os.path.abspath(sys.argv[1]) if len(sys.argv) > 1 else os.path.join(here, "..", "..", "build", "Release", "bedrock-protocol-cross-verify.exe")

    proc = subprocess.run(
        ["go", "-C", here, "run", ".", cpp_exe],
        capture_output=True,
        text=True,
    )
    if proc.returncode != 0:
        print(f"{RED}Go verifier failed:{RESET}\n{proc.stderr}", file=sys.stderr)
        return proc.returncode

    results = json.loads(proc.stdout)
    counts: dict[str, int] = {}
    for r in results:
        counts[r["status"]] = counts.get(r["status"], 0) + 1

    print(f"\n{BOLD}{CYAN}=== bedrock-protocol <-> gophertunnel Cross-Verify ==={RESET}\n")

    sections = [
        ("go_decode_fail", "GO DECODE FAIL", RED),
        ("exception", "C++ EXCEPTION", RED),
        ("overflow", "C++ OVERFLOW", RED),
        ("mismatch", "BYTE MISMATCH", YELLOW),
        ("unsupported", "UNSUPPORTED", GRAY),
    ]

    for key, title, color in sections:
        items = [r for r in results if r["status"] == key]
        if not items:
            continue
        print(f"{BOLD}{color}{title} ({len(items)}){RESET}")
        for r in items:
            detail = r.get("error", "")
            if not detail and r.get("in_hex") and r.get("out_hex"):
                detail = f"in={r['in_hex'][:24]}.. out={r['out_hex'][:24]}.."
            print(f"  {color}#{r['id']:<4}{RESET} {r['name']:<36} {DIM}{detail}{RESET}")
        print()

    total = len(results)
    ok = counts.get("ok", 0)
    pct = (ok / total * 100.0) if total else 0.0
    print(
        f"{BOLD}Summary:{RESET} Total={CYAN}{total}{RESET} | "
        f"{GREEN}OK={ok} ({pct:.1f}%){RESET} | "
        f"{YELLOW}Mismatch={counts.get('mismatch', 0)}{RESET} | "
        f"{RED}Overflow={counts.get('overflow', 0)}{RESET} | "
        f"{RED}Exception={counts.get('exception', 0)}{RESET} | "
        f"{RED}GoDecodeFail={counts.get('go_decode_fail', 0)}{RESET} | "
        f"{GRAY}Unsupported={counts.get('unsupported', 0)}{RESET} | "
        f"{GRAY}GoPanic={counts.get('go_panic', 0)}{RESET}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
