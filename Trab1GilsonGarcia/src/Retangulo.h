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
    void drawBoundingBox() {
        CV::color(0,0,0);
        CV::translate(0, 0);
        CV::polygon(vx.data(), vy.data(), 4);

        //resize buttons
        BoundingBtn* botao = boundingButtons[LEFT_SIDE];
        botao->setCoord(segmentCenter(vx[0], vx[3]) , segmentCenter(vy[0], vy[3]));
        botao->Render();

        botao = boundingButtons[RIGHT_SIDE];
        botao->setCoord(segmentCenter(vx[1], vx[2]) , segmentCenter(vy[1], vy[2]));
        botao->Render();

        botao = boundingButtons[TOP_SIDE];
        botao->setCoord(segmentCenter(vx[0], vx[1]) , segmentCenter(vy[0], vy[1]));
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(segmentCenter(vx[3], vx[2]) , segmentCenter(vy[3], vy[2]));
        botao->Render();

        attRotateBtnCoordinate();
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
        rotatePoints(pivotX, pivotY, newAngle);
    }

    void resize(float mx, float my) override {
        float tempWidth = width;
        float tempHeight = height;

        if (selectedBoundingButton == RIGHT_SIDE) {
            float vetLen = dist(vx[0], vy[0], vx[1], vy[1]);
            float vetUnitarioX = (vx[1] - vx[0]) / vetLen;
            float vetUnitarioY = (vy[1] - vy[0]) / vetLen;

            float deltax = 1;
            float deltay= 1;
            if (mx - boundingButtons[RIGHT_SIDE]->getCenterX() < 0) deltax = -1;
            if (my - boundingButtons[RIGHT_SIDE]->getCenterY() < 0) deltay = -1; 

            float dist = distance(mx, my, boundingButtons[RIGHT_SIDE]->getCenterX(), boundingButtons[RIGHT_SIDE]->getCenterY(), vetUnitarioX, vetUnitarioY); 
            float newLenX = dist + vetLen;
            float newLenY = dist + vetLen;

            vx[1] = vx[0] + vetUnitarioX * newLenX;
            vy[1] = vy[0] + vetUnitarioY * newLenY;

            vx[2] = vx[3] + vetUnitarioX * newLenX;
            vy[2] = vy[3] + vetUnitarioY * newLenY;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            tempWidth = width + vx[0] - mx;
            if (tempWidth >= MIN_SIZE) vx[0] = mx;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            tempHeight = my - vy[0];
        } else if (selectedBoundingButton == TOP_SIDE) {
            tempHeight = height + vy[0] - my;
            if (tempHeight >= MIN_SIZE) vy[0] = my;
        }

        if (tempWidth >= MIN_SIZE) width = tempWidth;
        if (tempHeight >= MIN_SIZE) height = tempHeight;
        
    }

    void resizeProportionally(float mx, float my) override{
        rotatePoints(getCenterX(), getCenterY(), -angle);

        float tempWidth = width;
        float tempHeight = height;
        if (selectedBoundingButton == RIGHT_SIDE) {
            tempWidth = mx - vx[0];
            tempHeight += tempWidth - width;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            tempWidth = width + vx[0] - mx;
            tempHeight += tempWidth - width;
            if (tempWidth >= MIN_SIZE) vx[0] = mx;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            tempHeight = my - vy[0];
            tempWidth += tempHeight - height;
        } else if (selectedBoundingButton == TOP_SIDE) {
            tempHeight = height + vy[0] - my;
            tempWidth += tempHeight - height;
            if (tempHeight >= MIN_SIZE) vy[0] = my;
        }

        if (tempWidth >= MIN_SIZE) width = tempWidth;
        if (tempHeight >= MIN_SIZE) height = tempHeight;
        attPointsCoord();

        rotatePoints(getCenterX(), getCenterY(), angle);
    }

    bool hasRotateButtonCollided(float mx, float my) override{
        return rotateBtn->hasCollided(mx, my);
    }
};


#endif // RETANGULO_H_INCLUDED
