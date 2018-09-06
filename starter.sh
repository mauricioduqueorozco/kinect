#!/bin/sh

echo "Clean up"
rm -rf /hello

echo "Create hello"
gcc -o hello hello.c

echo "Open up"
open ./hello
