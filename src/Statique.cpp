#include "Statique.hpp"

void Statique::solve(Instance Inst){
  vector<int> d(Inst.NT);
  int i,s,t,max;
  
  for(i=0; i<Inst.Td; i++)
    d[i] = Inst.demandes_det[i];
  
  for(i=Inst.Td; i<Inst.NT; i++){
    max = 0;
    for(s=0; s<Inst.s; s++)
      if(Inst.demandes[s][i] > max)
	max = Inst.demandes[s][i];
    d[i] = max;
  }
  
  try{
    IloEnv env;
    IloModel model(env);
    IloArray<IloIntVarArray> x(env,Inst.I);
    for(i=0; i<Inst.I; i++){
      //x[i]=IloIntVarArray(env, Inst.NT,0,IloInfinity);
      x[i]=IloIntVarArray(env, Inst.NT);
      for(t=0; t<Inst.NT; t++){
	x[i][t]=IloIntVar(env,0,IloInfinity);
      }
    }
    IloIntVarArray s(env,Inst.NT,0,IloInfinity);
    //IloIntVarArray s(env, Inst.NT);
    //for(t=0; t<Inst.NT; t++){
    //s[t]=IloIntVar(env,0,IloInfinity);
    //}

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
      for(t=1; t<Inst.NT; t++)
	Obj += x[i][t] * Inst.p[i][t];
    model.add(IloMinimize(env,Obj));
    Obj.end();

    IloCplex cplex(model);
    cplex.exportModel("model1.lp");
    cplex.setOut(env.getNullStream());
    cplex.solve();
    
    IloAlgorithm::Status algStatus=cplex.getStatus();
    if (algStatus != IloAlgorithm::Optimal){
      cout<<"Erreur lors de la resolution de la relaxation lineaire."<<endl;
    } else {
      cout << "Objective value: " << cplex.getObjValue() << endl;
    }
    
    env.end();
  } catch(IloException& e) {
    std::cout << e.getMessage() << std::endl;
    cout<<"The pb could not be solved"<<endl;
  }
  return;
}
