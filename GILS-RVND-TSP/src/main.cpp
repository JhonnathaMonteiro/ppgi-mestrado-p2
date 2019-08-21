#include "readData.h"
#include "construction.h"
#include "gils_rvnd.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

double **matrizAdj; // matriz de adjacencia
int dimension;      // quantidade total de vertices

int main()
{

    // filepath para instancia teste gr17
    const char *filepath = "../instances/gr17.tsp";
    // const char *filepath = "../instances/pr226.tsp";

    // Ler instancia
    readData(filepath, &dimension, &matrizAdj);

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

    std::vector<int> tour = gils_rvnd(Imax, Iils, dimension, matrizAdj);

    // Saida de dados --------------------------------

    std::cout << " ---SOLUCAO--- " << std::endl;
    std::cout << "Instancia: " << filepath << endl;

    // for (auto &s : tour)
    // {
    //     std::cout << s << " ";
    // }
    double distancia = 0;
    for (int i = 0, j = 1; i < tour.size() - 1; i++, j++)
    {
        distancia += matrizAdj[tour[i]][tour[j]];
    }

    std::cout << "Distancia -> " << distancia;
    std::cout << std::endl;
}
