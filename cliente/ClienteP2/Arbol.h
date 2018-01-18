#ifndef ARBOL_H
#define ARBOL_H
#include <QString>
#include <iostream>
#include <string>
using namespace std;

class nodoA
{
    public:
        string idEncargado;

        string id;
        string nombre;
        string apellido;
        int edad;
        string genero;

        nodoA* izq;
        nodoA* der;

        nodoA(string id_,string nombre_,string apellido_,int edad_,string genero_,string encargado):id(id_),nombre(nombre_),
            apellido(apellido_),edad(edad_),genero(genero_),izq(0),der(0),idEncargado(encargado){}
};

class Arbol
{
    public:
        int elementos;
        nodoA* raiz;
        bool agregar(string id,string nombre,string apellido,int edad,string genero,string encargado);
        bool agregar(nodoA* actual,nodoA* nuevo);
        nodoA* buscar(string id);
        nodoA* buscar(string id,nodoA* actual);
        bool modificar(string id,string idN,string nombre,string apellido,int edad,string genero);
        bool eliminar(string id);
        bool eliminar(nodoA* actual,nodoA* padre,string id);
        void graficar();
        void aux_graficar(nodoA* actual);
        Arbol():elementos(0),raiz(0) {}
};

#endif // ARBOL_H
