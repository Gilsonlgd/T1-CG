#ifndef __BOUNDINGBTN_H__
#define __BOUNDINGBTN_H__

#include "gl_canvas2d.h"
#include "constants.h"
#include <cmath>

class BoundingBtn {
  float radius, x, y;
  bool isVisible;
  int colorScale;
  float r, g, b, a;
  int indexColor;

protected:


  void rotatePoint(float pivotX, float pivotY, float angle) {
    // Converte o ângulo para radianos
    float rad = (angle) * PI / 180.0;
    float rotMatrix[2][2] = {{cos(rad), -sin(rad)}, {sin(rad), cos(rad)}};

    x -= pivotX;
    y -= pivotY;

    // Rotaciona o ponto utilizando a matriz de rotação
    float newX = x * rotMatrix[0][0] + y * rotMatrix[0][1];
    float newY = x * rotMatrix[1][0] + y * rotMatrix[1][1];

    x = newX;
    y = newY;

    // Translada o ponto de volta para a sua posição original
    x += pivotX;
    y += pivotY;
  }

public:
  BoundingBtn(float x, float y, float radius, int colorScale)
  {
    this->isVisible = false;
    this->x = x; 
    this->y = y;
    this->r = 0;
    this->g = 1;
    this->b = 0;
    this->a = 1;
    this->indexColor = 0;
    this->radius = radius;
    this->colorScale = colorScale;
}

  void Render()
  {  
    CV::translate(x, y);
    if (colorScale == RGBA) CV::color(r,g,b); 
    else if (colorScale == INDEX14)  CV::color(indexColor);
    CV::circleFill(0, 0, radius, NUM_SEGMENTS);

    CV::color(CONTRAST_BOUND_COLOR); 
    CV::circle(0, 0, radius, NUM_SEGMENTS);
    CV::translate(0, 0);
  }

  float getCenterX() {
    return x;
  }

  float getCenterY() {
    return y;
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool hasCollided(int mx, int my)
  {
      float mouseDist = sqrt( pow(mx - x, 2) + pow(my - y, 2));
      if (mouseDist < radius) return true;
      return false;
  }

  void setCoord(float x, float y) {
      this->x = x;
      this->y = y;
  }

  void setColor(float r, float g, float b, float a) {
      this->r = r/255;
      this->g = g/255;
      this->b = b/255;
      this->a = a;
  }

  void setColor(int index) {
      indexColor = index;
  }

  int getColorIndex() {
      return indexColor;
  }

  void rotate(float pivotX, float pivotY, float angle) {
    rotatePoint(pivotX, pivotY, angle);
  }
};

#endif
