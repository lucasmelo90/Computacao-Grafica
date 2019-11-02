//---------------------------------------------------------------------------


#pragma hdrstop
#include "uPoligono.h"
#include <math.h>


//---------------------------------------------------------------------------

#pragma package(smart_init)

void Poligono::mostra(TListBox *retorno){
    retorno->Items->Clear();
    for(unsigned int i = 0; i < pontos.size(); i++){
        retorno->Items->Add(pontos[i].mostra());
    }
}


void Poligono::desenha(TCanvas *canvas, Janela mundo, Janela vp, int tipoReta){
    int x, y, x1, y1, x2, y2;

        if(tipoReta == 0){
            for(unsigned int i = 0; i < pontos.size(); i++){
                x = pontos[i].xw2vp(mundo, vp);
                y = pontos[i].yw2vp(mundo, vp);

                if (i==0) {
                        canvas->MoveTo(x, y);
                } else {
                        canvas->LineTo(x, y);
                }
            }
        }
        else
        if(tipoReta == 1){
            for(unsigned int i = 1; i < pontos.size(); i++){
                x1 = pontos[i-1].xw2vp(mundo, vp);
                y1 = pontos[i-1].yw2vp(mundo, vp);
                x2 = pontos[i].xw2vp(mundo, vp);
                y2 = pontos[i].yw2vp(mundo, vp);

                drawDDA(canvas, x1, y1, x2, y2);
            }
        }
        if(tipoReta == 2){
                for(unsigned int i = 1; i < pontos.size(); i++){
                        x1 = pontos[i-1].xw2vp(mundo, vp);
                        y1 = pontos[i-1].yw2vp(mundo, vp);
                        x2 = pontos[i].xw2vp(mundo, vp);
                        y2 = pontos[i].yw2vp(mundo, vp);

                        drawBresenham(canvas, x1, y1, x2, y2);

            }
        }
        if(tipoReta == 3){
                for(unsigned int i = 1; i < pontos.size(); i++){
                        x1 = pontos[i].xw2vp(mundo, vp);
                        y1 = pontos[i].yw2vp(mundo, vp);
                        x2 = (pontos[i].xw2vp(mundo, vp) - pontos[i].yw2vp(mundo, vp)) /2;
                DesenhaCircuferencia(canvas, x1, y1,x2,mundo, vp);
                }
        }
}

void Poligono::drawDDA(TCanvas *canvas, int x1, int y1, int x2, int y2) {
#define SIGN(x)  ((x) < 0 ? (-1) : (1))
#define ABS(x)   ((x) < 0 ? (-x) : (x))
#define FLOOR(x) ((x) < 0 ? ( (x) - (int)(x) != 0 ? ((int)(x) - 1) : ((int)(x))) : (int)(x))
int length;
float x, y, deltay, deltax;

if( ABS( (x2 - x1) ) >= ABS( (y2 - y1) ))
        length = ABS( (x2 - x1) );
else
        length = ABS( (y2 - y1) );

deltax = (float) (x2 - x1) / (float) length;
deltay = (float) (y2 - y1) / (float) length;
x = x1 + 0.5 * SIGN (deltax);
y = y1 + 0.5 * SIGN (deltay);

for (int i = 0; i < length; i++) {
        canvas->Pixels[x][y] = clBlue;
        x += deltax;
        y += deltay;
}
}


void Poligono::drawBresenham(TCanvas *canvas, int x1, int y1, int x2, int y2) {
#define SIGN(x) ((x) < 0 ? (-1) : (1))
#define ABS(x) ((x) < 0 ? (-x) : (x))
#define FALSE 0
#define TRUE 1
int x, dx, y, dy, deltax, deltay, interchange, tmp, signalx, signaly;
double erro;

int i = pontos.size();

if (i > 1) {
        erro = 0;
        for (int j = 0; j < i - 1; j++) {


        deltax = ABS ( (x2 - x1) );
        deltay = ABS ( (y2 - y1) );
        signalx = SIGN ( (x2 - x1) );
        signaly = SIGN ( (y2 - y1) );

        x = x1;
        y = y1;

        if (signalx < 0){
                x -=1;
        }
        if (signaly < 0 ){
                y -= 1;
        }

        interchange = FALSE;
        if ( deltay > deltax) {
                tmp = deltax;
                deltax = deltay;
                deltay = tmp;
                interchange = TRUE;
        }
        erro = 2 * deltay - deltax;

        for (i = 0; i < deltax; i++) {
                canvas->Pixels[x][y] = clBlack;

                while (erro >= 0) {
                        if (interchange){
                                x = x + signalx;
                        }else{
                                y = y + signaly;
                        }
                erro = erro-2 * deltax;
                } // encerra while
                if (interchange){
                        y = y + signaly;
                }else{
                        x = x + signalx;
                }
                erro = erro+2 * deltay;
        }//fim for
    }//fim for
  }//fim if
}//fim void


void Poligono::translada(double x, double y, double *px, double *py,boolean homogenea){

        double ponto[3] = {*px,*py,1};
        double translacao[3][3] = {1, 0, 0, 0, 1, 0, x, y, 1};
        double resultado[3];

        if(homogenea){
                resultado[0] = ponto[0] * translacao[0][0] + ponto[1]*translacao[1][0] + ponto[2]*translacao[2][0];
                resultado[1] = ponto[0] * translacao[0][1] + ponto[1]*translacao[1][1] + ponto[2]*translacao[2][1];
                resultado[2] = ponto[0] * translacao[0][2] + ponto[1]*translacao[1][2] + ponto[2]*translacao[2][2];

                *px = resultado[0];
                *py = resultado[1];
        }else{
                *px = *px+x;
                *py = *py+y;
        }
}

void Poligono::escalona (Janela mundo,Janela vp,double x, double y, double *px, double *py,double Xc, double Yc,boolean homogenea) {
        double ponto[3] = {*px,*py,1};

        double escalonamento[3][3] = {x, 0, 0, 0, y, 0, 0, 0, 1};
        double translacao[3][3] = {1, 0, 0, 0, 1, 0, -Xc, -Yc, 1};
        double translacaoBack[3][3] = {1, 0, 0, 0, 1, 0, Xc, Yc, 1};
        double resultado[3];
        if(homogenea){
                resultado[0] = ponto[0] * translacao[0][0] + ponto[1]*translacao[1][0] + ponto[2]*translacao[2][0];
                resultado[1] = ponto[0] * translacao[0][1] + ponto[1]*translacao[1][1] + ponto[2]*translacao[2][1];
                resultado[2] = ponto[0] * translacao[0][2] + ponto[1]*translacao[1][2] + ponto[2]*translacao[2][2];
                resultado[2] = 1;

                resultado[0] = resultado[0] * escalonamento[0][0] + resultado[1] * escalonamento[1][0] + resultado[2]*escalonamento[2][0];
                resultado[1] = resultado[0] * escalonamento[0][1] + resultado[1] * escalonamento[1][1] + resultado[2]*escalonamento[2][1];
                resultado[2] = resultado[0] * escalonamento[0][2] + resultado[1] * escalonamento[1][2] + resultado[2]*escalonamento[2][2];

                resultado[0] = resultado[0] * translacaoBack[0][0] + resultado[1]*translacaoBack[1][0] + resultado[2]*translacaoBack[2][0];
                resultado[1] = resultado[0] * translacaoBack[0][1] + resultado[1]*translacaoBack[1][1] + resultado[2]*translacaoBack[2][1];
                resultado[2] = resultado[0] * translacaoBack[0][2] + resultado[1]*translacaoBack[1][2] + resultado[2]*translacaoBack[2][2];

                *px = resultado[0];
                *py = resultado[1];
        }else{
                *px = *px*x;
                *py = *py*y;

        }

}

void Poligono::rotacao(double teta, double *px, double *py,double Xc, double Yc,boolean homogenea){
        double x,y;
        double ponto[3] = {*px,*py,1};
        double angulo =  teta*PI/180;
        double rotacao[3][3] = {{ cos(angulo), sin(angulo), 0},
                         {-sin(angulo), cos(angulo), 0},
                         {           0,           0, 1}};
        double resultado[3];
        double translacaoBack[3][3] = {1, 0, 0, 0, 1, 0, Xc, Yc, 1};
        double translacao[3][3] = {1, 0, 0, 0, 1, 0, -Xc, -Yc, 1};
        if(homogenea){
                resultado[0] = ponto[0] * translacao[0][0] + ponto[1]*translacao[1][0] + ponto[2]*translacao[2][0];
                resultado[1] = ponto[0] * translacao[0][1] + ponto[1]*translacao[1][1] + ponto[2]*translacao[2][1];
                resultado[2] = ponto[0] * translacao[0][2] + ponto[1]*translacao[1][2] + ponto[2]*translacao[2][2];

                x=resultado[0];
                y=resultado[1];
                resultado[0] = x * rotacao[0][0] + y*rotacao[1][0] + resultado[2]*rotacao[2][0];
                resultado[1] = x * rotacao[0][1] + y*rotacao[1][1] + resultado[2]*rotacao[2][1];
                resultado[2] = x * rotacao[0][2] + y*rotacao[1][2] + resultado[2]*rotacao[2][2];

                resultado[0] = resultado[0]*translacaoBack[0][0] + resultado[1]*translacaoBack[1][0] + resultado[2]*translacaoBack[2][0];
                resultado[1] = resultado[0]*translacaoBack[0][1] + resultado[1]*translacaoBack[1][1] + resultado[2]*translacaoBack[2][1];
                resultado[2] = resultado[0]*translacaoBack[0][2] + resultado[1]*translacaoBack[1][2] + resultado[2]*translacaoBack[2][2];

                *px = resultado[0];
                *py = resultado[1];
        }else{
                x = *px;
                y = *py;
                *px = x*cos(angulo)-y*sin(angulo);
                *py = x*sin(angulo)+y*cos(angulo);
        }
}

void Poligono::reflexo() {
        int i = pontos.size();

        for (int j = 0; j < i; j++) {
                pontos[j].reflexoX();
        }
}

double Poligono::PontoCentralX(){
  double Xc;
  for(int i = 0; i < pontos.size(); i++){
    Xc = Xc + pontos[i].x;

  }
  Xc = Xc/(pontos.size());
  return Xc;
}

double Poligono::PontoCentralY()
{
  double Yc;
  for(int i =0;i<pontos.size();i++){

    Yc = Yc + pontos[i].y;
  }

  Yc = Yc/(pontos.size());
  return Yc;
}

void Poligono::DesenhaPontoCircuferenca(TCanvas *canvas,int xc,int yc,int x,int y,Janela mundo, Janela vp,int raio){
            canvas->Ellipse(xc + raio, yc + raio, xc - raio, yc - raio);

}

void Poligono::DesenhaCircuferencia(TCanvas *canvas,int xc,int yc,int r,Janela mundo, Janela vp){
        int x,y,p;
        x = 0;
        y = r;
        p=1-r;

        while(x<y){
                if(p<0)
                        x++;
                else{
                        x++;
                        y--;
                }
                if(p<0)
                        p+=2*x+1;
                else
                p+=2*(x-y)+1;
                DesenhaPontoCircuferenca(canvas,xc,yc,x,y,mundo,vp,r);
        }
}
