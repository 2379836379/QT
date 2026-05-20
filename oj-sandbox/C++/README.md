# oj-sandbox

Minimal Docker sandbox for local C++ judging.

## Files

- `Dockerfile`: builds the sandbox image
- `judge_cpp.py`: compiles and runs a single C++ source file

## Build

From `E:\csqt\oj-sandbox`:

```powershell
docker build -t oj-sandbox-cpp .
```

## Run

Prepare on the host:

- `Main.cpp`
- `input.txt`
- optional `answer.txt`

Example:

```powershell
docker run --rm `
  --network none `
  --cpus=1 `
  --memory=256m `
  --pids-limit=64 `
  -v "${PWD}:/work" `
  oj-sandbox-cpp `
  --source /work/Main.cpp `
  --input /work/input.txt `
  --output /work/result.json `
  --answer /work/answer.txt `
  --time-limit-ms 2000
```

If you do not want answer checking, omit `--answer`.

## Result Format

The container writes a JSON file to `--output`.

Possible `status` values:

- `ACCEPTED`
- `WRONG_ANSWER`
- `COMPILE_ERROR`
- `RUNTIME_ERROR`
- `TIME_LIMIT_EXCEEDED`
- `OK`
- `SYSTEM_ERROR`

## Notes

- Current image supports C++ only.
- Time limit is enforced inside the container.
- The recommended Docker flags above also disable networking and limit host resources.
