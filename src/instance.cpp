#include <fstream>
#include <iostream>

#include "instance.hpp"

using namespace std;

/*Instance::Instance(){
  NT = 0;
  I = 0 ;
  h = 0;
  Td = 0;
  s = 0;

  vector<int> c, d;
  capacite_usine = c;
  demandes_det = d;

  vector<vector<int> > v1, v2;
  p = v1;
  demandes = v2;
}*/

Instance::Instance(char* file){
  cout << "Lecture donnee: " << file << endl;
  ifstream infile(file);

  infile>>NT;
  infile>>I;
  infile>>h;
  infile>>Td;
  infile>>s;

  vector<int> c (I);
  capacite_usine = c;
  vector<vector<int> > v1 (I);
  for(int i=0; i<I; i++){
    vector<int> v (NT);
    v1[i]=v;
  }
  p = v1;

  for(int i=0; i<I; i++){
    infile>>capacite_usine[i];
    for(int t=0; t<NT; t++){
      infile>>p[i][t];
    }
  }

  vector<int> d(Td);
  demandes_det=d;
  for(int i=0 ; i<Td; i++)
    infile>>demandes_det[i];

  vector<vector<int> > v2 (s);
  for(int sc=0; sc<s; sc++){
    vector<int> v (NT-Td);
    v2[sc]=v;
  }
  demandes = v2;
    
  for(int sc=0; sc<s ; sc++)
    for(int t=0 ; t<NT-Td; t++)
      infile>>demandes[sc][t];
    
}
