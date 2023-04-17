#ifndef LINHA_H_INCLUDED
#define LINHA_H_INCLUDED

#include "Figura.h"
#include "constants.h"

#define TOP_SIDE         0
#define BOTTOM_SIDE      1
#define MIN_LEN          5
#define START_LEN       40
#define X_MARGIN         2
#define TOL              2

using namespace std;

class Linha : public Figura {
    float x1, x2;
    float y1, y2;

protected:
    void createBoundingButtons() {
        for (int i = 0; i < 2; i++) {
            Botao* btn = new Botao(0, 0, BOUNDING_BTN_SIZE, BOUNDING_BTN_SIZE, "", RGBA);
            btn->setRGBA(0, 0, 0, 1);
            boundingButtons.push_back(btn);
        }
    }

    void drawBoundingBox() {
        int offset = BOUNDING_BTN_SIZE / 2;
        //resize buttons
        Botao* botao = boundingButtons[TOP_SIDE];
        botao->setCoord(x1 - offset, y1 - offset);
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(x2 - offset, y2 - offset);
        botao->Render();
    }

    
public:
    using Figura::setVisible;

    Linha() {
        x1 = 0;
        x2 = 0;
        y1 = 0;
        y2 = 0;
        offsetX = 0;
        offsetY = 0;
        createBoundingButtons();
    }

    void render() override {
        if (colorScale == RGBA) CV::color(r,g,b); 
        else if (colorScale == INDEX14)  CV::color(indexColor);
        CV::translate(0,0);
        CV::line(x1, y1, x2, y2);
        if(selected) drawBoundingBox();
    }

    float getCenterX() override{
        return x1;
    }

    float getCenterY() override{
        return (y2-y1)/2;
    }

    void setOffset(float x, float y) override {
        offsetX = x - this->x1;
        offsetY = y - this->y1;
    }

    void setVisible(float x, float y) override {
        float start_len = START_LEN;
        x1 = x;
        y1 = y - start_len/2;
        x2 = x;
        y2 = y + start_len/2;
        visible = true;
    }

    bool hasCollided(int mx, int my) override {
        if (selectedBoundingButton >= 0 && selected) return false;
        float d1 = dist(mx,my, x1,y1);
        float d2 = dist(mx,my, x2,y2);

        float lineLen = dist(x1,y1, x2,y2);   
        
        if (d1 + d2 >= lineLen - TOL && d1 + d2 <= lineLen + TOL) {
            return true;
        }
        return false;
    }

    void setMousePosition(float mx, float my) override {
        float lineLen = dist(x1, y1, x2, y2);
        float xDif = x2 - x1;
        float yDif = y2 - y1;
        x1 = mx - offsetX;
        y1 = my - offsetY;
        x2 = mx + xDif - offsetX;
        y2 = my + yDif - offsetY;
    }

    void resize(float mx, float my) override {
        if (selectedBoundingButton == BOTTOM_SIDE) {
            if(dist(mx, my, x1, y1) > MIN_LEN) {
                x2 = mx;
                y2 = my;
            }
        } else if (selectedBoundingButton == TOP_SIDE) {
            if(dist(mx, my, x2, y2) > MIN_LEN) {
                x1 = mx;
                y1 = my;
            }
        }
    }

    void resizeProportionally(float mx, float my) override{
        resize(mx,my);
    }
};


#endif // LINHA_H_INCLUDED
