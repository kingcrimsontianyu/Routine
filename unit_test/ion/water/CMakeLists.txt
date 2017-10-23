if(BUILD_TESTING)
    add_test(NAME test_carbon_12
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-ion-Z=6
             --source-ion-A=6
             --source-energy=2400
             --num-history=1e4
             --num-thread=20
             --source-position=0,9.999,0
             --square-beam-width=0.1
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --output-suffix=qgsp_bic_hp
             --output-dir=output
             --log-physics-info
             --physics=qgsp_bic_hp)

    file(COPY compare input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
endif()