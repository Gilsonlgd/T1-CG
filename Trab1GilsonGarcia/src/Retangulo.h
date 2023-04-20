#ifndef RETANGULO_H_INCLUDED
#define RETANGULO_H_INCLUDED

#include "Figura.h"
#include "constants.h"
#include "Circulo.h"
#include <stdio.h>

#define LEFT_SIDE 0
#define RIGHT_SIDE 1
#define TOP_SIDE 2
#define BOTTOM_SIDE 3
#define MIN_SIZE 5
// Limita a variação de ângulo a uma volta completa
#define MAX_ANGLE_DELTA = 360.0f;

using namespace std;

class Retangulo : public Figura {
    float width;
    float height;
    BoundingBtn* rotateBtn;

protected:

    void attBoundingButtonsPosition() {
        BoundingBtn* botao = boundingButtons[LEFT_SIDE];
        botao->setCoord(segmentCenter(vx[0], vx[3]) , segmentCenter(vy[0], vy[3]));

        botao = boundingButtons[RIGHT_SIDE];
        botao->setCoord(segmentCenter(vx[1], vx[2]) , segmentCenter(vy[1], vy[2]));

        botao = boundingButtons[TOP_SIDE];
        botao->setCoord(segmentCenter(vx[0], vx[1]) , segmentCenter(vy[0], vy[1]));

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(segmentCenter(vx[3], vx[2]) , segmentCenter(vy[3], vy[2]));
    }

    void drawBoundingBox() {
        CV::color(0,0,0);
        CV::translate(0, 0);
        CV::polygon(vx.data(), vy.data(), 4);
        attBoundingButtonsPosition();
        for (auto btn : boundingButtons) {
            btn->Render();
        }

        rotateBtn->Render();
    }

    void attPointsCoord() {
        float origemX = vx[0];
        float origemY = vy[0];

        vx[1] = origemX + width;
        vy[1] = origemY;

        vx[2] = origemX + width;
        vy[2] = origemY + height;

        vx[3] = origemX;
        vy[3] = origemY + height;
    }

    void attRotateBtnCoordinate() {
        float dx = vx[2] - vx[3];
        float dy = vy[2] - vy[3];
        float len = sqrt(dx*dx + dy*dy);
        dx /= len;
        dy /= len;

        float px_new = vx[2] + dx * 20;
        float py_new = vy[2] + dy * 20;
        rotateBtn->setCoord(px_new, py_new);
    }


public:
    using Figura::setVisible;

    Retangulo() : Figura(4) {
        width = 40;
        height = 40;
        offsetX = 0;
        offsetY = 0;
        rotateBtn = new BoundingBtn(0, 0, BOUNDING_BTN_SIZE, INDEX14);
        attPointsCoord();
    }

    void render() override {
        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(r,g,b);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), 4);
        if(selected) drawBoundingBox();
    }

    void setOffset(float x, float y) override {
        rotatePoints(getCenterX(), getCenterY(), -angle);
        float origemX = vx[0];
        float origemY = vy[0];

        offsetX = x - origemX;
        offsetY = y - origemY;
        rotatePoints(getCenterX(), getCenterY(), angle);  
    }

    void setVisible(float x, float y) override {
        vx[0] = x - getCenterX();
        vy[0] = y - getCenterY();

        attPointsCoord();
        visible = true;
        rotateBtn->setCoord(vx[2] + 20, vy[2]);
    }

    void setMousePosition(float mx, float my) override {
        rotatePoints(getCenterX(), getCenterY(), -angle);

        float xDif = vx[1] - vx[0];
        float yDif = vy[1] - vy[0];
        vx[0] = mx - offsetX;
        vy[0] = my - offsetY;
        vx[1] = mx + xDif - offsetX;
        vy[1] = my + yDif - offsetY;

        xDif = vx[2] - vx[3];
        yDif = vy[2] - vy[3];
        vx[3] = mx - offsetX;
        vy[3] = my - offsetY + height;
        vx[2] = mx + xDif - offsetX;
        vy[2] = my + yDif - offsetY + height;     

        rotatePoints(getCenterX(), getCenterY(), angle);  
        attRotateBtnCoordinate(); 
    }

    void rotate(float mx, float my) override {
        float pivotX = getCenterX();
        float pivotY = getCenterY();

        float CBx = rotateBtn->getCenterX() - pivotX;
        float CBy = rotateBtn->getCenterY() - pivotY;

        float CMx = mx - pivotX;
        float CMy = my - pivotY;

        float newAngle = angleRAD(CMx, CMy, CBx, CBy);

        if (abs(newAngle) > 180.0) {
            newAngle = newAngle > 0 ? newAngle - 360.0 : newAngle + 360.0;
        }
        
        angle += newAngle;
        if(angle <= -360 || angle >= 360) angle = 0;

        rotateBtn->rotate(pivotX, pivotY, newAngle);
        rotatePoints(pivotX, pivotY, newAngle);
    }

    void resize(float mx, float my) override {
        float pivotX = getCenterX();
        float pivotY = getCenterY();

        float mxC = mx;
        float myC = my;

        rotatePoint(mxC, myC, pivotX, pivotY, -angle);
        rotatePoints(pivotX, pivotY, -angle);
        attBoundingButtonsPosition();

        if (selectedBoundingButton == RIGHT_SIDE) {
            vx[1] = mxC;
            vx[2] = mxC;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            vx[0] = mxC; 
            vx[3] = mxC;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            vy[3] = myC; 
            vy[2] = myC;
        } else if (selectedBoundingButton == TOP_SIDE) {
            vy[0] = myC; 
            vy[1] = myC;
        } 
        
        width = vx[1] - vy[0]; 
        height = vy[3] - vy[0];

        rotatePoints(pivotX, pivotY, angle);
        attRotateBtnCoordinate();
        attBoundingButtonsPosition();
    }

    void resizeProportionally(float mx, float my) override{
        float pivotX = getCenterX();
        float pivotY = getCenterY();

        float mxC = mx;
        float myC = my;

        rotatePoint(mxC, myC, pivotX, pivotY, -angle);
        rotatePoints(pivotX, pivotY, -angle);
        attBoundingButtonsPosition();

        if (selectedBoundingButton == RIGHT_SIDE) {
            float distancia = mxC - boundingButtons[RIGHT_SIDE]->getCenterX();
            vx[1] = mxC;
            vx[2] = mxC;
            vy[0] -= distancia;
            vy[1] -= distancia;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            float distancia = mxC - boundingButtons[LEFT_SIDE]->getCenterX();
            vx[0] = mxC; 
            vx[3] = mxC;
            vy[3] -= distancia;
            vy[2] -= distancia;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            float distancia = myC - boundingButtons[BOTTOM_SIDE]->getCenterY();
            vy[3] = myC; 
            vy[2] = myC;
            vx[0] -= distancia;
            vx[3] -= distancia;
        } else if (selectedBoundingButton == TOP_SIDE) {
            float distancia = myC - boundingButtons[TOP_SIDE]->getCenterY();
            vy[0] = myC; 
            vy[1] = myC;
            vx[1] -= distancia;
            vx[2] -= distancia;
        } 
        
        width = vx[1] - vy[0]; 
        height = vy[3] - vy[0];

        rotatePoints(pivotX, pivotY, angle);
        attRotateBtnCoordinate();
        attBoundingButtonsPosition();
    }

    bool hasRotateButtonCollided(float mx, float my) override{
        return rotateBtn->hasCollided(mx, my);
    }
};


#endif // RETANGULO_H_INCLUDED
