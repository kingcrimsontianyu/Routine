if(BUILD_TESTING)
    add_test(NAME test_proton_200MeV
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
             --num-history=1e5
             --num-thread=20
             --source-position=0,21,0
             --square-beam-width=2
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --output-suffix=qgsp_bic_hp
             --output-dir=output
             --log-physics-info)
    file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY mcnp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY compare DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
endif()