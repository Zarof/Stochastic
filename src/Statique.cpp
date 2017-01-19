#include "Statique.hpp"

void Statique::solve(Instance Inst){  
  vector<int> d(Inst.NT);
  int i,s,t,max;
  
  for(i=0; i<Inst.Td; i++)
    d[i] = Inst.demandes_det[i];

  /*for(s=0; s<Inst.s; s++){
    for(int t=0; t<Inst.NT-Inst.Td; t++)
      cout << Inst.demandes[s][t] << " " ;
    cout << endl;
  }*/
  
  for(t=Inst.Td; t<Inst.NT; t++){
    max = 0;
    for(s=0; s<Inst.s; s++)
      if(Inst.demandes[s][t-Inst.Td] > max)
	max = Inst.demandes[s][t-Inst.Td];
    d[t] = max;
  }

  //  for(i=0; i<Inst.NT; i++)
  //cout<<"pire demande en " << i << " : " << d[i]<<endl;
  
  
  try{
    IloEnv env;
    IloModel model(env);
    IloArray<IloIntVarArray> x(env,Inst.I);
    for(i=0; i<Inst.I; i++){
      //x[i]=IloIntVarArray(env, Inst.NT,0,IloInfinity);
      x[i]=IloIntVarArray(env, Inst.NT);
      for(t=0; t<Inst.NT; t++){
	stringstream var;
	var<<"x_"<<i<<t;
	x[i][t]=IloIntVar(env,0,IloInfinity,var.str().c_str());
      }
    }
      
    // IloIntVarArray s(env,Inst.NT,0,IloInfinity);
    IloIntVarArray s(env, Inst.NT);
    for(t=0; t<Inst.NT; t++){
      stringstream var;
      var<<"s_"<<t;
      s[t]=IloIntVar(env,0,IloInfinity,var.str().c_str());
    }

    for(i=0; i<Inst.I; i++)
      for(t=0; t<Inst.NT; t++)
	model.add(x[i][t] <= Inst.capacite_usine[i]);

    IloExpr sum0(env);
    for(i=0; i<Inst.I; i++)
      sum0 += x[i][0];
    model.add(sum0 == d[0] + s[0]);
    sum0.end();
    for(t=1; t<Inst.NT; t++){
      IloExpr sum(env);
      for(i=0; i<Inst.I; i++)
	sum += x[i][t];
      model.add(s[t-1] + sum == d[t] + s[t]);
      sum.end();
    }

    IloExpr Obj(env);
    for(i=0; i<Inst.I; i++)
      for(t=0; t<Inst.NT; t++)
	Obj += x[i][t] * Inst.p[i][t];
    for(t=0; t<Inst.NT; t++)
      Obj += s[t] * Inst.h;

    model.add(IloMinimize(env,Obj));
    Obj.end();

    IloCplex cplex(model);
    cplex.exportModel("robuste_statique.lp");
    cplex.setOut(env.getNullStream());
    cplex.setWarning(env.getNullStream());
    cplex.solve();

    val = cplex.getObjValue();
    
    for(int i=0; i<Inst.I; i++){
      vector<int> v;
      production.push_back(v);
      for(int t=0; t<Inst.NT; t++){
	production[i].push_back(cplex.getValue(x[i][t]));
	
      }
    }

    for(int t=0; t<Inst.NT; t++)
      stock.push_back(cplex.getValue(s[t]));

    env.end();
  } catch(IloException& e) {
    std::cout << e.getMessage() << std::endl;
    cout<<"The pb could not be solved"<<endl;
  }
  return ;
}
