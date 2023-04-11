#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"
#include "constants.h"

class Botao{
  float altura, largura, x, y;
  char label[100];

public:
  Botao(float x, float y, float largura, float altura, char *label)
  {
     this->altura  = altura;
     this->largura = largura;
     this->x = x;
     this->y = y;
     strcpy(this->label, label);
  }

  void Render()
  {
      float labelX = x + (largura - strlen(label)*CHAR_WIDTH)/2;
      float labelY = y + (altura + CHAR_HEIGHT)/2;
      CV::color(0, 1, 0);
      CV::rectFill(x, y, x + largura, y + altura);
      CV::color(0, 0, 0);
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
};

#endif
