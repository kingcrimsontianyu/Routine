if(BUILD_TESTING)
    add_test(NAME test_photon_electron_full
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=gamma
             --source-energy=10
             --num-history=1e5
             --num-thread=10
             --source-position=0,19.999,0
             --square-beam-width=0
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --output-suffix=qgsp_bic_hp
             --output-dir=output
             --log-physics-info
             --physics=qgsp_bic_hp)

    file(COPY compare input mcnp memcheck.sh run.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
endif()