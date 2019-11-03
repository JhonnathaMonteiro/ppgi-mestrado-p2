#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <limits>
#include <algorithm> // std::fill
#include <iterator>  // std::distance
#include <math.h>	//pow

using namespace std;

typedef pair<int, int> ii;
typedef std::vector<std::vector<double>> vvi;
typedef vector<ii> vii;

class Kruskal
{
public:
	Kruskal(vvi &dist);

	double MST(int nodes);
	vii getEdges();
	void mst_to_oneTree();
	double cost;
	size_t N;

private:
	priority_queue<pair<double, ii>> graph;
	vector<int> pset;
	vii edges;
	vvi d;

	void initDisjoint(int n);
	int findSet(int i);
	void unionSet(int i, int j);
	bool isSameSet(int i, int j);
};

#endif