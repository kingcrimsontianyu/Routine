#!/bin/bash

BIN=../routine-ir
VALGRIND="valgrind --tool=memcheck --track-origins=yes --leak-check=full"

# photon
TEST_DIR=/home/kingcrimson/research/geant4/g4_routine-ir/g4_routine-ir/test/test_eight_material
${VALGRIND} \
${BIN} \
--source-type=gamma \
--source-energy=0.08 \
--num-history=1e2 \
--num-thread=14 \
--source-position=0,20,0 \
--square-beam-width=20 \
--phantom-path=${TEST_DIR}/input/phantom \
--material-path=${TEST_DIR}/input/material \
--universe-to-material-path=${TEST_DIR}/input/universe_to_material \
--output-suffix=qbbc




