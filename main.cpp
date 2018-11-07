#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

#define LARGURA_JANELA 900
#define ALTURA_JANELA 700

char nome_arquivo[100];
vector<> vertices;
GLfloat verticesCB[8][3] = { {-1.0,-1.0,1.0},{-1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,-1.0,1.0},
{-1.0,-1.0,-1.0},{-1.0,1.0,-1.0},{1.0,1.0,-1.0},{1.0,-1.0,-1.0} };
GLdouble viewer[] = {2.0, 2.0, 3.0};

//cor de cada face do cubo
GLfloat colors[6][3] = { {0.0,0.0,0.0} , {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
{0.0,0.0,1.0} , {1.0,0.0,1.0}};

void contaTriangulos(){}

void calculaFrames(){}

void submenu(){}

void criaViewports(){}

void desenhaObj() {}

void quad(int a, int b, int c, int d, int ncolor) {
    glColor3fv(colors[ncolor]);
    glBegin(GL_QUADS);
        glVertex3fv(vertices[a]);
        glVertex3fv(vertices[b]);
        glVertex3fv(vertices[c]);
        glVertex3fv(vertices[d]);
    glEnd();
}

void tri(int a, int b, int c, int ncolor) {
    glColor3fv(colors[ncolor]);
    glBegin(GL_TRIANGLES);
        glVertex3fv(vertices[a]);
        glVertex3fv(vertices[b]);
        glVertex3fv(vertices[c]);
    glEnd();
}

//desenha o cubo (faces no sentido anti-horario - face externa)
void colorcube() {

    // Faces em sentido anti-horario
    quad(0,3,2,1,0);
    quad(2,3,7,6,1);
    quad(0,4,7,3,2);
    quad(1,2,6,5,3);
    quad(4,5,6,7,4);
    quad(0,1,5,4,5);
}

void leObj(){

    ifstream arquivo;

	cin >> nome_arquivo;

	arquivo.open(nome_arquivo);

	if(arquivo.fail()){
		cerr << "Erro ao abrir arquivo." << endl;
		exit(1);
	}

	string tipo;
    GLfloat x;
	GLfloat y;
	GLfloat z;

	while(!arquivo.eof()){

		arquivo>>tipo;
		if(tipo == "v"){
            //getline(arquivo, aux);
            arquivo>>x;
            arquivo>>y;
            arquivo>>z;
            cout<<tipo<<" "<<x<<" "<<y<<" "<<z<<endl;
            vertices.push(x, y, z);

		}

		//cout<<aux<<endl;
	}
}

void keyboardHandler(unsigned char key, int x, int y){}

void mouseHandler(int button, int state, int x, int y){}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();
    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    0.0, 0.0, 0.0,                           // ponto de interesse (foco)
    0.0, 1.0, 0.0);                          // vetor de "view up"

    colorcube();
    desenhaObj();

    glutSwapBuffers();
}

void init(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0);
    glMatrixMode(GL_MODELVIEW);

    criaViewports();
    leObj();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Blender HD");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
