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
        self.tweaker_routineTallyList = []
        self.tweaker_routineSDList    = []
        self.ionization_routineTallyList = []
        self.ionization_routineSDList    = []
        self.ionization_nofluc_routineTallyList = []
        self.ionization_nofluc_routineSDList    = []
        self.multiple_scattering_routineTallyList = []
        self.multiple_scattering_routineSDList    = []
        self.single_scattering_routineTallyList = []
        self.single_scattering_routineSDList    = []
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

        depthList = np.arange(len(self.full_routineTallyList))
        depthList = depthList * self.dim_voxel[1] + self.dim_voxel[1] / 2.0
        # print(depthList)

        # my_color = '#00cc66'
        # mcnpLine, = plt.plot(depthList, self.mcnpTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)
        # plt.errorbar(depthList, self.mcnpTallyList, yerr=self.mcnpSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        my_color = '#ff8000'
        full_routineLine, = plt.plot(depthList, self.full_routineTallyList, linestyle='-', color=my_color,  markerfacecolor=my_color, markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=5)
        plt.errorbar(depthList, self.full_routineTallyList, yerr=self.full_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')
        idx = 127
        ax.annotate('Our goal,\nmost accurate',
                    xy=(depthList[idx], self.full_routineTallyList[idx]), xycoords='data',
                    xytext=(100, 10), textcoords='offset points',
                    arrowprops=dict(arrowstyle="->", color=my_color), color=my_color,
                    verticalalignment='center')

        # my_color = '#ff0000'
        # tweaker_routineLine, = plt.plot(depthList, self.tweaker_routineTallyList, linestyle='-', color=my_color,  markerfacecolor=my_color, markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=5)
        # plt.errorbar(depthList, self.tweaker_routineTallyList, yerr=self.tweaker_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        my_color = '#00994c'
        ionization_nofluc_routineLine, = plt.plot(depthList, self.ionization_nofluc_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)
        plt.errorbar(depthList, self.ionization_nofluc_routineTallyList, yerr=self.ionization_nofluc_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')
        idx = 128
        ax.annotate('Minimum physics',
                    xy=(depthList[idx], self.ionization_nofluc_routineTallyList[idx]), xycoords='data',
                    xytext=(100, 10), textcoords='offset points',
                    arrowprops=dict(arrowstyle="->", color=my_color), color=my_color,
                    verticalalignment='center')

        my_color = '#0000ff'
        ionization_routineLine, = plt.plot(depthList, self.ionization_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)
        plt.errorbar(depthList, self.ionization_routineTallyList, yerr=self.ionization_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')
        idx = 127
        ax.annotate('Where we are now',
                    xy=(depthList[idx], self.ionization_routineTallyList[idx]), xycoords='data',
                    xytext=(100, 20), textcoords='offset points',
                    arrowprops=dict(arrowstyle="->", color=my_color), color=my_color,
                    verticalalignment='center')

        my_color = '#cc0066'
        multiple_scattering_routineLine, = plt.plot(depthList, self.multiple_scattering_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)
        plt.errorbar(depthList, self.multiple_scattering_routineTallyList, yerr=self.multiple_scattering_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')
        idx = 128
        ax.annotate('Very important\nbut likely underappreciated',
                    xy=(depthList[idx], self.multiple_scattering_routineTallyList[idx]), xycoords='data',
                    xytext=(100, 10), textcoords='offset points',
                    arrowprops=dict(arrowstyle="->", color=my_color), color=my_color,
                    verticalalignment='center')

        # my_color = '#0000ff'
        # single_scattering_routineLine, = plt.plot(depthList, self.single_scattering_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)
        # plt.errorbar(depthList, self.single_scattering_routineTallyList, yerr=self.single_scattering_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        # my_color = '#ff0000'
        # archerLine, = plt.plot(depthList, self.archerTallyList, linestyle='None', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker='x', markersize=8)

        ax.set_xlabel("Depth [cm]")
        ax.set_ylabel("Absorbed dose [MeV/g]")

        plt.legend([ionization_nofluc_routineLine, ionization_routineLine, multiple_scattering_routineLine, full_routineLine],
        ["Routine (Geant4 10.3.2, ionization, no fluctuation)", "Routine (Geant4 10.3.2, ionization)", "Routine (Geant4 10.3.2, EM multiple scattering)", "Routine (Geant4 10.3.2, full physics)"],
        loc='best', shadow=True, fontsize=12)

        self.title = self.title.replace(' ', '_')
        plt.savefig(self.title + ".pdf", bbox_inches='tight')
        plt.show()

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager((1, 200, 1), (40, 0.2, 40), "Proton 200 MeV in water absorbed dose")
    # (m.mcnpTallyList, m.mcnpSDList) = m.InputMCNPTally("../mcnp/mctal")
    (m.full_routineTallyList, m.full_routineSDList) = m.InputRoutineTally("../output/dose_voxel_qgsp_bic_hp.txt")
    # (m.tweaker_routineTallyList, m.tweaker_routineSDList) = m.InputRoutineTally("../output/dose_voxel_tweaker.txt")
    (m.ionization_routineTallyList, m.ionization_routineSDList) = m.InputRoutineTally("../output/dose_voxel_ionization.txt")
    (m.ionization_nofluc_routineTallyList, m.ionization_nofluc_routineSDList) = m.InputRoutineTally("../output/dose_voxel_ionization-no-fluc.txt")
    (m.multiple_scattering_routineTallyList, m.multiple_scattering_routineSDList) = m.InputRoutineTally("../output/dose_voxel_multiple-scattering-proton.txt")
    # (m.single_scattering_routineTallyList, m.single_scattering_routineSDList) = m.InputRoutineTally("../output/dose_voxel_single-scattering-proton.txt")
    # hardcoded
    # m.InputArcherTally("/home/kingcrimson/research/archer_build_debug/unit_test/physics/proton/water/result.txt")
    m.Compare()

