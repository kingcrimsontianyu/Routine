#[[
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 \
-DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" \
-DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" \
../g4_routine

cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 \
-DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_CXX_FLAGS_DEBUG="-g -O0" \
-DCMAKE_C_FLAGS_DEBUG="-g -O0" \
../g4_routine
]]

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Routine)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#----------------------------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#----------------------------------------------------------------------------
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#----------------------------------------------------------------------------
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#----------------------------------------------------------------------------
add_executable(routine ${sources} ${headers})
target_link_libraries(routine ${Geant4_LIBRARIES})
target_compile_options(routine PUBLIC -fdiagnostics-color)

#----------------------------------------------------------------------------
# add scripts
#----------------------------------------------------------------------------
# copy at build time
file(COPY script DESTINATION ${PROJECT_BINARY_DIR})

#----------------------------------------------------------------------------
# add test
#----------------------------------------------------------------------------
include(CTest)
add_subdirectory(unit_test)





