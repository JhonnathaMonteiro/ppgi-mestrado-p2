#include <iostream>
#include <vector>
#include <pthread.h>
#include <cmath>
#include <ilcplex/ilocplex.h>

#include "models.h"

int N_ITEMS;
double CAPACIDADE_BIN;
std::vector<double> PesoItens;

void ler_dados(std::string arquivo)
{
    std::ifstream in(arquivo.c_str());

    in >> N_ITEMS;
    std::cout << "Numero de itens: " << N_ITEMS << std::endl;

    in >> CAPACIDADE_BIN;
    std::cout << "Capacidade da Bin: " << CAPACIDADE_BIN << std::endl;

    double aux;

    std::cout << "Pesos dos itens:" << std::endl;
    for (auto i = 0; i < N_ITEMS; i++)
    {
        in >> aux;
        PesoItens.push_back(aux);
        std::cout << " " << PesoItens[i];
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cout << "Argumento invalido." << std::endl;
        return 0;
    }

    ler_dados(argv[1]);

    std::cout << "\n== Geracao de colunas ==============\n"
              << std::endl;

    //criar o mestre
    Master mestre(N_ITEMS, CAPACIDADE_BIN, PesoItens);
    mestre.setStreamOff();
    // mestre.criar_colunas_artificiais();

    //criar o subproblema
    Subproblem sub(N_ITEMS, CAPACIDADE_BIN, PesoItens);
    sub.setStreamOff();

    //algoritmo de geracao de colunas
    const double EPS = 1.0e-5;
    double custoSub;
    int novaColuna = 0;
    int iter = 0;

    // Exportando o modelo inicial
    mestre.cplex.exportModel("model/modelo_incial.lp"); // exportando o modelo

    do
    {

        iter++;
        std::cout << "Iteracao " << iter << std::endl;

        mestre.solve();
        std::cout << std::setprecision(10) << "Mestre = " << mestre.getObjective() << std::endl;

        novaColuna = 0;

        sub.update_objective(&mestre);
        sub.solve();

        custoSub = std::min(sub.getObjective(), 0.0);

        if (custoSub < -EPS)
        {
            mestre.adicionar_coluna(&sub);
            novaColuna++;
            std::cout << std::setprecision(10) << "Suproblema = " << custoSub << std::endl;
        }

        std::cout << std::endl;
    } while (novaColuna > 0);

    mestre.cplex.exportModel("model/modelo_final.lp"); // exportando o modelo

    std::cout << "\n\n============ RESULTADO ==============" << std::endl;
    // mestre.imprimir_solucao(); // imprimir lambda
    double gc = mestre.getObjective();
    std::cout << std::setprecision(10) << "SOLUCAO (MESTRE) = " << mestre.getObjective() << std::endl;
    std::cout << std::setprecision(10) << "Quantidade de Bins = " << std::ceil(gc) << std::endl;

    return 0;
}
