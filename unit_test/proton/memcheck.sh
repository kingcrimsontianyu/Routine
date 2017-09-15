#!/bin/bash

BIN=/home/kingcrimson/research/geant4/g4_routine/build/routine
# VALGRIND="valgrind --tool=memcheck --track-origins=yes --leak-check=full"

# photon
${VALGRIND} \
${BIN} \
--source-type=proton \
--source-energy=200 \
--num-history=1e2 \
--num-thread=4 \
--source-position=0,21,0 \
--square-beam-width=2 \
--world-dim=200,200,200 \
--phantom-path=input/phantom \
--material-path=input/material \
--universe-to-material-path=input/universe_to_material \
--output-suffix=qbbc \
--output-dir=output \
--log-physics-info




