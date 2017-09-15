# Routine

Tianyu Liu<br>
kingcrimsontianyu@gmail.com<br>
2017

------

Routine is a Geant4-based, simple application for radiation dose calculation.

# Build
- ```
  git clone git@gitlab.com:kingcrimsontianyu/Routine.git
  ```
  Download the source code

- ```
  mkdir build
  ```
  Where directory build and routine are in the same parent directory.

- ```
  cd build
  cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 \
  -DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" \
  -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" \
  ../g4_routine
  ```
  Use cmake to generate makefile. Specifically, use the compiler supporting C++11 to build the program. Use the release build type. Use O2 for the release build type.

