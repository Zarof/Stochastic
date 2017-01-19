#include "Light.hpp"

void Light::solve(Instance Inst){  

  // Phase 1
  
  vector<int> d(Inst.NT);
  vector<int> d_max(Inst.NT);
  vector<int> d_min(Inst.NT);
  int i,j,s,t,max,min;

  for(i=0; i<Inst.Td; i++){
    d[i] = Inst.demandes_det[i];
    d_max[i] = d[i];
    d_min[i] = d[i];
  }
  
  for(t=Inst.Td; t<Inst.NT; t++){
    max = 0;
    min = Inst.demandes[0][t-Inst.Td];
    for(s=0; s<Inst.s; s++){
      if(Inst.demandes[s][t-Inst.Td] > max)
	max = Inst.demandes[s][t-Inst.Td];
      if(Inst.demandes[s][t-Inst.Td] < min)
	min = Inst.demandes[s][t-Inst.Td];
    }
    srand (time(NULL));
    d[t] = min + rand() % (max-min+1);
    d_max[t] = max;
    d_min[t] = min;
  }

  // for(i=0; i<Inst.NT; i++)
  //cout<<"demande alea en " << i << " : " << d[i]<<endl;
  
  try{
    IloEnv env;
    IloModel model(env);
    IloArray<IloIntVarArray> x(env,Inst.I);
    for(i=0; i<Inst.I; i++){
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
    //cplex.exportModel("robuste_statique.lp");
    cplex.setOut(env.getNullStream());
    cplex.setWarning(env.getNullStream());
    cplex.solve();

    val2 = cplex.getObjValue();
    
    for(int i=0; i<Inst.I; i++){
      vector<int> v;
      production2.push_back(v);
      for(int t=0; t<Inst.NT; t++){
	//cout<<"Usine "<<i<<" produit x_" << i + t*Inst.I << " à la période t"<< t << " : " << cplex.getValue(x[i][t])<<endl;
	production2[i].push_back(cplex.getValue(x[i][t]));
	
      }
    }

    for(int t=0; t<Inst.NT; t++)
      stock2.push_back(cplex.getValue(s[t]));

    env.end();
  } catch(IloException& e) {
    std::cout << e.getMessage() << std::endl;
    cout<<"The pb could not be solved"<<endl;
  }


  

  //cout << " PHASE 2 -------- " << endl;


  
  // Phase 2

  int M = Inst.NT;
  int N = Inst.NT * (Inst.I + 1);

  //cout<< "M " << M << " N " << N <<endl;

  vector<int> T(M,2);
  double delta = 0.5;
  int z_ = val2;
  vector< vector<int> > a;
  vector< vector<double> > a_;
  for(i=0; i<M; i++){
    vector<int> v;
    vector<double> v_;
    a.push_back(v);
    a_.push_back(v_);
    for(j=0 ; j<N; j++){
      a[i].push_back(0);
      a_[i].push_back(0.0);
    }
    
    for(j= Inst.I * i; j<Inst.I * (i+1); j++){
      a[i][j] = 1;
      a_[i][j] = (double)d_max[i]/d_min[i] - 1;
    }
    
    /*    for(j=0 ; j<M; j++)
      cout << a[i][j] << " " << a_[i][j] << " - ";
      cout << endl;*/
    
    //a associés aux stocks:
    a[i][Inst.NT*Inst.I + i] = - (double)d_max[i]/d_min[i];
    a_[i][Inst.NT*Inst.I + i] = (double)d_max[i]/d_min[i] - 1;   
    
    if( i > 0 ){
      a[i][Inst.NT*Inst.I + i-1] = 1;
      a_[i][Inst.NT*Inst.I + i-1] = (double)d_max[i]/d_min[i] - 1;
    }    
  }
  /*
  cout << "Apres A " << endl;
  for(i=0; i<M; i++){
    for(j=0 ; j<N; j++){
      cout << a[i][j] << " " ;//<< a_[i][j] << " - ";
    }
    cout << endl;
  }
  for(i=0; i<M; i++){
    for(j=0 ; j<N; j++){
      cout << a_[i][j] << " " ;//<< a_[i][j] << " - ";
    }
    cout << endl;
  }
  */
  vector<int> b(M,0);
  for(i=0; i<M; i++)
    b[i] = d_max[i];

  vector<int> c(N,0);
  for(j=0; j<Inst.I * Inst.NT; j++){
    c[j] = Inst.p[j%Inst.I][j/Inst.I];
  }
  for(j=Inst.I * Inst.NT; j<N; j++){
    c[j] = Inst.h;
  }

  /*  cout<<"Vecteur c"<<endl;
  for(i=0; i<N; i++)
    cout<<c[i]<<"\t";
    cout<<endl;*/
  
  try{
    IloEnv env;
    IloModel model(env);

    IloNumVarArray y(env, M);
    for(i=0; i<M; i++){
      stringstream var;
      var<<"y_"<<i;
      y[i]=IloNumVar(env,0,IloInfinity,var.str().c_str());
    }

    IloIntVarArray x(env, N);
    for(j=0; j<N; j++){
      stringstream var;
      var<<"x_"<<j;
      x[j]=IloIntVar(env,0,IloInfinity,var.str().c_str());
    }

    IloNumVarArray z(env, M);
    for(i=0; i<M; i++){
      stringstream var;
      var<<"z_"<<i;
      z[i]=IloNumVar(env,0,IloInfinity,var.str().c_str());
    }

    IloArray<IloNumVarArray> p(env, M);
    for(i=0; i<M; i++){
      p[i]=IloNumVarArray(env, N);
      for(j=0; j<N; j++){
  	stringstream var;
  	var<<"p_"<<i<<j;
  	p[i][j]=IloNumVar(env,0,IloInfinity,var.str().c_str());
      }
    }

    for(j=0; j<Inst.I*Inst.NT; j++){
      IloExpr sum(env);
      sum += x[j];
      model.add(sum <= Inst.capacite_usine[j%Inst.I]);
      //cout << Inst.capacite_usine[j%Inst.I] << endl;
    }
    
    for(i=0; i<M; i++){
      IloExpr sum(env);
      for(j=0; j<N; j++)
  	sum += a[i][j] * x[j] + p[i][j];  
      model.add(sum + T[i]*z[i] - y[i] == b[i]);
      sum.end();
    }

    for(i=0; i<M; i++)
      for(j=0; j<N; j++)
  	model.add(-a_[i][j]*x[j] + z[i] + p[i][j] == 0);
    
    for(i=0; i<M; i++){
      IloExpr sum(env);
      for(j=0; j<N; j++)
  	sum += a[i][j] * x[j];  
      model.add(sum <= b[i]);
      sum.end();
    }

    IloExpr sum(env);
    for(j=0; j<N; j++)
      sum += c[j] * x[j];  
    model.add(sum <= (1 + delta) * z_);
    sum.end();
    
    
    IloExpr Obj(env);
    for(i=0; i<M; i++)
      Obj += y[i];

    model.add(IloMinimize(env,Obj));
    Obj.end();

    IloCplex cplex(model);
    cplex.exportModel("light_robuste.lp");
    cplex.setOut(env.getNullStream());
    cplex.setWarning(env.getNullStream());
    cplex.solve();

    val = cplex.getObjValue();
    // cout << "sum y " << val << endl;
    for(int i=0; i<Inst.I; i++){
      //pour chaque usine
      vector<int> v;		  
      production.push_back(v);
      for(int t=0; t<Inst.NT; t++){
	//on a quantité produite à chaque période
	//	cout<<"Usine "<<i<<" produit x_" << i + t*Inst.I << " à la période t : ";
  	production[i].push_back(cplex.getValue(x[i + t*Inst.I]));
	//cout<<cplex.getValue(x[i + t*Inst.I])<<endl;
      }
     }
    
    for(int t=0; t<Inst.NT; t++)
      stock.push_back(cplex.getValue(x[t+ Inst.I*Inst.NT]));
    
    env.end();
  } catch(IloException& e) {
    std::cout << e.getMessage() << std::endl;
    cout<<"The pb could not be solved"<<endl;
  }

  //calcul de la vraie valeur de la solution :
  int objective_value = 0;
  for(int i = 0 ; i<Inst.I ; i++){
    for(int t=0 ; t < Inst.NT ; t++){
      objective_value += production[i][t]*Inst.p[i][t];
    }
  }
  for(int t=0 ; t < Inst.NT ; t++){
    objective_value += stock[t]*Inst.h;
  }
  
  val = objective_value;
  cout << "end solving" << endl;
  return ;
}
  
