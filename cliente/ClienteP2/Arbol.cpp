
#include "Arbol.h"
#include <iostream>
#include <QString>
#include <string>
#include <fstream>
using namespace std;

nodoA* Arbol::buscar(string id){
    if(raiz!=0){
        return buscar(id,raiz);
    }
    return 0;
}

nodoA* Arbol::buscar(string id, nodoA *actual){
    if(actual!=0){
        if(id.compare(actual->id)==0){
            return actual;
        }else if(id.compare(actual->id)>0){//a>b
            return buscar(id,actual->der);
        }else if(id.compare(actual->id)<0){//a<b
            return buscar(id,actual->izq);
        }
    }
    return 0;
}

bool Arbol::agregar(string id, string nombre, string apellido, int edad, string genero,string encargado){
    if(raiz!=0){
        nodoA* nuevo = new nodoA(id,nombre,apellido,edad,genero,encargado);
        return agregar(raiz,nuevo);
    }else{
        raiz = new nodoA(id,nombre,apellido,edad,genero,encargado);
        return true;
    }
}

bool Arbol::agregar(nodoA *actual, nodoA *nuevo){
    if(actual!=0){
        if(nuevo->id.compare(actual->id)>0){//a>b
            if(actual->der!=0){
                return agregar(actual->der,nuevo);
            }else{
                actual->der = nuevo;
                return true;
            }
        }else if(nuevo->id.compare(actual->id)<0){//a<b
            if(actual->izq!=0){
                return agregar(actual->izq,nuevo);
            }else{
                actual->izq = nuevo;
                return true;
            }
        }
    }
    return false;
}

bool Arbol::modificar(string id, string idN, string nombre, string apellido, int edad, string genero){
    nodoA* busqueda = buscar(id);
    if(busqueda!=0){
        if(id.compare(idN)==0){//no se modifica el arbol
            busqueda->nombre = nombre;
            busqueda->apellido = apellido;
            busqueda->edad = edad;
            busqueda->genero = genero;
            return true;
        }else{//el arbol debe modificarse
            eliminar(id);
            agregar(idN,nombre,apellido,edad,genero,busqueda->idEncargado);
            return true;
        }
    }
    return false;
}

bool Arbol::eliminar(string id){
    if(raiz!=0){
        return eliminar(raiz,0,id);
    }
    return false;
}

bool Arbol::eliminar(nodoA *actual, nodoA *padre,string id){
    if(actual!=0){
        if(actual->id.compare(id)==0){//es el nodo a eliminar

                if(actual->izq!=0){//tien hijo izq
                    nodoA* temp = actual->izq;
                    nodoA* father = 0;
                    while(temp->der!=0){
                        father = temp;
                        temp = temp->der;
                    }
                    if(father!=0){//si hay padre
                        actual->id = temp->id;
                        actual->nombre = temp->nombre;
                        actual->apellido = temp->apellido;
                        actual->edad = temp->edad;
                        actual->genero = temp->genero;
                        father->der = 0;
                        delete temp;
                        return true;
                    }else{//el nodo izq no tiene hijos
                        if(padre!=0){
                            if(padre->der==actual){
                                padre->der = temp;
                            }else{
                                padre->izq = temp;
                            }
                            temp->der = actual->der;
                        }else{
                            temp->der = raiz->der;
                            raiz = temp;
                        }
                        delete actual;
                        return true;
                    }
                }else if(actual->der!=0){//no tiene hijo izq, pero si der
                    if(padre!=0){
                        if(padre->der==actual)
                            padre->der = actual->der;
                        else
                            padre->izq = actual->der;
                    }else{
                        raiz = actual->der;
                    }
                    delete actual;
                    return true;
                }else{//solo tiene la raiz
                    if(padre!=0){
                        if(padre->der==actual)
                            padre->der = 0;
                        else
                            padre->izq = 0;
                    }else{
                        raiz = 0;
                    }
                    delete actual;
                    return true;
                }

        }else if(id.compare(actual->id)>0){
            return eliminar(actual->der,actual,id);
        }else
            return eliminar(actual->izq,actual,id);
    }
    return false;
}

void Arbol::graficar(){
    ofstream graph;
    graph.open("binario.txt",ios::out);
    graph<<"Digraph g{\n";
    graph.close();
    if(raiz!=0){
        aux_graficar(raiz);
    }else{
        graph.open("binario.txt",ios::out | ios::app);
        graph<<"\"no hay datos\"";
        graph.close();
    }
    graph.open("binario.txt",ios::out | ios::app);
    graph<<"\n}";
    graph.close();
    system("dot -Tpng binario.txt -o binario.png");
}

void Arbol::aux_graficar(nodoA *actual){
    ofstream graph;
    if(actual!=0){
        graph.open("binario.txt",ios::out | ios::app);
        graph<<"\"nodo"<<actual->id<<actual->nombre<<"\" [shape=record,label=\"{"<<actual->id<<"|"<<actual->nombre<<"\\l"<<actual->apellido
            <<"\\l"<<actual->edad<<"\\l"<<actual->genero<<"\\l}\"];\n";
        if(actual->izq!=0){
            graph<<"\"nodo"<<actual->id<<actual->nombre<<"\" -> \"nodo"<<actual->izq->id<<actual->izq->nombre<<"\"[color=blue];\n";
            aux_graficar(actual->izq);
        }
        if(actual->der!=0){
            graph<<"\"nodo"<<actual->id<<actual->nombre<<"\" -> \"nodo"<<actual->der->id<<actual->der->nombre<<"\"[color=red];\n";
            aux_graficar(actual->der);
        }
    }
}
