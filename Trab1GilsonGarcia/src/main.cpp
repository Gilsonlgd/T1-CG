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
#include "Linha.h"
#include "Relogio.h"
#include "Botao.h"
#include "ToolBar.h"
#include "Export.h"

#include "constants.h"

using namespace std;

Figura *newFigura = NULL;
Figura *shapeToResize = NULL;
Figura *shapeToRotate = NULL;

Retangulo *rect = NULL;
ToolBar *toolBar = NULL;
Export *exportData = NULL;
list<Figura*> shapesList;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1200, screenHeight = 700; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool isDragging = false;
bool isResizing = false;
bool isCTRLdown = false;
bool isSHIFTdown = false;
bool criarFigura = false;
bool isColoring = false;
bool isRotating = false;
int selectedColor = 0;


bool isMouseInsideDrawBounds(float x, float y) {
   if (x > 0 && x < screenWidth) {
      if (y > toolBar->getHeight() && y < screenHeight) {
         return true;
      }
   };
   return false;
}

void DrawMouseScreenCoords()
{
   char str[100];
   sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
   CV::text(10,300, str);
   sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
   CV::text(10,320, str);
}

void DrawShapes()
{
   for (auto shape : shapesList) {
      shape->render();
   }
}

void DrawColoringPointer() {
   if (!isMouseInsideDrawBounds(mouseX, mouseY)) return;
   
   CV::color(selectedColor);
   float pointerCenter = COLOR_POINTER_SIZE / 2;
   CV::rectFill(mouseX - pointerCenter, mouseY - pointerCenter, mouseX + pointerCenter, mouseY + pointerCenter);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   DrawShapes();
   toolBar->render();
   if (isColoring) DrawColoringPointer();
}

void unselectAllShapes() {
   for (auto shape : shapesList) {
      if (!shape->isResizing() && !shape->isRotating()) {
         shape->setUnselected();
      }
   }
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
      case ESC:
	     unselectAllShapes();
        isColoring = false;
        selectedColor = NO_SELECTION;
	   break;
      case DEL:
        handleDeleteSelectedShapes();
      break;
      case CTRL:
        isCTRLdown = true;
      break;
      case SHIFT:
        isSHIFTdown = true;
      break;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
   switch(key)
   {
      case CTRL:
        isCTRLdown = false;
      break;
      case SHIFT:
        isSHIFTdown = false;
      break;
   }
}

void handleCreateShape(float x, float y)
{
   if (criarFigura && !newFigura->isVisible()) {
      shapesList.push_back(newFigura);
      newFigura->setVisible(x, y);
      criarFigura = false;
   }

   Figura* fig = toolBar->checkShapeButtonClicked(x, y);
   if (fig != NULL) {
      newFigura = fig;
      criarFigura = true;
      isColoring = false;
      selectedColor = NO_SELECTION;
   }
}

void handleStartResizingShape(float x, float y) {
   if(isColoring) return;

   bool justStartResizing = false;
   for (auto shape : shapesList) {
      if (shape->isSelected() && shape->hasBoundingBtnCollided(x, y)) {
         shape->setResizing(true);
         shapeToResize = shape;
         isResizing = true;

         justStartResizing = true;
      }
   }
   
   if (justStartResizing) unselectAllShapes();
   
}

void handleResizeShape(float x, float y) {
   if (isSHIFTdown) {
      shapeToResize->resizeProportionally(x, y);
   } else {
      shapeToResize->resize(x, y);
   }
}

void handleStopResizingShape() {
   if (isResizing) {
      shapeToResize->setResizing(false);
      shapeToResize = NULL;
      isResizing = false;
   }
}

void handleShapesSelection(float x, float y)
{
   list<Figura*> auxList = {};
   auto listBegin = shapesList.begin();
   auto listEnd = shapesList.end();

   if(!isCTRLdown) {
      unselectAllShapes();
   }

   for (auto shape = prev(listEnd); shape != prev(listBegin); shape--){
      if ( (*shape)->hasCollided(x, y) ) {
         shapesList.erase(shape);
         (*shape)->setSelected();
         auxList.push_front(*shape);
         break;
      } 
   }

   for (auto shape : auxList) {
      shapesList.push_back(shape);
   }
}

void handleStartDragShape(float x, float y) {  
   if(isColoring) return;

   bool hasSomeCollision = false;
   for (auto shape : shapesList) {
      if (shape->hasCollided(x, y)) {
         hasSomeCollision = true;
         break;
      }  
   }

   if (hasSomeCollision) {
      for (auto shape : shapesList) {
         if (shape->isSelected()) {
            shape->setOffset(x, y);
         }
      }
      isDragging = true;
   }
}

void handleDragShape(float x, float y) {
   if (!isMouseInsideDrawBounds(x, y)) return;

   for (auto shape : shapesList) {
      if(shape->isSelected() && isDragging) {
         shape->setMousePosition(x,y);
      }
   }
}

void handleStartChangeShapeColor(float x, float y) {
   int tempColor = toolBar->checkColorButtonClicked(x, y);
   if (tempColor != NO_SELECTION && tempColor != selectedColor) {
      selectedColor = tempColor;
      isColoring = true;
   }
}

void handleChangeShapeColor(float x, float y) {
   if (!isColoring) return;
   bool hasSomeCollision = false;

   for (auto shape : shapesList) {
      if (shape->hasCollided(x, y)) {
         shape->setColor(selectedColor);
         hasSomeCollision = true;
         break;
      } 
   }

   if (hasSomeCollision) {
      for (auto shape : shapesList) {
         if (shape->isSelected()) {
            shape->setColor(selectedColor);
            hasSomeCollision = true;
         } 
      }
   }
}

void deleteAllShapes() {
   for (auto shape : shapesList) {
      delete shape;
   }
   shapesList.clear();
}

void handleStartShapeRotation(float x, float y) {
   if (isColoring) return;

   bool justStartRotating = false;
   for(auto shape : shapesList) {
      if (shape->isSelected() && shape->hasRotateButtonCollided(x, y)) {
         shape->setRotating(true);
         shapeToRotate = shape;
         isRotating = true;
         justStartRotating = true;
      }
   }

   if(justStartRotating) unselectAllShapes();
}

void handleRotateShape(float x, float y) {
   shapeToRotate->rotate(x, y);
}

void handelStopRotatingShape() {
   if (isRotating) {
      shapeToRotate->setRotating(false);
      shapeToRotate = NULL;
      isRotating = false;
   }
}

void handleManageFile(float x, float y) {
   int button = toolBar->checkMngButtonClicked(x, y);

   if (button != NO_SELECTION) {
      isColoring = false;
      selectedColor = NO_SELECTION;
   }

   if (button == DELETE_ALL) {
      deleteAllShapes();
   } else if (button == SAVE_ALL) {
      exportData = new Export(shapesList);
   }
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   if(state == 1) {
      isDragging = false;
      handelStopRotatingShape();
      handleStopResizingShape();
   };

   if( state == 0 ) //clicou
   {
      handleCreateShape(x, y);
      handleStartResizingShape(x,y);
      handleStartShapeRotation(x, y);

      handleShapesSelection(x, y);
      handleStartDragShape(x, y);

      handleStartChangeShapeColor(x,y);
      handleChangeShapeColor(x, y);

      handleManageFile(x,y);
   }

   if (isDragging) handleDragShape(x, y);
   if (isResizing) handleResizeShape(x, y);
   if (isRotating) handleRotateShape(x, y);
}

int main(void)
{
   toolBar = new ToolBar(0, 0, screenWidth, 100);
   newFigura = new Figura(0);

   CV::init(&screenWidth, &screenHeight, "");
   CV::run();
}
