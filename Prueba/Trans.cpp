#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


int main (){
  int count=0;
  int Cant;
  cout << "Cantidad de Nodos: ";
  cin >> Cant;
  ifstream leer("coord3.txt", ios::in);
  ofstream copiar("coord.txt",ios::out);
  string valor1,valor2,aux;
  leer >> aux >> aux >> valor1 >> aux >> valor2;
  while (!leer.eof()){
    if (count>=2958-Cant) copiar << stod(valor1) << " "<< stod(valor2) << endl;
    leer >> aux >> aux >> valor1 >> aux >> valor2;
    ++count;
  }
  return 0;
}
