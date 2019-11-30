#ifndef DATA_H
#define DATA_H

// TODO: Refatorar depois para ler todas as instancias do arquivo, atualmente
// esta lendo apenas a primeira instancia por conveniencia

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

template <class T>
class DataReader
{
private:
    std::string _problem_id;
    int _num_of_problems;
    int _num_of_itens;
    int _opt_solution;
    T _bin_cap;
    std::vector<T> _w;

public:
    DataReader(int, char *);
    int getCap() { return _bin_cap; }
    int getN() { return _num_of_itens; }
    int getOpt() { return _opt_solution; }

    std::string getID() { return _problem_id; }
    std::vector<T> getW() { return _w; }
};

template <class T>
DataReader<T>::DataReader(int argc, char *file_path)
{
    if (argc != 2)
    {
        std::cerr << "Wrong Input! (argc != 2)";
        exit(EXIT_FAILURE);
    }
    std::ifstream file(file_path);
    file >> _num_of_problems >> _problem_id;
    file >> _bin_cap >> _num_of_itens >> _opt_solution;
    this->_w.resize(_num_of_itens, 0);

    for (int i = 0; i < this->_num_of_itens; ++i)
    {
        file >> this->_w[i];
    }
}

#endif // DATA_H
