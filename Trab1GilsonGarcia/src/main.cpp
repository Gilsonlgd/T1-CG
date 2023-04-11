/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>

#include "gl_canvas2d.h"

#include "Retangulo.h"
#include "Figura.h"
#include "Bola.h"
#include "Relogio.h"
#include "Botao.h"
#include "ToolBar.h"

#include "constants.h"

using namespace std;

Figura *figura = NULL;
Retangulo *rect = NULL;
Bola    *b = NULL;
Relogio *r = NULL;
Botao   *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
ToolBar *toolBar = NULL;
list<Figura*> shapesList;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1200, screenHeight = 700; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool isDragging = false;
bool criarFigura = false;


void DrawMouseScreenCoords()
{
   char str[100];
   sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
   CV::text(10,300, str);
   sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
   CV::text(10,320, str);
}

void DrawCircle()
{
   if (b->getVisible()) {
      CV::color(10);
      float tam = b->getTam();
      CV::translate(b->getX(), b->getY());
      CV::rectFill(Vector2(0,0), Vector2(tam, tam));
   }
}

void DrawShapes()
{
   for (auto shape : shapesList) {
      shape->render();
   }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   toolBar->render();
   DrawShapes();
}

void handleDeleteSelectedShapes()
{
   for (auto it = shapesList.begin(); it != shapesList.end(); ++it) {
      if((*it)->isSelected()) {
         delete *it;
         shapesList.erase(it);
      }
   }
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if( key < 200 )
   {
      opcao = key;
   }

   switch(key)
   {
      case 27:
	     exit(0);
	   break;
      case DELETE:
        handleDeleteSelectedShapes();
      break;


   }
}

//funcao chamada toda vez que uma te cla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}


void handleCreateShape(float x, float y)
{
   if (criarFigura && !figura->isVisible()) {
      shapesList.push_back(figura);
      figura->setVisible(x, y);
      criarFigura = false;
   }

   Figura* fig = toolBar->checkShapeButtonClicked(x, y);
   if (fig != NULL) {
      figura = fig;
      criarFigura = true;
   }
}

void handleStartDragShape(float x, float y)
{
   if (figura->hasCollided(x,y)) {
      figura->setOffset(x, y);
      isDragging = true;
   }
}

void handleDragShape(float x, float y)
{
   if(figura->isVisible() && isDragging) {
      figura->setMousePosition(x,y);
   }
}

void handleSelectShape(float x, float y)
{
   for (auto shape : shapesList) {
      shape->setUnselected();
      if (shape->hasCollided(x, y)) {
         shape->setSelected();
         figura = shape;
      }
   }
}
//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   if(state == 1) isDragging = false;

   printf("\nmouse %d %d %d %d %d %d %d", button, state, wheel, direction,  x, y, isDragging);

   if( state == 0 ) //clicou
   {
      handleCreateShape(x, y);
      handleSelectShape(x, y);
      handleStartDragShape(x, y);
   }

   if (isDragging) handleDragShape(x, y);
}

int main(void)
{
   b = new Bola(120);
   r = new Relogio();
   rect = new Retangulo(60, 60);
   toolBar = new ToolBar(0, 0, screenWidth, 100);
   figura = new Figura();

   CV::init(&screenWidth, &screenHeight, "");
   CV::run();
}
