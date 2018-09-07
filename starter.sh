#!/bin/sh

echo "Clean up"
rm -rf /main

echo "Create hello"
# gcc -o main main.c -lusb-1.0 -framework GLUT OpenGL lGLU lglut -lGL -lGLU -lglut -Wno-deprecated-declarations
gcc -o main main.c -lusb-1.0 -framework GLUT -pthread -Wno-deprecated-declarations
echo "Open up"
./main
