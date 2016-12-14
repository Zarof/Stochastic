#include <iostream>
#include <fstream>
#include <vector>

#include "instance.hpp"
#include "Statique.hpp"

using namespace std;

int main(int argc, char** argv){

  if(argc > 1) {

    Instance Inst(argv[1]);

    Statique s;
    s.solve(Inst);

    cout<<"Fin Programme"<<endl;
  }
  else
    cout<<"Aucun fichier de données en paramètre"<<endl;
  return 0;
}
