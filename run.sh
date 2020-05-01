#!/bin/bash

if [[ $# != 1 ]]; then
    echo "Usage: $(basename $0) SECRET_WORD"
    exit 1
fi

if [[ ! -d build ]]; then
    mkdir build
    cd build
    cmake ..
    make
    cd ..
fi

ATTACKER_THREAD=0
VICTIM_THREAD=$(($(nproc) / 2))

taskset -c ${ATTACKER_THREAD} ./build/attacker &
ATTACKER_PID=$!

taskset -c ${VICTIM_THREAD} ./build/victim $1

kill ${ATTACKER_PID}
