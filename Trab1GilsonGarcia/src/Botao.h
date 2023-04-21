#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"
#include "constants.h"

/*
##### BOTAO #####
Botao disponibilizado pelo professor
foram feitas algumas alterações como centralização do label
e alteração da escala de cores
######################
*/

class Botao{
  float altura, largura, x, y;
  bool isVisible;
  char label[100];
  int colorScale;
  float r, g, b, a;
  int indexColor;

public:
  Botao(float x, float y, float largura, float altura, char *label, int colorScale)
  {
     this->isVisible = false;
     this->altura  = altura;
     this->largura = largura;
     this->x = x;
     this->y = y;
     this->r = 0;
     this->g = 1;
     this->b = 0;
     this->a = 1;
     this->indexColor = 0;
     this->colorScale = colorScale;
     strcpy(this->label, label);
  }

  void Render()
  {
      float labelX = x + (largura - strlen(label)*CHAR_WIDTH)/2;
      float labelY = y + (altura + CHAR_HEIGHT)/2;
      
      if (colorScale == RGBA) CV::color(r,g,b); 
      else if (colorScale == INDEX14)  CV::color(indexColor);
      CV::rectFill(x, y, x + largura, y + altura);
      CV::color(1, 1, 1);
      CV::text(labelX, labelY, label); //escreve o label do botao mais ou menos ao centro.
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
    bool hasCollided(int x, int y)
    {
        if( x >= this->x && x <= (this->x + largura) && y >= this->y && y <= (this->y + altura) )
        {
            return true;
        }
        return false;
    }

    char* getLabel() {
        return label;
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
};

#endif
