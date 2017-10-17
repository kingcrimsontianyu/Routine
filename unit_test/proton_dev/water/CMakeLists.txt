if(BUILD_TESTING)
    add_test(NAME dev
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
             --num-history=1
             --num-thread=1
             --source-position=0,19.999,0
             --square-beam-width=0
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --disable-fluctuation
             --output-suffix=mini-proton-nofluc
             --output-dir=output
             --log-physics-info
             --store-physics-table
             --physics=mini-proton)
    file(COPY compare input mcnp memcheck.sh run.sh debug.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output ${CMAKE_CURRENT_BINARY_DIR}/xs_data)
endif()