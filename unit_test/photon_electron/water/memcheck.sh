#!/bin/bash

BIN=../../../routine
VALGRIND="valgrind --tool=memcheck --track-origins=yes --leak-check=full"

${VALGRIND} \
${BIN} \
--source-type=gamma \
--source-energy=10 \
--num-history=1e2 \
--num-thread=10 \
--source-position=0,19.999,0 \
--square-beam-width=0 \
--world-dim=200,200,200 \
--phantom-path=input/phantom \
--material-path=input/material \
--universe-to-material-path=input/universe_to_material \
--output-suffix=mini-gamma \
--output-dir=output \
--log-physics-info \
--physics=mini-gamma




