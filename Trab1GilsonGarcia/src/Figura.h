#ifndef FIGURA_H_INCLUDED
#define FIGURA_H_INCLUDED

#include <vector>
#include <cmath>
#include "BoundingBtn.h"

using namespace std;

class Figura {
protected:
    bool visible, selected, resizing, rotating;

    int nPoints;
    vector<float> vx, vy;
    float angle;
    
    float x, y;
    float offsetX, offsetY;

    float r, g, b;
    int indexColor;
    int colorScale;

    vector<BoundingBtn*> boundingButtons;
    int selectedBoundingButton;

    float segmentCenter(float x1, float x2) {
        return (x1 + x2) / 2;
    }

    float dist(float x1, float y1, float x2, float y2) {
        return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    }
    
    // retorna o angulo entre dois vetores (x1, y1) e (x2, y2) em rad
    float angleRAD(float x1, float y1, float x2, float y2) {
        return (atan2(y1, x1) - atan2(y2, x2)) * 180.0 / PI;
    }
    
    void rotatePoint(float& x1, float& y1, float pivotX, float pivotY, float angle) {
        // Converte o ângulo para radianos
        float rad = (angle) * PI / 180.0;
        float rotMatrix[2][2] = {{cos(rad), -sin(rad)}, {sin(rad), cos(rad)}};

        x1 -= pivotX;
        y1 -= pivotY;

        // Rotaciona o ponto utilizando a matriz de rotação
        float newX = x1 * rotMatrix[0][0] + y1 * rotMatrix[0][1];
        float newY = x1 * rotMatrix[1][0] + y1 * rotMatrix[1][1];

        x1 = newX;
        y1 = newY;

        // Translada o ponto de volta para a sua posição original
        x1 += pivotX;
        y1 += pivotY;
    }


    void rotatePoints(float pivotX, float pivotY, float angle) {
        // Converte o ângulo para radianos
        float rad = (angle) * PI / 180.0;

        float rotMatrix[2][2] = {{cos(rad), -sin(rad)}, {sin(rad), cos(rad)}};

        for (int i = 0; i < nPoints; i++) {
            // Translada o ponto para o ponto de pivot
            vx[i] -= pivotX;
            vy[i] -= pivotY;

            // Rotaciona o ponto utilizando a matriz de rotação
            float x = vx[i] * rotMatrix[0][0] + vy[i] * rotMatrix[0][1];
            float y = vx[i] * rotMatrix[1][0] + vy[i] * rotMatrix[1][1];

            vx[i] = x;
            vy[i] = y;

            // Translada o ponto de volta para a sua posição original
            vx[i] += pivotX;
            vy[i] += pivotY;
        }
    }

    void createBoundingButtons() {
        for (int i = 0; i < nPoints + 1; i++) {
            BoundingBtn* btn = new BoundingBtn(0, 0, BOUNDING_BTN_SIZE, INDEX14);
            btn->setColor(0);
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
        angle = 0;
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

    void setRotating(bool value) {
        rotating = value;
    }
    bool isRotating() {
        return rotating;
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

    virtual float getCenterX() {
        float sum = 0;
        for (int i = 0; i < nPoints; i++) {
           sum += vx[i]; 
        }

        return sum / nPoints;
    }

    virtual float getCenterY() {
        float sum = 0;
        for (int i = 0; i < nPoints; i++) {
           sum += vy[i]; 
        }

        return sum / nPoints;
    }

    virtual bool hasRotateButtonCollided(float mx, float my) { return false; };
    virtual void rotate(float mx, float my) {}
    virtual void resize(float mx, float my) {}
    virtual void resizeProportionally(float mx, float my) {}
    virtual void setVisible(float x, float y) {}
    virtual void setOffset(float x, float y) {}
    virtual void setMousePosition(float mx, float my) {}
};

#endif // FIGURA_H_INCLUDED
