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
        self.routineTallyList = []
        self.routineSDList    = []
        self.archerTallyList       = []
        self.archerSDList          = []
        self.archerGPUTallyList    = []
        self.archerGPUSDList       = []
        self.title                 = title

        # info
        print("--> numpy version: ", np.version.version)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputMCNPTally(self, input_path):
        print("--> Input MCNP result")
        mcnpTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)
        mcnpRSDList   = np.zeros(self.totalNumVoxel, dtype = np.float64)

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
                        mcnpTallyList[count] = tally
                        mcnpRSDList[count]   = rsd
                        count += 1

        # source position is y positive
        # source direction is y negative
        # so we reverse the data for better visualization
        mcnpTallyList = mcnpTallyList[::-1]
        mcnpRSDList   = mcnpRSDList[::-1]

        mcnpSDList = np.multiply(mcnpTallyList, mcnpRSDList)
        print("    total number of tally data = ", count)
        return (mcnpTallyList, mcnpSDList)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputRoutineTally(self, input_path):
        print("--> Input Routine result")
        routineTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)
        routineRSDList   = np.zeros(self.totalNumVoxel, dtype = np.float64)

        count = 0
        with open(input_path, 'r') as file:
            for line in file:
                lineString = line.split()
                routineTallyList[count] = float(lineString[3])
                routineRSDList[count]   = float(lineString[4])
                count += 1

        # source position is y positive
        # source direction is y negative
        # so we reverse the data for better visualization
        routineTallyList = routineTallyList[::-1]
        routineRSDList   = routineRSDList[::-1]

        routineSDList = np.multiply(routineTallyList, routineRSDList)
        print("    total number of tally data = ", count)
        return (routineTallyList, routineSDList)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputArcherTally(self, input_path):
        print("--> Input ARCHER result")
        archerTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)
        archerSDTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)
        archerRSDTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)

        count = 0
        with open(input_path, 'r') as file:
            for line in file:
                lineString = line.split()
                archerTallyList[count] = float(lineString[3])
                archerRSDTallyList[count] = float(lineString[4])
                count += 1

        # source position is y positive
        # source direction is y negative
        # so we reverse the data for better visualization
        archerTallyList = archerTallyList[::-1]
        archerRSDTallyList = archerRSDTallyList[::-1]

        for i in range(len(archerTallyList)):
            archerSDTallyList[i] = archerTallyList[i] * archerRSDTallyList[i]


        print("    total number of tally data = ", count)
        return (archerTallyList, archerSDTallyList)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def Compare(self):
        fig = plt.figure(figsize=(12, 6))
        plt.suptitle(self.title, fontsize=16)
        ax = plt.subplot(1, 1, 1)

        depthList = np.arange(len(self.routineTallyList))
        depthList = depthList * self.dim_voxel[1] + self.dim_voxel[1] / 2.0
        # print(depthList)

        my_color = '#008800'
        routineLine, = plt.plot(depthList, self.routineTallyList, linestyle='-', linewidth=1, color=my_color,  marker='None', markersize=8)
        plt.errorbar(depthList, self.routineTallyList, yerr=self.routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        my_color = '#0000ff'
        archerLine, = plt.plot(depthList, self.archerTallyList, linestyle='None', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker='o', markersize=8)
        plt.errorbar(depthList, self.archerTallyList, yerr=self.archerSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        my_color = '#ff0000'
        archerGPULine, = plt.plot(depthList, self.archerGPUTallyList, linestyle='None', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker='x', markersize=8)
        # gpu uses batch method
        # we set the batch number to one in order to be consistent with cpu result
        # rsd is not valid for batch == 1
        # so here we don't plot the errorbars

        ax.set_xlabel("Depth [cm]")
        ax.set_ylabel("Absorbed dose [MeV/g]")

        plt.legend([routineLine, archerLine, archerGPULine],
        ["Routine (Geant4 10.3.2, ionization)", "ARCHER-CPU (based on Geant4)", "ARCHER-GPU (based on Geant4)"],
        loc='best', shadow=True, fontsize=12)

        self.title = self.title.replace(' ', '_')
        plt.savefig(self.title + ".pdf", bbox_inches='tight')
        plt.show()

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager((1, 200, 1), (40, 0.2, 40), "Proton 200 MeV in water absorbed dose")
    (m.routineTallyList, m.routineSDList) = m.InputRoutineTally("../output/dose_voxel_ionization.txt")
    # hardcoded
    (m.archerTallyList, m.archerSDList) = m.InputArcherTally("/home/kingcrimson/research/archer_build_release/unit_test/physics/proton/water/output_cpu_FP64/dose.txt")
    (m.archerGPUTallyList, m.archerGPUSDList) = m.InputArcherTally("/home/kingcrimson/research/archer_build_release/unit_test/physics/proton/water/output_gpu_FP64/dose.txt")

    m.Compare()

