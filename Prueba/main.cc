#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "grafo.h"
#include "delaunay.h"
#include "aest.h"

#define GLUT_DISABLE_ATEXIT_HACK

#include <sstream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#define KEY_ESC 27
#define SIZEX 1000
#define SIZEY 1000
#define WHITE   "\033[37m"      /* White */

using namespace std;
typedef pair<double,int> PII;

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
void glPaint();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);
GLvoid window_idle();
int Cant;
double rotate_by_key=0;
double rotate_x=0.5;
double eje_x=0.5;
double eje_y=0.5;
int izq=0,der=1000,aba=0,arr=1000;
float posX =0;
float posY =0;
float posZ =0;
float xx =0;
float yy =0;
float zz =0;
//const double Screen=500;
const double INF = 2000000000;
vector <int> Camino;
vector <pair<double,double> > lines;
vector <pair<pair<double,double>,pair<double,double> > > mapa;

ifstream leer("coord.txt", ios::in);
ifstream leer2("aristas.txt", ios::in);
ofstream copiar2("camino.txt",ios::out);
ofstream copiar3("aristas.txt",ios::out);
ofstream copiar5("comprobar2.txt",ios::out);


void keyPress(int key,int x,int y){
    if(key==27)
          exit(0);
    if (key == GLUT_KEY_F1)
            rotate_x += .5;
            cout<<"rotate_x  "<<rotate_x<<endl;
        if (key == GLUT_KEY_F2)
            rotate_x -= .5;
            cout<<"rotate_x  "<<rotate_x<<endl;
    if (key== GLUT_KEY_RIGHT)
            eje_x += 10.5;
            cout<<"eje_x  "<<eje_x<<endl;
    if (key== GLUT_KEY_LEFT)
            eje_x -= 10.5;
            cout<<"eje_x  "<<eje_x<<endl;
    if (key== GLUT_KEY_UP)
            eje_y += 10.5;
            cout<<"eje_y  "<<eje_y<<endl;
    if (key== GLUT_KEY_DOWN)
            eje_y -= 10.5;
            cout<<"eje_y  "<<eje_y<<endl;
    glutPostRedisplay();

}

void initRendering() {glEnable(GL_DEPTH_TEST);}

class Strategy
{
public:
    virtual void ACM1metodo(vector <vector<PII> > edges,int s, int t) = 0;
};

class Dijkstra: public Strategy
{
public:
    Dijkstra()
    {
        
    }
    void ACM1metodo(vector <vector<PII> > edges,int s, int t) {
        int N=edges.size();
        int count=0;
        for (int i=0;i<N;i++){
            for (int j=0;j<edges[i].size();j++){
                copiar5 << count << "-> ";
                copiar5 << i << " " <<  edges[i][j].first << " ";
                copiar5 << edges[i][j].second << endl;
                count+=1;
            }
        }
        priority_queue<PII, vector<PII>, greater<PII> > Q;
        vector<double> dist(N, INF);
        vector<int> dad(N, -1);
        Q.push (make_pair (0, s));
        dist[s] = 0;
        while (!Q.empty()){
            PII p = Q.top();
            if (p.second == t) break;
            Q.pop();
            int here = p.second;
            for (vector<PII>::iterator it=edges[here].begin(); it!=edges[here].end(); it++){
                if (dist[here] + it->first < dist[it->second]){
                    dist[it->second] = dist[here] + it->first;
                    dad[it->second] = here;
                    Q.push (make_pair (dist[it->second], it->second));
                }
            }
        }
        cout << dist[t] << endl ;
        if (dist[t] < INF){
            for(int i=t;i!=-1;i=dad[i]){
                Camino.push_back(i);
                cout << i << ' ';
                if (i==s) 
                    cout << endl;
            }
        }
    }
};

class Algorithm2: public Strategy
{
public:
    Algorithm2()
    {
        
    }
    void ACM1metodo(vector <vector<PII> > edges, int s, int t) //override
    {
        return;
    }
};

class Contexto
{
    Strategy* Algoritmo;
public:
    Contexto()
    {
        
    }
    void set_algorithm(Strategy* Algorithm)
    {
        Algoritmo = Algorithm;
    }
    void metodo(vector <vector<PII> > edges, int s, int t) const
    {
        Algoritmo->ACM1metodo(edges,s, t);
    }
};


void displayGizmo() {
    glRotatef(posX,0.0,1.0,0.0);
    glRotatef(posY,1.0,0.0,0.0);
    glTranslatef(xx,yy,zz);
    
    glBegin(GL_LINES);
    glColor3d(250,0,0);
    glVertex3d(0,0,0);
    glVertex3d(500,0,0);
    
    glColor3d(0,250,0);
    glVertex3d(0,0, 0);
    glVertex3d(0,500,0);
    
    glColor3d(0,0,250);
    glVertex3d(0,0,0);
    glVertex3d(0,0,500);
    glEnd();
}

GLvoid callback_special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            zz+=30;
            glutPostRedisplay();
            break;
            
        case GLUT_KEY_DOWN:
            zz-=30;
            glutPostRedisplay();
            break;
            
        case GLUT_KEY_LEFT:
            xx-=30;
            glutPostRedisplay();
            break;
            
        case GLUT_KEY_RIGHT:
            xx+=30;
            glutPostRedisplay();
            break;
    }
}

GLvoid callback_mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        printf("move \n");
    }
}

GLvoid callback_motion(int x, int y)
{
    glutPostRedisplay();
    posX=x;
    posY=y;
    printf("moving %d \n ",x);
}

double ACM1D(string st){
  double dob;
  if(!(istringstream(st) >> dob)) dob=0; 
  return dob;
}

void drawScene(){
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(eje_x,eje_y,0);
  glScalef( rotate_x,rotate_x,1.0f );
  glRotatef(  rotate_by_key,-1.0f, 1.5f, -5.0f );
  int i;
  string val1,val2,val3,val4;
  glColor3f(1.0f,1.0f,0);
  leer2>>val1>>val2 >> val3 >> val4;
  while (!leer2.eof()){
    mapa.push_back(make_pair(make_pair(ACM1D(val1),ACM1D(val2)),
          make_pair(ACM1D(val3),ACM1D(val4))));
    leer2>>val1>>val2>>val3>>val4;
  }
  
  glBegin(GL_LINES);
  for (i=0;i<mapa.size();i++){
    glVertex2f(mapa[i].first.first,mapa[i].first.second);
    glVertex2f(mapa[i].second.first,mapa[i].second.second);
  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  glColor3f(1.0f,0.0,0.0);
  for (i=0;i<lines.size();i++){
      glVertex2f(lines[i].first,lines[i].second);
  }
  glEnd();
  glutSwapBuffers();
};

/*
void dibuja(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    cuadrado();
    glutSwapBuffers();
}*/

void init_GL(void){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void handleResize(int w, int h) {
    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
    glOrtho(izq, der, aba, arr, -50, 50);
    glMatrixMode(GL_MODELVIEW);
}

/*
GLvoid window_redraw(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //gluPerspective(60.0,1,0.1,1500);
    //gluLookAt(0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
    //glOrtho(-80.0f,  80.0f,-80.0f, 80.0f, -1.0f, 1.0f);
    gluOrtho2D(-1000.0f, 1000.0f, -1000.0f, 1000.0f);
    // todas la informaciones previas se aplican al la matrice del ModelView
    glMatrixMode(GL_MODELVIEW);
}
GLvoid window_key(unsigned char key, int x, int y) {
    switch (key) {
        case KEY_ESC:
            exit(0);
            break;
        case 'w':
            zz+=30;
            glutPostRedisplay();
            cout<<"W"<<endl;
            break;
            
        case 's':
            zz-=30;
            glutPostRedisplay();
            cout <<"S"<<endl;
            break;
            
        case 'a':
            xx-=30;
            glutPostRedisplay();
            cout << "A" << endl;
            break;
            
        case 'd':
            xx+=30;
            glutPostRedisplay();
            cout << "D" << endl;
            break;

        default:
            break;
    }
}
void ControlRaton( int button, int state, int x, int y ){
    if (button==GLUT_LEFT_BUTTON and state==GLUT_DOWN){
        printf( "Cerramos la aplicacion");
        exit(-1);
    }
}*/
int main(int argc, char **argv)
{
  int terminal;
  cout << "Cantidad de nodos: ";
  cin >> Cant;
  cout << "Elija opcion" << endl;
  cout << "1=terminal" << endl;
  cout << "2=Opengl" << endl;
  cin >> terminal;
  terminal-=1;
  if (!(terminal)){
    //HANDLE hConsole;
    //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    vector<nodito*> agua;
    vector<string> places;
    places.push_back("Water");
    places.push_back("Building");
    places.push_back("Fire");
    cout<<"Ingrese los puntos de inicio y de llegada" <<endl;
    nodito* mapa[alto][largo];
    for(int i = 0; i < alto; i++)
    {
        for(int j = 0; j < largo; j++)
        {
            mapa[i][j] = new nodito(i, j, "Free");
        }
    }

    int a, b, c, d, swaps;
    cin>>a;
    cin>>b;
    cin>>c;
    cin>>d;
    pair <int, int> s;
    pair <int, int> f;
    s = make_pair(a,b);
    f = make_pair(c,d);
    cout<<"Ingrese los obstaculos"<<endl;
    int z, w;
    string terrain;
    srand  (time(NULL));
    for(int i = 0; i < largo*alto/5;i++)
    {
        z = rand() % alto;
        w = rand() % largo;
        terrain = places[rand() % places.size()];
        agua.push_back(new nodito(z, w, terrain));
        mapa[z][w] = agua[i];
    }

    Astar(mapa, s, f, agua);
    for(int i = 0; i < alto; i++)
    {
        for(int j = 0; j < largo; j++)
        {
            delete mapa[i][j];
            mapa[i][j] = NULL;
        }
    }
    cout << WHITE << endl;
  } else{
    string valor1,valor2;
    int cont=0;
    int inicio,fin;
    cout << "inicio fin" << endl;
    cin >> inicio >> fin;
    vector<Nodo*> Esquinas;
    vector<Arista*> Calles;
    double xs[Cant];
    double ys[Cant];
    vector<vector<pair<double,int> > > lista_adyacencia(Cant);
    leer >> valor1 >> valor2;
    while (!leer.eof()){
      if (!(istringstream(valor1)>>xs[cont])) xs[cont]=0;
      if (!(istringstream(valor2)>>ys[cont])) ys[cont]=0;
      cout << xs[cont] << " " << ys[cont] << endl;
      leer >> valor1 >> valor2;
      cont+=1;
    }
    cont=0;
    vector<double> x(&xs[0], &xs[Cant]), y(&ys[0], &ys[Cant]);
    vector<triple> tri = delaunayTriangulation(x, y);
    int vertices[Cant][Cant];
    for (int i=0;i<Cant;i++){ for (int j=0;j<Cant;j++){ vertices[i][j]=0;}}
    for(int i = 0; i < tri.size(); i++){
        cout <<  tri[i].i << " " <<  tri[i].j << " " <<  tri[i].k << endl;
        vertices[tri[i].i][tri[i].j]=1; vertices[tri[i].j][tri[i].i]=1;
        vertices[tri[i].i][tri[i].k]=1; vertices[tri[i].k][tri[i].i]=1;
        vertices[tri[i].j][tri[i].k]=1; vertices[tri[i].k][tri[i].j]=1;
    }
    for (int i=0;i<Cant;i++){
        for(int j=0;j<Cant;j++){
            if (vertices[i][j]==1){
                copiar3 << xs[i] << " " << ys[i] << " " << xs[j] << " " << ys[j] << endl;
                Esquinas.push_back(new Nodo("ACM1NODO",xs[i],ys[i],i));
                Esquinas.push_back(new Nodo("ACM1NODO2",xs[j],ys[j],j));
                Calles.push_back(new Arista("ACM1ARISTA",Esquinas[Esquinas.size()-2] ,Esquinas[Esquinas.size()-1]));
            }
        }
    }
    Grafo ACM1GRAFO(Calles);
    Dijkstra ACM1Dijkstra;
    Contexto Algoritmos;
    Algoritmos.set_algorithm(&ACM1Dijkstra);
    vector<vector<pair<double,int> > > Hacerdijkstra;
    Hacerdijkstra = ACM1GRAFO.GetGrafo();
    Algoritmos.metodo(Hacerdijkstra,inicio,fin);
    cout << endl;
    int k=0;
    cout << "CAMINO:" << endl;
    cout << endl;
    for (int i=Camino.size()-1;i>-1;i--){
      copiar2<<xs[Camino[i]] << " "<< ys[Camino[i]] << endl;
      lines.push_back(make_pair(xs[Camino[i]],ys[Camino[i]]));
      cout << lines[k].first << " " << lines[k].second << endl;
      k+=1;
    }
    glutInit(&argc,argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize(SIZEX, SIZEY); 
    glutInitWindowPosition(0, 0);
    glutCreateWindow("..!"); 
    init_GL();
    //glutSpecialFunc(&callback_special);
    //glutMouseFunc(&callback_mouse);
    //glutMotionFunc(&callback_motion);
    glutDisplayFunc(drawScene);
    glutSpecialFunc(keyPress);
    glutReshapeFunc(handleResize);
    //glutKeyboardFunc(&window_key);
    glutMainLoop();
  }
  return 0;
}
