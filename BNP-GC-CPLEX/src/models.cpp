#include "models.h"

Master::Master(int N_ITEMS, double CAPACIDADE_BIN, std::vector<double> PesoItens)
{
  this->N_ITEMS = N_ITEMS;
  this->CAPACIDADE_BIN = CAPACIDADE_BIN;
  this->PesoItens = PesoItens;
  this->criar_modelo();
  this->model.add(this->objective);
  this->cplex.extract(this->model);
}

// Utilizando formulacao set coverage
void Master::criar_modelo()
{
  //criar array de variaveis e coeficientes
  lambda = IloNumVarArray(this->env, N_ITEMS, 0.0, 1.0);
  Range1 = IloRangeArray(this->env);

  // Definindo a FO e as Retricoes
  const double BIGM = 10.0;
  IloExpr fo_sum(this->env);
  for (int i = 0; i < N_ITEMS; i++)
  {
    fo_sum += BIGM * lambda[i];
    Range1.add(lambda[i] == 1);
  }
  this->objective = IloMinimize(this->env, fo_sum);
  this->model.add(Range1);
}

void Master::adicionar_coluna(Subproblem *sub)
{
  const double custo = 1.0;
  IloNumColumn newCol = this->objective(custo);

  //Range1
  for (auto i = 0; i < N_ITEMS; i++)
  {
    newCol += Range1[i](sub->get_sub_val(i));
  }

  IloNumVar newVar(newCol, 0.0, IloInfinity);
  this->model.add(newVar);
  lambda.add(newVar);
}

void Master::imprimir_solucao()
{

  std::cout << "\nPadroes:" << std::endl;
  for (auto i = 0; i < lambda.getSize(); i++)
  {
    std::cout << "lambda[" << i << "] = " << this->cplex.getValue(lambda[i]) << std::endl;
  }
}

Subproblem::Subproblem(int N_ITEMS, double CAPACIDADE_BIN, std::vector<double> PesoItens)
{
  this->N_ITEMS = N_ITEMS;
  this->CAPACIDADE_BIN = CAPACIDADE_BIN;
  this->PesoItens = PesoItens;
  this->objective = IloMinimize(this->env);
  this->criar_modelo();
  this->model.add(this->objective);
  this->cplex.extract(this->model);
}

void Subproblem::criar_modelo()
{
  //define as variaveis do modelo
  Y = IloNumVarArray(this->env, N_ITEMS);
  for (auto i = 0; i < N_ITEMS; i++)
  {
    Y[i] = IloNumVar(this->env, 0, IloInfinity, ILOINT);
  }

  //define as restricoes
  IloExpr expr(this->env);
  for (auto i = 0; i < N_ITEMS; i++)
  {
    expr += this->PesoItens[i] * Y[i];
  }
  this->model.add(expr <= CAPACIDADE_BIN);
  // Se converter PesoItens para IloNumArray, da pra usar a seguinte sintaxe:
  // this->model.add(IloScalProd(PesoItens, Y) <= CAPACIDADE_BIN);
}

void Subproblem::update_objective(Master *mestre)
{
  //definindo os coeficientes das variaveis
  for (auto i = 0; i < N_ITEMS; i++)
  {
    double dual = mestre->cplex.getDual(mestre->Range1[i]);
    this->objective.setLinearCoef(Y[i], -dual); // -dual * Y[i] ...
  }
  this->objective.setConstant(1.0); // sum 1 - coef*pi ...
}

double Subproblem::get_sub_val(int i)
{
  return this->cplex.getValue(Y[i]);
}