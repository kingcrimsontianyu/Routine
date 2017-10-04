# Routine

Tianyu Liu<br>
kingcrimsontianyu@gmail.com<br>
2017

------
# Introduction
Routine is a Geant4-based, simple application for radiation dose calculation. It supports MCNP-like definition of voxel phantoms. For example, a water phantom can be defined by creating three plain text files.

- phantom: Specify voxel numbers and voxel dimensions (unit: cm), then specify the universe array as in MCNP. Just like in MCNP, 99r means to repeat the preceding element 99 times.
    ```
    num-voxel-x                 = 1
    num-voxel-y                 = 100
    num-voxel-z                 = 1
    dim-voxel-x                 = 40.0
    dim-voxel-y                 = 0.4
    dim-voxel-z                 = 40.0
    1 99r
    ```

- material: Define material in MCNP way. In MCNP, a material is not associated with density.
    ```
    c water
    c http://physics.nist.gov/cgi-bin/Star/compos.pl?refer=ap&matno=276
    m1
         1000   -0.111894
         8000   -0.888106
    c
    c air
    c http://physics.nist.gov/cgi-bin/Star/compos.pl?refer=ap&matno=104
    m99999
         6000   -0.000124
         7000   -0.755267
         8000   -0.231781
         18000  -0.012827
    ```

- universe_to_material: In MCNP, a material is not associated with density, and a universe is associated with a material and a density. So conceptually MCNP's universe is identical to Geant4's material.
    ```
    c universe       material   density
        1      1     -1.0
    99999  99999     -0.00120479
    ```

To run the simulation, use the command:
```
./routine \
--source-type=proton \
--source-energy=200 \
--num-history=1e5 \
--num-thread=20 \
--source-position=0,21,0 \
--square-beam-width=2 \
--world-dim=200,200,200 \
--phantom-path=input/phantom \
--material-path=input/material \
--universe-to-material-path=input/universe_to_material \
--output-suffix=qgsp_bic_hp \
--output-dir=output \
--log-physics-info \
--physics=qgsp_bic_hp
```

# Build
- ```
  git clone git@gitlab.com:kingcrimsontianyu/Routine.git
  ```
  Download the source code.

- ```
  mkdir build
  ```
  Create a build directory, which should be placed in the same parent directory with the git repository.

- ```
  cd build
  cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 \
  -DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" \
  -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" \
  ../Routine
  ```
  Use cmake to generate makefile. Specifically, use the compiler supporting C++11 to build the program. Use the release build type. Use O2 for the release build type.

- ```
  make
  ```
  Build the routine binary.
