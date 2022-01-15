#include "player.h"
#include "tiro.h"
#include <cmath>
#include <iostream>
#include <cstdlib>


float cabecaRadius = 1;
float troncoHeight  = 3.8;
float troncoWidth  = 2;
float bracoHeight  = 3;
float bracoWidth  = 0.5;
float pernaHeight  = 1.8;
float pernaWidth  = 0.5;

float jumpUpDistanceTraveled = 0;
float maxJumpHeight = 0 ; //TODO tamanho do corpo do boneco
float previousPosY;

int calls1 = 0;
int calls2 = 0;

void Player::DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B,GLfloat A)
{
    glBegin(GL_QUADS);
    glColor4f(R,G,B,A);// Face posterior
    glVertex3f(width/2, height, 1.0);
    glVertex3f(-width/2, height, 1.0);
    glVertex3f(-width/2, 0.0, 1.0);
    glVertex3f(width/2, 0.0, 1.0);
    glEnd();

}
void Player::DesenhaCabeca(GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(facingRight)
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
    if(facingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);

    DesenhaRect(bracoHeight,bracoWidth,1,1,0,1);
    glPopMatrix();
}

void Player::DesenhaPerna(GLfloat x, GLfloat y, GLfloat pEtheta1, GLfloat pEtheta2, GLfloat pDtheta1, GLfloat pDtheta2)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(facingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    glRotatef(pEtheta1,0,0,1);
    DesenhaRect(pernaHeight,pernaWidth,0,1,0,1); //desenhando primeira perna esquerda
    glTranslatef(0,-pernaHeight,0);
    glRotatef(pEtheta2,0,0,1);
    DesenhaRect(pernaHeight,pernaWidth,1,1,0,1); //desenhando segunda perna esquerda
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x,y,0);
    if(facingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    glRotatef(pDtheta1,0,0,1);
    DesenhaRect(-pernaHeight,pernaWidth,1,0,0,1); //desenhando primeira perna direita
    glTranslatef(0,-pernaHeight,0);
    glRotatef(pDtheta2,0,0,1);
    DesenhaRect(-pernaHeight,pernaWidth,0,0,0,1); //desenhando segunda perna direita*/
    glPopMatrix();
}

void Player::DesenhaCollider() {
    glPushMatrix();
    glTranslatef(0,-troncoHeight,0);
    DesenhaRect(cabecaRadius*2+troncoHeight+pernaHeight*2, troncoWidth, 1,1,1,0);
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
    if(facingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    DesenhaCollider();
    DesenhaRect(troncoHeight,troncoWidth,0,1,0.3,1); //desenhando base
    DesenhaCabeca(0, troncoHeight + cabecaRadius /*offset*/, cabecaRadius,0,1,0.3);
    DesenhaBraco(0,troncoHeight/2,bTheta);
    DesenhaPerna(0, 0,pETheta1,pETheta2, pDTheta1, pDTheta2);

    glPopMatrix();
}

Tiro* Player::Atira() {

    float angleSumTheta1 = (bTheta) * M_PI / 180;

    if(facingRight) {
        float bulletX = gX - bracoHeight * sin(angleSumTheta1);
        float bulletY = gY + troncoHeight / 2 + bracoHeight * cos(angleSumTheta1);
        return new Tiro(bulletX, bulletY, 90 + bTheta, facingRight);
    }
    else{
        float bulletX = gX + bracoHeight * sin(angleSumTheta1) ;
        float bulletY = gY + troncoHeight/2 + bracoHeight * cos(angleSumTheta1);
        return new Tiro(bulletX, bulletY, -90 + bTheta, facingRight);
    }

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

void Player::MoveEmX(GLfloat dx, GLfloat timeDifference)
{
    if(dx > 0){
        facingRight = true;
    }
    if(dx < 0){
        facingRight = false;
    }

    gX += dx * timeDifference;

}

void Player::MoveEmY(GLfloat dy)
{
    float speed = 25;
    maxJumpHeight = ((cabecaRadius*2+troncoHeight+pernaHeight*2) * 3);

    if(fabs(jumpUpDistanceTraveled) <= maxJumpHeight){
        gY += dy * speed;
        jumpUpDistanceTraveled += previousPosY - gY;
    }
    previousPosY = gY;
}
void Player::FreeFall(GLfloat dy)
{
    float speed = 45;
    gY -= dy * speed;
    previousPosY = gY;
}

void Player::ResetJumpDistance(){
    jumpUpDistanceTraveled = 0;
}

void Player::MoveEmMenosY(GLfloat dy, bool jumping)
{
    if(!jumping) return;

    float speed = dy * 75;
    //float speed = dy * 3;
    jumpUpDistanceTraveled += speed;

    gY -= speed;

}