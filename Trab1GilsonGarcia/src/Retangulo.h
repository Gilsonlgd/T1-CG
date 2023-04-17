#ifndef RETANGULO_H_INCLUDED
#define RETANGULO_H_INCLUDED

#include "Figura.h"
#include "constants.h"

#define LEFT_SIDE 0
#define RIGHT_SIDE 1
#define TOP_SIDE 2
#define BOTTOM_SIDE 3
#define MIN_SIZE 5

using namespace std;

class Retangulo : public Figura {
    float vx[4], vy[4]; 
    float width;
    float height;

protected:
    void createBoundingButtons() {
        for (int i = 0; i < 4; i++) {
            Botao* btn = new Botao(0, 0, BOUNDING_BTN_SIZE, BOUNDING_BTN_SIZE, "", RGBA);
            btn->setRGBA(0, 0, 0, 1);
            boundingButtons.push_back(btn);
        }
    }

    void drawBoundingBox() {
        CV::color(0,0,0);
        CV::translate(0, 0);
        CV::polygon(vx, vy, 4);

        int offset = BOUNDING_BTN_SIZE / 2;
        //resize buttons
        Botao* botao = boundingButtons[LEFT_SIDE];
        botao->setCoord(vx[0] - offset, vy[0] - offset + height / 2);
        botao->Render();

        botao = boundingButtons[RIGHT_SIDE];
        botao->setCoord(vx[0] - offset + width, vy[0] - offset + height / 2);
        botao->Render();

        botao = boundingButtons[TOP_SIDE];
        botao->setCoord(vx[0] - offset + width / 2, vy[0] - offset);
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(vx[0] - offset + width / 2, vy[0] - offset + height);
        botao->Render();
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

public:
    using Figura::setVisible;

    Retangulo(float width, float height)
    {
        this->width = width;
        this->height = height;
        offsetX = 0;
        offsetY = 0;
        vx[0] = 0;
        vy[0] = 0;
        attPointsCoord();
        createBoundingButtons();
    }

    Retangulo() {
        width = 40;
        height = 40;
        offsetX = 0;
        offsetY = 0;
        vx[0] = 0;
        vy[0] = 0;
        attPointsCoord();
        createBoundingButtons();
    }

    void render() override {
        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(r,g,b); 
        else if (colorScale == INDEX14)  CV::color(indexColor);
        CV::polygonFill(vx, vy, 4);
        if(selected) drawBoundingBox();
    }

    float getCenterX() override{
        return width/2;
    }
    float getCenterY() override{
        return height/2;
    }

    void setOffset(float x, float y) override {
        float origemX = vx[0];
        float origemY = vy[0];

        offsetX = x - origemX;
        offsetY = y - origemY;
    }

    void setVisible(float x, float y) override {
        vx[0] = x - getCenterX();
        vy[0] = y - getCenterY();
        attPointsCoord();
        visible = true;
    }

    bool hasCollided(int x, int y) override {
        if (selectedBoundingButton >= 0 && selected) return false;

        float origemX = x - vx[0];
        float origemY = y - vy[0];

        if (origemX >= 0 && origemX <= width && origemY >= 0 && origemY <= height) {
            return true;
        }
        return false;
    }

    void setMousePosition(float mx, float my) override {
        vx[0] = mx - offsetX;
        vy[0] = my - offsetY;

        vx[1] = mx - offsetX + width;
        vy[1] = my - offsetY;

        vx[2] = mx - offsetX + width;
        vy[2] = my - offsetY + height;

        vx[3] = mx - offsetX;
        vy[3] = my - offsetY + height;
    }

    void resize(float mx, float my) override {
        float tempWidth = width;
        float tempHeight = height;
        if (selectedBoundingButton == RIGHT_SIDE) {
            tempWidth = mx - vx[0];
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
        attPointsCoord();
    }

    void resizeProportionally(float mx, float my) override{
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
    }
};


#endif // RETANGULO_H_INCLUDED
