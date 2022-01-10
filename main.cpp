#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "player.h"
#include "tinyxml2.h"
#include "Cenario.h"
#define INC_KEY 1
#define INC_KEYIDLE 0.01

//Key status
int keyStatus[256];

Player Player;
Cenario Cenario;
float camMove = 0;

// Window dimensions
const GLint Width = 500;
const GLint Height = 500;

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    Cenario.Desenha();
    Player.Desenha();


   /* glPushMatrix();
    glLoadIdentity();
    glTranslatef(364.13/2,-91.65/2,0);
    glBegin(GL_QUADS);

    glColor3f(0,0,0);// Face posterior
    //glNormal3f(0.0, 0.0, 1.0);	// Normal da face
    glVertex2f(364.13/2, 91.65/2);
    glVertex2f(-364.13/2,  91.65/2);
    glVertex2f(-364.13/2, -91.65/2);
    glVertex2f(364.13/2, -91.65/2);
    glEnd();*/
/*
    //###############
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0,0,0);
    glBegin(GL_QUADS);

    glColor3f(0,0,0);// Face posterior
    //glNormal3f(0.0, 0.0, 1.0);	// Normal da face
    glVertex2f(364.13/2, 91.65);
    glVertex2f(-364.13/2,  91.65);
    glVertex2f(-364.13/2, 0);
    glVertex2f(364.13/2, 0);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(20.16,-180.7007,0);
    glBegin(GL_QUADS);

    glColor3f(0,0,1);// Face posterior

    glVertex2f(5.85/2, 6.61);
    glVertex2f(-5.85/2,  6.61);
    glVertex2f(-5.85/2, 0);
    glVertex2f(5.85/2, 0);
    glEnd();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(195.45644,-95.349701,0);
    glBegin(GL_QUADS);

    glColor3f(0,0,1);// Face posterior

    glVertex2f(6.0476213/2, 61.799107);
    glVertex2f(-6.0476213/2,  61.799107);
    glVertex2f(-6.0476213/2, 0);
    glVertex2f(6.0476213/2, 0);
    glEnd();

    glPopMatrix();

    //########################
*/

    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
            keyStatus[(int)('a')] = 1; //Using keyStatus trick
            break;
        case 'd':
        case 'D':
            keyStatus[(int)('d')] = 1; //Using keyStatus trick
            break;

        case 27 :
            exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
        keyStatus[i] = 0;
}

void init(int w, int h)
{
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    int R  = 1;
    int G = 0.2;
    int B = 1;
    glClearColor(R,G,B, 1);
    glMatrixMode(GL_PROJECTION); // Select the projection matrix

    //glTranslatef(4,3.68,0);
    //gluOrtho2D(-40,40,40,-40);
    gluOrtho2D(-250,250,-250,250);
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();
}

void idle(void)
{
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime, timeDiference;
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDiference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;


    double inc = INC_KEYIDLE;
    //Treat keyPress
    if(keyStatus[(int)('a')])
    {
        camMove++;
        Player.MoveEmX(-inc);
    }
    if(keyStatus[(int)('d')])
    {
        camMove--;
        Player.MoveEmX(inc);
        //mover personagem e camera
    }

    glMatrixMode(GL_PROJECTION); // Select the projection matrix
    glLoadIdentity();

    glTranslatef(camMove/45.9 +3.427/*offset*/,3.08/*offset*/,0);

    gluOrtho2D(-45.9,45.9,-45.9,45.9);
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,150);
    glutCreateWindow("Trab 2D - Lucas Rios");

    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);

    glutReshapeFunc(init);

    glutMainLoop();

    return 0;
}

