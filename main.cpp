#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <bits/stdc++.h>
#include <sstream>


using namespace std;

#define LARGURA_JANELA 900
#define ALTURA_JANELA 700
#define ESQ_IMPORTAR 5
#define DIR_IMPORTAR 65
#define BAIXO_IMPORTAR 10
#define CIMA_IMPORTAR 50

//char nome_arquivo[100];
int objs = 0;
string nomes[3];
vector<vector<GLfloat> > vertices;
const int font = (int)GLUT_BITMAP_TIMES_ROMAN_24;

GLdouble viewer[] = {20.0, 20.0, 30.0};

void contaTriangulos(){}

void calculaFrames(){}

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);

    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void submenu(int x_ini, int y_ini){
    int x = x_ini, y = y_ini;
    int x_inc = 15, x_spacer = 25;
    int y_inc = 40, y_spacer = 55;
    // Escreve nome do arquivo
    // Desenha inputs translação
    cout << y << endl;
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(x,y,(void *)font,"Transsicao");
    glColor3f(1.0, 0.5, 0.0);
    for(int j=0; j<=2;j++){
        glBegin(GL_POLYGON);
            glVertex2f(x, y);
            glVertex2f(x + x_inc, y);
            glVertex2f(x + x_inc, y + y_inc);
            glVertex2f(x, y + y_inc);
        glEnd();

        x += x_spacer;
    }
    x = x_ini;

    y = y + y_spacer;
    // Desenha inputs rotação
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(x,y,(void *)font,"Rotacao");
    glColor3f(1.0, 0.5, 0.0);
    for(int j=0; j<=3;j++){
        glBegin(GL_POLYGON);
            glVertex2f(x, y);
            glVertex2f(x + x_inc, y);
            glVertex2f(x + x_inc, y + y_inc);
            glVertex2f(x, y + y_inc);
        glEnd();

        x += x_spacer;
    }
    x = x_ini;
    y = y + y_spacer;
    // Desenha inputs escala
    for(int j=0; j<=2;j++){
        glBegin(GL_POLYGON);
            glVertex2f(x, y);
            glVertex2f(x + x_inc, y);
            glVertex2f(x + x_inc, y + y_inc);
            glVertex2f(x, y + y_inc);
        glEnd();

        x += x_spacer;
    }


}

void desenhaEixos() {}

void desenhaMenuLateral(){
    glColor3f(1.0, 0.5, 0.0);
    /*
    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(100, 0);
        glVertex2f(100, ALTURA_JANELA);
        glVertex2f(0, ALTURA_JANELA);
    glEnd();
    */

    // Desenha input importar
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(ESQ_IMPORTAR, BAIXO_IMPORTAR);
        glVertex2f(DIR_IMPORTAR, BAIXO_IMPORTAR);
        glVertex2f(DIR_IMPORTAR, CIMA_IMPORTAR);
        glVertex2f(ESQ_IMPORTAR, CIMA_IMPORTAR);
    glEnd();

    int alt = 80;
    for(int i=0;i<objs;i++){
        cout << objs << endl;
        submenu(5, alt+(180*i));
    }
}

void tri(int a, int b, int c) {
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(vertices[a][0],vertices[a][1],vertices[a][2]);
        glVertex3f(vertices[b][0],vertices[b][1],vertices[b][2]);
        glVertex3f(vertices[c][0],vertices[c][1],vertices[c][2]);
    glEnd();
}

void desenhaObj() {
    for(int i=0; i< vertices.size()-2; i++){
        tri(i,i+1,i+2);
    }
}

void leObj(){

    ifstream arquivo;

	// cin >> nome_arquivo;
	string nome = "boneco.obj";
    const char * nome_arquivo = nome.c_str();

	arquivo.open(nome_arquivo);
	objs++;

	if(arquivo.fail()){
        objs--;
		cerr << "Erro ao abrir arquivo." << endl;
		exit(1);
	}

	string tipo;
    GLfloat x;
	GLfloat y;
	GLfloat z;
	vector<GLfloat> ponto;
    ponto.push_back(0);
    ponto.push_back(0);
    ponto.push_back(0);

	while(!arquivo.eof()){

		arquivo>>tipo;
		if(tipo == "v"){
            //getline(arquivo, aux);
            arquivo>>x;
            arquivo>>y;
            arquivo>>z;

            ponto[0] = x;
            ponto[1] = y;
            ponto[2] = z;
            //cout<<tipo<<" "<<x<<" "<<y<<" "<<z<<endl;
            vertices.push_back(ponto);

		}

		//cout<<aux<<endl;
	}
}

void mouseHandler(int button, int state, int x, int y){}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, (LARGURA_JANELA/3)*2, ALTURA_JANELA);
    glLoadIdentity();
    glFrustum(0.0, 1.0, -1.0, 1.0, 1.0, 150.0);
    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    0.0, 0.0, 0.0,                           // ponto de interesse (foco)
    0.0, 1.0, 0.0);                          // vetor de "view up"
    glMatrixMode(GL_MODELVIEW);
    desenhaObj();
    glFlush();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport((LARGURA_JANELA/3)*2, 0, LARGURA_JANELA, ALTURA_JANELA);
    gluOrtho2D(0, LARGURA_JANELA/3, ALTURA_JANELA, 0);
    glMatrixMode(GL_MODELVIEW);
    desenhaMenuLateral();

    glutSwapBuffers();
    glutPostRedisplay();
}

void leVertices(){
    for(int i=0; i < vertices.size(); i++){
        cout<<"Ponto["<<i<<"] = ";
        for(int j=0; j<3; j++)
            cout<<vertices[i][j]<<" ";
        cout<< endl;
    }
}

void keyboardHandler(unsigned char key, int x, int y){
    if (key == 27) exit(0); //ESC
    if (key == 'x') viewer[0] -= 1.0;
    if (key == 'X') viewer[0] += 1.0;
    if (key == 'y') viewer[1] -= 1.0;
    if (key == 'Y') viewer[1] += 1.0;
    if (key == 'z') viewer[2] -= 1.0;
    if (key == 'Z') viewer[2] += 1.0;
    display();
}

void reshape(int x, int y) {
    glutReshapeWindow(LARGURA_JANELA, ALTURA_JANELA);
}

void init(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    leObj();
    //leVertices();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutInitWindowPosition(5, 50);
    glutCreateWindow("Blender HD");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
