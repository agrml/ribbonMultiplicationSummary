import itertools
import os

nCoresList = [16]
# nThreadsList = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
sizesList = [512, 1024, 1546, 2048, 2660, 3072]

# os.system("rm main *.out *.err")
# os.system("xlC_r -qsmp=omp -o main OpenMP.cpp")  # note: XlC_r does not exist

# for nCores, sz in itertools.product(nCoresList, sizesList):
#     for v in range(3):
fName = "test1"
os.system("ompsubmit -n " + "16" + " -w 15:00"
          " -stdout " + fName + ".out"
          " -stderr " + fName + ".err"
          " main -- " + str(512) + " " + str(16))
# llcancel -u edu-cmc-pod16-026
