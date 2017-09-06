#!/usr/bin/env python3
# The 1D flat binary data must be such that, when interpreted as 3D,
# its x-index changes the fastest.

# usage
#
# sys.path.append(path_to_PlotBinary_module)
# import PlotBinary as plb
#
# p = plb.Manager("tally.bin",
#               36,
#               12,
#               10)
# p.InputBinaryData()
#
# # single color plot
# # ((17, 20), (6, 8), (5, 6)) means: for the 3-D binary data array,
# # to generate a 2-D plot perpendicular to x axis, average over the slices whose x index is from 17 to 20 (inclusive)
# # to generate a 2-D plot perpendicular to y axis, average over the slices whose y index is from 6 to 8 (inclusive)
# # to generate a 2-D plot perpendicular to z axis, average over the slices whose z index is from 5 to 6 (inclusive)
# figX = p.CreateSingleColorPlot(((17, 20), (6, 8), (5, 6)), "x")
# figX.savefig("tally_color_x.pdf", bbox_inches = 'tight')
#
# figY = p.CreateSingleColorPlot(((17, 20), (6, 8), (5, 6)), "y")
# figY.savefig("tally_color_y.pdf", bbox_inches = 'tight')
#
# figZ = p.CreateSingleColorPlot(((17, 20), (6, 8), (5, 6)), "z")
# figZ.savefig("tally_color_z.pdf", bbox_inches = 'tight')
#
#
# # single contour plot
# figX = p.CreateSingleContourPlot(((17, 20), (6, 8), (5, 6)), "x")
# figX.savefig("tally_contour_x.pdf", bbox_inches = 'tight')
#
# figY = p.CreateSingleContourPlot(((17, 20), (6, 8), (5, 6)), "y")
# figY.savefig("tally_contour_y.pdf", bbox_inches = 'tight')
#
# figZ = p.CreateSingleContourPlot(((17, 20), (6, 8), (5, 6)), "z")
# figZ.savefig("tally_contour_z.pdf", bbox_inches = 'tight')
#
#
# # xyz color plot
# figXYZ = p.CreateXYZColorPlot(((17, 20), (6, 8), (5, 6)))
# figXYZ.savefig("tally_color_xyz.pdf", bbox_inches = 'tight')
#
#
# # xyz contour plot
# figXYZ = p.CreateXYZContourPlot(((17, 20), (6, 8), (5, 6)))
# figXYZ.savefig("tally_contour_xyz.pdf", bbox_inches = 'tight')





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
    def InternalPlotPre(self, plotSize, plotType, fig, ax):
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

        ax.set_xlabel(hLabel)
        ax.set_ylabel(vLabel)
        return (fig, ax, plotData)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def InternalPlotPost(self, mappable, fig, ax, plotData):
        ax.set_aspect('equal')
        cax = fig.add_axes([0.9, 0.1, 0.02, 0.8]) # left, bottom, width, height
        fig.colorbar(mappable, cax = cax, format='%.2e')
        return (fig, ax)

    #------------------------------------------------------------
    #------------------------------------------------------------
    def CreateSingleColorPlot(self, plotSize, plotType):
        print("--> Create single color plot")
        fig = plt.figure(figsize = (5, 5))
        ax = fig.add_subplot(1, 1, 1)
        (fig, ax, plotData) = self.InternalPlotPre(plotSize, plotType, fig, ax)
        mappable = ax.imshow(plotData, interpolation = 'none', origin = 'lower')
        (fig, ax) = self.InternalPlotPost(mappable, fig, ax, plotData)
        return fig

    #------------------------------------------------------------
    #------------------------------------------------------------
    def CreateSingleContourPlot(self, plotSize, plotType):
        print("--> Create single contour plot")
        fig = plt.figure(figsize = (5, 5))
        ax = fig.add_subplot(1, 1, 1)
        (fig, ax, plotData) = self.InternalPlotPre(plotSize, plotType, fig, ax)
        mappable = ax.contour(plotData, origin = 'lower')
        (fig, ax) = self.InternalPlotPost(mappable, fig, ax, plotData)
        return fig






    #------------------------------------------------------------
    #------------------------------------------------------------
    def InternalPlotPostXYZ(self, fig,
                                  mappable1, mappable2, mappable3,
                                  ax1, ax2, ax3):
        ax1.set_aspect('equal')
        ax2.set_aspect('equal')
        ax3.set_aspect('equal')
        cax = fig.add_axes([0.9, 0.1, 0.02, 0.8]) # left, bottom, width, height
        fig.colorbar(mappable3, cax = cax, format='%.2e')
        return fig

    #------------------------------------------------------------
    #------------------------------------------------------------
    def CreateXYZColorPlot(self, plotSize):
        print("--> Create xyz color plot")
        fig = plt.figure(figsize = (10, 5))

        ax1 = fig.add_subplot(1, 3, 1)
        (fig, ax1, plotData1) = self.InternalPlotPre(plotSize, "x", fig, ax1)
        ax2 = fig.add_subplot(1, 3, 2)
        (fig, ax2, plotData2) = self.InternalPlotPre(plotSize, "y", fig, ax2)
        ax3 = fig.add_subplot(1, 3, 3)
        (fig, ax3, plotData3) = self.InternalPlotPre(plotSize, "z", fig, ax3)

        maxV = np.amax([np.amax(plotData1),
                        np.amax(plotData2),
                        np.amax(plotData3)])
        minV = np.amin([np.amin(plotData1),
                        np.amin(plotData2),
                        np.amin(plotData3)])

        mappable1 = ax1.imshow(plotData1, interpolation = 'none', origin = 'lower', vmin = minV, vmax = maxV)
        mappable2 = ax2.imshow(plotData2, interpolation = 'none', origin = 'lower', vmin = minV, vmax = maxV)
        mappable3 = ax3.imshow(plotData3, interpolation = 'none', origin = 'lower', vmin = minV, vmax = maxV)

        fig = self.InternalPlotPostXYZ(fig,
                                       mappable1, mappable2, mappable3,
                                       ax1, ax2, ax3)
        return fig

    #------------------------------------------------------------
    #------------------------------------------------------------
    def CreateXYZContourPlot(self, plotSize):
        print("--> Create xyz color plot")
        fig = plt.figure(figsize = (10, 5))

        ax1 = fig.add_subplot(1, 3, 1)
        (fig, ax1, plotData1) = self.InternalPlotPre(plotSize, "x", fig, ax1)
        ax2 = fig.add_subplot(1, 3, 2)
        (fig, ax2, plotData2) = self.InternalPlotPre(plotSize, "y", fig, ax2)
        ax3 = fig.add_subplot(1, 3, 3)
        (fig, ax3, plotData3) = self.InternalPlotPre(plotSize, "z", fig, ax3)

        maxV = np.amax([np.amax(plotData1),
                        np.amax(plotData2),
                        np.amax(plotData3)])
        minV = np.amin([np.amin(plotData1),
                        np.amin(plotData2),
                        np.amin(plotData3)])

        mappable1 = ax1.contour(plotData1, origin = 'lower', vmin = minV, vmax = maxV)
        mappable2 = ax2.contour(plotData2, origin = 'lower', vmin = minV, vmax = maxV)
        mappable3 = ax3.contour(plotData3, origin = 'lower', vmin = minV, vmax = maxV)

        fig = self.InternalPlotPostXYZ(fig,
                                       mappable1, mappable2, mappable3,
                                       ax1, ax2, ax3)
        return fig



