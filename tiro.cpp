#include "tiro.h"
#include "player.h"
#include <math.h>
#include <iostream>

#define DISTANCIA_MAX 50

Player player1;

void Tiro::DesenhaCirc(GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R,G,B);

    glBegin(GL_POLYGON);
    for(int i=0; i<20; i++){
        double angle = i *(2.0 * M_PI/20);
        glVertex2f(radius*cos(angle), radius*sin(angle));
    }
    glEnd();
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x,y,0);
    DesenhaCirc(0.5,1,1,1);
    glPopMatrix();
}

void Tiro::Move(double timeDifference)
{
    /*if(Player1.ObtemDirection())
        glScalef(1,1,1);
    else*/
    gX += gVel * timeDifference * cos(gDirectionAng*M_PI/180);
    if(gFireRightDirection){
        gY += gVel * timeDifference * sin(gDirectionAng*M_PI/180);
    }else{
        gY -= gVel * timeDifference * sin(gDirectionAng*M_PI/180);
    }



    //std::cout << gX << std::endl;
   // std::cout << gY << std::endl;
}

bool Tiro::Valido()
{
    if(abs(gX - gXInit) >= DISTANCIA_MAX || abs(gY - gYInit) >= DISTANCIA_MAX)
        return false;
    else
        return true;
}
