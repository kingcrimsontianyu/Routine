#!/usr/bin/env python3

import numpy as np
import matplotlib
matplotlib.use('PDF')
import matplotlib.pyplot as plt

class Manager:
    #------------------------------------------------------------
    #------------------------------------------------------------
    def __init__(self, title):
        self.multiple_scattering_routineTallyList = []
        self.multiple_scattering_routineSDList    = []
        self.single_scattering_routineTallyList = []
        self.single_scattering_routineSDList    = []
        self.title = title
        self.numData = 1000
        self.depthList = []

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

        # convert to numpy array
        routineTallyList = np.array(routineTallyList)

        return (routineTallyList)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def Compare(self):
        fig = plt.figure(figsize=(12, 6))
        plt.suptitle(self.title, fontsize=16)
        ax = plt.subplot(1, 1, 1)

        my_color = '#ff0000'
        multiple_scattering_routineLine, = plt.semilogy(self.depthList, self.multiple_scattering_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)

        my_color = '#0000ff'
        single_scattering_routineLine, = plt.semilogy(self.depthList, self.single_scattering_routineTallyList, linestyle='-', color=my_color,  markerfacecolor='None', markeredgecolor=my_color, markeredgewidth=1, marker=',', markersize=8)

        ax.set_xlabel("Cosine of polar angle")
        ax.set_ylabel("Distribution")

        plt.legend([multiple_scattering_routineLine, single_scattering_routineLine],
        ["Routine (Geant4 10.3.2, EM multiple scattering)", "Routine (Geant4 10.3.2, EM single scattering)"],
        loc='upper left', shadow=True, fontsize=12)

        plt.grid(b=True, which='major')

        self.title = self.title.replace(' ', '_')
        plt.savefig(self.title + ".pdf", bbox_inches='tight')
        plt.show()

    #------------------------------------------------------------
    #------------------------------------------------------------
    def Process(self):
        angle = np.radians(10.0)
        step = (1.0 - np.cos(angle)) / self.numData
        start = np.cos(angle) + step / 2.0
        stop = 1.0
        self.depthList = np.arange(start, stop, step)
        print("--> num data of depthList = ", len(self.depthList))

        self.multiple_scattering_routineTallyList /= step
        area = np.sum(self.multiple_scattering_routineTallyList) * step
        self.multiple_scattering_routineTallyList /= area
        area = np.sum(self.multiple_scattering_routineTallyList) * step
        maxv = np.amax(self.multiple_scattering_routineTallyList)
        print("    area = ", area)
        print("    max = ", maxv)

        self.single_scattering_routineTallyList /= step
        area = np.sum(self.single_scattering_routineTallyList) * step
        self.single_scattering_routineTallyList /= area
        area = np.sum(self.single_scattering_routineTallyList) * step
        maxv = np.amax(self.single_scattering_routineTallyList)
        print("    area = ", area)
        print("    max = ", maxv)

#------------------------------------------------------------
#------------------------------------------------------------
if __name__ == "__main__":
    m = Manager("Proton 200 MeV in water scatter angular distribution (0-10 degree)")
    (m.multiple_scattering_routineTallyList) = m.InputRoutineTally("../output/multiple-scattering-proton_h1_scatter_polar_angle_cosine.csv")
    (m.single_scattering_routineTallyList) = m.InputRoutineTally("../output/single-scattering-proton_h1_scatter_polar_angle_cosine.csv")
    m.Process()
    m.Compare()





