#!/bin/bash

GDB=/usr/local/bin/gdb
BIN=/home/kingcrimson/research/geant4/g4_routine/build_debug/routine

${GDB} --args \
${BIN} \
--source-type=proton \
--source-energy=200 \
--num-history=1 \
--num-thread=1 \
--source-position=0,19.999,0 \
--square-beam-width=0 \
--world-dim=200,200,200 \
--phantom-path=input/phantom \
--material-path=input/material \
--universe-to-material-path=input/universe_to_material \
--disable-fluctuation \
--output-suffix=mini-proton-nofluc \
--output-dir=output \
--log-physics-info \
--physics=mini-proton




