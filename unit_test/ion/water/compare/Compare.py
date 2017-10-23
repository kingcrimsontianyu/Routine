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
        self.full_routineTallyList = []
        self.full_routineSDList    = []
        self.title                 = title

        # info
        print("--> numpy version: ", np.version.version)

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
    def Compare(self):
        fig = plt.figure(figsize=(12, 6))
        plt.suptitle(self.title, fontsize=16)
        ax = plt.subplot(1, 1, 1)

        depthList = np.arange(len(self.full_routineTallyList))
        depthList = depthList * self.dim_voxel[1] + self.dim_voxel[1] / 2.0
        # print(depthList)

        my_color = '#ff8000'
        full_routineLine, = plt.plot(depthList, self.full_routineTallyList, linestyle='-', color=my_color,  markerfacecolor=my_color, markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=5)
        plt.errorbar(depthList, self.full_routineTallyList, yerr=self.full_routineSDList, ecolor=my_color, elinewidth=0.8, linestyle='None')

        ax.set_xlabel("Depth [cm]")
        ax.set_ylabel("Absorbed dose [MeV/g]")

        plt.legend([full_routineLine],
        ["Routine (Geant4 10.3.2, QGSP_BIC_HP)"],
        loc='best', shadow=True, fontsize=12)
        plt.savefig(self.title + ".pdf", bbox_inches='tight')
        plt.show()

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager((1, 100, 1), (20, 0.2, 20), "Carbon 2400 MeV in water")
    (m.full_routineTallyList, m.full_routineSDList) = m.InputRoutineTally("../output/dose_voxel_qgsp_bic_hp.txt")
    m.Compare()

