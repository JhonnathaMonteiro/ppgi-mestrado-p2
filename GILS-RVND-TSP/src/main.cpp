#include <readData.h>
#include <construction.h>
#include <gils_rvnd.h>
#include <fstream>
#include <iostream>

using namespace std;

double **matrizAdj; // matriz de adjacencia
int dimension;      // quantidade total de vertices

int main(int argc, char **argv)
{

    // Ler instancia
    readData(argc, argv, &dimension, &matrizAdj);

    int Imax = 50;
    int Iils;
    if (dimension >= 150)
    {
        Iils = dimension / 2;
    }
    else
    {
        Iils = dimension;
    }

    std::cout << " ---SOLUCAO--- " << std::endl;
    std::cout << "Instancia: " << argv[1] << endl;

    std::vector<int> tour = gils_rvnd(Imax, Iils, dimension, matrizAdj);

    double distancia = 0;
    const int tourSize = tour.size() - 1;
    for (int i = 0, j = 1; i < tourSize; i++, j++)
    {
        distancia += matrizAdj[tour[i]][tour[j]];
    }
    std::cout << " Distancia -> " << distancia << std::endl;
}
