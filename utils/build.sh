#!/bin/sh

rm -rf $(realpath $(dirname $0))/../Testing

cmake -G Ninja \
    -S $(realpath $(dirname $0))/.. \
    -B $(realpath $(dirname $0))/../Testing

cmake --build $(realpath $(dirname $0))/../Testing
