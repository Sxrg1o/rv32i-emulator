#!/bin/bash

mkdir -p build

gcc ./src/*.c -Wall -g -Iinclude -o ./build/rv32iemu
