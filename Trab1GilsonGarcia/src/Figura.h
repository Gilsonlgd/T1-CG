#ifndef FIGURA_H_INCLUDED
#define FIGURA_H_INCLUDED

#include <vector>
#include <cmath>
#include "Botao.h"

using namespace std;

class Figura {
protected:
    bool visible, selected, resizing;
    float r, g, b; //cor da figura
    int nPoints;
    vector<float> vx, vy;
    
    float x, y;
    float offsetX, offsetY;

    int indexColor;
    int colorScale;
    vector<Botao*> boundingButtons;
    int selectedBoundingButton;

    float dist(float x1, float y1, float x2, float y2) {
        return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    }
    
    void rotatePoint(float& px, float& py, double angle) {
        double s = sin(angle);
        double c = cos(angle);

        double x = px * c - py * s;
        double y = px * s + py * c;

        px = x;
        py = y;
    }

    void rotatePoints(float pivotX, float pivotY, double angle) {
        for (int i = 0; i < nPoints; i++) {
            // translada o ponto para o ponto de pivot
            vx[i] -= pivotX;
            vy[i] -= pivotY;

            rotatePoint(vx[i], vy[i], angle);

            // translada o ponto de volta para a sua posição original
            vx[i] += pivotX;
            vy[i] += pivotY;
        }
    }

    void createBoundingButtons() {
        for (int i = 0; i < nPoints + 1; i++) {
            Botao* btn = new Botao(0, 0, BOUNDING_BTN_SIZE, BOUNDING_BTN_SIZE, "", RGBA);
            btn->setColor(0, 0, 0, 1);
            boundingButtons.push_back(btn);
        }
    }
public:
    Figura(int nPoints) {
        r = 1;
        g = 0;
        b = 0;
        visible = false;
        selected = false;
        resizing = false;
        selectedBoundingButton = NO_SELECTION;
        indexColor = 2;
        colorScale = INDEX14;
        
        this->nPoints = nPoints;
        for (int i = 0; i < nPoints; i++) {
            vx.push_back(0);
            vy.push_back(0);
        }
        createBoundingButtons();
    }

    virtual ~Figura() {}

    virtual void render() {}

    void setColor(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void setColor(int index) {
        indexColor = index;
    }

    void setVisible() {
        visible = true;
    }
    void setUnvisible() {
        visible = false;
    }

    bool isVisible() {
        return visible;
    }

    void setSelected() {
        selected = true;
    }
    void setUnselected() {
        selected = false;
        selectedBoundingButton = NO_SELECTION;
    }

    void setResizing(bool value) {
        if (value == false) {
            selectedBoundingButton = NO_SELECTION;
        }
        resizing = value;
    }
    bool isResizing() {
        return resizing;
    }

    bool isSelected() {
        return selected;
    }

    int hasBoundingBtnCollided(float mx, float my) {
        for (unsigned int i = 0; i < boundingButtons.size(); i++) {
            auto btn = boundingButtons[i];
            if (btn->hasCollided(mx, my)) {
                selectedBoundingButton = i;
                return true;
            }
        }
        return false;
    }

    // Algorimot Ray Casting de detecção de colisão.
    virtual bool hasCollided(int x, int y) {
        if (selectedBoundingButton >= 0 && selected) return false;

        int n = nPoints;
        int count = 0;

        for (int i = 0; i < n; i++) {
            float x1 = vx[i];
            float y1 = vy[i];
            float x2 = vx[(i + 1) % n];
            float y2 = vy[(i + 1) % n];
            
            if ((y1 > y) != (y2 > y) && (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)) {
                count++;
            }    
        }
        return count % 2 == 1;
    }

    virtual void rotate(float mx, float my) {}
    virtual void resize(float mx, float my) {}
    virtual void resizeProportionally(float mx, float my) {}
    virtual void setVisible(float x, float y) {}
    virtual void setOffset(float x, float y) {}
    virtual void setMousePosition(float mx, float my) {}
    virtual float getCenterX() {}
    virtual float getCenterY() {}
};

#endif // FIGURA_H_INCLUDED
