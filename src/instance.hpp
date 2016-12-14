#pragma once

#include <vector>

using namespace std;

class Instance {
public:
  int NT; //nb total de périodes
  int I;  //nb d'usines
  int h;  //cout de stockage unitaire
  int Td; //nb de périodes que l'on connait à l'avance
  int s;  //nb de scenarios

  vector<int> capacite_usine; //pour chaque usine, sa capacité de production
  vector<vector<int> > p; //p[i][t] = cout unitaire de produire sur usine i au temps t

  vector<int> demandes_det; //pour les premiers Td unités de temps, les demandes (deterministes) associées
  vector<vector<int> > demandes; //demandes[s][t] = demande à la période t dans le scénario s 
  
  Instance();
  Instance(char* file);
};
