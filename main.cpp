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

#define LARGURA_JANELA 900
#define ALTURA_JANELA 700
#define ESQ_IMPORTAR 5
#define DIR_IMPORTAR 65
#define BAIXO_IMPORTAR 10
#define CIMA_IMPORTAR 50

class vertice {
public:
    GLfloat x;
    GLfloat y;
    GLfloat z;

    vertice(){}
    vertice(GLfloat x, GLfloat y, GLfloat z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    vertice(const vertice& v){
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    vertice& operator=(vertice v){
        GLfloat a, b, c;
        a = v.x;
        b = v.y;
        c = v.z;

        vertice *novo = new vertice(a, b, c);
        return *novo;
    }
};

class face {
public:
    vertice * um;
    vertice * dois;
    vertice * tres;

    face(){}
    face(vertice a, vertice b, vertice c){
        um = new vertice(a);
        dois = new vertice(b);
        tres = new vertice(c);
    }
};

class objeto{
public:
    string nome;
    float transX, transY, transZ;
    float scaleX, scaleY, scaleZ;
    float rotX, rotY, rotZ, rotAngle;
    vector<face> faces;
    GLfloat cor[3];

    objeto(){
        nome = "";
        transX = 0; transY = 0; transZ = 0;
        scaleX = 1; scaleY = 1; scaleZ = 1;
        rotX = 0; rotY = 0; rotZ = 0; rotAngle = 0;
    }
    void imprime(){
        cout<<"X: "<<transX<<" Y: "<<transY<<" Z: "<<transZ<<endl;
        cout<<"Angle: "<<rotAngle<<" X: "<<rotX<<" Y: "<<rotY<<" Z: "<<rotZ<<endl;
        cout<<"X: "<<scaleX<<" Y: "<<scaleY<<" Z: "<<scaleZ<<endl;
        cout<<endl;
    }
};

int objs = 0, num_tri = 0;
bool importado[3];
bool clicked[3][3][4]; // 0 PARA FALSO, 1 PARA TRUE
bool enviar = false;
bool nomeValido = false;
bool wire = true;
unsigned short int arq = 0;
unsigned short int selecionado = 0; // 0 PARA O OBJETO 3D, 1 PARA BARRA DE PESQUISA
const int font = (int)GLUT_BITMAP_TIMES_ROMAN_24;
const int font2 = (int)GLUT_BITMAP_HELVETICA_18;
string nomeArq, novoP = "";
vector<vertice> listaVertices;
GLfloat colors[6][3] = { {0.2,0.9,0.8} , {0.8,0.4,0.2}, {1.0,1.0,0.3}, {0.5,1.0,0.5},{0.3,0.4,1.0} , {1.0,0.7,1.0}};
GLdouble viewer[] = {2.0, 2.0, 2.0};
GLdouble focus[] = {0.0, 0.0, 0.0};
GLdouble up[] = {0.0, 1.0, 0.0};
objeto obj[3];

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
    stringstream ss;
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

            if(j == 0)
                ss <<obj[index].transX;
            else if(j == 1)
                ss << obj[index].transY;
            else if(j == 2)
                ss << obj[index].transZ;

            std::string s = ss.str ();
            char* char_type = (char*) s.c_str();
            renderBitmapString(x+1,y+y_inc-6,(void *)font2,char_type);

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
            renderBitmapString(x+1,y+y_inc-6,(void *)font2,char_type);

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
            renderBitmapString(x+1,y+y_inc-6,(void *)font2,char_type);

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

void mudaParam(int j, int i, int ob, float valor){

                    if( j == 0 ){ // TRANSLACAO
                        if( i == 0 ){
                            obj[ob].transX = valor;
                        }
                        if( i == 1 ){
                            obj[ob].transY = valor;
                        }
                        if( i == 2 ){
                            obj[ob].transZ = valor;
                        }
                    }
                    else if( j == 1 ){ // ROTACAO
                        if( i == 0 ){
                            obj[ob].rotAngle = valor;
                        }
                        if( i == 1 ){
                            obj[ob].rotX = valor;
                        }
                        if( i == 2 ){
                            obj[ob].rotY = valor;
                        }
                        if( i == 3 ){
                            obj[ob].rotZ = valor;
                        }
                        }
                    else if( j == 2 ){ // ESCALA
                        if( i == 0 ){
                            obj[ob].scaleX =  valor;
                        }
                        if( i == 1 ){
                            obj[ob].scaleY =  valor;
                        }
                        if( i == 2 ){
                            obj[ob].scaleZ = valor;
                        }
                    }
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

void desenhaMenuLateral(){

    // Desenha input importar
    glColor3f(0.0, 0.0, 0.0);
    std::stringstream ss;
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

void tri(vertice a, vertice b, vertice c, int ind) {
    glColor3f(obj[ind].cor[0], obj[ind].cor[1], obj[ind].cor[2]);
    glPushMatrix();
    glTranslatef(obj[ind].transX, obj[ind].transY, obj[ind].transZ);
    glScalef(obj[ind].scaleX, obj[ind].scaleY, obj[ind].scaleZ);
    glRotatef(obj[ind].rotAngle, obj[ind].rotX, obj[ind].rotY, obj[ind].rotZ);
    if(!wire){
       glBegin(GL_TRIANGLES);
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        glEnd();
    }else {
        glBegin(GL_LINE_LOOP);
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        glEnd();
    }
    glPopMatrix();
}

void desenhaObj(int x) {
    for(int i=0; i<obj[x].faces.size(); i++){
        tri(*obj[x].faces[i].um, *obj[x].faces[i].dois, *obj[x].faces[i].tres, x);
        num_tri++;
    }
}

void leObj(string nome){


    ifstream arquivo;

	arquivo.open(nome);

	if(arquivo.fail()){
		cerr << "Erro ao abrir arquivo." << endl;
		exit(1);
	}
	listaVertices.clear();
	obj[objs].cor[0] = colors[objs][0];
	obj[objs].cor[1] = colors[objs][1];
	obj[objs].cor[2] = colors[objs][2];
	objs++;

	string tipo;
    GLfloat x;
	GLfloat y;
	GLfloat z;

    int indice_v1, indice_v2, indice_v3;
    string s1, s2, s3;

	while(!arquivo.eof()){

		arquivo>>tipo;
		if(tipo == "v"){
            arquivo>>x;
            arquivo>>y;
            arquivo>>z;

            vertice * vertice_aux = new vertice(x, y, z);
            listaVertices.push_back(vertice(x, y, z));

		}
		else if(tipo == "f"){
            arquivo >> s1;
            arquivo >> s2;
            arquivo >> s3;

            std::string::size_type sz;   // alias of size_t

            int pos = s1.find('/');
            indice_v1 = std::stoi( s1.substr(0, pos), &sz, 10 );
            pos = s2.find('/');
            indice_v2 = std::stoi( s2.substr(0, pos), &sz, 10 );
            pos = s3.find('/');
            indice_v3 = std::stoi( s3.substr(0, pos), &sz, 10 );

            if(indice_v1 < 0) {
                indice_v1 = listaVertices.size() + indice_v1;
            }
            if(indice_v2 < 0) {
                indice_v2 = listaVertices.size() + indice_v2;
            }
            if(indice_v3 < 0) {
                indice_v3 = listaVertices.size() + indice_v3;
            }

            vertice * v1 = new vertice(listaVertices[indice_v1].x, listaVertices[indice_v1].y, listaVertices[indice_v1].z);
            vertice * v2 = new vertice(listaVertices[indice_v2].x, listaVertices[indice_v2].y, listaVertices[indice_v2].z);
            vertice * v3 = new vertice(listaVertices[indice_v3].x, listaVertices[indice_v3].y, listaVertices[indice_v3].z);

            face * face_aux = new face(*v1, *v2, *v3);
            obj[objs-1].faces.push_back(*face_aux);

		}

	}

}

void mouseHandler(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN) {
            if( x > LARGURA_JANELA - LARGURA_JANELA/3){
                if(x> 615 && x<795 && y>BAIXO_IMPORTAR && y<CIMA_IMPORTAR)
                    selecionado = 1;
                //unsigned short int limite = 0;
                for(int k=0; k<3; k++){
                    for(int i=0; i < 4; i++){
                        for(int j=0; j < 3; j++){
                            if(i == 3 && j!= 1){
                                continue;
                            }
                            if(x> 615+(i*60) && x<660+(i*60) && y>105+(j*55)+(k*210) && y<133+(j*55)+(k*210)){
                                selecionado = 2;
                                novoP = "";
                                for(int k2=0; k2<3; k2++){
                                    for (int i2=0; i2<4; i2++){
                                        for(int j2=0; j2<3; j2++){
                                            clicked[k2][j2][i2] = 0;
                                        }
                                    }
                                }
                                clicked[k][j][i] = 1;
                            }
                        }
                    }
                }
                if(arq < 3) {
                    if(x> 804 && x<890 && y>BAIXO_IMPORTAR && y<CIMA_IMPORTAR){
                        leObj(obj[arq].nome);
                        obj[arq].nome = obj[arq].nome;
                        importado[arq] = true;
                        arq ++;
                        selecionado = 0;
                    }
                }
            }
            else
                selecionado = 0;
        }
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, (LARGURA_JANELA/3)*2, ALTURA_JANELA);
    glLoadIdentity();
    glFrustum(-2.0, 2.0, -2.0, 2.0, 1.0, 100.0);
    //gluPerspective(70.0,1.0,2.0,100.0);
    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    focus[0],focus[1],focus[2],                           // ponto de interesse (foco)
    up[0],up[1],up[2]);                          // vetor de "view up"
    glMatrixMode(GL_MODELVIEW);
    desenhaEixos();
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

void keyboardHandler(unsigned char key, int x, int y){
    if (key == 27) exit(0); //ESC
    if (key == '1') glEnable(GL_LIGHTING);
    if (key == '0') glDisable(GL_LIGHTING);

   cout<< "ASCII de "<<key<< ": "<<(int)key << endl;

    if(selecionado == 1){
        if(arq < 3){ //IMPEDIR USUARIO MANDAR MAIS DE 3 ARQUIVOS
            if((int)key == 46 || ((int)key >= 65 && (int)key <= 90 ) || ((int)key >= 97 && (int)key <= 122 ) || ((int)key >= 48 && (int)key <= 57 )){ //Codigo ASCII apenas de letras e numeros
                string aux;
                aux = key;
                obj[arq].nome = obj[arq].nome + aux;
            }
            else if( (int)key == 8 && obj[arq].nome.size()>0){ // CODIGO ASCII 8 DO BOTÃO BACKSPACE
                obj[arq].nome.erase(obj[arq].nome.size()-1); // APAGA ULTIMO CARACTERE DA STRIING
            }
            else if((int)key == 13){
                leObj(obj[arq].nome);
                obj[arq].nome = obj[arq].nome;
                importado[arq] = true;
                arq ++;
                selecionado = 0;
            }
        }
    }
    else if(selecionado == 2){
        for(int k=0; k<3; k++){
            for (int i=0; i<4; i++){
                for(int j=0; j<3; j++){
                    if(clicked[k][j][i] == 1){
                        if((int)key == 46 || ((int)key >= 48 && (int)key <= 57 ) || (int)key == 13 || (int)key == 8){ //Codigo ASCII apenas de numeros e ponto
                            if( (int)key == 8){ // CODIGO ASCII 8 DO BOTÃO BACKSPACE
                                if(novoP.size()>0){
                                    novoP.erase(novoP.size()-1);
                                    if(novoP.size() == 0)
                                        mudaParam(j,i,k,0);
                                    else
                                        mudaParam(j,i,k,stof(novoP));
                                }
                            }
                            else {
                                string aux;
                                aux = key;
                                if(novoP.size()<4){
                                    novoP = novoP + aux;
                                    mudaParam(j,i,k,stof(novoP));
                                }
                                if((int)key == 13){
                                    mudaParam(j,i,k,stof(novoP));
                                    clicked[k][j][i] == 0;
                                    novoP = "";
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        if (key == 'a') focus[2] -= 1.0;
        //if (key == 's') focus[1] += 1.0;
        //if (key == 'w') focus[1] -= 1.0;
        if (key == 'd') focus[0] -= 1.0;
        if (key == 37) focus[2] -= 1.0;
        if (key == 40) focus[1] += 1.0;
        if (key == 39) focus[1] -= 1.0;
        if (key == 38) focus[0] -= 1.0;
        if (key == 'e') wire = true; //empty
        if (key == 'f') wire = false; //full
        if (key == 's') {
            viewer[0] -= 1.0;
            viewer[1] -= 1.0;
            viewer[2] -= 1.0;
        }
        if (key == 'w') {
            viewer[0] += 1.0;
            viewer[1] += 1.0;
            viewer[2] += 1.0;
        }

    }
    display();
}

void reshape(int x, int y) {
    glutReshapeWindow(LARGURA_JANELA, ALTURA_JANELA);
}

void init(){
    for(int k=0; k<3; k++){
        importado[k] = false;
        for(int i=0; i<3; i++){
            for(int j=0; j<4; j++)
                clicked[k][i][j] = 0;
        }
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
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
