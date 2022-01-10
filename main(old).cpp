#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "player.h"

void display();
void reshape(int,int);
void timer(int);

//Player player;

void init(){
    glClearColor(0.0,0.0,0.0,1.0); //deixa o fundo preto
}

int main(int argc, char**argv) {

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(200,100);
    glutInitWindowSize(500,500);

    glutCreateWindow("Trab 2D - Lucas Rios");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer,0);
    init();

    glutMainLoop();
}

float x_position = -10.0;
int state = 1;

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glPointSize(10.0);
    glBegin(GL_POLYGON);

    glVertex2f(x_position,1.0);
    glVertex2f(x_position,-1.0);
    glVertex2f(x_position + 2.0,-1.0);
    glVertex2f(x_position + 2.0,1.0);


    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(-1.0,1.0);
    glVertex2f(-1,-1.0);
    glVertex2f(1,-1.0);
    glVertex2f(1,1.0);
    glEnd();

    glTranslatef(x_position,0,0);
    glColor3ub(254,0,0);
    glBegin(GL_LINES);
    glVertex3f(-500.0,0.0,0.0);
    glVertex3f(500.0,0.0,0.0);
    glEnd();

    glColor3ub(31,255,0);
    glBegin(GL_LINES);
    glVertex3f(0.0,-500.0,0.0);
    glVertex3f(0.0,500.0,0.0);
    glEnd();
    glLoadIdentity();

    glutSwapBuffers();
}

void reshape(int w,int h){
    glViewport(0,0,(GLsizei) w,(GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10,10,-10,10);
    glMatrixMode(GL_MODELVIEW);

}

void timer(int){
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);


    glMatrixMode(GL_PROJECTION); // Select the projection matrix
    glLoadIdentity();

    glTranslatef(-x_position/10,0,0);


    gluOrtho2D(-10,10,-10,10);
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();

    switch (state) {
        case 1:
            if(x_position <8){
                x_position += 0.15;
            }else{
                state = -1;
            }
            break;
        case -1:
            if(x_position > -10){
                x_position -= 0.15;
            }
            else{
                state = 1;
            }
            break;
    }



}