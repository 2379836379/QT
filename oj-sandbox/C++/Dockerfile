FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    bash \
    coreutils \
    g++ \
    python3 \
    time \
    diffutils \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /sandbox

COPY judge_cpp.py /usr/local/bin/judge_cpp.py

ENTRYPOINT ["python3", "/usr/local/bin/judge_cpp.py"]
