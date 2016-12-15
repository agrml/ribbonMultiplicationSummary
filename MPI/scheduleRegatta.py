import itertools
import os

nCpusList = [1, 2, 4, 8, 16]
sizesList = [1024, 1546, 2048, 2660, 3072]

# os.system("rm *.out *.err")
# os.system("rm main")
# os.system("mpiCC -o main main.cpp")

for nCpus in nCpusList:
    for sz in sizesList:
        for v in range(3):
            fName = str(nCpus) + "_" + str(sz) + "_v" + str(v)
            if os.path.exists(fName + ".out") or os.path.exists(fName + ".err"):
                continue
            os.system("mpisubmit -n " + str(nCpus) +
                      " -w 15:00"
                      " -stdout " + fName + ".out"
                      " -stderr " + fName + ".err"
                      " main -- " + str(sz))
# llcancel -u edu-cmc-pod16-026
