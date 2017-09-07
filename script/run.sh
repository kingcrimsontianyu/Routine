#!/bin/bash

BIN=../routine-ir

# photon
TEST_DIR=/home/kingcrimson/research/geant4/g4_routine-ir/g4_routine-ir/test/test_eight_material
${BIN} \
--source-type=gamma \
--source-energy=0.08 \
--num-history=1e5 \
--num-thread=14 \
--source-position=0,20,0 \
--square-beam-width=20 \
--phantom-path=${TEST_DIR}/input/phantom \
--material-path=${TEST_DIR}/input/material \
--universe-to-material-path=${TEST_DIR}/input/universe_to_material \
--output-suffix=qbbc

# photon
# TEST_DIR=/home/kingcrimson/research/geant4/g4_routine-ir/g4_routine-ir/test/test_rpi_am_122kg
# ${BIN} \
# --source-type=gamma \
# --source-energy=20 \
# --num-history=1e2 \
# --num-thread=14 \
# --phantom-dim=40.0,40.0,40.0 \
# --num-voxel=100,100,100 \
# --source-position=0,0,-20.01 \
# --phantom-path=${TEST_DIR}/input/rpi_obese_female_122.geo \
# --material-path=${TEST_DIR}/input/rpi_obese_phantom.mat-p \
# --universe-to-material-path=${TEST_DIR}/input/universe_to_material_rpi_obese_female_122







