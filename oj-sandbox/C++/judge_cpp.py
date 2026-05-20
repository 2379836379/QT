#!/usr/bin/env python3
import argparse
import json
import os
import shutil
import signal
import subprocess
import sys
import tempfile
import time
from pathlib import Path


def write_result(output_path: Path, payload: dict) -> int:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(
        json.dumps(payload, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    return 0


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def normalize_text(text: str) -> str:
    lines = [line.rstrip() for line in text.replace("\r\n", "\n").split("\n")]
    while lines and lines[-1] == "":
        lines.pop()
    return "\n".join(lines)


def run_with_timeout(command: list[str], stdin_data: bytes, timeout_s: float):
    start = time.perf_counter()
    process = subprocess.Popen(
        command,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        start_new_session=True,
    )
    try:
        stdout_data, stderr_data = process.communicate(
            input=stdin_data,
            timeout=timeout_s,
        )
        elapsed_ms = int((time.perf_counter() - start) * 1000)
        return {
            "timed_out": False,
            "returncode": process.returncode,
            "stdout": stdout_data.decode("utf-8", errors="replace"),
            "stderr": stderr_data.decode("utf-8", errors="replace"),
            "time_ms": elapsed_ms,
        }
    except subprocess.TimeoutExpired:
        try:
            os.killpg(process.pid, signal.SIGKILL)
        except ProcessLookupError:
            pass
        stdout_data, stderr_data = process.communicate()
        elapsed_ms = int((time.perf_counter() - start) * 1000)
        return {
            "timed_out": True,
            "returncode": None,
            "stdout": stdout_data.decode("utf-8", errors="replace"),
            "stderr": stderr_data.decode("utf-8", errors="replace"),
            "time_ms": elapsed_ms,
        }


def main() -> int:
    parser = argparse.ArgumentParser(description="Minimal C++ local judge")
    parser.add_argument("--source", required=True, help="Path to source file")
    parser.add_argument("--input", required=True, help="Path to input file")
    parser.add_argument("--output", required=True, help="Path to result json")
    parser.add_argument("--answer", help="Optional expected output file")
    parser.add_argument("--time-limit-ms", type=int, default=2000)
    args = parser.parse_args()

    source_path = Path(args.source)
    input_path = Path(args.input)
    output_path = Path(args.output)
    answer_path = Path(args.answer) if args.answer else None

    if not source_path.exists():
        return write_result(output_path, {
            "status": "SYSTEM_ERROR",
            "message": f"source not found: {source_path}",
        })
    if not input_path.exists():
        return write_result(output_path, {
            "status": "SYSTEM_ERROR",
            "message": f"input not found: {input_path}",
        })
    if answer_path is not None and not answer_path.exists():
        return write_result(output_path, {
            "status": "SYSTEM_ERROR",
            "message": f"answer not found: {answer_path}",
        })

    with tempfile.TemporaryDirectory(prefix="oj_cpp_") as temp_dir:
        temp_root = Path(temp_dir)
        work_source = temp_root / "Main.cpp"
        binary_path = temp_root / "main"
        shutil.copyfile(source_path, work_source)

        compile_command = [
            "g++",
            str(work_source),
            "-O2",
            "-std=c++17",
            "-o",
            str(binary_path),
        ]
        compile_process = subprocess.run(
            compile_command,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        if compile_process.returncode != 0:
            return write_result(output_path, {
                "status": "COMPILE_ERROR",
                "message": "Compilation failed.",
                "compile_stdout": compile_process.stdout,
                "compile_stderr": compile_process.stderr,
            })

        run_result = run_with_timeout(
            [str(binary_path)],
            input_path.read_bytes(),
            args.time_limit_ms / 1000.0,
        )

        if run_result["timed_out"]:
            return write_result(output_path, {
                "status": "TIME_LIMIT_EXCEEDED",
                "message": "Program exceeded time limit.",
                "stdout": run_result["stdout"],
                "stderr": run_result["stderr"],
                "time_ms": run_result["time_ms"],
            })

        if run_result["returncode"] != 0:
            return write_result(output_path, {
                "status": "RUNTIME_ERROR",
                "message": "Program exited with non-zero code.",
                "exit_code": run_result["returncode"],
                "stdout": run_result["stdout"],
                "stderr": run_result["stderr"],
                "time_ms": run_result["time_ms"],
            })

        result_payload = {
            "status": "OK",
            "message": "Program finished.",
            "exit_code": run_result["returncode"],
            "stdout": run_result["stdout"],
            "stderr": run_result["stderr"],
            "time_ms": run_result["time_ms"],
        }

        if answer_path is not None:
            actual = normalize_text(run_result["stdout"])
            expected = normalize_text(read_text(answer_path))
            result_payload["expected_stdout"] = expected
            if actual == expected:
                result_payload["status"] = "ACCEPTED"
                result_payload["message"] = "Output matches expected output."
            else:
                result_payload["status"] = "WRONG_ANSWER"
                result_payload["message"] = "Output does not match expected output."

        return write_result(output_path, result_payload)


if __name__ == "__main__":
    sys.exit(main())
