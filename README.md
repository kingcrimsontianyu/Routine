# Routine

Tianyu Liu<br>
kingcrimsontianyu@gmail.com<br>
2017

------

Routine is a Geant4-based, simple application for radiation dose calculation.

# Build
- <tt>git clone git@gitlab.com:kingcrimsontianyu/Routine.git</tt><br>
  Download the source code

- <tt>mkdir build</tt><br>
  Where directory build and routine are in the same parent directory.

- <tt>cd build</tt><br>
  <tt>cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 -DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" -DCMAKE_BUILD_TYPE=Release ../routine</tt><br>
  Use cmake to generate makefile. Use the compiler supporting C++11 to build the program. Use O2 for release build type. Use release build type.

