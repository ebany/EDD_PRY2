#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <QString>
#include <QList>
#include <string>
using namespace std;

class informacion{
    public:
        string nombre;
        int cantidad;

        informacion(string nombre_,int cantidad_):nombre(nombre_),cantidad(cantidad_){}
};

class nodoSub{
    public:
        string categoria;
        int noElementos;
        QList<informacion *> productos;

        nodoSub(string categoria_):categoria(categoria_),noElementos(0){}

        int total();
        bool agregar(string nombre,int cantidad);
        informacion* buscar(string nombre);
        bool verificar();//verificar que tenga elementos y que la cantidad de sus elementos no sea 0. si es true, eliminar de nodoHash
};

class nodoHash
{
    public:
        int noElementos;
        QList<nodoSub *> elementos;
        bool agregar(string categoria,string nombre,int cantidad);
        nodoHash():noElementos(0){}
};

class Tabla
{
    int F(string k);
    bool hayDatos();
    public:
        int size;
        nodoHash* tablaHash[23];

        bool agregar(string categoria,string nombre,int cantidad);
        bool entregar(string categoria,string nombre,int cantidad);
        bool eliminarAcopio(string categoria);
        bool modificarRecurso(string categoria,string nombre,string categoriaN,string nombreN,int cantidad);
        bool modificarAcopio(string categoria,string categoriaN);
        nodoSub* buscar(int indice,string categoria);
        void graficar();
        string json();

        Tabla():size(23){
            for(int a=0;a<23;a++)
                tablaHash[a] = 0;
        }
};

#endif // HASH_H
