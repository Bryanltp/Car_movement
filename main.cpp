#include <iostream>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415
#include "CarregarArquivo.cpp"
using namespace std;

CarregarArquivo automovel;
GLubyte black[256][256][3];
GLubyte piso1[256][256][3];
GLuint textura_id;
GLuint textura_id2;

int Rot_carro = 0;
float rotY = 0;
int camera = 0;
float Trans_carro_x = 0.0, Trans_carro_z = 0.0, velocidade = 0.0;
float camera_x = 0.0, camera_z = -13.0;


void Inicializa(void){
  glClearColor (1.0, 1.0, 1.0, 0.0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(100, 1, 0.5, 500);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0,6,13, 0,0,0, 0,1,0);
  automovel.Carregar("automovel.obj");
  printf("Load ok");
   try
    {
        ifstream arq("piso1.bmp" ,ios::binary);
        char c;
        if(!arq)
            cout << "Erro ao abrir";

        for(int i = 0; i < 0 ; i++)
            c = arq.get();
        for(int i = 0; i < 256 ; i++)
            for(int j = 0; j < 256 ; j++)
            {
                c = arq.get();
                piso1[i][j][2] = c;
                c =  arq.get();
                piso1[i][j][1] = c ;
                c =  arq.get();
                piso1[i][j][0] = c;
            }

        arq.close();
        arq.clear();
    }
    catch(...)
    {
        cout << "Erro ao ler imagem" << endl;
    }
     glGenTextures(1,&textura_id);
     printf("%d", textura_id);

    glBindTexture(GL_TEXTURE_2D, textura_id);

    // Envia a textura para uso pela OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB,GL_UNSIGNED_BYTE, piso1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    try
    {
        ifstream arq("black.bmp" ,ios::binary);
        char c;
        if(!arq)
            cout << "Erro ao abrir";

        for(int i = 0; i < 0 ; i++)
            c = arq.get();
        for(int i = 0; i < 256 ; i++)
            for(int j = 0; j < 256 ; j++)
            {
                c = arq.get();
                black[i][j][2] = c;
                c =  arq.get();
                black[i][j][1] = c ;
                c =  arq.get();
                black[i][j][0] = c;
            }

        arq.close();
        arq.clear();
    }
    catch(...)
    {
        cout << "Erro ao ler imagem" << endl;
    }
     glGenTextures(1,&textura_id2);
     printf("%d", textura_id2);

    glBindTexture(GL_TEXTURE_2D, textura_id2);

    // Envia a textura para uso pela OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB,GL_UNSIGNED_BYTE, black);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void DefineIluminacao (void){
    GLfloat luzAmbiente[4]= {0.1,0.1,0.1,1.0};
    GLfloat luzDifusa[4]= {0.7,0.7,0.7,1.0}; // "cor"
    GLfloat luzEspecular[4]= {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicaoLuz[4]= {0.0, 0.0, 13.0, 1.0};
    GLfloat especularidade[4]= {1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void Piso (float escala, float altura){
 glColor3f(0.0, 1.0, 0.0);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, textura_id);
 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 glPushMatrix();
   glTranslatef(0.0, altura, 0.0); //translação depois da escala
   glScalef(escala, escala, escala); //escala na origem
   glBegin(GL_POLYGON);
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0);
    glVertex3f(100.0, 0.0, 100.0);
    //glTexCoord2f(1,0);
    glTexCoord2f(10,0);
    glVertex3f(-100.0, 0.0, 100.0);
    //glTexCoord2f(1,1);
    glTexCoord2f(10,10);
    glVertex3f(-100.0, 0.0, -100.0);
    //glTexCoord2f(0,1);
    glTexCoord2f(0,10);
    glVertex3f(100.0, 0.0, -100.0);
   glEnd();
 glPopMatrix();
 glDisable(GL_TEXTURE_2D);

}

void automovelWireFrame(void)  {
    glPushMatrix();
    for (unsigned int j = 0; j < (automovel.faces).size(); ++j ){
        glBegin ( GL_LINE_LOOP );
         for (unsigned int i = 0; i < (automovel.faces[j]).size() ; ++i )        {
            GLfloat vert[3] = {(automovel.vertices[automovel.faces[j][i][0]][0]),(automovel.vertices[automovel.faces[j][i][0]][1]),(automovel.vertices[automovel.faces[j][i][0]][2])};
            GLfloat nor[3] =  {(automovel.normais[automovel.faces[j][i][2]][0]),(automovel.normais[automovel.faces[j][i][2]][1]),(automovel.normais[automovel.faces[j][i][2]][2])};
            GLfloat tex[2] =  {automovel.texturas[automovel.faces[j][i][1]][0],automovel.texturas[automovel.faces[j][i][1]][1]};
            glVertex3fv ( vert );
            glNormal3fv (nor);
            glTexCoord2fv(tex);
         }
        glEnd( );
    }
    glPopMatrix();
}

void automovelSolid(int rotY,float trans_x,float trans_z)  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura_id2);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPushMatrix();

    glTranslatef(trans_x,0.0,trans_z);
    //glRotatef(rot_y,0.0,1.0,0.0);
    glRotatef(rotY,0,1,0);


    for (unsigned int j = 0; j < (automovel.faces).size(); ++j )
    {
        glBegin ( GL_POLYGON );
        for (unsigned int i = 0; i < (automovel.faces[j]).size() ; ++i )        {
            GLfloat vert[3] = {(automovel.vertices[automovel.faces[j][i][0]][0]),(automovel.vertices[automovel.faces[j][i][0]][1]),(automovel.vertices[automovel.faces[j][i][0]][2])};
            GLfloat nor[3] =  {(automovel.normais[automovel.faces[j][i][2]][0]),(automovel.normais[automovel.faces[j][i][2]][1]),(automovel.normais[automovel.faces[j][i][2]][2])};
            GLfloat tex[2] = {automovel.texturas[automovel.faces[j][i][1]][0],automovel.texturas[automovel.faces[j][i][1]][1]};
            glVertex3fv (vert);
            glTexCoord2fv(tex);
            glNormal3fv (nor);
        }
        glEnd( );
    }

    glPopMatrix();
}

void Desenha(void){
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if(camera == 0){
        gluLookAt(0,100,13, 0,0,0, 0,1,0);
    }else{
        gluLookAt(camera_x, 3, camera_z,Trans_carro_x,0,Trans_carro_z,0,1,0);
    }
  glPushMatrix();
  glRotatef(rotY,0,1,0);
  Piso(1.0, -2.0);
  automovelSolid(Rot_carro, Trans_carro_x, Trans_carro_z);
  glPopMatrix();
  glutSwapBuffers();
}

void Teclado(unsigned char key, int x, int y){
  if(key == 'a' || key == 'A'){
    Rot_carro = (Rot_carro + 5) % 360;
    glutPostRedisplay();
  }
  else if(key == 'd' || key == 'D'){
    Rot_carro = (Rot_carro - 5) % 360;
    glutPostRedisplay();
  }
  else if(key == 'w' || key == 'W'){
    if (velocidade == 0.0) velocidade = 0.5;
    else velocidade = 0;
    glutPostRedisplay();
  }
  else if(key == 's' || key == 'S'){
    if (velocidade == 0.0) velocidade = -0.5;
    else velocidade = 0;
    glutPostRedisplay();
  }
  else if(key == 'e' || key == 'E'){
    if(camera == 0) camera = 1;
    else camera = 0;
    glutPostRedisplay();
  }
}

void idle(){
    Trans_carro_x = Trans_carro_x + velocidade*sin(Rot_carro*PI/180);
    Trans_carro_z = Trans_carro_z + velocidade*cos(Rot_carro*PI/180);

    camera_x = Trans_carro_x + 13*sin((Rot_carro+180)*PI/180);
    camera_z = Trans_carro_z + 13.0*cos((Rot_carro+180)*PI/180);

    glutPostRedisplay();
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize (800, 800);
  glutInitWindowPosition (0,0);
  glutCreateWindow ("Trabalho Pratico: Nave");
  Inicializa();
  glutDisplayFunc(Desenha);
  glutKeyboardFunc(Teclado);
  glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}
