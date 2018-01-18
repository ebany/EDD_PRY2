
#include "Diccionario.h"
#include "Arbol.h"
#include <iostream>
#include <QString>
#include <QList>
#include <string>
#include <fstream>
using namespace std;

int Diccionario::funcionHash(string k){
    int llave = 0;
    QString cadena = k.c_str();
    for(int a = 0; a<cadena.length();a++){
        llave += cadena[a].toLatin1();
    }

    return llave%this->size;
}

int Diccionario::dobleDispersion(string k, int intento, int posicion){
    int llave = 0;
    QString cadena = k.c_str();
    for(int a = 0; a<cadena.length();a++){
        llave += cadena[a].toLatin1();
    }

    int nueva = (posicion+(llave%49 + 1)*intento);
    while(nueva>=this->size)
        nueva = nueva - this->size;
    return nueva;
}

bool Diccionario::eliminar(string id, string encargado){
    int indice = funcionHash(encargado);
    if(this->tabla[indice]!=0){
        if(this->tabla[indice]->encargado.compare(encargado)==0){
            if(id.compare(encargado)==0){
                this->tabla[indice] = 0;
                this->elementos--;
                return true;
            }else{
                return this->tabla[indice]->arbol->eliminar(id);
            }
        }else{
            for(int a=1;a<6;a++){
                indice = dobleDispersion(encargado,a,indice);
                if(this->tabla[indice]!=0 && this->tabla[indice]->encargado.compare(encargado)==0){
                    if(id.compare(encargado)==0){
                        this->tabla[indice] = 0;
                        this->elementos--;
                        return true;
                    }else{
                        return this->tabla[indice]->arbol->eliminar(id);
                    }
                }
            }
        }
    }
    return false;
}

bool Diccionario::agregar(string id, string nombre, string apellido, int edad, string genero, string encargado){
    int indice = funcionHash(encargado);
    if(this->tabla[indice]!=0){//ya hay un elemento
        if(this->tabla[indice]->encargado.compare(encargado)==0){//familia
            return this->tabla[indice]->arbol->agregar(id,nombre,apellido,edad,genero,encargado);
        }else{//colision
            for(int a=1;a<6;a++){
                indice = dobleDispersion(encargado,a,indice);
                if(this->tabla[indice]==0){//agregar
                    this->tabla[indice] = new nodoD();
                    this->tabla[indice]->encargado = encargado;
                    this->tabla[indice]->arbol->agregar(id,nombre,apellido,edad,genero,encargado);
                    this->elementos++;
                    return true;
                }
            }
        }
    }else{//agregar
        this->tabla[indice] = new nodoD();
        this->tabla[indice]->encargado = encargado;
        this->tabla[indice]->arbol->agregar(id,nombre,apellido,edad,genero,encargado);
        this->elementos++;
        return true;
    }
    return false;
}

void Diccionario::graficar(){
    ofstream graph;
    graph.open("diccionario.txt",ios::out);
    graph<<"Digraph g{\n";
    if(this->elementos!=0){
        graph<<"node0 [shape=record,label =\"";
        for(int a=0;a<this->size;a++){
            if(this->tabla[a]!=0){
                graph<<"<f"<<a<<">"<<this->tabla[a]->encargado<<"|";
            }
        }
        graph<<"\"];\n";
        for(int a=0;a<this->size;a++){
            if(this->tabla[a]!=0){
                graph<<"node0:f"<<a<<" -> \"node"<<this->tabla[a]->encargado<<this->tabla[a]->arbol->raiz->id<<"\";\n";
                QList<nodoA *> pila ;
                pila.push_back(this->tabla[a]->arbol->raiz);
                while(!pila.isEmpty()){
                    nodoA* temp = pila[0];

                    graph<<"\"node"<<temp->idEncargado<<temp->id<<"\" [shape=record,label=\"{"<<temp->id<<"|"<<temp->nombre<<"\\l"<<temp->apellido
                        <<"\\l"<<temp->edad<<"\\l"<<temp->genero<<"\\l}\"];\n";
                    pila.pop_front();
                    if(temp->izq!=0){
                        graph<<"\"node"<<temp->idEncargado<<temp->id<<"\" -> \"node"<<temp->izq->idEncargado<<temp->izq->id<<"\"[color=blue];\n";
                        pila.push_back(temp->izq);
                    }
                    if(temp->der!=0){
                        graph<<"\"node"<<temp->idEncargado<<temp->id<<"\" -> \"node"<<temp->der->idEncargado<<temp->der->id<<"\"[color=red];\n";
                        pila.push_back(temp->der);
                    }
                }
            }
        }
    }else
        graph<<"\"No hay personas registradas\"";

    graph<<"\n}";
    graph.close();
    system("dot -Tpng diccionario.txt -o diccionario.png");
}
