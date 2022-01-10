#include "player.h"
#include <cmath>

float cabecaRadius = 1.5;
float troncoHeight  = 3.9;
float troncoWidth  = 2;
float bracoHeight  = 3;
float bracoWidth  = 0.5;
float pernaHeight  = 1.9;
float pernaWidth  = 0.5;
bool movingRight = true;


void Player::DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B)
{
    glBegin(GL_QUADS);
    glColor3f(R,G,B);// Face posterior
    glNormal3f(0.0, 0.0, 1.0);	// Normal da face
    glVertex3f(width/2, height, 1.0);
    glVertex3f(-width/2, height, 1.0);
    glVertex3f(-width/2, 0.0, 1.0);
    glVertex3f(width/2, 0.0, 1.0);
    glEnd();

}
void Player::DesenhaCabeca(GLfloat x, GLfloat y, GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(movingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);

    glColor3f(R,G,B);
    glBegin(GL_POLYGON);
    for(int i=0; i<20; i++){
        double angle = i *(2.0 * M_PI/20);
        glVertex2f(radius*cos(angle), radius*sin(angle));
    }

    glEnd();
    glPopMatrix();
}

void Player::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(theta1,0,0,1);
    if(movingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);

    DesenhaRect(bracoHeight,bracoWidth,1,1,0);
    glPopMatrix();
}

void Player::DesenhaPerna(GLfloat x, GLfloat y, GLfloat pEtheta1, GLfloat pEtheta2, GLfloat pDtheta1, GLfloat pDtheta2)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(movingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    glRotatef(pEtheta1,0,0,1);
    DesenhaRect(pernaHeight,pernaWidth,0,1,0); //desenhando primeira perna esquerda
    glTranslatef(0,-pernaHeight,0);
    glRotatef(pEtheta2,0,0,1);
    DesenhaRect(pernaHeight,pernaWidth,0,1,0); //desenhando segunda perna esquerda
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x,y,0);
    if(movingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    glRotatef(pDtheta1,0,0,1);
    DesenhaRect(pernaHeight,pernaWidth,1,0,0); //desenhando primeira perna direita
    glTranslatef(0,-pernaHeight,0);
    glRotatef(pDtheta2,0,0,1);
    DesenhaRect(pernaHeight,pernaWidth,1,0,0); //desenhando segunda perna direita
    glPopMatrix();
}


void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2)
{
    glLoadIdentity();

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

    glFlush();

    glPushMatrix();
    glTranslatef(x,y,0);
    if(movingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    DesenhaRect(troncoHeight,troncoWidth,0,1,0.3); //desenhando base
    DesenhaCabeca(0, troncoHeight + 1 /*offset*/, cabecaRadius,0,1,0.3);
    DesenhaBraco(0,troncoHeight/2,bTheta);
    DesenhaPerna(0,-pernaHeight,pETheta1,pETheta2, pDTheta1, pDTheta2);
    glPopMatrix();
}

void Player::RodaBraco(GLfloat inc)
{
    bTheta =  inc - Player::initialBTheta;
}

void Player::RodaPernaE1(GLfloat inc)
{
    float speed = 1.5;
    pETheta1 += inc * speed;
}

void Player::RodaPernaE2(GLfloat inc)
{
    float speed = 1.5;
    pETheta2 += inc * speed;
}

void Player::RodaPernaD1(GLfloat inc)
{
    float speed = 1.5;
    pDTheta1 += inc * speed;
}

void Player::RodaPernaD2(GLfloat inc)
{
    float speed = 1.5;
    pDTheta2 += inc * speed;
}

void Player::MoveEmX(GLfloat dx)
{
    if(dx > 0)
        movingRight = true;
    if(dx < 0)
        movingRight = false;
    float speed = dx * 100;
    gX += speed;
}