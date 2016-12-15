import itertools
import os

nCpusList = [1, 2, 4, 8, 16, 32, 64, 128, 256]  # 512, 1024
sizesList = [512, 1024, 1546, 2048, 2660, 3072]

# os.system("rm *.out *.err")
# os.system("rm main")
# os.system("mpixlcxx_r -o main main.cpp")

for nCpus in nCpusList:
    for sz in sizesList:
        for v in range(3):
            fName = str(nCpus) + "_" + str(sz) + "_v" + str(v)
            if os.path.exists(fName + ".out") or os.path.exists(fName + ".err"):
                continue
            os.system("mpisubmit.bg -n " + str(nCpus) +
                      " -w 10:00"
                      " -stdout " + fName + ".out"
                      " -stderr " + fName + ".err"
                      " main -- " + str(sz))
# llcancel -u edu-cmc-pod16-026
