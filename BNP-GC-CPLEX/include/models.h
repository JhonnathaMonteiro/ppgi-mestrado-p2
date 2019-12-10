#ifndef MODELS_H_
#define MODELS_H_

#include <iostream>
#include <vector>
#include <pthread.h>
#include <ilcplex/ilocplex.h>

class Master;
class Subproblem;

// classe base
class Modelo
{
public:
  IloEnv env;
  IloModel model;
  IloCplex cplex;
  IloObjective objective;

  Modelo()
  {
    env = IloEnv();
    model = IloModel(env);
    cplex = IloCplex(env);
  }

  ~Modelo()
  {
    model.end();
    cplex.end();
    env.end();
  }

  //metodos
  void setStream(std::ostream &st) { cplex.setOut(st); }
  void setStreamOff() { cplex.setOut(env.getNullStream()); }
  IloNum getObjective() { return cplex.getObjValue(); }
  IloCplex::CplexStatus getStatus() { return this->cplex.getCplexStatus(); }
  void solve() { cplex.solve(); }
};

// Modelo do Mestre
class Master : public Modelo
{
public:
  IloNumVarArray lambda; //variaveis lambda
  IloNumArray Coef;      //coeficientes de lambda
  IloRangeArray Range1;

  int N_ITEMS;
  double CAPACIDADE_BIN;
  std::vector<double> PesoItens;

  Master(int N_ITEMS, double CAPACIDADE_BIN, std::vector<double> PesoItens);

  void criar_modelo();
  void adicionar_coluna(Subproblem *sub);
  void imprimir_solucao();
};

//Modelo do Subproblema
class Subproblem : public Modelo
{
public:
  IloNumVarArray Y;

  int N_ITEMS;
  double CAPACIDADE_BIN;

  std::vector<double> PesoItens;
  Subproblem(int N_ITEMS, double CAPACIDADE_BIN, std::vector<double> PesoItens);

  //metodos
  void criar_modelo();
  void update_objective(Master *mestre);
  double get_sub_val(int i);
};

#endif // MODELS_H_