#ifndef DICCIONARIO_H
#define DICCIONARIO_H
#include <iostream>
#include <QString>
#include <string>
#include "Arbol.h"
using namespace std;

class nodoD
{
    public:
        Arbol* arbol;
        string encargado;
        nodoD():arbol(new Arbol()) {}
};

class Diccionario
{
    int funcionHash(string k);
    int dobleDispersion(string k,int intento,int posicion);
    public:
        int elementos;
        int size;
        nodoD* tabla[97];

        bool agregar(string id,string nombre,string apellido,int edad,string genero,string encargado);
        bool eliminar(string id,string encargado);
        void graficar();

        Diccionario():size(97),elementos(0){
            for(int a=0;a<97;a++)
                tabla[a]=0;
        }
};

#endif // DICCIONARIO_H
