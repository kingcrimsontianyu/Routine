#!/usr/bin/env python3
# The 1D flat binary data must be such that, when interpreted as 3D,
# its x-index changes the fastest.

# example
# sys.path.append(path_to_PlotBinary_module)
# import PlotBinary as plb
#
# figX = p.CreateColorPlot(((17, 20), (6, 8), (5, 6)), "x")
# figX.savefig("tally_color_x.pdf", bbox_inches = 'tight')
#
# figY = p.CreateColorPlot(((17, 20), (6, 8), (5, 6)), "y")
# figY.savefig("tally_color_y.pdf", bbox_inches = 'tight')
#
# figZ = p.CreateColorPlot(((17, 20), (6, 8), (5, 6)), "z")
# figZ.savefig("tally_color_z.pdf", bbox_inches = 'tight')
#
# # contour plot
# figX = p.CreateContourPlot(((17, 20), (6, 8), (5, 6)), "x")
# figX.savefig("tally_contour_x.pdf", bbox_inches = 'tight')
#
# figY = p.CreateContourPlot(((17, 20), (6, 8), (5, 6)), "y")
# figY.savefig("tally_contour_y.pdf", bbox_inches = 'tight')
#
# figZ = p.CreateContourPlot(((17, 20), (6, 8), (5, 6)), "z")
# figZ.savefig("tally_contour_z.pdf", bbox_inches = 'tight')

import numpy as np
import matplotlib
matplotlib.use('PDF') # non-interactive plot
import matplotlib.pyplot as plt

class Manager:
    #------------------------------------------------------------
    #------------------------------------------------------------
    def __init__(self,
                 input_path,
                 num_voxel_x,
                 num_voxel_y,
                 num_voxel_z):

        self.input_path           = input_path
        self.num_voxel_x          = num_voxel_x
        self.num_voxel_y          = num_voxel_y
        self.num_voxel_z          = num_voxel_z

        self.totalNumVoxel        = self.num_voxel_x * self.num_voxel_y * self.num_voxel_z
        self.dataList             = []
        self.data3DList           = []

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InputBinaryData(self):
        print("--> Input binary data")
        self.dataList = np.fromfile(self.input_path, dtype = float, count = -1, sep = '')

        # 1D to 3D, x-index changes the fastest
        self.data3DList = np.reshape(self.dataList, (self.num_voxel_x, self.num_voxel_y, self.num_voxel_z), order='F')

        print("    num of elements = ", len(self.dataList))

    #------------------------------------------------------------
    # plotSize is a tuple ((xStart, xEnd), (yStart, yEnd), (zStart, zEnd))
    # plotType is one of "x", "y", "z", meaning the plot is perpendicular to x, y, or z axis
    #
    # default axis orientation of the original images
    # x-y: x down, y right
    # y-z: y down, z right
    # x-z: x down, z right
    # if rot90() is used, the vertical axis should be flipped.
    # to simplify matters, just use origin='lower' in imshow().
    # now the axis orientation becomes
    # x-y: x up, y right
    # y-z: y up, z right
    # x-z: x up, z right
    #------------------------------------------------------------
    def InternalPlot(self, plotSize, plotType):
        # determine plot data size
        plotData = []
        hLabel = ""
        vLabel = ""
        if plotType == "x":
            temp = self.data3DList[plotSize[0][0] : plotSize[0][1] + 1, :, :]
            plotData = np.average(temp, axis = 0)
            hLabel = "z"
            vLabel = "y"
        elif plotType == "y":
            temp = self.data3DList[:, plotSize[1][0] : plotSize[1][1] + 1, :]
            plotData = np.average(temp, axis = 1)
            hLabel = "z"
            vLabel = "x"
        elif plotType == "z":
            temp = self.data3DList[:, :, plotSize[2][0] : plotSize[2][1] + 1]
            plotData = np.average(temp, axis = 2)
            hLabel = "y"
            vLabel = "x"
        else:
            sys.exit("--> Error: given plot type is not supported.")

        print("  2D data size = ", plotData.shape)

        fig = plt.figure(figsize = (5, 5))
        ax = fig.add_subplot(1, 1, 1)
        ax.set_xlabel(hLabel)
        ax.set_ylabel(vLabel)
        return (fig, ax, plotData)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def CreateColorPlot(self, plotSize, plotType):
        print("--> Create color plot")
        (fig, ax, plotData) = self.InternalPlot(plotSize, plotType)
        ax.imshow(plotData, interpolation = 'none', origin = 'lower')
        ax.set_aspect('equal')
        return fig

    #------------------------------------------------------------
    #------------------------------------------------------------
    def CreateContourPlot(self, plotSize, plotType):
        print("--> Create contour plot")
        (fig, ax, plotData) = self.InternalPlot(plotSize, plotType)
        ax.contour(plotData, origin = 'lower')
        ax.set_aspect('equal')
        return fig
