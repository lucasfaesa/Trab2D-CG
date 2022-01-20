#include "enemy.h"
#include "tiro.h"
#include "tinyxml2.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

float enemyCabecaRadius = 1;
float enemyTroncoHeight  = 3.8;
float enemyTroncoWidth  = 2;
float enemyBracoHeight  = 3;
float enemyBracoWidth  = 0.5;
float enemyPernaHeight  = 1.8;
float enemyPernaWidth  = 0.5;
bool enemyPreviousPE1FacingRight;
bool enemyPreviousPD1FacingRight;
float enemyPreviousPosY;

bool enemyRotatePernaD1Backwards;
bool enemyRotatePernaE1Backwards;
int contEnemies = 0;

void Enemy::DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B,GLfloat A)
{
    glBegin(GL_QUADS);
    glColor4f(R,G,B,A);// Face posterior
    glVertex3f(width/2, height, 1.0);
    glVertex3f(-width/2, height, 1.0);
    glVertex3f(-width/2, 0.0, 1.0);
    glVertex3f(width/2, 0.0, 1.0);
    glEnd();

}
void Enemy::DesenhaCabeca(GLfloat x, GLfloat y, GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(enemyFacingRight)
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

void Enemy::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(theta1,0,0,1);
    if(enemyFacingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);

    DesenhaRect(enemyBracoHeight,enemyBracoWidth,1,1,0,1);
    glPopMatrix();
}

void Enemy::DesenhaPerna(GLfloat x, GLfloat y, GLfloat pEtheta1, GLfloat pEtheta2, GLfloat pDtheta1, GLfloat pDtheta2)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    if(enemyFacingRight){ //TODO TROCAR PARA LOOP FOR
        if(enemyFacingRight)
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pEtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,0,1,0,1); //desenhando primeira perna esquerda
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pEtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,0,1); //desenhando segunda perna esquerda
        glPopMatrix();

        glPushMatrix();
        glTranslatef(x,y,0);
        if(enemyFacingRight)
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pDtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,0,0,1); //desenhando primeira perna direita
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pDtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,1,1); //desenhando segunda perna direita*/
        glPopMatrix();
    }else{
        glPushMatrix();
        glTranslatef(x,y,0);
        if(enemyFacingRight)
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pDtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,0,0,1); //desenhando primeira perna direita
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pDtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,1,1); //desenhando segunda perna direita*/
        glPopMatrix();

        if(enemyFacingRight)
            glScalef(1,1,1);
        else
            glScalef(-1,1,1);
        glRotatef(pEtheta1,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,0,1,0,1); //desenhando primeira perna esquerda
        glTranslatef(0,-enemyPernaHeight,0);
        glRotatef(pEtheta2,0,0,1);
        DesenhaRect(-enemyPernaHeight,enemyPernaWidth,1,1,0,1); //desenhando segunda perna esquerda
        glPopMatrix();
    }

}

void Enemy::DesenhaCollider() {
    glPushMatrix();
    glTranslatef(0,-enemyTroncoHeight,0);
    DesenhaRect(enemyCabecaRadius * 2 + enemyTroncoHeight + enemyPernaHeight * 2, enemyTroncoWidth, 1, 1, 1, 0);
    glPopMatrix();

}

void Enemy::DesenhaEnemy(GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2)
{
    glLoadIdentity();
    glFlush();

    glPushMatrix();
    glTranslatef(x,y,0);
    if(enemyFacingRight)
        glScalef(1,1,1);
    else
        glScalef(-1,1,1);
    DesenhaCollider();
    DesenhaRect(enemyTroncoHeight,enemyTroncoWidth,1,0,0,1); //desenhando base
    DesenhaCabeca(0, enemyTroncoHeight + enemyCabecaRadius /*offset*/, enemyCabecaRadius, 1, 0, 0);
    DesenhaBraco(0,enemyTroncoHeight/2,bTheta);
    DesenhaPerna(0, 0,pETheta1,pETheta2, pDTheta1, pDTheta2);

    glPopMatrix();



}

Tiro* Enemy::Atira() {

    float angleSumTheta1 = (bTheta) * M_PI / 180;

    if(enemyFacingRight) {
        float bulletX = gX - enemyBracoHeight * sin(angleSumTheta1);
        float bulletY = gY + enemyTroncoHeight / 2 + enemyBracoHeight * cos(angleSumTheta1);
        return new Tiro(bulletX, bulletY, 90 + bTheta, enemyFacingRight);
    }
    else{
        float bulletX = gX + enemyBracoHeight * sin(angleSumTheta1) ;
        float bulletY = gY + enemyTroncoHeight/2 + enemyBracoHeight * cos(angleSumTheta1);
        return new Tiro(bulletX, bulletY, -90 + bTheta, enemyFacingRight);
    }

}

void Enemy::GetEnemiesFromSvg() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(
            "C:/Users/lucas/Desktop/Trab2D/arena_teste.svg"); //TODO modificar para input externo via linha de comando futuramente

    tinyxml2::XMLElement *levelElement = doc.FirstChildElement("svg");

    for (tinyxml2::XMLElement *child = levelElement->FirstChildElement("circle");
         child != NULL; child = child->NextSiblingElement("circle")) {
        const char *pAttrX = child->Attribute("cx");
        const char *pAttrY = child->Attribute("cy");
        const char *pAttrR = child->Attribute("r");
        const char *pAttrFill = child->Attribute("fill");

        float iX = strtof(pAttrX, NULL);
        float iY = strtof(pAttrY, NULL);
        float iR = strtof(pAttrR, NULL);


        DesenhaEnemy(iX, -iY - 1.5 /*offset*/, -90, 30, 0, -30, 0);

        Enemy::AddEnemiesToArray(contEnemies, iX, -iY - 1.5, -90, 30, 0, -30, 0);

        contEnemies++;
    }
    contEnemies = 0;
}
void Enemy::DesenhaTodos() {

    for(int i =0; i<sizeof(enemiesObj); i++){
        if(enemiesObj[i].gY == 0) break;

        if(enemiesObj[i].canBeDrawn)
            DesenhaEnemy(enemiesObj[i].gX,enemiesObj[i].gY,enemiesObj[i].bTheta,enemiesObj[i].pETheta1,enemiesObj[i].pETheta2,enemiesObj[i].pDTheta1,enemiesObj[i].pDTheta2);
    }
}

void Enemy::RodaBraco(GLfloat inc)
{
    bTheta =  inc - Enemy::initialBTheta;
}

void Enemy::RodaPernaE1(GLfloat inc)
{
    if(enemyPreviousPE1FacingRight != enemyFacingRight){
        pETheta1 = pETheta1 * -1;
    }

    float maxRot = 30;
    float minRot = -30;

    float speed = 100;
    if(pETheta1 >= maxRot){
        if(enemyFacingRight)
            enemyRotatePernaE1Backwards = false;
        else
            enemyRotatePernaE1Backwards = true;
    }
    if(pETheta1 <= minRot){
        if(enemyFacingRight)
            enemyRotatePernaE1Backwards = true;
        else
            enemyRotatePernaE1Backwards = false;
    }

    if(enemyRotatePernaE1Backwards){
        pETheta1 += inc * speed;
    }else{
        pETheta1 -= inc * speed;
    }

    enemyPreviousPE1FacingRight = enemyFacingRight;
}

void Enemy::RodaPernaE2(GLfloat inc)
{
}

void Enemy::RodaPernaD1(GLfloat inc)
{
    if(enemyPreviousPD1FacingRight != enemyFacingRight){
        pDTheta1 = pDTheta1 * -1;
    }

    float maxRot = 30;
    float minRot = -30;

    float speed = 100;
    if(pDTheta1 >= maxRot){
        if(enemyFacingRight)
            enemyRotatePernaD1Backwards = true;
        else
            enemyRotatePernaD1Backwards = false;
    }
    if(pDTheta1 <= minRot){
        if(enemyFacingRight)
            enemyRotatePernaD1Backwards = false;
        else
            enemyRotatePernaD1Backwards = true;
    }

    if(enemyRotatePernaD1Backwards){
        pDTheta1 -= inc * speed;
    }else{
        pDTheta1 += inc * speed;
    }

    enemyPreviousPD1FacingRight = enemyFacingRight;

}

void Enemy::RodaPernaD2(GLfloat inc)
{
}

void Enemy::MoveEmX(GLfloat dx, GLfloat timeDifference)
{
    if(dx > 0){
        enemyFacingRight = true;
    }
    if(dx < 0){
        enemyFacingRight = false;
    }

    gX += dx * timeDifference;

}

/*void Enemy::MoveEmY(GLfloat dy, bool &isJumping)
{
    float speed = 25;
    enemymaxJumpHeight = ((CabecaRadius * 2 + troncoHeight + pernaHeight * 2) * 3);

    if(fabs(jumpUpDistanceTraveled) <= maxJumpHeight){
        gY += dy * speed;
        jumpUpDistanceTraveled += previousPosY - gY;
        isJumping = true;
    }else{
        isJumping = false;
    }
    previousPosY = gY;
}*/

/*void Enemy::FreeFall(GLfloat dy)
{
    float speed = 25;
    gY -= dy * speed;
    previousPosY = gY;
}*/

