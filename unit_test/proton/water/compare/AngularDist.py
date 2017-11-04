#!/usr/bin/env python3

import numpy as np
import matplotlib
matplotlib.use('PDF')
import matplotlib.pyplot as plt

class Manager:
    #------------------------------------------------------------
    #------------------------------------------------------------
    def __init__(self, title):
        self.mini_routineTallyList = []
        self.mini_routineSDList    = []
        self.single_scattering_routineTallyList = []
        self.single_scattering_routineSDList    = []
        self.title = title
        self.numData = 1000

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputRoutineTally(self, input_path):
        print("--> Input Routine result")
        count = 0
        routineTallyList = []
        with open(input_path, 'r') as file:
            for line in file:
                count += 1
                if count < 9 or count > 1008:
                    continue
                else:
                    lineString = line.split(',')
                    routineTallyList.append(float(lineString[0]))
        print("--> num data of routineTallyList = ", len(routineTallyList))

        return (routineTallyList)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def Compare(self):
        fig = plt.figure(figsize=(12, 6))
        plt.suptitle(self.title, fontsize=16)
        ax = plt.subplot(1, 1, 1)

        angle = np.radians(10.0)
        step = (1.0 - np.cos(angle)) / self.numData
        start = np.cos(angle) + step / 2.0
        stop = 1.0
        depthList = np.arange(start, stop, step)
        print("--> num data of depthList = ", len(depthList))

        my_color = '#0000ff'
        single_scattering_routineLine, = plt.semilogy(depthList, self.single_scattering_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)

        ax.set_xlabel("Cosine of polar angle")
        ax.set_ylabel("Distribution")

        plt.legend([single_scattering_routineLine],
        ["Routine (Geant4 10.3.2, EM single scattering)"],
        loc='best', shadow=True, fontsize=12)

        self.title = self.title.replace(' ', '_')
        plt.savefig(self.title + ".pdf", bbox_inches='tight')
        plt.show()

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager("Proton 200 MeV in water scatter angular distribution (0-10 degree)")
    # (m.mini_routineTallyList) = m.InputRoutineTally("../output/dose_voxel_mini-proton.txt")
    (m.single_scattering_routineTallyList) = m.InputRoutineTally("../output/single-scattering-proton_h1_single_scatter_polar_angle_cosine.csv")
    m.Compare()



