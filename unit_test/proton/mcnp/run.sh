#!/bin/bash
set -ex

BIN=/opt/mcnp/bin/mcnp6_linux_x86_64_omp

export DATAPATH=/opt/mcnp/data_mcnp6/MCNP_DATA

${BIN} inp=inp