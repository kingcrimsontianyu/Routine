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
        self.mcnpSDList       = []
        self.full_routineTallyList = []
        self.full_routineSDList    = []
        self.mini_routineTallyList = []
        self.mini_routineSDList    = []
        self.mini_nofluc_routineTallyList = []
        self.mini_nofluc_routineSDList    = []
        self.archerTallyList       = []
        self.archerRSDList         = []
        self.archerSDList          = []
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
        self.archerTallyList = np.zeros(self.totalNumVoxel, dtype = np.float64)

        count = 0
        with open(input_path, 'r') as file:
            for line in file:
                lineString = line.split()
                self.archerTallyList[count] = float(lineString[3])
                count += 1

        # source position is y positive
        # source direction is y negative
        # so we reverse the data for better visualization
        self.archerTallyList = self.archerTallyList[::-1]

        print("    total number of tally data = ", count)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def Compare(self):
        fig = plt.figure(figsize=(12, 6))
        plt.suptitle(self.title, fontsize=16)
        ax = plt.subplot(1, 1, 1)

        depthList = np.arange(len(self.mcnpTallyList))
        depthList = depthList * self.dim_voxel[1] + self.dim_voxel[1] / 2.0
        # print(depthList)

        # my_color = '#00cc66'
        # mcnpLine, = plt.plot(depthList, self.mcnpTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)
        # plt.errorbar(depthList, self.mcnpTallyList, yerr=self.mcnpSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        # my_color = '#ff8000'
        my_color = '#ff0000'
        full_routineLine, = plt.plot(depthList, self.full_routineTallyList, linestyle='-', color=my_color,  markerfacecolor=my_color, markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=5)
        plt.errorbar(depthList, self.full_routineTallyList, yerr=self.full_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        # my_color = '#cc0066'
        my_color = '#0000ff'
        mini_routineLine, = plt.plot(depthList, self.mini_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)
        plt.errorbar(depthList, self.mini_routineTallyList, yerr=self.mini_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        # my_color = '#0000ff'
        # mini_nofluc_routineLine, = plt.plot(depthList, self.mini_nofluc_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker='o', markersize=8)
        # plt.errorbar(depthList, self.mini_routineTallyList, yerr=self.mini_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        # my_color = '#ff0000'
        # archerLine, = plt.plot(depthList, self.archerTallyList, linestyle='None', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker='x', markersize=8)

        ax.set_xlabel("Depth [cm]")
        ax.set_ylabel("Absorbed dose [MeV/g]")

        # plt.legend([mcnpLine, full_routineLine, mini_routineLine, mini_nofluc_routineLine, archerLine],
        # ["MCNP 6.1", "Routine (Geant4 10.3.2, full physics)", "Routine (Geant4 10.3.2, mini physics)", "Routine (Geant4 10.3.2, mini physics no fluctuation)", "Archer (mini physics no fluctuation)"],
        # loc='best', shadow=True, fontsize=12)
        plt.legend([full_routineLine, mini_routineLine],
        ["Routine (Geant4 10.3.2, full physics)", "Routine (Geant4 10.3.2, mini physics)"],
        loc='best', shadow=True, fontsize=12)

        plt.savefig(self.title + ".pdf", bbox_inches='tight')
        plt.show()

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager((1, 100, 1), (40, 0.4, 40), "Proton 200 MeV in water")
    (m.mcnpTallyList, m.mcnpSDList) = m.InputMCNPTally("../mcnp/mctal")
    (m.full_routineTallyList, m.full_routineSDList) = m.InputRoutineTally("../output/dose_voxel_qgsp_bic_hp.txt")
    (m.mini_routineTallyList, m.mini_routineSDList) = m.InputRoutineTally("../output/dose_voxel_mini-proton.txt")
    # (m.mini_nofluc_routineTallyList, m.mini_nofluc_routineSDList) = m.InputRoutineTally("../output/dose_voxel_mini-proton-nofluc.txt")
    # hardcoded
    # m.InputArcherTally("/home/kingcrimson/research/archer_build_debug/unit_test/physics/proton/water/result.txt")
    m.Compare()

