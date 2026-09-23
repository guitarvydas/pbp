#!/bin/bash
gcc -std=c89 -pedantic -Wall -Wextra -o kernel0d_demo \
    kernel0d_util.c kernel0d_core.c kernel0d_registry.c \
    kernel0d_builtins.c kernel0d_main.c \
    kernel0d_example_stubs_and_main.c
