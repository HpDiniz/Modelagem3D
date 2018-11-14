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
string nomeArq;
string nomes[3];
bool importado[3];
unsigned short int arq = 0;
bool ClickImport = false;
bool enviar = false;
bool nomeValido = false;
vector<vector<GLfloat> > vertices;
vector<vector<GLfloat> > vertices2;
vector<vector<GLfloat> > vertices3;
vector<vector<int> > faces;
vector<vector<int> > faces2;
vector<vector<int> > faces3;
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

void submenu(int x_ini, int y_ini, int index){
    int x = x_ini, y = y_ini;
    int x_inc = 15, x_spacer = 25;
    int y_inc = 28, y_spacer = 55;

    std::stringstream ss; //from
    ss << nomes[index];
    std::string s = ss.str ();
    char* char_type = (char*) s.c_str();
    glColor3f(1.0, 0.0, 0.4);
    renderBitmapString(x,y-33,(void *)font,char_type);
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(x,y-5,(void *)font,"Translacao");
    glColor3f(1.0, 1.0, 1.0);
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
    renderBitmapString(x,y-5,(void *)font,"Rotacao");
    glColor3f(1.0, 1.0, 1.0);
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
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(x,y-5,(void *)font,"Escala");
    glColor3f(1.0, 1.0, 1.0);
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

    // Desenha input importar
    glColor3f(0.0, 0.0, 0.0);
    std::stringstream ss; //from
    ss << nomes[arq];
    std::string s = ss.str ();
    char* char_type = (char*) s.c_str();
    renderBitmapString(ESQ_IMPORTAR+ESQ_IMPORTAR/2,CIMA_IMPORTAR-CIMA_IMPORTAR/3,(void *)font,char_type);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(ESQ_IMPORTAR, BAIXO_IMPORTAR);
        glVertex2f(DIR_IMPORTAR, BAIXO_IMPORTAR);
        glVertex2f(DIR_IMPORTAR, CIMA_IMPORTAR-8);
        glVertex2f(ESQ_IMPORTAR, CIMA_IMPORTAR-8);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(DIR_IMPORTAR+6,CIMA_IMPORTAR-CIMA_IMPORTAR/3,(void *)font,"Import");
     glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(DIR_IMPORTAR+3, BAIXO_IMPORTAR);
        glVertex2f(DIR_IMPORTAR+32, BAIXO_IMPORTAR);
        glVertex2f(DIR_IMPORTAR+32, CIMA_IMPORTAR-8);
        glVertex2f(DIR_IMPORTAR+3, CIMA_IMPORTAR-8);
    glEnd();

    int alt = 80;
    for(int i=0;i<objs;i++){
        submenu(5, alt+25+(210*i), i);
    }

    glColor3f(0.4, 0.8, 0.4);

    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(100, 0);
        glVertex2f(100, ALTURA_JANELA);
        glVertex2f(0, ALTURA_JANELA);
    glEnd();
}

void tri(int a, int b, int c) {
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(vertices[a][0],vertices[a][1],vertices[a][2]);
        glVertex3f(vertices[b][0],vertices[b][1],vertices[b][2]);
        glVertex3f(vertices[c][0],vertices[c][1],vertices[c][2]);
    glEnd();
}

void desenhaObj(int x) {
    if(x == 0){
        for(int i=0; i< vertices.size()-2; i++){
            tri(i,i+1,i+2);
        }
    }
    else if(x == 1){
        for(int i=0; i< vertices2.size()-2; i++){
            tri(i,i+1,i+2);
        }
    }
    else if(x == 2){
        for(int i=0; i< vertices3.size()-2; i++){
            tri(i,i+1,i+2);
        }
    }
}

void leObj(string nome){


    ifstream arquivo;

	// cin >> nome_arquivo;
	//string nome = "boneco.obj";
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
    vector<int> face;
    face.push_back(0);
    face.push_back(0);
    face.push_back(0);
    int v1, v2, v3, ignora;
    char barra;

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
            if(arq == 0)
                vertices.push_back(ponto);
            else if(arq == 1)
                vertices2.push_back(ponto);
            else if(arq == 2)
                vertices3.push_back(ponto);

		}
		else if(tipo == "f"){
            arquivo>>v1;
            cout << "vertice: " << v1;
            arquivo>>barra;
            arquivo>>ignora;
            arquivo>>barra;
            arquivo>>ignora;
            arquivo>>v2;
            cout << "vertice: " << v2;
            arquivo>>barra;
            arquivo>>ignora;
            arquivo>>barra;
            arquivo>>ignora;
            arquivo>>v3;
            cout << "vertice: " << v3;
            arquivo>>barra;
            arquivo>>ignora;
            arquivo>>barra;
            arquivo>>ignora;

            face[0] = v1;
            face[1] = v2;
            face[2] = v3;

            cout << "face: " << face[0] << ' ' << face[1] << ' ' << face[2] << endl;
    /*
            if(arq == 0)
                faces.push_back(face);
            else if(arq == 1)
                faces2.push_back(face);
            else if(arq == 2)
                faces3.push_back(face);
*/
		}

		//cout<<aux<<endl;
	}

}

void mouseHandler(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN) {
            if( x > LARGURA_JANELA - LARGURA_JANELA/3){
                cout<< "Cliquei em " << x << " " << y << endl;
                ClickImport = true;
                cout<< DIR_IMPORTAR+3 << endl;
                if(arq < 3) {
                    if(x> 804 && x<890 && y>BAIXO_IMPORTAR && y<CIMA_IMPORTAR){
                        leObj(nomes[arq]);
                        importado[arq] = true;
                        arq ++;
                        cout << arq << endl;
                    }
                }
            }
            else
                ClickImport = false;
        }
    }
}

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
    for(int i=0; i<arq; i++){
        if(importado[i] == true)
            desenhaObj(i);
    }
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

    cout<< "ASCII de "<<key<< ": "<<(int)key << endl;

    if(arq>0){
        for(int i=0; i<arq; i++)
            cout<<i<<": "<< nomes[i] << " ";
        cout<<endl;
    }

    if(ClickImport == true){
        if(arq < 3){ //IMPEDIR USUARIO MANDAR MAIS DE 3 ARQUIVOS
            if((int)key == 46 || ((int)key >= 65 && (int)key <= 90 ) || ((int)key >= 97 && (int)key <= 122 ) || ((int)key >= 48 && (int)key <= 57 )){ //Codigo ASCII apenas de letras e numeros
                string aux;
                aux = key;
                nomes[arq] = nomes[arq] + aux;
            }
            else if( (int)key == 8 && nomes[arq].size()>0){ // CODIGO ASCII 8 DO BOTÃO BACKSPACE
                nomes[arq].erase(nomes[arq].size()-1); // APAGA ULTIMO CARACTERE DA STRIING
            }
        }
    }
    else{
        if (key == 'x') viewer[0] -= 1.0;
        if (key == 'X') viewer[0] += 1.0;
        if (key == 'y') viewer[1] -= 1.0;
        if (key == 'Y') viewer[1] += 1.0;
        if (key == 'z') viewer[2] -= 1.0;
        if (key == 'Z') viewer[2] += 1.0;
    }
    display();
}

void reshape(int x, int y) {
    glutReshapeWindow(LARGURA_JANELA, ALTURA_JANELA);
}

void init(){
    for(int i=0; i<3; i++)
        importado[i] = false;
    glClearColor(1.0, 1.0, 1.0, 1.0);
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
