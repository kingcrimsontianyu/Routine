#!/usr/bin/env python3

import numpy as np

class Manager:
    #------------------------------------------------------------
    #------------------------------------------------------------
    def __init__(self,
                 input_path,
                 output_tally_path,
                 output_rsd_path,
                 num_voxel_x,
                 num_voxel_y,
                 num_voxel_z):

        self.input_path           = input_path
        self.output_tally_path    = output_tally_path
        self.output_rsd_path      = output_rsd_path
        self.num_voxel_x          = num_voxel_x
        self.num_voxel_y          = num_voxel_y
        self.num_voxel_z          = num_voxel_z

        self.totalNumVoxel        = self.num_voxel_x * self.num_voxel_y * self.num_voxel_z
        self.tallyList            = []
        self.rsdList              = []

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputMCNPMesh(self):
        print("--> Input MCNP result")
        self.tallyList     = np.zeros(self.totalNumVoxel, dtype = np.float64)
        self.rsdList       = np.zeros(self.totalNumVoxel, dtype = np.float64)

        isToRead = False

        currentTallyIdx = -1
        count = 0

        with open(self.input_path, 'r') as file:
            for line in file:
                if line.find("vals") != -1:
                    isToRead = True
                    continue

                if line.find("tfc") != -1:
                    isToRead = False
                    continue

                if isToRead:
                    lineString = line.split()
                    for i in range(int(len(lineString) / 2)):
                        tally = float(lineString[2 * i])
                        rsd = float(lineString[2 * i + 1])
                        self.tallyList[count] = tally
                        self.rsdList[count]   = rsd
                        count += 1


        print("    total number of tally data = ", count)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def ExportBinaryPhantom(self):
        # save to the standardized npy format
        # np.save(output_phantom_material_path, phantomMaterial1D)
        # np.save(output_phantom_density_path , phantomDensity1D)

        # save to regular binary format
        self.tallyList.tofile(self.output_tally_path, sep="", format="%.16f")
        self.rsdList.tofile(self.output_rsd_path, sep="", format="%.16f")
        print("    The tally and rsd are stored in binary format as an array of 8-byte floating points.")

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager("mctal_test.inp",
                "tally.bin",
                "rsd.bin",
                36,
                12,
                10)
    m.InputMCNPMesh()
    m.ExportBinaryPhantom()

