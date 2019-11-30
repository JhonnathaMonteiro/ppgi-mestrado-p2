#include <iostream>

#include "data_reader.h"
#include "bnp_gc.h"

int main(int argc, char **argv)
{

    // Leitura da instancia para o bnp
    DataReader<int> *data = new DataReader<int>(argc, argv[1]);
    std::vector<int> w = data->getW();

    for (auto &i : w)
    {
        std::cout << i << std::endl;
    }

    return 0;
}
