#include <ilcplex/ilocplex.h>
#include "instance.hpp"
#pragma once

class Statique{
public:
  int val;
  vector<vector<int> > production;
  vector<int> stock;
  
  void solve(Instance Inst);
};
