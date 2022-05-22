CC = gcc
CFLAGS = -g -Wall -pedantic

ALL = test_unbounded calc_unbounded_int

all: $(ALL)
test_unbounded : test_unbounded.c
calc_unbounded_int: calc_unbounded_int.c