#include <iostream>
#include <assert.h>
#include <vector>
#include <iomanip>
#include <omp.h>
#include <sstream>
#include <cstdlib>
#include <cstdio>


void ribbonMultBasic(int *a, int *b, int *c, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int *pElem = &(c[i * n + j]);
            *pElem = 0;
            for (int k = 0; k < n; k++) {
                *pElem += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

void ribbonMultOpenMp(int *a, int *b, int *c, int n, int nThreads)
{
    int i, j, k;
    int sum;
    #pragma omp parallel for private(k,j,sum) num_threads(nThreads)
    for (i = 0; i < n; i++) {
        if (omp_get_num_threads() != int(nThreads)) {
            std::cerr << omp_get_num_threads() << std::endl;
        }
        for (j = 0; j < n; j++) {
            sum = 0;
            for (k = 0; k < n; k++) {
                sum += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = sum;
        }
    }
}

void printMatrix(int *c, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << c[i * n + j] << "\t";
        }
        std::cout << std::endl;
    }
}

bool check(std::vector<int *> matrices, int n)
{
    for (int k = 1; k < matrices.size(); k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
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
    int nRows, nThreads;
    sscanf(argv[1], "%d", &nRows);
    sscanf(argv[2], "%d", &nThreads);

    int *a = new int[nRows * nRows];
    int *b = new int[nRows * nRows];
    int *cBasic = new int[nRows * nRows];
    int *cOpenMp = new int[nRows * nRows];

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nRows; j++) {
            a[i * nRows + j] = rand();
            b[i * nRows + j] = rand();
            cBasic[i * nRows + j] = 0;
            cOpenMp[i * nRows + j] = 0;
        }
    }
//    std::cout << "Basic: iter..." << std::flush;
//    double timerBasic = omp_get_wtime();
//    ribbonMultBasic(a, b, cBasic, nRows);
//    timerBasic = omp_get_wtime() - timerBasic;
//    std::cout << " ok" << std::endl;

//    std::cout << "OpenMP: iter..."  << std::flush;
    double timerOpenMp = omp_get_wtime();
    ribbonMultOpenMp(a, b, cOpenMp, nRows, nThreads);
    timerOpenMp = omp_get_wtime() - timerOpenMp;
//    std::cout << " ok" << std::endl;

    std::cout << std::fixed << std::setprecision(10);
//    std::vector<int *> tmp;
//    tmp.push_back(cBasic);
//    tmp.push_back(cOpenMp);
//    std::cout << "\nCheck done: " << check(tmp, nRows) << std::endl;

//    std::cout << "\nTime consumption:\n";
//    std::cout << "Basic:\t" << timerBasic << "s\n";
    std::cout << timerOpenMp << "\t\t" << "// OpenMP. nTreads: " << nThreads << " nRows: " << nRows << std::endl;

    delete[] a;
    delete[] b;
    delete[] cBasic;
    delete[] cOpenMp;
    return 0;
}