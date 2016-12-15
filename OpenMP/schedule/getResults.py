import itertools
import os

nThreadsList = [1, 2, 4, 8, 256, 512]
sizesList = [512, 1024, 1536, 2048, 2560]

for nThreads in nThreadsList:
    for sz in sizesList:
        for v in range(3):
            fName = str(nThreads) + "_" + str(sz) + "_v" + str(v)
            if os.path.exists(fName + ".out") or os.path.exists(fName + ".err"):
                continue
            #  -e "OMP_NUM_THREADS=4
            os.system("../cmake-build-debug/src/project " + str(sz) + " " + str(nThreads) + " > " + fName + ".out")
# llcancel -u edu-cmc-pod16-026
