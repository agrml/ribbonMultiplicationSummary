#include <iostream>
#include <assert.h>
#include <vector>
#include <iomanip>
#include <mpi.h>
#include <cmath>
#include <sstream>
#include <limits.h>
#include "cstdlib"

void ribbonMultBasic(int *a, int *b, int *c, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            int *pElem = &(c[i * n + j]);
            *pElem = 0;
            for (size_t k = 0; k < n; k++) {
                *pElem += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

void ribbonMultMpi(int *a, int *b, int *c, size_t nRows, size_t startIdx, size_t step)
{
    size_t i, j, k;
    int sum;
    for (i = 0; i < nRows; i++) {
        for (j = startIdx; j < nRows && j < startIdx + step; j++) {
            sum = 0;
            for (k = 0; k < nRows; k++) {
                sum += a[i * nRows + k] * b[k * nRows + j];
            }
            c[i * nRows + j /*- startIdx*/] = sum;
        }
    }
}

//////////////////////////////////////

void printMatrix(int *c, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            printf("%d\t", c[i * n + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void initMatrix(int *c, size_t n, bool isRand=0)
{
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            c[i * n + j] = isRand? rand() : 0;
        }
    }
}


bool check(std::vector<int *> matrices, size_t n)
{
    for (size_t k = 1; k < matrices.size(); k++) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (matrices[k][i * n + j] != matrices[0][i * n + j]) {
                    return false;
                }
            }
        }
    }
    return true;
}


int main(int argc, char *argv[])
{
    char buf[PATH_MAX];
    setbuf(stdout, buf);

    int err;
    if ((err = MPI_Init(&argc, &argv)))  {
        std::cerr << "MPI startup error!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    int nProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t nRows;
    sscanf(argv[1], "%lu", &nRows);

    int *a = new int[nRows * nRows];
    int *b = new int[nRows * nRows];
//    int *cBasic = new int[nRows * nRows];
    if (!rank){
        initMatrix(a, nRows, 1);
        initMatrix(b, nRows, 1);
    }
    MPI_Bcast(a, nRows * nRows, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, nRows * nRows, MPI_INT, 0, MPI_COMM_WORLD);

//    initMatrix(cBasic, nRows);

    double timerBasic, timerMpi;

//    if (!rank) {
//        timerBasic = MPI_Wtime();
//        ribbonMultBasic(a, b, cBasic, nRows);
//        timerBasic = MPI_Wtime() - timerBasic;
//    }

    size_t startIdx = nRows / nProcs * rank + std::min<size_t>(rank, nRows % nProcs);
    size_t step = nRows / nProcs + (rank < nRows % nProcs);
    const size_t cSz = nRows */* step*/ nRows;
    int *cMpi = new int[cSz];
    initMatrix(cMpi, nRows);
    int *cMpiRes = new int[cSz];
    initMatrix(cMpiRes, nRows);

    MPI_Barrier(MPI_COMM_WORLD);
    timerMpi = MPI_Wtime() - timerMpi;
    ribbonMultMpi(a, b, cMpi, nRows, startIdx, step);
    MPI_Barrier(MPI_COMM_WORLD);
    timerMpi = MPI_Wtime() - timerMpi;

    // send c rows to #0
    MPI_Reduce(cMpi, cMpiRes, cSz, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (!rank) {
//        std::vector<int *> tmp;
//        tmp.push_back(cBasic);
//        tmp.push_back(cMpiRes);
//        printf("\nCheck done: %d", check(tmp, nRows));
//        printf("\nTime consumption:\n");
//        printf("Basic:\t %lfs\n", timerBasic);
//        printf("MPI:\t %lfs\t\t # process: %d\n", timerMpi, nProcs);
        std::cout << timerMpi << "\t\t" << "// MPI. nRows: " << nRows << std::endl;
    }

    delete[] a;
    delete[] b;
//    delete[] cBasic;
    delete[] cMpi;
    delete[] cMpiRes;

    MPI_Finalize();
    return 0;
}
