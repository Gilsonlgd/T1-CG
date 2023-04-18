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
protected:
    void drawBoundingBox() {
        int offset = BOUNDING_BTN_SIZE / 2;
        //resize buttons
        Botao* botao = boundingButtons[TOP_SIDE];
        botao->setCoord(vx[0] - offset, vy[0] - offset);
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(vx[1] - offset, vy[1] - offset);
        botao->Render();
    }

    
public:
    using Figura::setVisible;

    Linha() : Figura(2) {
        offsetX = 0;
        offsetY = 0;
    }

    void render() override {
        if (colorScale == RGBA) CV::color(r,g,b); 
        else if (colorScale == INDEX14)  CV::color(indexColor);
        CV::translate(0,0);
        CV::line(vx[0], vy[0], vx[1], vy[1]);
        if(selected) drawBoundingBox();
    }

    float getCenterX() override{
        return (vx[1]-vx[0])/2;
    }

    float getCenterY() override{
        return (vy[1]-vy[0])/2;
    }

    void setOffset(float x, float y) override {
        offsetX = x - this->vx[0];
        offsetY = y - this->vy[0];
    }

    void setVisible(float x, float y) override {
        float start_len = START_LEN;
        vx[0] = x;
        vy[0] = y - start_len/2;
        vx[1] = x;
        vy[1] = y + start_len/2;
        visible = true;
    }

    bool hasCollided(int mx, int my) override {
        if (selectedBoundingButton >= 0 && selected) return false;

        float d1 = dist(mx,my, vx[0],vy[0]);
        float d2 = dist(mx,my, vx[1],vy[1]);
        float lineLen = dist(vx[0],vy[0], vx[1],vy[1]);   
        
        if (d1 + d2 >= lineLen - TOL && d1 + d2 <= lineLen + TOL) {
            return true;
        }
        return false;
    }

    void setMousePosition(float mx, float my) override {
        float xDif = vx[1] - vx[0];
        float yDif = vy[1] - vy[0];
        vx[0] = mx - offsetX;
        vy[0] = my - offsetY;
        vx[1] = mx + xDif - offsetX;
        vy[1] = my + yDif - offsetY;
    }

    void resize(float mx, float my) override {
        if (selectedBoundingButton == BOTTOM_SIDE) {
            if(dist(mx, my, vx[0], vy[0]) > MIN_LEN) {
                vx[1] = mx;
                vy[1] = my;
            }
        } else if (selectedBoundingButton == TOP_SIDE) {
            if(dist(mx, my, vx[1], vy[1]) > MIN_LEN) {
                vx[0] = mx;
                vy[0] = my;
            }
        }
    }

    void resizeProportionally(float mx, float my) override{
        if (selectedBoundingButton == BOTTOM_SIDE) {
            if(dist(mx, my, vx[0], vy[0]) > MIN_LEN) {
                vx[0] += -(mx - vx[1]);
                vy[0] += -(my - vy[1]);
                vx[1] = mx;
                vy[1] = my;
            }
        } else if (selectedBoundingButton == TOP_SIDE) {
            if(dist(mx, my, vx[1], vy[1]) > MIN_LEN) {
                vx[1] += -(mx - vx[0]);
                vy[1] += -(my - vy[0]);
                vx[0] = mx;
                vy[0] = my;
            }
        }
    }
};


#endif // LINHA_H_INCLUDED
