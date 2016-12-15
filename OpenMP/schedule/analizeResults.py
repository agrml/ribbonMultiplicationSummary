#! /usr/bin/python2

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import itertools
import os


def do(path, title, xlabel, nCoresList, sizesList):
    fig = plt.figure()
    plt.title(title)
    ax = fig.add_subplot(111, projection='3d')
    xPos = []
    ax.set_ylabel('Matrix size')

    yPos = []
    ax.set_xlabel(xlabel)
    zPos = []
    ax.set_zlabel('Time, min')
    dz = []
    sheet = []

    tab = "\t\t\t\t\t"

    for nCores, sz in itertools.product(nCoresList, sizesList):
        time = 0
        count = 0
        for v in range(3):
            fName = path + str(nCores) + "_" + str(sz) + "_v" + str(v) + ".out"  # +"_on_1"
            try:
                f = open(fName)
                tmp = f.readline()
                if not len(tmp):
                    continue
                tmp = tmp.split('\t')[0]
                time += float(tmp)
                count += 1
            except IOError as err:
                print(err, err)

        if count:
            xPos.append(sz)
            yPos.append(nCores)
            zPos.append(0)
            time = time / count / 60
            dz.append(time)
            sheet.append(time)
        else:
            sheet.append("TL")

    dx = [3 for i in dz]
    dy = [100 for i in dz]

    print("\t\t\t\t\t\t\t\t\t\t***", title, "***")
    print(xlabel + " \ size", *sizesList, sep="\t" + tab)

    it = iter(sheet)
    for i in nCoresList:
        print(i, end="\t" + tab)
        for j in sizesList:
            f = next(it)
            try:
                f = float(f)
                print("%.10f" % f, end=tab)
            except:
                print(f, end="            " + tab)
        print()

    ax.bar3d(yPos, xPos, zPos, dx, dy, dz, color='#00ceaa')
    plt.show()


nThreadsList = [1, 2, 4, 8, 256, 512]
sizesList = [512, 1024, 1536, 2048, 2560]
do("./", "OpenMP on laptop", "# cores", nThreadsList, sizesList)



# print("Matrix size / CPUs number", *nCoresList, sep="\t\t\t")
# [print(i, end="\t\t\t") for i in dz [print() for i, j in dz] ]

