
#include <iostream>
#include <QString>
#include <QList>
#include <string>
#include <fstream>
#include "Hash.h"
using namespace std;

bool nodoSub::agregar(string nombre, int cantidad){
    informacion* busqueda = buscar(nombre);
    if(busqueda!=0){
        busqueda->cantidad += cantidad;
        return true;
    }else{
        informacion* nuevo = new informacion(nombre,cantidad);
        this->productos.append(nuevo);
        this->noElementos++;
        return true;
    }
    return false;
}

informacion* nodoSub::buscar(string nombre){
    for(int a=0;a<this->noElementos;a++){
        if(this->productos[a]->nombre.compare(nombre)==0)
            return this->productos[a];
    }
    return 0;
}

int nodoSub::total(){
    int cantidad = 0;
    for(int a=0;a<this->noElementos;a++){
        cantidad += this->productos[a]->cantidad;
    }
    return cantidad;
}

bool nodoSub::verificar(){

}

/*-------------------------------------------------------------------*/

//no existe la categoria
bool nodoHash::agregar(string categoria, string nombre, int cantidad){
    nodoSub* nuevo = new nodoSub(categoria);
    nuevo->agregar(nombre,cantidad);
    this->elementos.append(nuevo);
    this->noElementos++;
    return true;
}

/*-------------------------------------------------------------------*/
int Tabla::F(string k){
    QString cadena = k.c_str();
    //cadena = cadena.toLower();
    int valor = 0;
    for(int a = 0; a<cadena.length();a++){
        //cout<<cadena[a].toLatin1()<<endl;
        valor += cadena[a].toLatin1();
    }
    //cout<<valor%this->size<<endl;
    return valor%this->size;
}

bool Tabla::hayDatos(){
    int elementos = 0;
    for(int a=0;a<this->size;a++){
        if(this->tablaHash[a]!=0)
            elementos++;
    }
    if(elementos!=0)
        return true;
    return false;
}

nodoSub* Tabla::buscar(int indice,string categoria){
    if(this->tablaHash[indice]!=0){
        nodoHash* aux = this->tablaHash[indice];
        for(int a=0;a<aux->noElementos;a++){
            if(aux->elementos[a]->categoria.compare(categoria)==0)
                return aux->elementos[a];
        }
    }
    return 0;
}

bool Tabla::modificarRecurso(string categoria, string nombre, string categoriaN, string nombreN, int cantidad){

    int indice = F(categoria);
    if(this->tablaHash[indice]!=0){
        for(int a=0; a<this->tablaHash[indice]->noElementos;a++){
            if(this->tablaHash[indice]->elementos[a]->categoria.compare(categoria)==0){
                for(int b=0;b<this->tablaHash[indice]->elementos[a]->noElementos;b++){
                    if(this->tablaHash[indice]->elementos[a]->productos[b]->nombre.compare(nombre)==0){
                        if(categoria.compare(categoriaN)==0){//misma categoria
                            this->tablaHash[indice]->elementos[a]->productos[b]->nombre = nombreN;
                            this->tablaHash[indice]->elementos[a]->productos[b]->cantidad = cantidad;
                            return true;
                        }else{
                            entregar(categoria,nombre,this->tablaHash[indice]->elementos[a]->productos[b]->cantidad);
                            agregar(categoriaN,nombreN,cantidad);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Tabla::eliminarAcopio(string categoria){
    int indice = F(categoria);
    if(this->tablaHash[indice]!=0){
        for(int a=0; a<this->tablaHash[indice]->noElementos;a++){
            if(this->tablaHash[indice]->elementos[a]->categoria.compare(categoria)==0){
                this->tablaHash[indice]->elementos.removeAt(a);
                this->tablaHash[indice]->noElementos--;
                if(this->tablaHash[indice]->noElementos==0){//no hay mas elementos en el indice
                    this->tablaHash[indice]=0;
                }
                return true;
            }
        }
    }
    return false;
}

bool Tabla::modificarAcopio(string categoria,string categoriaN){
    int indice = F(categoria);
    if(this->tablaHash[indice]!=0){
        for(int a=0; a<this->tablaHash[indice]->noElementos;a++){
            if(this->tablaHash[indice]->elementos[a]->categoria.compare(categoria)==0){
                for(int b=0;b<this->tablaHash[indice]->elementos[a]->noElementos;b++){
                    agregar(categoriaN,this->tablaHash[indice]->elementos[a]->productos[b]->nombre,this->tablaHash[indice]->elementos[a]->productos[b]->cantidad);
                }
                eliminarAcopio(categoria);
                return true;
            }
        }
    }
    return false;
}

bool Tabla::entregar(string categoria, string nombre, int cantidad){
    int indice = F(categoria);
    if(this->tablaHash[indice]!=0){
        for(int a=0; a<this->tablaHash[indice]->noElementos;a++){
            if(this->tablaHash[indice]->elementos[a]->categoria.compare(categoria)==0){
                for(int b=0;b<this->tablaHash[indice]->elementos[a]->noElementos;b++){
                    if(this->tablaHash[indice]->elementos[a]->productos[b]->nombre.compare(nombre)==0){
                        this->tablaHash[indice]->elementos[a]->productos[b]->cantidad -= cantidad;
                        if(this->tablaHash[indice]->elementos[a]->productos[b]->cantidad==0){//se agoto el elemento de ese nombre
                            this->tablaHash[indice]->elementos[a]->productos.removeAt(b);
                            this->tablaHash[indice]->elementos[a]->noElementos--;
                            if(this->tablaHash[indice]->elementos[a]->noElementos==0){//se agoto los productos en la categoria
                                this->tablaHash[indice]->elementos.removeAt(a);
                                this->tablaHash[indice]->noElementos--;
                                if(this->tablaHash[indice]->noElementos==0){//el indice no tiene mas categorias
                                    this->tablaHash[indice] = 0;
                                }
                            }
                        }
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Tabla::agregar(string categoria, string nombre, int cantidad){
    int indice = F(categoria);
    if(this->tablaHash[indice]!=0){//Colision, ya hay elementos
        nodoSub* busqueda = buscar(indice,categoria);
        if(busqueda!=0){//existe la categoria
            return busqueda->agregar(nombre,cantidad);
        }else{//no existe la categoria
            return this->tablaHash[indice]->agregar(categoria,nombre,cantidad);
        }
    }else{//esta vacia la posicion, no hay elementos
        this->tablaHash[indice] = new nodoHash();
        return this->tablaHash[indice]->agregar(categoria,nombre,cantidad);
    }
    return false;
}

void Tabla::graficar(){
    ofstream graph;
    graph.open("hash.txt",ios::out);
    graph<<"Digraph g{\nnodesep=.05\nrankdir=LR;\nnode [shape=record,width=.1,height=.1];\nnodeA [label=\"K: Llave\\l|E: Elementos en total\\l|G: Elementos por nombre\\l\"];\n";

    if(hayDatos()){
        //llaves
        graph<<"node0 [label = \"K|";
        for(int a=0;a<this->size;a++){
            graph<<"<f"<<a<<">"<<a;
            if(a!=this->size-1)
                graph<<"|";
        }
        graph<<"\",height=2.5];\n";

        graph<<"node [width = 2.5]\n";

        //elementos y colisiones
        for(int a=0;a<this->size;a++){
            if(this->tablaHash[a]!=0){
                graph<<"node"<<a+1<<" [label = \"{<n>"<<this->tablaHash[a]->elementos[0]->categoria<<"|G: "
                    <<this->tablaHash[a]->elementos[0]->noElementos<<"|<p>E: "
                    <<this->tablaHash[a]->elementos[0]->total()<<"}\"];\n";
                if(this->tablaHash[a]->noElementos>1){
                    for(int b=1;b<this->tablaHash[a]->noElementos;b++){
                        graph<<"node"<<a+1<<b<<"A [label = \"{<n>"<<this->tablaHash[a]->elementos[b]->categoria<<"|G: "
                            <<this->tablaHash[a]->elementos[b]->noElementos<<"|<p>E: "
                            <<this->tablaHash[a]->elementos[b]->total()<<"}\"];\n";
                        if(b==1)
                            graph<<"node"<<a+1<<":p"<<" -> node"<<a+1<<b<<"A:n;\n";
                        else
                            graph<<"node"<<a+1<<b-1<<"A:p"<<" -> node"<<a+1<<b<<"A:n;\n";
                    }
                }
                graph<<"node0:f"<<a<<" -> node"<<a+1<<":n;\n";
            }
        }
    }else
        graph<<"\"No hay Recursos. (Codos >:v)\"";

    graph<<"\n}";
    graph.close();
    system("dot -Tpng hash.txt -o hash.png");
}
