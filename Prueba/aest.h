#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define alto 100
#define largo 100
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
using namespace std;



class nodito
{
public:
    float G, F, H;//  peso - suma de g+h - heuristica
    string name;
    pair<int, int> coordenada;
    nodito* derecha;
    nodito* izquierda;
    nodito* arriba;
    nodito* abajo;
    nodito* diago1;
    nodito* diago2;
    nodito* diago3;
    nodito* diago4;
    nodito* ant;
public:
    nodito(int x, int y, string name){
        G = F = H = 0;
        ant = derecha = izquierda = arriba = abajo = diago1 = diago2 = diago3 = diago4 = NULL;
        coordenada = make_pair(x, y);
        this->name = name;
    }
    ~nodito(){
        /*delete derecha;
        delete izquierda;
        delete arriba;
        delete abajo;
        delete diago1;
        delete diago2;
        delete diago3;
        delete diago4;*/
        ant = derecha = izquierda = arriba = abajo = diago1 = diago2 = diago3 = diago4 = NULL;
    }
};

// Revisa si el nodo esta en el vector
bool searches(vector<nodito*> a, nodito* x){
    for(int i = 0; i < a.size(); i++)
        if(a[i] == x)
            return true;
    return false;
}

//Retorna el indice donde se encuentra el mejor candidato 
int search_lesser(vector<nodito*> a){
    float menor = a[0]->F;
    int index = 0;
    for(int i = 1; i< a.size(); i++){
        if(a[i]->F  <= menor){
            menor = a[i]->F;
            index = i;
        }
    }
    return index;
}

//Borra un puntero de su vector
void Borrar(vector<nodito*>& a, nodito* x){
    size_t j = 0;
    for(size_t i = 0; i < a.size(); i++)
    {
        if(a[i] != x)
        {
            a[j++]  = a[i];
        }
    }
    a.resize(j);
}

//Dibuja el mapa
void crearmapa(nodito* a[alto][largo], vector<nodito*> ag){
    /* HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 9);*/
    string water;
    bool wat = false;
    for(int i = 0; i < alto; i++){
        for(int j = 0; j < largo; j++){
            for(int m = 0; m < ag.size();m++){
                if(a[i][j] == ag[m]){
                    water.append("*");
                    wat = true;
                }
            }
            if(wat == false){
                 water.append(" ");
            } else {
                wat = false;
            }
        }
        cout<<water<<endl;
        wat = "";
    }
   // SetConsoleTextAttribute(hConsole, 15);
    return;
}

//La maravilla de ACM1estrella
void Astar(nodito* a[alto][largo], pair<int, int> start, pair<int, int> finale, vector<nodito*> obstaculos){
    ios_base::sync_with_stdio(false);
    //HANDLE hConsole;
    //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    float straight, nstraight;//valores arr,izq,der,ab - diag
    cin>>straight;
    cin>>nstraight;
    vector<nodito*> open;//nodo de los posibles cantidados
    vector<nodito*> closed;// nodos visitados
    vector<nodito*> bloqueado;//nodos obstaculos
    bool diagos1, diagos2, diagos3, diagos4;//true-> si una esquina bloquea false->si no bloquea
    diagos1 = diagos2 = diagos3 = diagos4 = false;
    //bloq = make_pair(50,50);
    //crearmapa(a, obstaculos);
    if(obstaculos[0]->coordenada.first != -1){// todos los obstaculos a -> bloqueado[] close[]
        for(int i = 0;i < obstaculos.size();i++){
          closed.push_back(a[obstaculos[i]->coordenada.first][obstaculos[i]->coordenada.second]);
        }
        bloqueado = closed;
    }

    vector<pair<int,int> > path;//almacena coordenda del camino correcto
    vector<nodito*> path2;
    nodito* camino;
    int index;
    open.push_back(a[start.first][start.second]);
    //Inicializa algoritmo aestrella orientado a if's
    while(searches(closed, a[finale.first][finale.second]) == false and open.size()!=0){
        index = search_lesser(open);
        if(open[index]->coordenada.first -1 < 0) {}//continue 
        else if(searches(closed, a[open[index]->coordenada.first - 1][open[index]->coordenada.second]) == true){
            if(searches(bloqueado, a[open[index]->coordenada.first - 1][open[index]->coordenada.second]) == true){
                diagos4 = diagos3 = true;
            }
        } else if(searches(open, a[open[index]->coordenada.first - 1][open[index]->coordenada.second]) == false){
               open[index]->izquierda = a[open[index]->coordenada.first - 1][open[index]->coordenada.second];
               open[index]->izquierda->G = open[index]->G + straight;
               open[index]->izquierda->H = abs(open[index]->izquierda->coordenada.first - finale.first + open[index]->izquierda->coordenada.second - finale.second);
               open[index]->izquierda->F = open[index]->izquierda->G + open[index]->izquierda->H;
               open[index]->izquierda->ant = open[index];
               open.push_back(a[open[index]->coordenada.first - 1][open[index]->coordenada.second]);
        } else if(searches(open, a[open[index]->coordenada.first - 1][open[index]->coordenada.second]) == true){
            open[index]->izquierda = a[open[index]->coordenada.first - 1][open[index]->coordenada.second];
            if(a[open[index]->coordenada.first - 1][open[index]->coordenada.second]->G > open[index]->G + straight){
                    a[open[index]->coordenada.first - 1][open[index]->coordenada.second]->ant = open[index];
                    a[open[index]->coordenada.first - 1][open[index]->coordenada.second]->G = 0;
                    a[open[index]->coordenada.first - 1][open[index]->coordenada.second]->G = open[index]->G + straight;
                    a[open[index]->coordenada.first - 1][open[index]->coordenada.second]->F = a[open[index]->coordenada.first - 1][open[index]->coordenada.second]->G + a[open[index]->coordenada.first - 1][open[index]->coordenada.second]->H;
            }
        }
        if(open[index]->coordenada.first +1>= alto){}
        else if(searches(closed,a[open[index]->coordenada.first +1][open[index]->coordenada.second]) == true){
            if(searches(bloqueado,a[open[index]->coordenada.first +1][open[index]->coordenada.second]) == true){
                diagos1 = true;
                diagos2 = true;
            }
        } else if(searches(open,a[open[index]->coordenada.first +1][open[index]->coordenada.second]) == false){
            open[index]->derecha = a[open[index]->coordenada.first +1][open[index]->coordenada.second];
            open[index]->derecha->G = open[index]->G + straight;
            open[index]->derecha->H = abs(open[index]->derecha->coordenada.first - finale.first + open[index]->derecha->coordenada.second - finale.second);
            open[index]->derecha->F = open[index]->derecha->G + open[index]->derecha->H;
            open[index]->derecha->ant = open[index];
            open.push_back(a[open[index]->coordenada.first +1][open[index]->coordenada.second]);
        } else if(searches(open,a[open[index]->coordenada.first +1][open[index]->coordenada.second]) == true){
            open[index]->derecha = a[open[index]->coordenada.first +1][open[index]->coordenada.second];
            if(a[open[index]->coordenada.first +1][open[index]->coordenada.second]->G > open[index]->G + straight){
                a[open[index]->coordenada.first + 1][open[index]->coordenada.second]->ant = open[index];
                a[open[index]->coordenada.first + 1][open[index]->coordenada.second]->G = 0;
                a[open[index]->coordenada.first + 1][open[index]->coordenada.second]->G = open[index]->G + straight;
                a[open[index]->coordenada.first + 1][open[index]->coordenada.second]->F = a[open[index]->coordenada.first + 1][open[index]->coordenada.second]->G + a[open[index]->coordenada.first + 1][open[index]->coordenada.second]->H;
            }
        }
        if(open[index]->coordenada.second - 1 < 0){}
        else if(searches(closed,a[open[index]->coordenada.first][open[index]->coordenada.second - 1]) == true){
            if(searches(bloqueado,a[open[index]->coordenada.first][open[index]->coordenada.second - 1]) == true){
                diagos1 = true;
                diagos4 = true;
            }
        } else if(searches(open,a[open[index]->coordenada.first][open[index]->coordenada.second -1]) == false){
            open[index]->arriba = a[open[index]->coordenada.first][open[index]->coordenada.second -1];
            open[index]->arriba->G = open[index]->G + straight;
            open[index]->arriba->H = abs(open[index]->arriba->coordenada.first - finale.first + open[index]->arriba->coordenada.second - finale.second);
            open[index]->arriba->F = open[index]->arriba->G + open[index]->arriba->H;
            open[index]->arriba->ant = open[index];
            open.push_back(a[open[index]->coordenada.first][open[index]->coordenada.second -1]);
        } else if(searches(open,a[open[index]->coordenada.first][open[index]->coordenada.second -1]) == true){
            open[index]->arriba = a[open[index]->coordenada.first][open[index]->coordenada.second -1];
            if(a[open[index]->coordenada.first][open[index]->coordenada.second -1]->G > open[index]->G+ straight){
                a[open[index]->coordenada.first][open[index]->coordenada.second-1]->ant = open[index];
                a[open[index]->coordenada.first][open[index]->coordenada.second-1]->G = 0;
                a[open[index]->coordenada.first][open[index]->coordenada.second-1]->G = open[index]->G + straight;
                a[open[index]->coordenada.first][open[index]->coordenada.second-1]->F = a[open[index]->coordenada.first][open[index]->coordenada.second-1]->G + a[open[index]->coordenada.first][open[index]->coordenada.second-1]->H;
            }
        }
        if(open[index]->coordenada.second +1 >= largo){}
        else if(searches(closed, a[open[index]->coordenada.first][open[index]->coordenada.second +1]) == true){
            if(searches(bloqueado, a[open[index]->coordenada.first][open[index]->coordenada.second +1]) == true){
                diagos2 = true;
                diagos3 = true;
            }
        } else if(searches(open, a[open[index]->coordenada.first][open[index]->coordenada.second +1]) == false)
        {
            open[index]->abajo = a[open[index]->coordenada.first][open[index]->coordenada.second +1];
            open[index]->abajo->G = open[index]->G + straight;
            open[index]->abajo->H = abs(open[index]->abajo->coordenada.first - finale.first + open[index]->abajo->coordenada.second - finale.second);
            open[index]->abajo->F = open[index]->abajo->G + open[index]->abajo->H;
            open[index]->abajo->ant = open[index];
            open.push_back(a[open[index]->coordenada.first][open[index]->coordenada.second +1]);
        } else if(searches(open, a[open[index]->coordenada.first][open[index]->coordenada.second +1]) == true){
            open[index]->abajo = a[open[index]->coordenada.first][open[index]->coordenada.second + 1];
            if(a[open[index]->coordenada.first][open[index]->coordenada.second +1]->G > open[index]->G+straight)
            {
                a[open[index]->coordenada.first][open[index]->coordenada.second+1]->ant = open[index];
                a[open[index]->coordenada.first][open[index]->coordenada.second+1]->G = 0;
                a[open[index]->coordenada.first][open[index]->coordenada.second+1]->G = open[index]->G + straight;
                a[open[index]->coordenada.first][open[index]->coordenada.second+1]->F = a[open[index]->coordenada.first][open[index]->coordenada.second+1]->G + a[open[index]->coordenada.first][open[index]->coordenada.second+1]->H;
            }
        }
        if(open[index]->coordenada.first +1 >= alto or open[index]->coordenada.second -1 <0)
        {

        }
        else if(searches(closed, a[open[index]->coordenada.first +1][open[index]->coordenada.second - 1]) == true)
        {

        }
        else if(diagos1 == true)
        {
            diagos1 = false;
        }
        else if(searches(open, a[open[index]->coordenada.first +1][open[index]->coordenada.second - 1]) == false)
        {
            open[index]->diago1 = a[open[index]->coordenada.first +1][open[index]->coordenada.second - 1];
            open[index]->diago1->G = open[index]->G + nstraight;
            open[index]->diago1->H = abs(open[index]->diago1->coordenada.first - finale.first + open[index]->diago1->coordenada.second - finale.second);
            open[index]->diago1->F = open[index]->diago1->G + open[index]->diago1->H;
            open[index]->diago1->ant = open[index];
            open.push_back(a[open[index]->coordenada.first +1][open[index]->coordenada.second - 1]);
        }
        else if(searches(open, a[open[index]->coordenada.first +1][open[index]->coordenada.second - 1]) == true)
        {
            open[index]->diago1 = a[open[index]->coordenada.first +1][open[index]->coordenada.second - 1];
            if(a[open[index]->coordenada.first +1][open[index]->coordenada.second - 1]->G > open[index]->G + nstraight)
            {
                a[open[index]->coordenada.first +1][open[index]->coordenada.second-1]->ant = open[index];
                a[open[index]->coordenada.first +1][open[index]->coordenada.second-1]->G = 0;
                a[open[index]->coordenada.first +1][open[index]->coordenada.second-1]->G = open[index]->G + nstraight;
                a[open[index]->coordenada.first +1][open[index]->coordenada.second-1]->F = a[open[index]->coordenada.first+1][open[index]->coordenada.second-1]->G + a[open[index]->coordenada.first+1][open[index]->coordenada.second-1]->H;
            }
        }
        if(open[index]->coordenada.first +1 >= alto or open[index]->coordenada.second +1 >= largo)
        {

        }
        else if(diagos2 == true)
        {
            diagos2 = false;
        }
        else if(searches(closed, a[open[index]->coordenada.first +1][open[index]->coordenada.second + 1]) == true)
        {

        }

        else if(searches(open, a[open[index]->coordenada.first +1][open[index]->coordenada.second + 1]) == false)
        {
            open[index]->diago2 = a[open[index]->coordenada.first +1][open[index]->coordenada.second + 1];
            open[index]->diago2->G = open[index]->G + nstraight;
            open[index]->diago2->H = abs(open[index]->diago2->coordenada.first - finale.first + open[index]->diago2->coordenada.second - finale.second);
            open[index]->diago2->F = open[index]->diago2->G + open[index]->diago2->H;
            open[index]->diago2->ant = open[index];
            open.push_back(a[open[index]->coordenada.first +1][open[index]->coordenada.second + 1]);
        }
        else if(searches(open, a[open[index]->coordenada.first +1][open[index]->coordenada.second + 1]) == true)
        {
            open[index]->diago2 = a[open[index]->coordenada.first +1][open[index]->coordenada.second + 1];
            if(a[open[index]->coordenada.first +1][open[index]->coordenada.second + 1]->G > open[index]->G+nstraight)
            {
                a[open[index]->coordenada.first +1][open[index]->coordenada.second+1]->ant = open[index];
                a[open[index]->coordenada.first +1][open[index]->coordenada.second+1]->G = 0;
                a[open[index]->coordenada.first +1][open[index]->coordenada.second+1]->G = open[index]->G + nstraight;
                a[open[index]->coordenada.first +1][open[index]->coordenada.second+1]->F = a[open[index]->coordenada.first+1][open[index]->coordenada.second+1]->G + a[open[index]->coordenada.first+1][open[index]->coordenada.second+1]->H;
            }
        }
        if(open[index]->coordenada.first -1 < 0 or open[index]->coordenada.second+1 >= largo)
        {

        }
        else if(diagos3 == true)
        {
            diagos3 = false;
        }
        else if(searches(closed, a[open[index]->coordenada.first -1][open[index]->coordenada.second + 1]) == true)
        {

        }
        else if(searches(open, a[open[index]->coordenada.first -1][open[index]->coordenada.second + 1]) == false)
        {
            open[index]->diago3 = a[open[index]->coordenada.first -1][open[index]->coordenada.second + 1];
            open[index]->diago3->G = open[index]->G + nstraight;
            open[index]->diago3->H = abs(open[index]->diago3->coordenada.first - finale.first + open[index]->diago3->coordenada.second - finale.second);
            open[index]->diago3->F = open[index]->diago3->G + open[index]->diago3->H;
            open[index]->diago3->ant = open[index];
            open.push_back(a[open[index]->coordenada.first -1][open[index]->coordenada.second + 1]);
        }
        else if(searches(open, a[open[index]->coordenada.first -1][open[index]->coordenada.second + 1]) == true)
        {
            open[index]->diago3 = a[open[index]->coordenada.first -1][open[index]->coordenada.second + 1];
            if(a[open[index]->coordenada.first -1][open[index]->coordenada.second + 1]->G > open[index]->G+nstraight)
            {
                a[open[index]->coordenada.first -1][open[index]->coordenada.second+1]->ant = open[index];
                a[open[index]->coordenada.first -1][open[index]->coordenada.second+1]->G = 0;
                a[open[index]->coordenada.first -1][open[index]->coordenada.second+1]->G = open[index]->G + nstraight;
                a[open[index]->coordenada.first -1][open[index]->coordenada.second+1]->F = a[open[index]->coordenada.first-1][open[index]->coordenada.second+1]->G + a[open[index]->coordenada.first-1][open[index]->coordenada.second+1]->H;
            }
        }
        if(open[index]->coordenada.first -1 < 0 or open[index]->coordenada.second -1 <0)
        {

        }
        else if(diagos4 == true)
        {
            diagos4 = false;
        }
        else if(searches(closed, a[open[index]->coordenada.first -1][open[index]->coordenada.second - 1]) == true)
        {

        }
        else if(searches(open, a[open[index]->coordenada.first -1][open[index]->coordenada.second - 1]) == false)
        {
            open[index]->diago4 = a[open[index]->coordenada.first -1][open[index]->coordenada.second - 1];
            open[index]->diago4->G = open[index]->G + nstraight;
            open[index]->diago4->H = abs(open[index]->diago4->coordenada.first - finale.first + open[index]->diago4->coordenada.second - finale.second);
            open[index]->diago4->F = open[index]->diago4->G + open[index]->diago4->H;
            open[index]->diago4->ant = open[index];
            open.push_back(a[open[index]->coordenada.first -1][open[index]->coordenada.second - 1]);
        }
        else if(searches(open, a[open[index]->coordenada.first -1][open[index]->coordenada.second - 1]) == true)
        {
            open[index]->diago4 = a[open[index]->coordenada.first -1][open[index]->coordenada.second - 1];
            if(a[open[index]->coordenada.first -1][open[index]->coordenada.second - 1]->G > open[index]->G+nstraight)
            {
                a[open[index]->coordenada.first -1][open[index]->coordenada.second-1]->ant = open[index];
                a[open[index]->coordenada.first -1][open[index]->coordenada.second-1]->G = 0;
                a[open[index]->coordenada.first -1][open[index]->coordenada.second-1]->G = open[index]->G + nstraight;
                a[open[index]->coordenada.first -1][open[index]->coordenada.second-1]->F = a[open[index]->coordenada.first-1][open[index]->coordenada.second-1]->G + a[open[index]->coordenada.first-1][open[index]->coordenada.second-1]->H;
            }
        }
        closed.push_back(open[index]);
        //open.erase(open.begin() + index);
        Borrar(open, open[index]);

    }
    camino = a[finale.first][finale.second];
    while(camino != NULL)
    {
        path.push_back(make_pair(camino->coordenada.first,camino->coordenada.second));
        path2.push_back(camino);
        camino = camino->ant;
    }
    path2.push_back(camino);
    string l, o;
    bool asterisk = false, obstacle = false;
    for(int i = path.size()-1;i>= 0; i--)
    {
        cout<<"("<<path[i].first<<","<<path[i].second<<")"<<endl;
    }
    if(obstaculos[0]->coordenada.first == -1)
    {
        for(int i = 0; i < alto; i++)
        {
            for(int j = 0;j < largo; j++)
            {
                for(int m = 0; m < path2.size(); m++)
                {
                    if(a[i][j] == path2[m])
                    {
                        l.append("*");
                        asterisk = true;
                    }
                }
                if(asterisk == false)
                {
                    l.append(" ");
                }
                if(asterisk == true)
                {
                    asterisk = false;
                }
            }
            //SetConsoleTextAttribute(hConsole, 15);
            cout<<l<<endl;
            l = "";
        }
        return;
    }
    for(int i = 0; i < alto; i++)
    {
        for(int j = 0;j < largo; j++)
        {
            for(int m = 0; m < path2.size(); m++)
            {
                if(a[i][j] == path2[m] and m == 0)
                {
                    l.append("T");
                    asterisk = true;
                }
                if(a[i][j] == path2[m] and m != 0)
                {
                    l.append("*");
                    asterisk = true;
                }
            }
            if(asterisk == false)
            {
                l.append(" ");
            }
            if(asterisk == true)
            {
                asterisk = false;
            }
            for(int m = 0; m < obstaculos.size(); m++)
             {
                if(a[i][j]->coordenada.first == obstaculos[m]->coordenada.first and a[i][j]->coordenada.second == obstaculos[m]->coordenada.second)
                {
                    if(a[i][j]->name == "Water")
                    {
                        o.append("x");
                        break;
                    }
                    if(a[i][j]->name == "Building")
                    {
                        o.append("B");
                        break;
                    }
                    if(a[i][j]->name == "Fire")
                    {
                        o.append("F");
                        break;
                    }
                }
                if(m == obstaculos.size()-1)
                 {
                    o.append(l.substr(j,1));
                 }
             }
        }
        for(int m = 0; m < o.size(); m++){

            if(o.substr(m,1) == "x")
            {
              cout << CYAN ;
                printf("%c", 'A');
            }
            else if(o.substr(m,1) == "B")
            {
              cout << WHITE ;
                printf("%c", 'B');
            }
            else if(o.substr(m,1) == "F")
            {
              cout << RED; 
                printf("%c", 'F');
            }
            else
            {
              cout << BOLDBLUE ;
                printf("%c", o[m]);
            }
        }
        //cout<<o<<endl;
        cout<<endl;
        l = "";
        o = "";
    }
    for(int i = 0; i < open.size(); i++)
        open[i] = NULL;
    for(int j = 0; j < closed.size(); j++)
        closed[j] = NULL;
    for(int m = 0; m < bloqueado.size(); m++)
        bloqueado[m] = NULL;
    return;
}
