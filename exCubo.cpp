//
// INF390 - Computação Gráfica
//
// Visualizaçao 3D
// Desenha um cubo centrado na origem com faces de -1 a 1
// coloridas com diferentes cores
//
// Marcus V. A. Andrade - 25/09/2008
//

#include <GL/gl.h>
#include <GL/glut.h>

#include <stdlib.h>

//tamanho da janela em pixels
int largurajanela = 400, alturajanela = 300;

//vertices do cubo
/* ------------------------------------------
              vertices do cubo

      5 ----- 6
     /|      /|
    1 ----- 2 |
    | 4 ....| 7
    |/      |/
    0 ----- 3

*/

GLfloat vertices[8][3] = { {-1.0,-1.0,1.0},{-1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,-1.0,1.0},
{-1.0,-1.0,-1.0},{-1.0,1.0,-1.0},{1.0,1.0,-1.0},{1.0,-1.0,-1.0} };

//cor de cada face do cubo
GLfloat colors[6][3] = { {0.0,0.0,0.0} , {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
{0.0,0.0,1.0} , {1.0,0.0,1.0}};

//posicao do observador (camera)
GLdouble viewer[] = {5.0, 5.0, 5.0};

// desenha uma face do cubo
// a, b, c e d sao indices no vetor de vertices
// ncolor eh um indice no vetor de cores
void quad(int a, int b, int c, int d, int ncolor) {
    glColor3fv(colors[ncolor]);
    glBegin(GL_QUADS);
        glVertex3fv(vertices[a]);
        glVertex3fv(vertices[b]);
        glVertex3fv(vertices[c]);
        glVertex3fv(vertices[d]);
    glEnd();
}

//desenha o cubo (faces no sentido anti-horario - face externa)
void colorcube() {
    quad(0,3,2,1,0);
    quad(2,3,7,6,1);
    quad(0,4,7,3,2);
    quad(1,2,6,5,3);
    quad(4,5,6,7,4);
    quad(0,1,5,4,5);
}


// define o tipo de projecao
// glOrtho(left, right, bottom, top, near, far) -> projecao paralela
// glFrustum(left, right, bottom, top, near, far) -> projecao perspectiva
// gluPerspective(viewAngle, aspectRatio, near, far) -> projecao perspectiva
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); //cor para limpeza do buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); //projeção paralela

    glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); //projeção perspectiva

    //gluPerspective(70.0,1.0,2.0,20.0); // projeção perspectiva

    glMatrixMode(GL_MODELVIEW);
}

void desenhaEixos() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(100.0, 0.0, 0.0);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 100.0);
    glEnd();
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();
    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    0.0, 0.0, 0.0,                           // ponto de interesse (foco)
    0.0, 1.0, 0.0);                          // vetor de "view up"

    desenhaEixos();
    colorcube(); // desenha o cubo

    glFlush();
}


int main(int argc, char **argv) {
    glutInit(&argc,argv); //inicializa a glut
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores/profundidade
    glutInitWindowSize(largurajanela, alturajanela); //dimensões da janela
    glutInitWindowPosition(200, 200); //posicao da janela
    glutCreateWindow("Visualizacao 3D - Exemplo 1"); //cria a janela
    init();
    glutDisplayFunc(display); //determina função callback de desenho
 //   glEnable(GL_DEPTH_TEST); //habilita remoção de superfícies ocultas (Z-Buffer)
 //   glEnable (GL_CULL_FACE); //habilita remoção de superfícies ocultas (Backface culling)
 //   glCullFace(GL_FRONT);

    glutMainLoop();
    return 0;
}
