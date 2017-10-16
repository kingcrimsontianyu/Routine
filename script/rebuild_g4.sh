#!/bin/bash

cd /home/kingcrimson/Downloads/geant4.10.3.2_build_debug
# make >/dev/null 2>&1
# make install >/dev/null 2>&1
make -j8
make install
cd -