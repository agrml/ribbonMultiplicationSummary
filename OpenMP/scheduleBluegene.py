import itertools
import os

nThreadsList = [1, 2, 4, 8, 256, 512]
sizesList = [512, 1024, 1536, 2048, 2560]

# os.system("rm *.out *.err")
# os.system("rm main")
# os.system("mpixlcxx_r -qsmp=omp -o main OpenMP.cpp")

for nThreads in nThreadsList:
    for sz in sizesList:
        for v in range(3):
            fName = str(nThreads) + str(sz) + "_v" + str(v)
            if os.path.exists(fName + ".out") or os.path.exists(fName + ".err"):
                continue
            #  -e "OMP_NUM_THREADS=4
            os.system("mpisubmit.bg -n 1 -m smp"
                      " -w 15:00"
                      " -stdout " + fName + ".out"
                      " -stderr " + fName + ".err"
                      " main -- " + str(sz) + " " + str(nThreads))
# llcancel -u edu-cmc-pod16-026
