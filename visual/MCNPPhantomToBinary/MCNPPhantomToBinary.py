#!/usr/bin/env python3

import numpy as np

class Manager:
    #------------------------------------------------------------
    #------------------------------------------------------------
    def __init__(self,
                 input_path,
                 output_path,
                 num_voxel_x,
                 num_voxel_y,
                 num_voxel_z):

        self.input_path           = input_path
        self.output_path          = output_path
        self.num_voxel_x          = num_voxel_x
        self.num_voxel_y          = num_voxel_y
        self.num_voxel_z          = num_voxel_z

        self.totalNumVoxel        = self.num_voxel_x * self.num_voxel_y * self.num_voxel_z
        self.phantomList          = []

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputMCNPPhantom(self):
        print("--> Input MCNP phantom")
        self.phantomList = np.zeros(self.totalNumVoxel, dtype = np.int32)

        # flag
        count = 0   # number of voxels having been read
        repeatTimes = 0
        voxelToRepeat = 0

        # read file line by line
        with open(self.input_path, 'r') as file:
            startParsing = False
            # read phantom matrix
            for line in file:
                # if a line has fill keyword
                # start parsing the phantom matrix from the next line
                if line.find("fill") != -1:
                    startParsing = True
                    continue

                if startParsing:
                    # tokenize
                    stringList = line.split()

                    for i in range(len(stringList)):
                        if stringList[i][-1] == 'r': # if the last character is r
                            repeatTimes = int(stringList[i][:-1]) # remove r, get the number of times to repeat

                            for j in range(repeatTimes):
                                self.phantomList[count] = voxelToRepeat
                                count += 1
                        else:
                            voxelToRepeat = int(stringList[i])
                            self.phantomList[count] = voxelToRepeat
                            count += 1

                    # if sufficient number of voxels have been parsed, stop parsing
                    if count == self.totalNumVoxel:
                        startParsing = False
                        break

        # display
        print("    Statistics:")
        print("    Number of voxels in total = {0:d}".format(self.totalNumVoxel))
        print("    Number of voxels having actually been read = {0:d}".format(count))

    #------------------------------------------------------------
    #------------------------------------------------------------
    def ExportBinaryPhantom(self):
        # save to the standardized npy format
        # np.save(output_phantom_material_path, phantomMaterial1D)
        # np.save(output_phantom_density_path , phantomDensity1D)

        # save to regular binary format
        self.phantomList.tofile(self.output_path, sep="", format="%d")
        print("    The phantom is stored in binary format as an array of 4-byte integers.")

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    p = Manager("rpi_obese_female_122.geo",
                "phantom.bin",
                136,
                133,
                469)
    p.InputMCNPPhantom()
    p.ExportBinaryPhantom()

