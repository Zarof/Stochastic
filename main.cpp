#include <iostream>
#include <fstream>
#include <vector>

#include "instance.hpp"

using namespace std;

int main(int argc, char** argv){

  if(argc > 1) {

    Instance Inst(argv[1]);

    cout<<"Fin Programme"<<endl;
  }
  else
    cout<<"Aucun fichier de données en paramètre"<<endl;
  return 0;
}
