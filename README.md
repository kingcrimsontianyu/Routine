# Routine

Tianyu Liu<br>
kingcrimsontianyu@gmail.com<br>
2017

------
# Introduction
Routine is a Geant4-based, simple application for radiation dose calculation. It supports MCNP-like definition of voxel phantoms. For example, a water phantom can be defined by creating three plain text files.

- phantom: Specify voxel numbers and voxel dimensions (unit: cm), then specify the universe array as in MCNP. Just like in MCNP, 99r means to repeat the preceding element 99 times.
    ```
    num-voxel-x                 = 1
    num-voxel-y                 = 100
    num-voxel-z                 = 1
    dim-voxel-x                 = 40.0
    dim-voxel-y                 = 0.4
    dim-voxel-z                 = 40.0
    1 99r
    ```

- material: Define material in MCNP way. In MCNP, a material is not associated with density.
    ```
    c water
    c http://physics.nist.gov/cgi-bin/Star/compos.pl?refer=ap&matno=276
    m1
         1000   -0.111894
         8000   -0.888106
    c
    c air
    c http://physics.nist.gov/cgi-bin/Star/compos.pl?refer=ap&matno=104
    m99999
         6000   -0.000124
         7000   -0.755267
         8000   -0.231781
         18000  -0.012827
    ```

- universe_to_material: In MCNP, a material is not associated with density, and a universe is associated with a material and a density. So conceptually MCNP's universe is identical to Geant4's material.
    ```
    c universe       material   density
        1      1     -1.0
    ```

To run a simulation with 200 MeV proton source, use the command:
```
./routine \
--source-type=proton \
--source-energy=200 \
--num-history=1e5 \
--num-thread=20 \
--source-position=0,21,0 \
--square-beam-width=2 \
--world-dim=200,200,200 \
--phantom-path=input/phantom \
--material-path=input/material \
--universe-to-material-path=input/universe_to_material \
--output-suffix=qgsp_bic_hp \
--output-dir=output \
--log-physics-info \
--physics=qgsp_bic_hp
```

To run a simulation with 2400 MeV Carbon-12 ion source, use the command:
```
./routine \
--source-ion-Z=6 \
--source-ion-A=6 \
--source-energy=2400 \
--num-history=1e4 \
--num-thread=20 \
--source-position=0,9.999,0 \
--square-beam-width=0.1 \
--world-dim=200,200,200 \
--phantom-path=input/phantom \
--material-path=input/material \
--universe-to-material-path=input/universe_to_material \
--output-suffix=qgsp_bic_hp \
--output-dir=output \
--log-physics-info \
--physics=qgsp_bic_hp
```

The output files include
- Voxel kerma and uncertainty
- Voxel dose and uncertainty
- Statistics of secondary particles --> this is truly great

Here is a sample output file:
```
--> total number of primary particles = 100000
    number of particles actually simulated = 45

--> particle                      count per source         dose per source          percent [%]              PDG encoding
    B10                           6.3600000000e-03         4.9026083634e-07         1.66018e-02              1000050100
    B11                           2.2200000000e-03         1.5808085314e-07         5.35313e-03              1000050110
    B12                           6.0000000000e-05         6.3403304618e-09         2.14704e-04              1000050120
    [lines omitted]
    alpha                         8.9750000000e-02         8.8297919037e-06         2.99006e-01              1000020040
    deuteron                      1.0849000000e-01         4.8385490644e-06         1.63849e-01              1000010020
    e+                            1.2620000000e-02         5.0006847093e-07         1.69340e-02              -11
    e-                            5.2619200000e+00         2.3903304106e-05         8.09444e-01              11
    gamma                         5.8010000000e-01         1.2418094232e-09         4.20517e-05              22
    neutron                       2.5426000000e-01         0.0000000000e+00         0.00000e+00              2112
    proton                        1.0287390000e+01         2.8982135910e-03         9.81430e+01              2212
    triton                        2.4000000000e-03         2.8479514683e-07         9.64409e-03              1000010030
    total dose = 2.9530523821e-03

--> process list
    B10
        Transportation                :
        msc                           : UrbanMsc
        ionIoni                       : BraggIon, BetheBloch
        ionInelastic                  : Binary Light Ion Cascade, FTFP
    B11
        Transportation                :
        msc                           : UrbanMsc
        ionIoni                       : BraggIon, BetheBloch
        ionInelastic                  : Binary Light Ion Cascade, FTFP
    B12
        Transportation                :
        msc                           : UrbanMsc
        ionIoni                       : BraggIon, BetheBloch
        ionInelastic                  : Binary Light Ion Cascade, FTFP
    [lines omitted]
    alpha
        Transportation                :
        msc                           : UrbanMsc
        ionIoni                       : BraggIon, BetheBloch
        hadElastic                    : hElasticLHEP
        alphaInelastic                : Binary Light Ion Cascade, FTFP
    deuteron
        Transportation                :
        msc                           : UrbanMsc
        hIoni                         : Bragg, BetheBloch
        hadElastic                    : hElasticLHEP
        dInelastic                    : Binary Light Ion Cascade, FTFP
    e+
        Transportation                :
        msc                           : UrbanMsc
        eIoni                         : MollerBhabha
        eBrem                         : eBremSB, eBremLPM
        annihil                       : eplus2gg
        CoulombScat                   : eCoulombScattering
        positronNuclear               : G4ElectroVDNuclearModel
    e-
        Transportation                :
        msc                           : UrbanMsc
        eIoni                         : MollerBhabha
        eBrem                         : eBremSB, eBremLPM
        CoulombScat                   : eCoulombScattering
        electronNuclear               : G4ElectroVDNuclearModel
    gamma
        Transportation                :
        phot                          : PhotoElectric
        compt                         : Klein-Nishina
        conv                          : BetheHeitler
        photonNuclear                 : BertiniCascade, TheoFSGenerator
    neutron
        Transportation                :
        Decay                         :
        hadElastic                    : hElasticCHIPS, NeutronHPElastic
        neutronInelastic              : QGSP, FTFP, Binary Cascade, NeutronHPInelastic
        nCapture                      : NeutronHPCapture, nRadCapture
        nFission                      : NeutronHPFission, G4LFission
    proton
        Transportation                :
        msc                           : UrbanMsc
        hIoni                         : Bragg, BetheBloch
        hBrems                        : hBrem
        hPairProd                     : hPairProd
        CoulombScat                   : eCoulombScattering
        hadElastic                    : hElasticCHIPS
        protonInelastic               : QGSP, FTFP, Binary Cascade
    triton
        Transportation                :
        msc                           : UrbanMsc
        hIoni                         : Bragg, BetheBloch
        Decay                         :
        hadElastic                    : hElasticLHEP
        tInelastic                    : Binary Light Ion Cascade, FTFP

```

# Build
- ```
  git clone git@gitlab.com:kingcrimsontianyu/Routine.git
  ```
  Download the source code.

- ```
  mkdir build
  ```
  Create a build directory, which should be placed in the same parent directory with the git repository.

- ```
  cd build
  cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.9.4 \
  -DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.9.4 \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" \
  -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG" \
  ../Routine
  ```
  Use cmake to generate makefile. Specifically, use the compiler supporting C++11 to build the program. Use the release build type. Use O2 for the release build type.

- ```
  make
  ```
  Build the routine binary.
