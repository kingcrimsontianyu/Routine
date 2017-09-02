# RoutineIR

Tianyu Liu
Li Mao
kingcrimsontianyu@gmail.com
2017

------

RoutineIR is a Geant4-based program for Interventional Radiology (IR) radiation dose calculation.

# Download
<tt>git clone</tt>

# Build
- <tt>mkdir build</tt>
  Where build and RoutineIR have the same parent directory.

- <tt>cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 -DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" -DCMAKE_BUILD_TYPE=Release ../RoutineIR</tt>
  Use cmake to generate makefile. Use more recent compilers that support C++11. Use O2 for release build type. Use release build type.

