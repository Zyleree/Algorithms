FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory specifically for Chess-E
WORKDIR /app/Chess-E

# Only copy Chess-E files
COPY ./Chess-E .

RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j$(nproc)

ENTRYPOINT ["./build/chess_engine"] 