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


//TRANSFORMAÇÃO
//---------------------------------------------------------------------------------------------------------------------------------------------
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


void Poligono::circunferencia(Ponto aux, int r){
   Ponto aux2 = aux;

   for (int i = 0; i <= 360; i++) {
        aux.x = r*cos(i*(PI/180)) + aux2.x;
        aux.y = r*sin(i*(PI/180)) + aux2.y;
        pontos.push_back(aux);
   }
}

//CURVAS
//---------------------------------------------------------------------------------------------------------------------------------------------
void Poligono::casteljau(Ponto p1, Ponto p2, Ponto p3, double limite){
double mxP12, myP12, mxP23, myP23;
double distancia;

mxP12 = (p1.x + p2.x) / 2;
myP12 = (p1.y + p2.y) / 2;

mxP23 = (p2.x + p3.x) / 2;
myP23 = (p2.y + p3.y) / 2;

Ponto nP1(mxP12,myP12);
Ponto nP2(mxP23,myP23);
Ponto nP3((mxP12 + mxP23)/2,(myP12 + myP23)/2);

distancia = (double) sqrt(((mxP12 - mxP23) * (mxP12 - mxP23)) + ((myP12 - myP23) * (myP12 - myP23)));

if(distancia > limite){
        casteljau(p1, nP1, nP3, limite);
        casteljau(nP3, nP2, p3, limite);
}
        else{
                pontos.push_back(nP3);
        }

}


void Poligono::hermite(Ponto p1, Ponto p2, Ponto p3, Ponto p4){
double xT;
double yT;
Ponto aux, r1, r4;

        r1.x = p2.x - p1.x;
        r1.y = p2.y - p1.y;

        r4.x = p4.x - p3.x;
        r4.y = p4.y - p3.y;

        for (double t = 0; t < 1; t += (double) 0.01)
            {

                xT = p1.x * (2 * t * t * t - 3 * t * t + 1) + p4.x * (-2 * t * t * t + 3 * t * t) + r1.x * (t * t * t - 2 * t * t + t) + r4.x * (t * t * t - t * t);
                yT = p1.y * (2 * t * t * t - 3 * t * t + 1) + p4.y * (-2 * t * t * t + 3 * t * t) + r1.y * (t * t * t - 2 * t * t + t) + r4.y * (t * t * t - t * t);

                aux.x = xT;
                aux.y = yT;
                pontos.push_back(aux);
            }
}

void Poligono::bezier(Ponto p1, Ponto p2, Ponto p3, Ponto p4){
double xT;
double yT;
Ponto aux;

        for (double t = 0; t < 1; t += (double) 0.01)
            {
                xT = (double) (p1.x * (pow(1 - t, 3)) + p2.x * 3 * t * (pow(1 - t, 2)) + 3 * t * t * p3.x * (1 - t) + t * t * t * p4.x);
                yT = (double) (p1.y * (pow(1 - t, 3)) + p2.y * 3 * t * (pow(1 - t, 2)) + 3 * t * t * p3.y * (1 - t) + t * t * t * p4.y);

                aux.x = xT;
                aux.y = yT;
                pontos.push_back(aux);
            }
}

/*void Poligono::bSpline(){
int contador = pontos.size();
int ancora = 0;


        for(int i = 0; i < contador-3; i++){
                double xT,yT;
                Ponto p1,p2,p3,p4;
                Ponto aux;

                p1.x = pontos[ancora].x;
                p1.y = pontos[ancora].y;

                p2.x = pontos[ancora+1].x;
                p2.y = pontos[ancora+1].y;

                p3.x = pontos[ancora+2].x;
                p3.y = pontos[ancora+2].y;

                p4.x = pontos[ancora+3].x;
                p4.y = pontos[ancora+3].y;

                for(double t = 0; t < 1; t += (double) 0.01){
                        xT = pow(t, 3) * (-p1.x / 6 + p2.x / 2 - p3.x / 2 + p4.x / 6) + pow(t, 2) * (p1.x / 2 - p2.x + p3.x / 2) + t * (-p1.x / 2 + p3.x / 2) + p1.x / 4 + 4 * p2.x / 6 + p3.x / 6;
                        yT = pow(t, 3) * (-p1.y / 6 + p2.y / 2 - p3.y / 2 + p4.y / 6) + pow(t, 2) * (p1.y / 2 - p2.y + p3.y / 2) + t * (-p1.y / 2 + p3.y / 2) + p1.y / 4 + 4 * p2.y / 6 + p3.y / 6;

                        aux.x = xT;
                        aux.y = yT;
                        pontos.push_back(aux);
                }
                ancora++;
        }
} */


/*void Poligono::bSpline(Ponto p1, Ponto p2, Ponto p3, Ponto p4){
Poligono auxPol;
double p1x,p2x,p3x,p4x,xT,yT;
double p1y,p2y,p3y,p4y;
Ponto aux;



        for(double t = 0; t < 1; t += (double) 0.01){
                        xT = pow(t, 3) * (-p1.x / 6 + p2.x / 2 - p3.x / 2 + p4.x / 6) + pow(t, 2) * (p1.x / 2 - p2.x + p3.x / 2) + t * (-p1.x / 2 + p3.x / 2) + p1.x / 4 + 4 * p2.x / 6 + p3.x / 6;
                        yT = pow(t, 3) * (-p1.y / 6 + p2.y / 2 - p3.y / 2 + p4.y / 6) + pow(t, 2) * (p1.y / 2 - p2.y + p3.y / 2) + t * (-p1.y / 2 + p3.y / 2) + p1.y / 4 + 4 * p2.y / 6 + p3.y / 6;

                        aux.x = xT;
                        aux.y = yT;
                        pontos.push_back(aux);
                }

}*/

/*void Poligono::bSpline2(){
Poligono auxPol;
double p1x,p2x,p3x,p4x,xT,yT;
double p1y,p2y,p3y,p4y;
Ponto aux;
Ponto p1,p2,p3,p4;

        p1x = p1.x;
        p1y = p1.y;
        p2x = p2.x;
        p2y = p2.y;
        p3x = p3.x;
        p3y = p3.y;
        p4x = p4.x;
        p4y = p4.y;

        for(double t = 0; t < 1; t += (double) 0.01){
                        xT = pow(t, 3) * (-p1.x / 6 + p2.x / 2 - p3.x / 2 + p4.x / 6) + pow(t, 2) * (p1.x / 2 - p2.x + p3.x / 2) + t * (-p1.x / 2 + p3.x / 2) + p1.x / 4 + 4 * p2.x / 6 + p3.x / 6;
                        yT = pow(t, 3) * (-p1.y / 6 + p2.y / 2 - p3.y / 2 + p4.y / 6) + pow(t, 2) * (p1.y / 2 - p2.y + p3.y / 2) + t * (-p1.y / 2 + p3.y / 2) + p1.y / 4 + 4 * p2.y / 6 + p3.y / 6;

                        aux.x = xT;
                        aux.y = yT;
                        pontos.push_back(aux);
                }
}*/

void Poligono::bSpline(Ponto p1, Ponto p2, Ponto p3, Ponto p4){
Poligono auxPol;
double xT,yT;
Ponto aux;



        for(double t = 0; t < 1; t += (double) 0.01){
                        xT = p1.x + 4 * p2.x + p3.x - p1.x*t*t*t + 3 * p2.x *t*t*t -3 * p3.x * t*t*t + p4.x *t*t*t + 3*p1.x *t*t -6*p2.x *t*t + 3*p3.x*t*t - 3*p1.x *t + 3*p3.x*t;
                        yT = p1.y + 4 * p2.y + p3.y - p1.y*t*t*t + 3 * p2.y *t*t*t -3 * p3.y * t*t*t + p4.y *t*t*t + 3*p1.y *t*t -6*p2.y *t*t + 3*p3.y*t*t - 3*p1.y *t + 3*p3.y*t;

                        aux.x = xT/6;
                        aux.y = yT/6;
                        pontos.push_back(aux);
                }

}

void Poligono::fwdDifferences(Ponto p1, Ponto p2, Ponto p3, Ponto p4){
Ponto aux;
double delta1 = (double)(1/100.0);
double delta2 = (double)(pow((double)delta1, 2));
double delta3 = (double)(pow((double)delta1, 3));

double MatrizBezier[4][4] = { { -1, 3, -3, 1 },
                              { 3, -6, 3, 0 },
                              { -3, 3, 0, 0 },
                              { 1, 0, 0, 0 } };

double mPtsX[4][1] = { { p1.x },
                       { p2.x },
                       { p3.x },
                       { p4.x }};

double mPtsY[4][1] = { { p1.y },
                       { p2.y },
                       { p3.y },
                       { p4.y }};

double matrizX[4][4];
double matrizY[4][4];

//MULTIPLICA MATRIZ BEZIER * MATRIZX
        for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 1; j++)
                {
                    for(int k = 0; k < 4; k++)
                    {
                        matrizX[i][j] += MatrizBezier[i][k] * mPtsX[k][j];
                    }
                }
            }
            
//MULTIPLICA MATRIZ BEZIER * MATRIZY
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 1; j++)
                {
                    for(int k = 0; k < 4; k++)
                    {
                        matrizY[i][j] += MatrizBezier[i][k] * mPtsY[k][j];
                    }
                }
            }
//MATRIZES FWD DIFFERENCES
double matrizDx[4][1];
double matrizDy[4][1];

            //MATRIZ FWD DELTA X

            matrizDx[0][0] = matrizX[3][0];
            matrizDx[1][0] = matrizX[0][0] * delta3 + matrizX[1][0] * delta2 + matrizX[2][0] * delta1;
            matrizDx[2][0] = 6 * matrizX[0][0] * delta3 + 2 * matrizX[1][0] * delta2;
            matrizDx[3][0] = 6 * matrizX[0][0] * delta3;

            //MATRIZ FWD DELTA Y

            matrizDy[0][0] = matrizY[3][0];
            matrizDy[1][0] = matrizY[0][0] * delta3 + matrizY[1][0] * delta2 + matrizY[2][0] * delta1;
            matrizDy[2][0] = 6 * matrizY[0][0] * delta3 + 2 * matrizY[1][0] * delta2;
            matrizDy[3][0] = 6 * matrizY[0][0] * delta3;

            //CALCULA FWD

            for (double t = 0; t < 100; t++)
            {
                matrizDx[0][0] += matrizDx[1][0];
                matrizDx[1][0] += matrizDx[2][0];
                matrizDx[2][0] += matrizDx[3][0];

                matrizDy[0][0] += matrizDy[1][0];
                matrizDy[1][0] += matrizDy[2][0];
                matrizDy[2][0] += matrizDy[3][0];

                aux.x = matrizDx[0][0];
                aux.y = matrizDy[0][0];
                pontos.push_back(aux);

            }



}
