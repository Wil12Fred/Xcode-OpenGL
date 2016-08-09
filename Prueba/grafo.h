#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

ofstream copiar4("comprobar1.txt",ios::out);

class Nodo
{
public:
  string nombre;
  int codigo;
  pair <double, double> coordenada;
  public:
  Nodo(string nombre, double x, double y ,int codigo)
  {
    this->nombre = nombre;
    coordenada = make_pair(x, y);
    this -> codigo = codigo;
  }
  pair <double,double> Getcoord(){
    return coordenada;
  };
};

class Arista
{
public:
  string nombre;
  pair<int,int> codigo;
  Nodo* ant;
  Nodo* sig;
  double peso;
  public:
  Arista(string nombre, Nodo* ant, Nodo* sig)
  {
    double x = ant->coordenada.first;
    double y = ant->coordenada.second;
    double x2 = sig->coordenada.first;
    double y2 = sig->coordenada.second;
    this->nombre = nombre;
    this->ant = ant;
    this->sig = sig;
    peso = sqrt((x-x2)*(x-x2)+(y-y2)*(y - y2));
    cout << peso << endl;
    codigo = make_pair(ant->codigo, sig->codigo);
  }
};

class Grafo
{
public:
  vector<vector<pair<double,int> > > lista_adyacencia;
public:
  Grafo(vector<Arista*> calle8)
  {
    vector<vector<pair<double,int> > > lista_adyacencia2(calle8.size());
    for(int i=0;i<calle8.size();i++){
      int v1=calle8[i]->codigo.first;
      //cout << i <<"-> "<< calle8[i]->codigo.first;
      copiar4 << i <<"-> "<< calle8[i]->codigo.first;
      int v2=calle8[i]->codigo.second;
      //cout <<" "<<calle8[i]->codigo.second << " " ;
      copiar4 <<" "<<calle8[i]->codigo.second << " ";
      double peso=calle8[i]->peso;
      //cout << peso << endl;
      copiar4 << peso << endl;
      lista_adyacencia2[v1].push_back(make_pair(peso,v2));
    }
    lista_adyacencia=lista_adyacencia2;
  }
  vector<vector<pair<double,int> > > GetGrafo(){
    return lista_adyacencia;
  }
};
