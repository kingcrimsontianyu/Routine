#!/bin/bash

BIN=../routine-ir

# proton
# ${BIN} \
# --source-type=proton \
# --source-energy=200 \
# --num-history=1e2 \
# --num-thread=1 \
# --phantom-dim=20.0, 20.0, 20.0 \
# --num-voxel=1, 1, 100 \
# --source-position=0, 0, -10.01 \
# --output-suffix=proton_200MeV \
# --store-physics-table \
# --physics-table-dir=xs_data

# C-12
# ${BIN} \
# --source-ion-Z=6 \
# --source-ion-A=12 \
# --source-energy=2400 \
# --num-history=1e5 \
# --num-thread=28 \
# --phantom-dim=20.0, 20.0, 20.0 \
# --num-voxel=1, 1, 100 \
# --source-position=0, 0, -10.01 \
# --output-suffix=C12_2400MeV

# alpha
# ${BIN} \
# --source-type=alpha \
# --source-energy=800 \
# --num-history=1e4 \
# --num-thread=28 \
# --phantom-dim=20.0, 20.0, 20.0 \
# --num-voxel=1, 1, 100 \
# --source-position=0, 0, -10.01 \
# --output-suffix=alpha_800MeV

# positron
# ${BIN} \
# --source-type=e+ \
# --source-energy=20 \
# --num-history=1e7 \
# --num-thread=28 \
# --phantom-dim=10.0,10.0,10.0 \
# --num-voxel=100,100,100 \
# --source-position=0,0,-5.01 \
# --magnetic-field=0,1.5,0

# electron
# ${BIN} \
# --source-type=e- \
# --source-energy=20 \
# --num-history=1e7 \
# --num-thread=28 \
# --phantom-dim=10.0,10.0,10.0 \
# --num-voxel=100,100,100 \
# --source-position=0,0,-5.01 \
# --magnetic-field=0,1.5,0

# photon
${BIN} \
--source-type=gamma \
--source-energy=20 \
--num-history=1e6 \
--num-thread=28 \
--phantom-dim=40.0,40.0,40.0 \
--num-voxel=100,100,100 \
--source-position=0,0,-20.01 \
--magnetic-field=0,0,0



