#include <iostream>
#include <fstream>
#include <vector>

#include "instance.hpp"
#include "Statique.hpp"
#include "Light.hpp"

using namespace std;

vector<float> calcul_cout_sol(vector<vector<int> > prod, vector<int> stock, Instance &Inst){
  //valeur1 = demandes satisfaites,
  //valeur2 = demandes INsatisfaites par solution
  //valeur3 = valeur moyenne de la solution
  cout<<"####################### calcul du cout de la solution #####################"<<endl;
  int produit = 0;
  float surplus = 0 ;
  float manque = 0 ;

  //on calcule quantités surproduites meme pendant périodes ou la demande était déterministe
  for(int t = 0 ; t<Inst.Td; t++){
    produit=0;
    for(int usine = 0 ; usine<Inst.I ; usine++){
      produit += prod[usine][t];
    }

    if(t>0){
      if((produit+stock[t-1]) - Inst.demandes_det[t] > 0){
	cout<<"Surplus de "<<(produit+stock[t-1]) - Inst.demandes_det[t]<<endl;
	surplus += (produit+stock[t-1]) - Inst.demandes_det[t];
      }
      else{
	cout<<"Il manque "<<-(produit+stock[t-1]) + Inst.demandes_det[t]<<endl;
	manque += -(produit+stock[t-1]) + Inst.demandes_det[t];
	cout<<endl;
      }
    }
    else{
      if((produit) - Inst.demandes_det[t] > 0){
      cout<<"Surplus de "<<(produit) - Inst.demandes_det[t]<<endl;
      surplus += (produit) - Inst.demandes_det[t];
    }
    else{
      cout<<"Il manque "<<-(produit+stock[t]) + Inst.demandes_det[t]<<endl;
      manque += -(produit+stock[t]) + Inst.demandes_det[t];
      cout<<endl;
    }
    }
  }
    
  cout<<"fin phase demandes det"<<surplus<<"\t"<<manque<<endl;
  
  for(int t = 0 ; t<Inst.NT-Inst.Td; t++){ //pour toutes les périodes
    cout<<"t:"<<t<<endl;
    produit = 0; 
    for(int usine = 0 ; usine<Inst.I ; usine++){
      produit += prod[usine][Inst.Td+t];
    }
    cout<<"on a produit:"<<produit<<endl;
    cout<<"en stock:"<<stock[Inst.Td+t-1]<<endl;
    for(int s = 0 ; s<Inst.s; s++){ //on calcule manque/surplus moyen sur tous les scénarios et ajoute ce cout moyen à la solution
      cout<<"s:"<<s<<endl;
      cout<<"demande :"<<Inst.demandes[s][t]<<endl;
      if((produit+stock[Inst.Td+t-1]) -  Inst.demandes[s][t] > 0){
	cout<<"Surplus de "<<(produit+stock[Inst.Td+t-1]) -  Inst.demandes[s][t]<<endl;
	surplus += ((produit+stock[Inst.Td+t-1]) -  Inst.demandes[s][t])/(float)Inst.s; //divisé par nb scénarios car manque moyen par unité de temps
	cout<<"surplus"<<surplus<<endl;
      }
      else{
	cout<<"Il manque "<<-(produit+stock[Inst.Td+t-1]) +  Inst.demandes[s][t]<<endl;
	manque += (-(produit+stock[Inst.Td+t-1]) +  Inst.demandes[s][t])/(float)Inst.s;
	cout<<"manque"<<manque<<endl;
      }
    }
  }
  
  float cout_sol = surplus*(float)Inst.h + manque*2*Inst.h ;
  //multiplié par qqch pour vrmnt pénaliser!
  //on multiplie par cout de l'usine la plus chère, qui a capacité illimitée normalement MAIS CE COUT VARIE EN FONCTION DE t, on peut multiplier par 2*h ? genre c'est deux fois pire de manquer que de stocker?
  //modifier, si nécessaire, pour calculer avec cout de l'usine la moins chère? mais pas très objectif...
  
  vector<float> res;
  res.push_back(surplus);
  res.push_back(manque);  
  res.push_back(cout_sol);
  return res;
}

int main(int argc, char** argv){

  if(argc > 1) {

    Instance Inst(argv[1]);
    
    Statique s;
    Light l;
    s.solve(Inst);
    cout <<"Objectif statique " << s.val<<endl;
    for(int t=0; t<Inst.NT; t++){
      cout << s.stock[t] << "\t" ;
      for(int i=0; i<Inst.I; i++)
	cout << s.production[i][t] << "\t" ;
      cout << endl;
    }


    l.solve(Inst);
    /* cout <<"Objectif light robuste alea " << l.val2<<endl;
    for(int t=0; t<Inst.NT; t++){
      cout << l.stock2[t] << "\t" ;
      for(int i=0; i<Inst.I; i++)
	cout << l.production2[i][t] << "\t" ;
      cout << endl;
    }*/
    
    cout <<"Objectif light robuste " << l.val<<endl;
    for(int t=0; t<Inst.NT; t++){
      cout << l.stock[t] << "\t" ;
      for(int i=0; i<Inst.I; i++)
	cout << l.production[i][t] << "\t" ;
      cout << endl;
    }

    /*calcul du coût des solutions*/
    //vector<int> cout_robuste = calcul_cout_sol(l.production, Inst.demandes_det, Inst.demandes);
    //vector<float> cout_robuste = calcul_cout_sol(s.production, s.stock, Inst);
    //cout<<"Cout de la solution robuste :"<<cout_robuste[0]<<"\t"<<cout_robuste[1]<<"\t"<<cout_robuste[2]<<endl;
    vector<float> cout_light = calcul_cout_sol(l.production, l.stock, Inst);
    cout<<"Cout de la solution light :"<<cout_light[0]<<"\t"<<cout_light[1]<<"\t"<<cout_light[2]<<endl;

    cout<<"Fin Programme"<<endl;
  }
  else
    cout<<"Aucun fichier de données en paramètre"<<endl;
  return 0;
}
