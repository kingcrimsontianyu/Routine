# RoutineIR

Tianyu Liu
Li Mao
kingcrimsontianyu@gmail.com
2017

------

RoutineIR is a Geant4-based program for Interventional Radiology (IR) radiation dose calculation.

# Build
- <tt>git clone git@gitlab.com:kingcrimsontianyu/routine-ir.git</tt><br>
  Download the source code

- <tt>mkdir build</tt><br>
  Where directory build and routine-ir are in the same parent directory.

- <tt>cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 -DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" -DCMAKE_BUILD_TYPE=Release ../routine-ir</tt><br>
  Use cmake to generate makefile. Use the compiler supporting C++11 to build the program. Use O2 for release build type. Use release build type.

