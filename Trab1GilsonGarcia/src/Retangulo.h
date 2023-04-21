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

/*
##### RETÂNGULO #####
Este retângulo foi implementado utilizando polígono.
Todos os métodos de translação e rotação são orientados a isso
######################
*/

using namespace std;

class Retangulo : public Figura {
    float width;
    float height;
    BoundingBtn* rotateBtn; // botão de rotação

protected:
    // atualiza a posição dos botões de redimensionamento 
    // de acordo com as arestas
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

    // desenha a caixa contorno (sinaliza que está selecionado)
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

    // atualiza as corrdenadas do retângulo de acordo com a origem, largura e altura
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

    //atualiza a coordenada do botão de rotação de acordo com a posição da figura
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

    void attDimensions() {
        width = vx[1] - vy[0]; 
        height = vy[3] - vy[0];
    }
public:
    using Figura::setVisible;

    Retangulo() : Figura(4) {
        ID = RECT_ID;
        width = 40;
        height = 40;
        offsetX = 0;
        offsetY = 0;
        rotateBtn = new BoundingBtn(0, 0, BOUNDING_BTN_SIZE, INDEX14);
        attPointsCoord();
    }

    // construtor utilizado na ao se instanciar um objeto originário de um arquivo
    Retangulo(vector<float> vx, vector<float> vy, float angle, 
    int colorScale, float r, float g, float b, int indexColor) : Figura(4) {
        
        ID = RECT_ID;
        this->vx = vx;
        this->vy = vy;
        this->angle = angle;
        this->colorScale = colorScale;
        this->r = r;
        this->g = g;
        this->b = b;
        this->indexColor = indexColor;
        rotateBtn = new BoundingBtn(0, 0, BOUNDING_BTN_SIZE, INDEX14);
        attDimensions();
        attBoundingButtonsPosition();
        attRotateBtnCoordinate();
    }

    void render() override {
        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(r,g,b);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), 4);
        if(selected) drawBoundingBox();
    }

    // define um valor que subtraído a posição do mouse 
    // evita saltos
    void setOffset(float x, float y) override {
        rotatePoints(getCenterX(), getCenterY(), -angle);
        float origemX = vx[0];
        float origemY = vy[0];

        offsetX = x - origemX;
        offsetY = y - origemY;
        rotatePoints(getCenterX(), getCenterY(), angle);  
    }

    // seta a figura visível em deperminada posição
    void setVisible(float x, float y) override {
        vx[0] = x - getCenterX();
        vy[0] = y - getCenterY();

        attPointsCoord();
        visible = true;
        rotateBtn->setCoord(vx[2] + 20, vy[2]);
    }

    // utiliza a posição do mouse para mover a figura
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

    // utiliza a posição do mouse para mover a figura.
    // - calcula o ângulo entre o botão de rotação da figura
    // e a nova posição do mouse para definir a rotação dos pontos da figura
    void rotate(float mx, float my) override {
        float pivotX = getCenterX();
        float pivotY = getCenterY();

        float CBx = rotateBtn->getCenterX() - pivotX;
        float CBy = rotateBtn->getCenterY() - pivotY;

        float CMx = mx - pivotX;
        float CMy = my - pivotY;

        float newAngle = angleDEG(CMx, CMy, CBx, CBy);
        
        angle += newAngle;
        if(abs(angle) > 360) {
            angle = angle > 0 ? angle - 360.0 : angle + 360.0;
        }

        rotateBtn->rotate(pivotX, pivotY, newAngle);
        rotatePoints(pivotX, pivotY, newAngle);
    }

    // rotaciona a figura no sentido oposto para poder
    // alterar os pontos do retângulo de acordo com os eixos coordenados
    void resize(float mx, float my) override {
        float pivotX = getCenterX();
        float pivotY = getCenterY();

        float mxC = mx;
        float myC = my;

        rotatePoint(mxC, myC, pivotX, pivotY, -angle);
        rotatePoints(pivotX, pivotY, -angle);
        attBoundingButtonsPosition();

        if (selectedBoundingButton == RIGHT_SIDE) {
            float distancia = mxC - boundingButtons[RIGHT_SIDE]->getCenterX();
            vx[1] += distancia;
            vx[2] += distancia;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            float distancia = mxC - boundingButtons[LEFT_SIDE]->getCenterX();
            vx[0] += distancia; 
            vx[3] += distancia;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            float distancia = myC - boundingButtons[BOTTOM_SIDE]->getCenterY();
            vy[3] += distancia; 
            vy[2] += distancia;
        } else if (selectedBoundingButton == TOP_SIDE) {
            float distancia = myC - boundingButtons[TOP_SIDE]->getCenterY();
            vy[0] += distancia; 
            vy[1] += distancia;
        } 
        
        attDimensions();
        rotatePoints(pivotX, pivotY, angle);
        attRotateBtnCoordinate();
        attBoundingButtonsPosition();
    }

    // faz o mesmo da função anterior porém de forma proporcional 
    // Movimenta a aresta selecionada e a aresta perpendicular a ela
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
            vx[1] += distancia;
            vx[2] += distancia;
            vy[0] -= distancia;
            vy[1] -= distancia;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            float distancia = mxC - boundingButtons[LEFT_SIDE]->getCenterX();
            vx[0] += distancia; 
            vx[3] += distancia;
            vy[3] -= distancia;
            vy[2] -= distancia;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            float distancia = myC - boundingButtons[BOTTOM_SIDE]->getCenterY();
            vy[3] += distancia; 
            vy[2] += distancia;
            vx[0] -= distancia;
            vx[3] -= distancia;
        } else if (selectedBoundingButton == TOP_SIDE) {
            float distancia = myC - boundingButtons[TOP_SIDE]->getCenterY();
            vy[0] += distancia; 
            vy[1] += distancia;
            vx[1] -= distancia;
            vx[2] -= distancia;
        } 
        
        attDimensions();
        rotatePoints(pivotX, pivotY, angle);
        attRotateBtnCoordinate();
        attBoundingButtonsPosition();
    }

    bool hasRotateButtonCollided(float mx, float my) override{
        return rotateBtn->hasCollided(mx, my);
    }
};


#endif // RETANGULO_H_INCLUDED
