# Micro Processors

All code from each TU857 Y2 microprocessors lab (CMPU1013)

## Note

In order to take intellisense from this you must run `make init` on first init and
when new projects are added.

## Lab[N]

All labs are in their own folders under the src directory (e.g. Lab 1 -> src/lab1)

## Building

This project uses PlatformIO as a baseline tool and also provides a Makefile to make things easier

- Build all: `make all`
- Build specific lab: `make build LAB=[n]`
- Upload lab to a board: `make upload LAB=[n]`
