#!/bin/bash

BIN=../routine-ir
VALGRIND="valgrind --tool=memcheck --track-origins=yes --leak-check=full"

# photon
${VALGRIND} \
${BIN} \
--source-type=gamma \
--source-energy=20 \
--num-history=1e1 \
--num-thread=1 \
--phantom-dim=40.0,40.0,40.0 \
--num-voxel=10,10,10 \
--source-position=0,0,-20.01 \
--magnetic-field=0,0,0




