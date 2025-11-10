#!/bin/bash

. $(realpath $(dirname $0))/setup-env.sh

CC=clang CXX=clang++ $(realpath $(dirname $0))/build.sh

cmake --build $(realpath $(dirname $0))/../Testing -t test
