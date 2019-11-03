#include "kruskal.h"

Kruskal::Kruskal(vvi &dist) : d(dist)
{
	N = dist.size();
	for (size_t i = 1; i < N; ++i) // <-- Modifiquei o indice inicial para 1
	{							   //     dessa forma ele ignora o primeiro no
		for (size_t j = 1; j < N; ++j)
		{
			graph.push(make_pair(-dist[i][j], make_pair(i, j)));
		}
	}
}

void Kruskal::initDisjoint(int n)
{
	pset.resize(n);
	for (int i = 0; i < n; ++i)
	{
		pset[i] = i;
	}
}

int Kruskal::findSet(int i)
{
	return (pset[i] == i) ? i : (pset[i] = findSet(pset[i]));
}

void Kruskal::unionSet(int i, int j)
{
	pset[findSet(i)] = findSet(j);
}

bool Kruskal::isSameSet(int i, int j)
{
	return (findSet(i) == findSet(j)) ? true : false;
}

vii Kruskal::getEdges()
{
	return edges;
}

void Kruskal::mst_to_oneTree()
{
	// custos das arestas associadas ao vertice 0 d[0];
	// iterando pelos valores de d[0] e escolhendo os dois menores valores
	// (custo da menor insercao)
	int menor = 1;
	int proximo_menor = 2;

	if (d[0][proximo_menor] < d[0][menor])
	{
		proximo_menor = 1;
		menor = 2;
	}

	for (size_t i = 3; i < N; ++i)
	{
		if (d[0][i] < d[0][proximo_menor])
		{
			proximo_menor = i;
			if (d[0][proximo_menor] < d[0][menor])
			{
				proximo_menor = menor;
				menor = i;
			}
		}
	}

	std::pair<int, int> aresta1 = std::make_pair(0, menor);
	std::pair<int, int> aresta2 = std::make_pair(0, proximo_menor);

	// insercao
	edges.push_back(aresta1);
	edges.push_back(aresta2);

	// alterando o custo
	cost += d[0][menor] + d[0][proximo_menor];
}

double Kruskal::MST(int nodes)
{
	initDisjoint(nodes);

	cost = 0;

	while (!graph.empty())
	{
		pair<double, ii> p = graph.top();
		graph.pop();

		if (!isSameSet(p.second.first, p.second.second))
		{
			edges.push_back(make_pair(p.second.first, p.second.second));
			cost += (-p.first);
			unionSet(p.second.first, p.second.second);
		}
	}

	return cost;
}