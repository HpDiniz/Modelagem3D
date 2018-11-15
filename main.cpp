#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <bits/stdc++.h>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;
//comit

#define LARGURA_JANELA 900
#define ALTURA_JANELA 700
#define ESQ_IMPORTAR 5
#define DIR_IMPORTAR 65
#define BAIXO_IMPORTAR 10
#define CIMA_IMPORTAR 50

class objeto{
public:
    string nome;
    float transX, transY, transZ;
    float scaleX, scaleY, scaleZ;
    float rotX, rotY, rotZ, rotAngle;

    objeto(){
        nome = "";
        transX = 1.1; transY = 1.2; transZ = 1.3;
        scaleX = 3.1; scaleY = 3.2; scaleZ = 3.3;
        rotX = 2.1; rotY = 2.2; rotZ = 2.3; rotAngle = 0.5;
    }
};

int objs = 0;
string nomeArq;
objeto obj[3];
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
    int x_inc = 15, x_spacer = 20;
    int y_inc = 28, y_spacer = 55;

    stringstream ss; //from
    ss << obj[index].nome;
    string s = ss.str ();
    char* char_type = (char*) s.c_str();
    glColor3f(1.0, 0.0, 0.4);
    renderBitmapString(x,y-33,(void *)font,char_type);
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(x,y-5,(void *)font,"Translacao");
    for(int j=0; j<=2;j++){

        glColor3f(0.0, 0.0, 0.0);
        stringstream ss;

       // cout<< setprecision(1) << obj[index].transX << fixed <<endl;

        if(j == 0)
            ss <<obj[index].transX;
        else if(j == 1)
            ss << obj[index].transY;
        else if(j == 2)
            ss << obj[index].transZ;

        std::string s = ss.str ();
        char* char_type = (char*) s.c_str();
        renderBitmapString(x+2,y+y_inc-4,(void *)font,char_type);

        glColor3f(1.0, 1.0, 1.0);
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

        glColor3f(0.0, 0.0, 0.0);
        stringstream ss;

        if(j == 0)
            ss << obj[index].rotAngle;
        else if(j == 1)
            ss << obj[index].rotX;
        else if(j == 2)
            ss << obj[index].rotY;
        else if(j == 3)
            ss << obj[index].rotZ;

        std::string s = ss.str ();
        char* char_type = (char*) s.c_str();
        renderBitmapString(x+2,y+y_inc-4,(void *)font,char_type);

        glColor3f(1.0, 1.0, 1.0);
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

        glColor3f(0.0, 0.0, 0.0);
        stringstream ss;

        if(j == 0)
            ss << obj[index].scaleX;
        else if(j == 1)
            ss << obj[index].scaleY;
        else if(j == 2)
            ss << obj[index].scaleZ;

        std::string s = ss.str ();
        char* char_type = (char*) s.c_str();
        renderBitmapString(x+2,y+y_inc-4,(void *)font,char_type);

        glColor3f(1.0, 1.0, 1.0);

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
    ss << obj[arq].nome;
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
    string s1, s2, s3;
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
            arquivo >> s1;
            arquivo >> s2;
            arquivo >> s3;

            std::string::size_type sz;   // alias of size_t

            int pos = s1.find('/');
            v1 = std::stoi( s1.substr(0, pos), &sz, 10 );
            pos = s2.find('/');
            v2 = std::stoi( s2.substr(0, pos), &sz, 10 );
            pos = s3.find('/');
            v3 = std::stoi( s3.substr(0, pos), &sz, 10 );

            face[0] = v1;
            face[1] = v2;
            face[2] = v3;

            cout << "face: " << face[0] << ' ' << face[1] << ' ' << face[2] << endl;

            if(arq == 0)
                faces.push_back(face);
            else if(arq == 1)
                faces2.push_back(face);
            else if(arq == 2)
                faces3.push_back(face);

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
                        leObj(obj[arq].nome);
                        obj[arq].nome = obj[arq].nome;
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
            cout<<i<<": "<< obj[i].nome << " ";
        cout<<endl;
    }

    if(ClickImport == true){
        if(arq < 3){ //IMPEDIR USUARIO MANDAR MAIS DE 3 ARQUIVOS
            if((int)key == 46 || ((int)key >= 65 && (int)key <= 90 ) || ((int)key >= 97 && (int)key <= 122 ) || ((int)key >= 48 && (int)key <= 57 )){ //Codigo ASCII apenas de letras e numeros
                string aux;
                aux = key;
                obj[arq].nome = obj[arq].nome + aux;
            }
            else if( (int)key == 8 && obj[arq].nome.size()>0){ // CODIGO ASCII 8 DO BOTÃO BACKSPACE
                obj[arq].nome.erase(obj[arq].nome.size()-1); // APAGA ULTIMO CARACTERE DA STRIING
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
