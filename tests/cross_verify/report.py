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


def run_step(label: str, cmd: list[str], cwd: str | None = None) -> subprocess.CompletedProcess[str]:
    done = threading.Event()

    def spin() -> None:
        for ch in itertools.cycle(["⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"]):
            if done.is_set():
                break
            sys.stdout.write(f"\r  {CYAN}{ch}{RESET} {label}...")
            sys.stdout.flush()
            time.sleep(0.08)

    t = threading.Thread(target=spin, daemon=True)
    t.start()
    proc = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    done.set()
    t.join()

    if proc.returncode == 0:
        sys.stdout.write(f"\r  {GREEN}[✓]{RESET} {label}   \n")
    else:
        sys.stdout.write(f"\r  {RED}[✗]{RESET} {label}   \n")
    sys.stdout.flush()
    return proc


def main() -> int:
    if os.name == "nt":
        os.system("")

    here = os.path.dirname(os.path.abspath(__file__))
    root = os.path.abspath(os.path.join(here, "..", ".."))

    print(f"\n{BOLD}{CYAN}=== bedrock-protocol Verification ==={RESET}")

    if len(sys.argv) > 1 and sys.argv[1] == "--all":
        build_dir = sys.argv[2] if len(sys.argv) > 2 else "build"
        config = sys.argv[3] if len(sys.argv) > 3 else "Release"
        cpp_exe = os.path.join(root, build_dir, config, "bedrock-protocol-cross-verify.exe")

        for label, cmd in [
            ("Configuring CMake", ["cmake", "-S", ".", "-B", build_dir, "-DBEDROCK_PROTOCOL_BUILD_POOL=ON", "-DBEDROCK_PROTOCOL_BUILD_TESTS=ON"]),
            ("Building C++ library & tests", ["cmake", "--build", build_dir, "--config", config, "-j", "8"]),
            ("Running C++ unit tests", ["ctest", "--test-dir", build_dir, "-C", config, "--output-on-failure"]),
        ]:
            proc = run_step(label, cmd, cwd=root)
            if proc.returncode != 0:
                print(proc.stdout or proc.stderr, file=sys.stderr)
                return proc.returncode
    else:
        cpp_exe = os.path.abspath(sys.argv[1]) if len(sys.argv) > 1 else os.path.join(root, "build", "Release", "bedrock-protocol-cross-verify.exe")

    proc = run_step("Running bidirectional protocol checks", ["go", "-C", here, "run", ".", cpp_exe])
    if proc.returncode != 0:
        print(proc.stderr, file=sys.stderr)
        return proc.returncode

    results = json.loads(proc.stdout)
    counts: dict[str, int] = {}
    for r in results:
        counts[r["status"]] = counts.get(r["status"], 0) + 1

    total = len(results)
    ok = counts.get("ok", 0)
    pct = (ok / total * 100.0) if total else 0.0
    print(f"  {GREEN}[✓]{RESET} Parity: {ok}/{total} exact ({pct:.1f}%)\n")

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
