#!/usr/bin/env python3

import numpy as np
import matplotlib
matplotlib.use('PDF')
import matplotlib.pyplot as plt

class Manager:
    #------------------------------------------------------------
    #------------------------------------------------------------
    def __init__(self, num_voxel, dim_voxel, title):

        self.num_voxel = num_voxel
        self.dim_voxel = dim_voxel
        self.totalNumVoxel    = self.num_voxel[0] * self.num_voxel[1] * self.num_voxel[2]
        self.mcnpTallyList    = []
        self.mcnpRSDList      = []
        self.mcnpSDList       = []
        self.routineTallyList = []
        self.routineRSDList   = []
        self.routineSDList    = []
        self.title            = title

        # info
        print("--> numpy version: ", np.version.version)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputMCNPTally(self, input_path):
        print("--> Input MCNP result")
        self.mcnpTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)
        self.mcnpRSDList   = np.zeros(self.totalNumVoxel, dtype = np.float64)

        isToRead = False
        count = 0
        with open(input_path, 'r') as file:
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
                        self.mcnpTallyList[count] = tally
                        self.mcnpRSDList[count]   = rsd
                        count += 1

        # source position is y positive
        # source direction is y negative
        # so we reverse the data for better visualization
        self.mcnpTallyList = self.mcnpTallyList[::-1]
        self.mcnpRSDList   = self.mcnpRSDList[::-1]

        self.mcnpSDList = np.multiply(self.mcnpTallyList, self.mcnpRSDList)
        print("    total number of tally data = ", count)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputRoutineTally(self, input_path):
        print("--> Input Routine result")
        self.routineTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)
        self.routineRSDList   = np.zeros(self.totalNumVoxel, dtype = np.float64)

        count = 0
        with open(input_path, 'r') as file:
            for line in file:
                lineString = line.split()
                self.routineTallyList[count] = float(lineString[3])
                self.routineRSDList[count]   = float(lineString[4])
                count += 1

        # source position is y positive
        # source direction is y negative
        # so we reverse the data for better visualization
        self.routineTallyList = self.routineTallyList[::-1]
        self.routineRSDList   = self.routineRSDList[::-1]

        self.routineSDList = np.multiply(self.routineTallyList, self.routineRSDList)
        print("    total number of tally data = ", count)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def Compare(self):
        fig = plt.figure(figsize=(10, 5))
        plt.suptitle(self.title, fontsize=16)
        ax = plt.subplot(1, 1, 1)

        depthList = np.arange(len(self.mcnpTallyList))
        depthList = depthList * self.dim_voxel[1] + self.dim_voxel[1] / 2.0
        # print(depthList)

        mcnpLine, = plt.plot(depthList, self.mcnpTallyList, linestyle='-', color='#ff0000',  markerfacecolor='None', markeredgecolor='#ff0000', markeredgewidth=1, marker='o', markersize=8)
        plt.errorbar(depthList, self.mcnpTallyList, yerr=self.mcnpSDList, ecolor='#ff0000', elinewidth=0.8, linestyle='None')

        routineLine, = plt.plot(depthList, self.routineTallyList, linestyle='-', color='#0000ff',  markerfacecolor='#0000ff', markeredgecolor='#0000ff', markeredgewidth=1, marker='x', markersize=5)
        plt.errorbar(depthList, self.routineTallyList, yerr=self.routineSDList, ecolor='#0000ff', elinewidth=0.8, linestyle='None')

        ax.set_xlabel("Depth [cm]")
        ax.set_ylabel("Absorbed dose [MeV/g]")
        plt.legend([mcnpLine, routineLine], ["MCNP 6.1", "Routine (Geant4 10.3.2)"], loc='best', shadow=True)
        plt.savefig(self.title + ".pdf", bbox_inches='tight')
        plt.show()

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager((1, 100, 1), (20, 0.2, 20), "Proton 200 MeV")
    m.InputMCNPTally("../mcnp/mctal")
    m.InputRoutineTally("../output/dose_voxel_qgsp_bic_hp.txt")
    m.Compare()

