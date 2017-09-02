#!/bin/bash

# proton
# ./routine \
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
# ./routine \
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
# ./routine \
# --source-type=alpha \
# --source-energy=800 \
# --num-history=1e4 \
# --num-thread=28 \
# --phantom-dim=20.0, 20.0, 20.0 \
# --num-voxel=1, 1, 100 \
# --source-position=0, 0, -10.01 \
# --output-suffix=alpha_800MeV

# positron
# ./routine \
# --source-type=e+ \
# --source-energy=20 \
# --num-history=1e7 \
# --num-thread=28 \
# --phantom-dim=10.0,10.0,10.0 \
# --num-voxel=100,100,100 \
# --source-position=0,0,-5.01 \
# --magnetic-field=0,1.5,0

# electron
# ./routine \
# --source-type=e- \
# --source-energy=20 \
# --num-history=1e7 \
# --num-thread=28 \
# --phantom-dim=10.0,10.0,10.0 \
# --num-voxel=100,100,100 \
# --source-position=0,0,-5.01 \
# --magnetic-field=0,1.5,0

# photon
./routine \
--source-type=gamma \
--source-energy=20 \
--num-history=1e6 \
--num-thread=28 \
--phantom-dim=40.0, 40.0, 40.0 \
--num-voxel=100, 100, 100 \
--source-position=0, 0, -20.01 \
--magnetic-field=0, 0, 0




