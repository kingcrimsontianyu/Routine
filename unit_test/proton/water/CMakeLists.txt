if(BUILD_TESTING)
    add_test(NAME test_proton_full
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
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

    add_test(NAME test_proton_tweaker
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
             --num-history=1e5
             --num-thread=10
             --source-position=0,19.999,0
             --square-beam-width=0
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --output-suffix=tweaker
             --output-dir=output
             --log-physics-info
             --physics=tweaker)

    add_test(NAME test_proton_mini
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
             --num-history=1e5
             --num-thread=10
             --source-position=0,19.999,0
             --square-beam-width=0
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --output-suffix=ionization
             --output-dir=output
             --log-physics-info
             --physics=mini-proton)

    add_test(NAME test_proton_nofluc_mini
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
             --num-history=1e5
             --num-thread=10
             --source-position=0,19.999,0
             --square-beam-width=0
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --disable-fluctuation
             --output-suffix=ionization-no-fluc
             --output-dir=output
             --log-physics-info
             --physics=mini-proton)

    add_test(NAME test_proton_multiple_scattering
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
             --num-history=1e5
             --num-thread=10
             --source-position=0,19.999,0
             --square-beam-width=0
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --output-suffix=multiple-scattering-proton
             --output-dir=output
             --log-physics-info
             --physics=multiple-scattering-proton)

    add_test(NAME test_proton_single_scattering
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND routine
             --source-type=proton
             --source-energy=200
             --num-history=1e2
             --num-thread=10
             --source-position=0,19.999,0
             --square-beam-width=0
             --world-dim=200,200,200
             --phantom-path=input/phantom
             --material-path=input/material
             --universe-to-material-path=input/universe_to_material
             --output-suffix=single-scattering-proton
             --output-dir=output
             --log-physics-info
             --physics=single-scattering-proton)
    file(COPY compare input mcnp memcheck.sh run.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
endif()