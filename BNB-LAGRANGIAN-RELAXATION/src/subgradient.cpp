#include <iostream>
#include <vector>

/**
 * Metodo do subgradiente para relaxacao lagrangiana do problema 
 * TSP simetrico.
 * 
 * 
 * @param c matriz de custo
 * @param UB upper bound
 * @param u vetor com os multiplicadores lagrangeanos
 * @param e_k paramentro de controle do passo (default = 2)
 * @param min_e_k criterio de parada (menor valor de e_k permitido)
 * @return data com melhor subgradiente/solucao_otima vetor gradiente
*/
void subgrad_method(std::vector<double> u)
{
    // Matriz de custo (teste)
    double c[5][5] = {{0, 30, 26, 50, 40},
                      {30, 0, 24, 40, 50},
                      {26, 24, 0, 24, 26},
                      {50, 40, 24, 0, 30},
                      {40, 50, 26, 30, 0}};

    // Transformar a matriz de custo em matriz de custo lagrangeano
    int N = sizeof c / sizeof c[0]; // tamanho

    // matriz de custo lagrangeano
    double _c[N][N];

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            _c[i][j] = c[i][j] - u[i] - u[j];
            _c[j][i] = _c[i][j];
        }
    }

    // Rodar o Kruskal para gerar a 1-tree e retornala com o custo

    // Criar uma funcao para verificar as arestas violadas e gerar o subgradiente

    // Varificar solucao otima

    // Atualizar os multiplicadores e chamar subgrad_method

    std::cout << N << std::endl;
}

int main()
{
    // TODO: criar o vetor u baseado no tamanho da matriz c e preencher ele com
    // 0s
    std::vector<double> u = {0, 0, 0, 0, 0};
    subgrad_method(u);
}