#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include <QString>
#include <QList>
using namespace std;

class nodoAG{
    public:
        string codigo;
        double peso;
        nodoAG(string codigo_,double peso_):codigo(codigo_),peso(peso_){}
};

class nodoG
{
    public:
        string codigo;
        double latitud,longitud;

        nodoAG* masCorto;
        double pesoPila;

        QList<nodoAG *> aristas;
        nodoG(string codigo_,double latitud_,double longitud_):codigo(codigo_),latitud(latitud_),longitud(longitud_),
            masCorto(0),pesoPila(0){}
};

#endif // GRAFO_H
