#ifndef MAIN_CPP_ENEMY_H
#define MAIN_CPP_ENEMY_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"

static bool enemyFacingRight = true;

class Enemy {
    GLfloat gX;
    GLfloat gY;
    GLfloat initialBTheta = 0;
    GLfloat bTheta = 0;
    GLfloat pETheta1 = 0;
    GLfloat pETheta2 = 0;
    GLfloat pDTheta1 = 0;
    GLfloat pDTheta2 = 0;


private:
    void DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B, GLfloat A);
    void DesenhaCabeca(GLfloat x, GLfloat y, GLfloat radius, GLfloat R,GLfloat G, GLfloat B);
    void DesenhaPerna(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3, GLfloat theta4);
    void DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1);
    void DesenhaCollider();
    void DesenhaEnemy(GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2);
    void DesenhaTodos();

public:

    void Desenha(){
        DesenhaTodos();
    };

    void RodaBraco(GLfloat inc);
    void RodaPernaE1(GLfloat inc);
    void RodaPernaE2(GLfloat inc);
    void RodaPernaD1(GLfloat inc);
    void RodaPernaD2(GLfloat inc);
    Tiro* Atira();
    void MoveEmX(GLfloat dx, GLfloat timeDifference);
    //void MoveEmY(GLfloat dy, bool &isJumping);
    //void FreeFall (GLfloat dy);
    /*void GetPos(GLfloat &xOut, GLfloat &yOut){
        xOut = gX;
        yOut = gY;
    };*/


};


#endif //MAIN_CPP_ENEMY_H
