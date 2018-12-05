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
#include <windows.h>
#include <stdio.h>
#include <math.h>
//#include "stb_image.h"

using namespace std;

#define LARGURA_JANELA 900
#define ALTURA_JANELA 700
#define ESQ_IMPORTAR 5
#define DIR_IMPORTAR 65
#define BAIXO_IMPORTAR 10
#define CIMA_IMPORTAR 50
#define STB_IMAGE_IMPLEMENTATION

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
    vertice& operator+(vertice v1){
        GLfloat a, b, c;
        a = v1.x + x;
        b = v1.y + y;
        c = v1.z + z;

        vertice *novo = new vertice(a, b, c);
        return *novo;
    }
    vertice& operator-(vertice v1){
        GLfloat a, b, c;
        a = x - v1.x;
        b = y - v1.y;
        c = z - v1.z;

        vertice *novo = new vertice(a, b, c);
        return *novo;
    }
    vertice& operator* (float k){
        GLfloat a, b, c;
        a = x * k;
        b = y * k;
        c = z * k;

        vertice *novo = new vertice(a,b,c);
        return *novo;

    }
    void setP(GLfloat a, GLfloat b, GLfloat c){
        x = a;
        y = b;
        z = c;
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
    vector<face> texturas;
    vector<face> normais;
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
GLdouble quadros, deltaTime, oldTime, newTime, FPS;
bool clicked[3][3][4]; // 0 PARA FALSO, 1 PARA TRUE
bool enviar = false;
bool nomeValido = false;
bool wire = true;
bool luz1=true,luz2=true,luz3=true;
unsigned short int arq = 0;
unsigned short int selecionado = 0; // 0 PARA O OBJETO 3D, 1 PARA BARRA DE PESQUISA
const int font = (int)GLUT_BITMAP_TIMES_ROMAN_24;
const int font2 = (int)GLUT_BITMAP_HELVETICA_18;
string nomeArq, novoP = "";
vector<vertice> listaVertices;
vector<vertice> listaNormais;
vector<vertice> listaTexturas;
vertice posicao, direcao, cima, direita;
GLfloat colors[6][4] = { {0.7,0.9,0.8,1.0} , {0.8,0.4,0.2,1.0}, {1.0,1.0,0.3,1.0}, {0.5,1.0,0.5,0.5},{0.3,0.4,1.0,0.5} , {1.0,0.7,1.0,0.5}};
GLdouble viewer[] = {2.0, 2.0, 2.0};
GLfloat viewerf[] = {2.0, 2.0, 2.0};
GLdouble focus[] = {0.0, 0.0, 0.0};
GLdouble up[] = {0.0, 1.0, 0.0};
vertice topoMundo(0.0, 1.0, 0.0);
objeto obj[3];
GLint Xinicial, Yinicial, Zinicial;
GLint Yaw = 0;
GLint Pitch = 0;

GLuint texture[2];

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};


typedef struct Image Image;

#define checkImageWidth 64
#define checkImageHeight 64

GLubyte checkImage[checkImageWidth][checkImageHeight][3];

void makeCheckImage(void){
    int i, j, c;
    for (i = 0; i < checkImageWidth; i++) {
        for (j = 0; j < checkImageHeight; j++) {
            c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
        }
    }
}

/*
void makeCheckImage(void){
    int i, j, c=1;
    for (i = 0; i < checkImageWidth; i++) {
        if (i % 8 == 0) c = !c;
        for (j = 0; j < checkImageHeight; j++) {
            if (j % 8 == 0) c = !c;
            int ca = c * 255;
            checkImage[i][j][0] = (GLubyte) ca;
            checkImage[i][j][1] = (GLubyte) ca;
            checkImage[i][j][2] = (GLubyte) ca;
        }
    }
}
*/

vertice vetorial(vertice a, vertice b){
    vertice *hg = new vertice(a.y*b.z - a.z * b.y, a.z*b.x - a.x * b.z  , a.x*b.y - a.y * b.x );
    return *hg;

}

vertice escalar(vertice a, float b){
    vertice *hp = new  vertice(a.x*b, a.y*b, a.z*b);
    return *hp;
}

vertice normalizar(vertice vetor){
    float raiz = sqrt(vetor.x*vetor.x + vetor.y*vetor.y + vetor.z*vetor.z );
    vertice *hg = new vertice(vetor.x/raiz, vetor.y/raiz, vetor.z/raiz);
    return *hg;

}


int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)

    char temp; // temporary color storage for bgr-rgb conversion.
    // make sure the file is there.

    if ((file = fopen(filename, "rb"))==NULL){
        printf("File Not Found : %s\n",filename);
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    // calculate the size (assuming 24 bits or 3 bytes per pixel).

    size = image->sizeX * image->sizeY * 3;
    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }

    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }


    // read the bitsperpixel

    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }

    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }
    // seek past the rest of the bitmap header.

    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }
    // we're done.
    return 1;
}



Image * loadTexture(){
    Image *image1;
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }

    if (!ImageLoad("floor.bmp", image1)) {
        exit(1);
    }

    return image1;
}


void initTexture(){
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);//
    Image *image1 = loadTexture();
    if(image1 == NULL){
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Texture
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth,
      //           checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,&checkImage[0][0][0]);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
}

void initLight(void)
{
    GLfloat light0_position[ ] = { 2.0, 2.0, 0.0, 0.0 };
    GLfloat light0_ambient [] = {0.3, 0.3, 0.3, 1.0};
    GLfloat light0_diffuse [] = {0.3, 0.3, 0.3, 1.0};
    GLfloat light0_specular [] = {0.3, 0.3, 0.3, 1.0};

    GLfloat light1_position[ ] = { 0.0, 2.0, 0.0, 1.0 };
    GLfloat light1_ambient [] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light1_diffuse [] = {1.0, 0.0, 0.0, 1.0};
    GLfloat light1_specular [] = {1.0, 1.0, 1.0, 1.0};

    GLfloat light2_ambient [] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light2_diffuse [] = {0.0, 1.0, 0.0, 1.0};
    GLfloat light2_specular [] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light2_position [] = {viewerf[0], viewerf[1], viewerf[2], 1.0};
    GLfloat spot_direction [] = {-2,-2,-2};

    GLfloat teto[] = { 0.0, 9.0, 0.0, 1.0 };
    GLfloat white_light[ ] = { 1.0, 1.0, 1.0, 0.0 };

    GLfloat mat_ambient[ ] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_diffuse[ ] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[ ] = { 0.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[ ] = { 1.0, 1.0, 1.0, 1.0 };


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    //glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2f );
     glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glLightfv (GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv (GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2);
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.03);

    glLightfv (GL_LIGHT2, GL_AMBIENT, light2_ambient);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv (GL_LIGHT2, GL_SPECULAR, light2_specular);
    glLightfv (GL_LIGHT2, GL_POSITION, viewerf);
     glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.4);
    glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5);
    glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.06);
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF, 10.0);
    glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, 0);

}

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

        stringstream ss2;
        ss2 << num_tri;
        std::string s2 = ss2.str ();
        char* char2_type = (char*) s2.c_str();
        glColor3f(0.0, 0.0, 0.0);
        renderBitmapString(44,690,(void *)font,"Faces: ");
        glColor3f(1.0, 0.0, 0.0);
        renderBitmapString(66,690,(void *)font,char2_type);

        stringstream ss3;
        ss3 << FPS;
        std::string s3 = ss3.str ();
        char* char3_type = (char*) s3.c_str();
        renderBitmapString(22,690,(void*)font, char3_type);
        glColor3f(0.0, 0.0, 0.0);
        renderBitmapString(5,690,(void*)font, "FPS: ");

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

void tri(vertice a, vertice b, vertice c, int ind, vertice ta, vertice tb, vertice tc, vertice na, vertice nb, vertice nc) {
    glColor4f(obj[ind].cor[0], obj[ind].cor[1], obj[ind].cor[2], obj[ind].cor[3]);
    glPushMatrix();
    glTranslatef(obj[ind].transX, obj[ind].transY, obj[ind].transZ);
    glScalef(obj[ind].scaleX, obj[ind].scaleY, obj[ind].scaleZ);
    glRotatef(obj[ind].rotAngle, obj[ind].rotX, obj[ind].rotY, obj[ind].rotZ);
    if(!wire){

        if(luz1==true)
            glColor3f(1.0,1.0,1.0);
        else
            glColor3f(0.5,0.5,0.5);

       glBegin(GL_TRIANGLES);
            glNormal3f(na.x, na.y, na.z);
            glTexCoord3f(ta.x, ta.y, ta.z);
            glVertex3f(a.x, a.y, a.z);
            glNormal3f(nb.x, nb.y, nb.z);
            glTexCoord3f(tb.x, tb.y, tb.z);
            glVertex3f(b.x, b.y, b.z);
            glNormal3f(nc.x, nc.y, nc.z);
            glTexCoord3f(tc.x, tc.y, tc.z);
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
        tri(*obj[x].faces[i].um, *obj[x].faces[i].dois, *obj[x].faces[i].tres, x,
            *obj[x].texturas[i].um, *obj[x].texturas[i].dois, *obj[x].texturas[i].tres,
            *obj[x].normais[i].um, *obj[x].normais[i].dois, *obj[x].normais[i].tres);
    }
}

void desenhaPlano() {
	glBegin(GL_QUADS);
		glVertex3f(2.0f, 0.0f,  0.0f);
		glVertex3f(0.0f, 0.0f,  2.0f);
		glVertex3f(0.0f,  2.0f,  2.0f);
		glVertex3f(2.0f,  2.0f,  0.0f);
	glEnd();
}

char verificaTipoFace(string str){
    size_t posi = str.find('//', 0);
    if(posi == string::npos) return 'v';
    size_t barra1 = posi;
    posi = str.find('//',posi+1);
    if(posi-barra1 == 1) return 'n';
    else if(posi == string::npos) return 't';
    else return 'c';
}

void leOBJ(string nome){
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

    string line;
    std::string::size_type sz;   // alias of size_t

    for (line; getline(arquivo, line); )
    {
        cout << "linha: " << line  << endl;
        if(line[0]=='v' && line[1]==' '){
            cout << "vertice\n";

            int comeco = 2;
            size_t posi = line.find(' ', comeco);
            int coord = 0;
            float coordenadas[3];
            while(posi != string::npos)
            {
                coordenadas[coord] = stof(line.substr(comeco, posi-comeco), &sz);
                comeco = posi+1;
                posi = line.find(' ',posi+1);
                coord++;
            }
            coordenadas[coord] = stof(line.substr(comeco, posi-comeco), &sz);

            listaVertices.push_back(vertice(coordenadas[0], coordenadas[1], coordenadas[2]));

        }
        else if(line[0]=='v' && line[1]=='t'){
            cout << "textura\n";
        }
        else if(line[0]=='v' && line[1]=='n'){
            cout << "normal\n";
        }
        else if(line[0]=='f' && line[1]==' '){
            cout << "face\n";

            int comeco = 2;
            size_t posi = line.find(' ', comeco);
            int pontos = 0;
            while(posi != string::npos)
            {
                cout << line.substr(comeco, posi-comeco) << endl;
                char tipoFace = verificaTipoFace(line.substr(comeco, posi-comeco));
                if(tipoFace == 'v'){
                    cout << "so vertices\n";

                } else if(tipoFace == 'c'){
                    cout << "completo\n";

                } else if(tipoFace == 'n'){
                    cout << "normais\n";

                } else if(tipoFace == 't'){
                    cout << "texturas\n";

                }
                comeco = posi+1;
                posi = line.find(' ',posi+1);
                pontos++;
            }
            cout << line.substr(comeco, posi-comeco) << endl;

        }


    }



    /*
    size_t posi = line.find(' ', 0);
    int espacos = 0, comeco = 0;
    vector<size_t> positions;
    while(posi != string::npos)
    {
        positions.push_back(posi);
        espacos++;
        cout << line.substr(comeco, posi) << endl;
        posi = line.find(' ',posi+1);
    }
    int num_faces = espacos + 1;
    */

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
    double t1, t2, t3;

	while(!arquivo.eof()){

		arquivo>>tipo;
		if(tipo == "v"){
            arquivo>>x;
            arquivo>>y;
            arquivo>>z;

            vertice * vertice_aux = new vertice(x, y, z);
            listaVertices.push_back(vertice(x, y, z));

		}
		else if(tipo == "vn"){
            arquivo>>x;
            arquivo>>y;
            arquivo>>z;

            vertice * vertice_aux = new vertice(x, y, z);
            listaNormais.push_back(vertice(x, y, z));

		}
		else if(tipo == "vt"){
            arquivo>>x;
            arquivo>>y;
            arquivo>>z;

            vertice * vertice_aux = new vertice(x, y, z);
            listaTexturas.push_back(vertice(x, y, z));
		}
		else if(tipo == "f"){
            std::string::size_type sz;   // alias of size_t
            num_tri++;

            int pos = s1.find('/');
            indice_v1 = std::stoi( s1.substr(0, pos), &sz, 10 );
            string subT1 = s1.substr(pos+1);
            pos = s2.find('/');
            indice_v2 = std::stoi( s2.substr(0, pos), &sz, 10 );
            string subT2 = s2.substr(pos+1);
            pos = s3.find('/');
            indice_v3 = std::stoi( s3.substr(0, pos), &sz, 10 );
            string subT3 = s3.substr(pos+1);

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

            // ------ TEXTURA ------- //
            pos = subT1.find('/');
            indice_v1 = std::stoi( subT1.substr(0, pos), &sz, 10 );
            string subN1 = subT1.substr(pos+1);
            pos = subT2.find('/');
            indice_v2 = std::stoi( subT2.substr(0, pos), &sz, 10 );
            string subN2 = subT2.substr(pos+1);
            pos = subT3.find('/');
            indice_v3 = std::stoi( subT3.substr(0, pos), &sz, 10 );
            string subN3 = subT3.substr(pos+1);

            if(indice_v1 < 0) {
                indice_v1 = listaTexturas.size() + indice_v1;
            }
            if(indice_v2 < 0) {
                indice_v2 = listaTexturas.size() + indice_v2;
            }
            if(indice_v3 < 0) {
                indice_v3 = listaTexturas.size() + indice_v3;
            }

            vertice * vt1 = new vertice(listaTexturas[indice_v1].x, listaTexturas[indice_v1].y, listaTexturas[indice_v1].z);
            vertice * vt2 = new vertice(listaTexturas[indice_v2].x, listaTexturas[indice_v2].y, listaTexturas[indice_v2].z);
            vertice * vt3 = new vertice(listaTexturas[indice_v3].x, listaTexturas[indice_v3].y, listaTexturas[indice_v3].z);

            face * face_tex = new face(*vt1, *vt2, *vt3);
            obj[objs-1].texturas.push_back(*face_tex);

            // ------ NORMAIS ------- //
            pos = subN1.find('/');
            indice_v1 = std::stoi( subN1.substr(0, pos), &sz, 10 );
            pos = subN2.find('/');
            indice_v2 = std::stoi( subN2.substr(0, pos), &sz, 10 );
            pos = subN3.find('/');
            indice_v3 = std::stoi( subN3.substr(0, pos), &sz, 10 );

            if(indice_v1 < 0) {
                indice_v1 = listaNormais.size() + indice_v1;
            }
            if(indice_v2 < 0) {
                indice_v2 = listaNormais.size() + indice_v2;
            }
            if(indice_v3 < 0) {
                indice_v3 = listaNormais.size() + indice_v3;
            }

            vertice * vn1 = new vertice(listaNormais[indice_v1].x, listaNormais[indice_v1].y, listaNormais[indice_v1].z);
            vertice * vn2 = new vertice(listaNormais[indice_v2].x, listaNormais[indice_v2].y, listaNormais[indice_v2].z);
            vertice * vn3 = new vertice(listaNormais[indice_v3].x, listaNormais[indice_v3].y, listaNormais[indice_v3].z);

            face * face_nor = new face(*vn1, *vn2, *vn3);
            obj[objs-1].normais.push_back(*face_nor);


		}

	}

}

void mouseMotion(int x, int y)
{
    if( x < LARGURA_JANELA - LARGURA_JANELA/3){
        if(x - Xinicial > 0)
            viewer[0]+=0.05;
        if(x - Xinicial < 0)
            viewer[0]-=0.05;
        if(y - Yinicial > 0)
            viewer[1]+=0.05;
        if(y - Yinicial < 0)
            viewer[1]-=0.05;
/*
    if(x - Xinicial > 0)
        Yaw+=5;
    if(x - Xinicial < 0)
        Yaw-=5;
    if(y - Yinicial > 0)
        Pitch+=5;
    if(y - Yinicial < 0)
        Pitch-=5;

    cout<<"Yaw"<<endl;

    direcao.x = cos(Pitch) * cos(Yaw);
    direcao.y = sin(Pitch);
    direcao.z = cos(Pitch) * sin(Yaw);

    direita = vetorial(topoMundo, direcao);
    cima = vetorial(direcao, direita);
    up[0] = cima.x;
    up[1] = cima.y;
    up[0] = cima.z;
    focus[0] = posicao.x - direcao.x;
    focus[1] = posicao.y - direcao.y;
    focus[2] = posicao.z - direcao.z;

*/
    }
}

void mouseHandler(int button, int state, int x, int y){

    cout<<"X: "<<x<<" Y: "<<y<<endl;
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
                        //leObj(obj[arq].nome);
                        leOBJ(obj[arq].nome);
                        obj[arq].nome = obj[arq].nome;
                        importado[arq] = true;
                        arq ++;
                        selecionado = 0;
                    }
                }
            }
            else{
                Xinicial = x;
                Yinicial = y;
                //Zinicial = z;
                selecionado = 0;

            }
        }
        else if (state == GLUT_UP) {
            Xinicial = x;
            Yinicial = y;
        }
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();

    glViewport((LARGURA_JANELA/3)*2, 0, LARGURA_JANELA, ALTURA_JANELA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, LARGURA_JANELA/3, ALTURA_JANELA, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

         glDisable(GL_LIGHT0);
          glDisable(GL_LIGHT1);
           glDisable(GL_LIGHT2);
              //glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    desenhaMenuLateral();

     glEnable(GL_LIGHTING);
    if(luz1==true)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
    if(luz2==true)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
    if(luz3==true)
        glEnable(GL_LIGHT2);
    else
        glDisable(GL_LIGHT2);


    glEnable(GL_COLOR_MATERIAL);
    //glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);


    glViewport(0, 0, (LARGURA_JANELA/3)*2, ALTURA_JANELA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2.0, 2.0, -2.0, 2.0, 1.0, 100.0);
    //gluPerspective(70.0,1.0,2.0,100.0);
    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    focus[0],focus[1],focus[2],                           // ponto de interesse (foco)
    up[0],up[1],up[2]);                          // vetor de "view up"
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    desenhaEixos();

    //glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    for(int i=0; i<arq; i++){
        if(importado[i] == true){
            //glEnable(GL_CULL_FACE);
            //glCullFace(GL_FRONT);
            desenhaObj(i);
        }
    }
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    glColor4f(1,0.5,0.7,0.5);
    desenhaPlano();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboardHandler(unsigned char key, int x, int y){
    if (key == 27) exit(0); //ESC

    if (key == '1')
    {
        luz1= (luz1==0) ? 1 : 0;
    }
    if (key == '2')
    {
        luz2= (luz2==0) ? 1 : 0;
    }
    if (key == '3')
    {
        luz3= (luz3==0) ? 1 : 0;
    }
    if (key == '0')
    {

       luz1 = false;
       luz2 = false;
       luz3 = false;

    }

   //cout<< "ASCII de "<<key<< ": "<<(int)key << endl;

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
                //leObj(obj[arq].nome);
                leOBJ(obj[arq].nome);
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
        if (key == 'a' || key == 'A') {
            focus[0] += 0.5;
        }
        if (key == 'd'|| key == 'D') {
            focus[0] -= 0.5;
        }
        if (key == 'e') wire = true; //empty
        if (key == 'f') wire = false; //full
        if (key == 's' || key == 'S') {
            viewer[0] += 0.5;
            viewer[1] += 0.5;
            viewer[2] += 0.5;
            viewerf[0] += 0.5;
            viewerf[1] += 0.5;
            viewerf[2] += 0.5;
        }
        if (key == 'w' || key == 'W' ) {
            viewer[0] -= 0.5;
            viewer[1] -= 0.5;
            viewer[2] -= 0.5;
            viewerf[0] -= 0.5;
            viewerf[1] -= 0.5;
            viewerf[2] -= 0.5;
        }


    }
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
    posicao.setP(0,0,10);
    cima.setP(0,1,0);
    direcao.setP(0,0,1);
    direita = vetorial(cima,direcao);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutInitWindowPosition(5, 50);
    glutCreateWindow("Blender HD");
    init();
    initTexture();
    initLight();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);
    glutMouseFunc(mouseHandler);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);

    oldTime = glutGet(GLUT_ELAPSED_TIME);

	while(true){
		newTime = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = newTime - oldTime;
		quadros++;
		if(deltaTime >= 1000){
            FPS=quadros;
            oldTime = newTime;
            quadros = 0;
		}
        glutMainLoopEvent();
	}

}
