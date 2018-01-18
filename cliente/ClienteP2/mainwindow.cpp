#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Hash.h"
#include "Arbol.h"
#include "Grafo.h"
#include "Diccionario.h"
#include <QFile>
#include <QtNetwork>
#include <QStandardItem>
#include <QList>
#include <sstream>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <iostream>
#include <QString>
using namespace std;

Tabla* tabla = new Tabla();
Diccionario* diccionario = new Diccionario();

QList<nodoG *> grafo;
QString mapaHtml = "";

QStandardItemModel* model;
QStandardItemModel* model2;
QStandardItemModel* model3;
QStandardItemModel* model4;

bool acopio = false;
string selCategoria = "";
string selNombre  = "";
int selCantidad = 0;
QString acopioName ="";

bool albergue = false;
string selId = "";
string selEncargado = "";
QString albergueName="";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    verificarLogin();

    this->ui->tabWidget->setTabText(0,"Donaciones");
    this->ui->tabWidget->setTabText(1,"Albergues");
    this->ui->tabWidget->setTabText(2,"Centros de acopio");
    this->ui->tabWidget->setTabText(3,"Mapa");

    model = new QStandardItemModel(0,3,this);
    model->setHorizontalHeaderItem(0,new QStandardItem("Fecha"));
    model->setHorizontalHeaderItem(1,new QStandardItem("Monto"));
    model->setHorizontalHeaderItem(2,new QStandardItem("Voucher"));
    this->ui->tableView->setModel(model);
    this->ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    model2 = new QStandardItemModel(0,3,this);
    model2->setHorizontalHeaderItem(0,new QStandardItem("Categoría"));
    model2->setHorizontalHeaderItem(1,new QStandardItem("Nombre"));
    model2->setHorizontalHeaderItem(2,new QStandardItem("Cantidad"));
    this->ui->tableView_2->setModel(model2);
    this->ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    model3 = new QStandardItemModel(0,6,this);
    model3->setHorizontalHeaderItem(0,new QStandardItem("Identificación"));
    model3->setHorizontalHeaderItem(1,new QStandardItem("Nombre"));
    model3->setHorizontalHeaderItem(2,new QStandardItem("Apellido"));
    model3->setHorizontalHeaderItem(3,new QStandardItem("Edad"));
    model3->setHorizontalHeaderItem(4,new QStandardItem("Género"));
    model3->setHorizontalHeaderItem(5,new QStandardItem("Encargado"));
    this->ui->tableView_3->setModel(model3);
    this->ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);

    model4 = new QStandardItemModel(0,3,this);
    model4->setHorizontalHeaderItem(0,new QStandardItem("ptoA"));
    model4->setHorizontalHeaderItem(1,new QStandardItem("Peso"));
    model4->setHorizontalHeaderItem(2,new QStandardItem("ptoB"));
    this->ui->tableView_4->setModel(model4);
    this->ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView_4->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::verificarLogin(){
    if(acopio){
        this->ui->groupBox_3->setEnabled(true);
        this->ui->groupBox_10->setEnabled(true);
        this->ui->groupBox_4->setEnabled(false);
    }else{
        this->ui->groupBox_3->setEnabled(false);
        this->ui->groupBox_10->setEnabled(false);
        this->ui->groupBox_4->setEnabled(true);
    }
    if(albergue){
        this->ui->groupBox_11->setEnabled(true);
        this->ui->groupBox_12->setEnabled(true);
        this->ui->groupBox_9->setEnabled(false);
    }else{
        this->ui->groupBox_11->setEnabled(false);
        this->ui->groupBox_12->setEnabled(false);
        this->ui->groupBox_9->setEnabled(true);
    }

}

void MainWindow::on_btnConsultaArbol_clicked()
{
    QString envio = this->ui->lineEdit_9->text();
    if(!envio.isEmpty()){
        QEventLoop eventLoop;
        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/getDonaciones") ) );
        req.setRawHeader("Content-Type", "text/plain");
        QNetworkReply *reply = mgr.post(req, envio.toUtf8());
        eventLoop.exec(); // blocks stack until "finished()" has been called

        if (reply->error() == QNetworkReply::NoError) {
            //success
            //qDebug() << "Success" <<reply->readAll();
            QString respuesta = reply->readAll();
            if(respuesta.compare("no")==0){
                QMessageBox::warning(this,"Informacion","Codigo Invalido. >:v");
            }else{
                model->clear();
                model->setHorizontalHeaderItem(0,new QStandardItem("Fecha"));
                model->setHorizontalHeaderItem(1,new QStandardItem("Monto"));
                model->setHorizontalHeaderItem(2,new QStandardItem("Voucher"));

                respuesta = "[" + respuesta;
                respuesta.append("]");

                QJsonDocument documento = QJsonDocument::fromJson(respuesta.toStdString().c_str());
                QJsonArray ab = documento.array();
                for(int i=0; i<ab.count();i++){
                    QJsonObject sublugar=ab.at(i).toObject();
                    string nombre = sublugar.value("nombre").toString().toStdString();
                    string apellido = sublugar.value("apellido").toString().toStdString();
                    this->ui->lineEdit_6->setText(nombre.c_str());
                    this->ui->lineEdit_7->setText(apellido.c_str());

                    QJsonValue value2 = sublugar.value("lista");
                    if(value2.isArray()){
                        QJsonArray ja2=value2.toArray();
                        for(int i=0; i<ja2.count();i++){
                            QJsonObject subtree2=ja2.at(i).toObject();
                            string voucher = subtree2.value("voucher").toString().toStdString();
                            string fecha = subtree2.value("fecha").toString().toStdString();
                            double monto = subtree2.value("monto").toDouble();
                            stringstream convert;
                            convert<<monto;

                            QStandardItem *valBA = new QStandardItem(voucher.c_str());
                            QStandardItem *valBB = new QStandardItem(fecha.c_str());
                            QStandardItem *valBD = new QStandardItem(convert.str().c_str());

                            model->setItem(i,0,valBB);
                            model->setItem(i,1,valBD);
                            model->setItem(i,2,valBA);

                        }
                    }
                }
            }
            delete reply;
        } else {
            //failure
            //qDebug() << "Failure" <<reply->errorString();
            QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion. :(");
            delete reply;
        }
    }else
        QMessageBox::warning(this,"Informacion","Ingrese un numero de identificacion. >:v");
}

void MainWindow::on_btnDonar_clicked()
{
    string dpi = this->ui->lineEdit->text().toStdString();
    string nombre = this->ui->lineEdit_2->text().toStdString();
    string apellido = this->ui->lineEdit_3->text().toStdString();
    string voucher = this->ui->lineEdit_4->text().toStdString();
    string monto = this->ui->lineEdit_5->text().toStdString();

    string ano = this->ui->comboBox_3->currentText().toStdString();
    string mes = this->ui->comboBox_4->currentText().toStdString();
    string dia = this->ui->comboBox_5->currentText().toStdString();

    string envio = "";
    bool valido = false;

    if(dpi.empty() && nombre.empty() && apellido.empty() && !voucher.empty() && !monto.empty()){
        dpi = "000";
        nombre = "anonimo";
        apellido = "anonimo";
        valido = true;
    }else if(!dpi.empty() && !nombre.empty() && !apellido.empty() && !voucher.empty() && !monto.empty()){
        valido = true;
    }

    if(valido){
        envio = dpi +"@@"+nombre +"@@"+apellido+"@@"+voucher+"@@"+monto+"@@"+ano+"-"+mes+"-"+dia;
        QString bueno = envio.c_str();

        QEventLoop eventLoop;
        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/makeDonacion") ) );
        req.setRawHeader("Content-Type", "text/plain");
        QNetworkReply *reply = mgr.post(req, bueno.toUtf8());
        eventLoop.exec(); // blocks stack until "finished()" has been called

        if (reply->error() == QNetworkReply::NoError) {
            //success
            //qDebug() << "Success" <<reply->readAll();
            QString respuesta = reply->readAll();
            if(respuesta.compare("si")==0){
                QMessageBox::warning(this,"Informacion","Donacion realizada con exito. :)");
                this->ui->lineEdit_4->setText("");
                this->ui->lineEdit_5->setText("");
            }else{
                QMessageBox::warning(this,"Informacion","No se pudo realizar la donacion. :(");
            }
            delete reply;
        } else {
            //failure
            //qDebug() << "Failure" <<reply->errorString();
            QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion. :(");
            delete reply;
        }
    }else{
        QMessageBox::warning(this,"Informacion","Faltan datos >:v");
    }

}
/*------------------------------------------------------------------------------------------------------*/

void MainWindow::actualizarAlbergue(){
    if(diccionario!=0){
        int contador = 0;
        model3->clear();
        model3->setHorizontalHeaderItem(0,new QStandardItem("Identificación"));
        model3->setHorizontalHeaderItem(1,new QStandardItem("Nombre"));
        model3->setHorizontalHeaderItem(2,new QStandardItem("Apellido"));
        model3->setHorizontalHeaderItem(3,new QStandardItem("Edad"));
        model3->setHorizontalHeaderItem(4,new QStandardItem("Género"));
        model3->setHorizontalHeaderItem(5,new QStandardItem("Encargado"));
        for(int a=0;a<diccionario->size;a++){
            if(diccionario->tabla[a]!=0){
                QList<nodoA *> pila ;
                pila.push_back(diccionario->tabla[a]->arbol->raiz);
                while(!pila.isEmpty()){
                    nodoA* temp = pila[0];
                    QStandardItem *valBA = new QStandardItem(temp->id.c_str());
                    QStandardItem *valBB = new QStandardItem(temp->nombre.c_str());
                    QStandardItem *valBD = new QStandardItem(temp->apellido.c_str());
                    QStandardItem *valBE = new QStandardItem(temp->genero.c_str());
                    QStandardItem *valBF = new QStandardItem(temp->idEncargado.c_str());
                    stringstream convert;
                    convert<<temp->edad;
                    QStandardItem *valBC = new QStandardItem(convert.str().c_str());

                    model3->setItem(contador,0,valBA);
                    model3->setItem(contador,1,valBB);
                    model3->setItem(contador,2,valBD);
                    model3->setItem(contador,3,valBC);
                    model3->setItem(contador,4,valBE);
                    model3->setItem(contador,5,valBF);
                    contador++;
                    pila.pop_front();
                    if(temp->der!=0)
                        pila.push_back(temp->der);
                    if(temp->izq!=0)
                        pila.push_back(temp->izq);
                }
            }
        }
    }
}

void MainWindow::limpiarAlbergue(){
    selId = "";
    selEncargado="";    
    this->ui->lineEdit_19->clear();
    this->ui->lineEdit_20->clear();
    this->ui->lineEdit_21->clear();
    this->ui->lineEdit_23->clear();
    this->ui->lineEdit_17->clear();
    this->ui->lineEdit_18->clear();
}

void MainWindow::on_btnLoginAlbergue_clicked()
{
    if(this->ui->lineEdit_17->text().startsWith('A',Qt::CaseSensitive)){
        string envio = this->ui->lineEdit_17->text().toStdString() +"@@" +this->ui->lineEdit_18->text().toStdString();
        QString bueno = envio.c_str();

        QEventLoop eventLoop;
        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/getLocalidad") ) );
        req.setRawHeader("Content-Type", "text/plain");
        QNetworkReply *reply = mgr.post(req, bueno.toUtf8());
        eventLoop.exec(); // blocks stack until "finished()" has been called

        if (reply->error() == QNetworkReply::NoError) {
            //success
            albergueName = this->ui->lineEdit_17->text().toStdString().c_str();
            QString respuesta = reply->readAll();
            //qDebug() << "Success" <<respuesta;
            delete reply;
            if(respuesta.compare("no")==0){
                QMessageBox::warning(this,"Informacion","Acceso denegado.\nLa localidad se encuentra ocupada o los datos son incorrectos. :(");
            }else{

                QJsonDocument documento = QJsonDocument::fromJson(respuesta.toStdString().c_str());
                QJsonArray ab = documento.array();
                for(int i=0; i<ab.count();i++){
                    QJsonObject sublugar=ab.at(i).toObject();

                    string encargado = sublugar.value("Encargado").toString().toStdString();
                    QJsonValue value2 = sublugar.value("Integrantes");
                    if(value2.isArray()){
                        QJsonArray ja2=value2.toArray();
                        for(int i=0; i<ja2.count();i++){
                            QJsonObject subtree2=ja2.at(i).toObject();
                            string id = subtree2.value("Id").toString().toStdString();
                            string nombre = subtree2.value("Nombre").toString().toStdString();
                            string apellido = subtree2.value("Apellido").toString().toStdString();
                            int edad = subtree2.value("Edad").toInt();
                            string genero = subtree2.value("Genero").toString().toStdString();
                            string encargado = subtree2.value("Encargdo").toString().toStdString();
                            diccionario->agregar(id,nombre,apellido,edad,genero,encargado);
                        }
                    }
                }
                albergue = true;
                limpiarAlbergue();
                actualizarAlbergue();
                verificarLogin();
            }
        } else {
            //failure
            //qDebug() << "Failure" <<reply->errorString();
            QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion.");
            delete reply;
        }
    }else{
        QMessageBox::warning(this,"Informacion","Solo Albergues >:v");
    }
}

void MainWindow::on_btnSaveAlbergue_clicked()
{
    string json = "[\n";
    if(diccionario!=0){
        if(diccionario->elementos!=0){
            for(int a=0;a<diccionario->size;a++){
                if(diccionario->tabla[a]!=0){
                    json += "\t{\n\t\"Encargado\":\""+diccionario->tabla[a]->encargado+"\",\n\t\"Integrantes\":[\n";

                    QList<nodoA *> pila ;
                    pila.push_back(diccionario->tabla[a]->arbol->raiz);
                    while(!pila.isEmpty()){
                        nodoA* temp = pila[0];

                        stringstream convert;
                        convert<<temp->edad;

                        json += "\t\t{\n\t\t\"Id\":\""+temp->id+"\",\n\t\t\"Nombre\":\""+temp->nombre+"\",\n\t\t\"Apellido\":\""+
                                temp->apellido+"\",\n\t\t\"Edad\":"+convert.str().c_str()+",\n\t\t\"Genero\":\""+
                                temp->genero+"\",\n\t\t\"Encargado\":\""+temp->idEncargado+"\"\n\t\t},\n";

                        pila.pop_front();
                        if(temp->izq!=0){
                            pila.push_back(temp->izq);
                        }
                        if(temp->der!=0){
                            pila.push_back(temp->der);
                        }
                    }
                    json.erase(json.length()-2,json.length());
                    json += "\n\t]\n\t},\n";
                }
            }
            json.erase(json.length()-2,json.length());
        }
    }
    json += "\n]";


    if(albergueName!=""){

        QString envio = albergueName + "@@" +json.c_str();

        QEventLoop eventLoop;
        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/saveLocalidad") ) );
        req.setRawHeader("Content-Type", "text/plain");
        QNetworkReply *reply = mgr.post(req, envio.toUtf8());
        eventLoop.exec(); // blocks stack until "finished()" has been called

        if (reply->error() == QNetworkReply::NoError) {
            //success
            //qDebug() << "Success" <<reply->readAll();
            QString respuesta = reply->readAll();
            if(respuesta.compare("si")==0){
                QMessageBox::warning(this,"Informacion","Informacion guardada. :)");
            }else{
                QMessageBox::warning(this,"Informacion","No se pudo guardar la informacion :(");
            }
            delete reply;
        } else {
            //failure
            //qDebug() << "Failure" <<reply->errorString();
            QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion. :(");
            delete reply;
        }
    }else{
        QMessageBox::warning(this,"Informacion","No se pudo obtener el codigo del albergue. :(");
    }
    //cout<<json;
}

void MainWindow::on_btnCerrarAlbergue_clicked()
{
    QEventLoop eventLoop;
    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/freeLocalidad") ) );
    req.setRawHeader("Content-Type", "text/plain");
    QNetworkReply *reply = mgr.post(req, albergueName.toUtf8());
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        //qDebug() << "Success" <<reply->readAll();
        QString respuesta = reply->readAll();
        if(respuesta.compare("si")==0){
            albergueName="";
            albergue = false;
            verificarLogin();
            diccionario = new Diccionario();
            this->ui->lineEdit_22->setText("");
            limpiarAlbergue();
            actualizarAlbergue();
        }else{
            QMessageBox::warning(this,"Informacion","No se pudo cerrar la sesion. :(");
        }
        delete reply;
    } else {
        //failure
        //qDebug() << "Failure" <<reply->errorString();
        QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion. :(");
        delete reply;
    }
}

void MainWindow::on_btnRegistroIndividual_clicked()
{
    string id = this->ui->lineEdit_19->text().toStdString();
    string nombre = this->ui->lineEdit_20->text().toStdString();
    string apellido = this->ui->lineEdit_23->text().toStdString();
    int edad = 0;
    if(this->ui->lineEdit_21->text().toStdString()!=""){
        edad = atoi(this->ui->lineEdit_21->text().toStdString().c_str());
    }
    string genero = this->ui->comboBox_2->currentText().toStdString();
    if(id!="" && nombre!="" && edad!=0 && genero!="" && apellido!=""){
        bool resultado = diccionario->agregar(id,nombre,apellido,edad,genero,id);
        if(resultado){
            this->ui->lineEdit_22->clear();
            limpiarAlbergue();
            actualizarAlbergue();
            QMessageBox::warning(this,"Informacion","Datos agregados");
        }else
            QMessageBox::warning(this,"Informacion","No se pudo agregar :(");
    }else
        QMessageBox::warning(this,"Informacion","Faltan datos >:v");
}

void MainWindow::on_btnRegistroFamiliar_clicked()
{
    string encargado = this->ui->lineEdit_22->text().toStdString();

    string id = this->ui->lineEdit_19->text().toStdString();
    string nombre = this->ui->lineEdit_20->text().toStdString();
    string apellido = this->ui->lineEdit_23->text().toStdString();
    int edad = 0;
    if(this->ui->lineEdit_21->text().toStdString()!=""){
        edad = atoi(this->ui->lineEdit_21->text().toStdString().c_str());
    }
    string genero = this->ui->comboBox_2->currentText().toStdString();
    if(id!="" && nombre!="" && edad!=0 && genero!="" && apellido!=""){
        if(encargado!=""){//ya hay un encargado designado
            bool resultado = diccionario->agregar(id,nombre,apellido,edad,genero,encargado);
            if(resultado){
                limpiarAlbergue();
                actualizarAlbergue();
                QMessageBox::warning(this,"Informacion","Familiar Agregado");
            }else
                QMessageBox::warning(this,"Informacion","No se pudo agregar :(");
        }else{//este primer elemnto sera el encargado
            bool resultado = diccionario->agregar(id,nombre,apellido,edad,genero,id);
            if(resultado){
                this->ui->lineEdit_22->setText(id.c_str());
                limpiarAlbergue();
                actualizarAlbergue();
                QMessageBox::warning(this,"Informacion","Datos agregados");
            }else
                QMessageBox::warning(this,"Informacion","No se pudo agregar :(");
        }
    }else
        QMessageBox::warning(this,"Informacion","Faltan datos >:v");
}

void MainWindow::on_btnModificarRegistro_clicked()
{

}

void MainWindow::on_btnEliminarRegistro_clicked()
{
    if(selId!="" && selEncargado!=""){
        if(selId.compare(selEncargado)==0){
            QMessageBox eliminarRecurso;
            string texto = "La persona seleccionada funciona como indice \nde busqueda para datos relacionados a el o miembros de su \nfamilia, al eliminarlo se elimina todos los datos relacionados.";
            eliminarRecurso.setText(texto.c_str());
            eliminarRecurso.setInformativeText("Desea seguir?");
            eliminarRecurso.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            eliminarRecurso.setDefaultButton(QMessageBox::Cancel);
            int decision = 0;
            decision = eliminarRecurso.exec();
            if(decision==QMessageBox::Ok){
                bool resultado = diccionario->eliminar(selId,selEncargado);
                if(resultado){
                    this->ui->lineEdit_22->setText("");
                    limpiarAlbergue();
                    actualizarAlbergue();
                    QMessageBox::warning(this,"Informacion","Se elimino el registro :)");
                }else
                    QMessageBox::warning(this,"Informacion","No se pudo eliminar >:v");
            }
        }else{
            bool resultado = diccionario->eliminar(selId,selEncargado);
            if(resultado){
                this->ui->lineEdit_22->setText("");
                limpiarAlbergue();
                actualizarAlbergue();
                QMessageBox::warning(this,"Informacion","Se elimino el registro :)");
            }else
                QMessageBox::warning(this,"Informacion","No se pudo eliminar >:v");
        }
    }else
        QMessageBox::warning(this,"Informacion","Seleccione un elemento >:v");
}

void MainWindow::on_btnReporteAlbergue_clicked()
{
    if(diccionario!=0){
        diccionario->graficar();
        system("diccionario.png");
    }else
        QMessageBox::warning(this,"Informacion","No hay diccionario :(");
}

void MainWindow::on_tableView_3_clicked(const QModelIndex &index)
{
    int b = this->ui->tableView_3->currentIndex().row();
    string id = model3->item(b,0)->text().toStdString();
    string nombre = model3->item(b,1)->text().toStdString();
    string apellido = model3->item(b,2)->text().toStdString();
    string edad = model3->item(b,3)->text().toStdString();
    string encargado = model3->item(b,5)->text().toStdString();

    if(id!="" && nombre!="" && apellido!="" && edad!="" && encargado!=""){

        this->ui->lineEdit_19->setText(id.c_str());
        this->ui->lineEdit_20->setText(nombre.c_str());
        this->ui->lineEdit_23->setText(apellido.c_str());
        this->ui->lineEdit_21->setText(edad.c_str());
        this->ui->lineEdit_22->setText(encargado.c_str());

        selId = id;
        selEncargado = encargado;
    }
}

/*-----------------------------------------------------------------------------------------------------------*/
void MainWindow::actualizarAcopio(){
    if(tabla!=0){
        int contador = 0;
        this->ui->comboBox->clear();
        model2->clear();
        model2->setHorizontalHeaderItem(0,new QStandardItem("Categoría"));
        model2->setHorizontalHeaderItem(1,new QStandardItem("Nombre"));
        model2->setHorizontalHeaderItem(2,new QStandardItem("Cantidad"));
        for(int a=0;a<tabla->size;a++){
            if(tabla->tablaHash[a]!=0){
                for(int b=0;b<tabla->tablaHash[a]->noElementos;b++){
                    this->ui->comboBox->addItem(tabla->tablaHash[a]->elementos[b]->categoria.c_str());
                    for(int c=0;c<tabla->tablaHash[a]->elementos[b]->noElementos;c++){
                        QStandardItem *valBA = new QStandardItem(tabla->tablaHash[a]->elementos[b]->categoria.c_str());
                        QStandardItem *valBB = new QStandardItem(tabla->tablaHash[a]->elementos[b]->productos[c]->nombre.c_str());
                        stringstream convert;
                        convert<<tabla->tablaHash[a]->elementos[b]->productos[c]->cantidad;
                        QStandardItem *valBC = new QStandardItem(convert.str().c_str());

                        model2->setItem(contador,0,valBA);
                        model2->setItem(contador,1,valBB);
                        model2->setItem(contador,2,valBC);
                        contador++;
                    }
                }
            }
        }
    }
}

void MainWindow::limpiarAcopio(){
    selCantidad = 0;
    selCategoria = "";
    selNombre = "";    
    this->ui->lineEdit_10->clear();
    this->ui->lineEdit_11->clear();
    this->ui->lineEdit_12->clear();
    this->ui->lineEdit_13->clear();
    this->ui->lineEdit_14->clear();
    this->ui->lineEdit_15->clear();
    this->ui->lineEdit_16->clear();
}

void MainWindow::on_btnLoginAcopio_clicked()
{
    if(this->ui->lineEdit_10->text().startsWith('C',Qt::CaseSensitive)){
        string envio = this->ui->lineEdit_10->text().toStdString() +"@@" +this->ui->lineEdit_11->text().toStdString();
        QString bueno = envio.c_str();

        QEventLoop eventLoop;
        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/getLocalidad") ) );
        req.setRawHeader("Content-Type", "text/plain");
        QNetworkReply *reply = mgr.post(req, bueno.toUtf8());
        eventLoop.exec(); // blocks stack until "finished()" has been called

        if (reply->error() == QNetworkReply::NoError) {
            //success
            acopioName = this->ui->lineEdit_10->text().toStdString().c_str();
            QString respuesta = reply->readAll();
            //qDebug() << "Success" <<respuesta;
            delete reply;
            if(respuesta.compare("no")==0){
                QMessageBox::warning(this,"Informacion","Acceso denegado.\nLa localidad se encuentra ocupada o los datos son incorrectos. :(");
            }else{
                //string prueba = "[{\"Categoria\":\"aldo\",\"Recursos\":[{\"Nombre\":\"a\",\"Cantidad\":20},{\"Nombre\":\"b\",\"Cantidad\":20},{\"Nombre\":\"c\",\"Cantidad\":20}]},{\"Categoria\":\"otros\",\"Recursos\":[{\"Nombre\":\"a\",\"Cantidad\":60}]}]";
                //string prueba = "[ ]";
                QJsonDocument documento = QJsonDocument::fromJson(respuesta.toStdString().c_str());
                QJsonArray ab = documento.array();
                for(int i=0; i<ab.count();i++){
                    QJsonObject sublugar=ab.at(i).toObject();
                    string categoria = sublugar.value("Categoria").toString().toStdString();
                    QJsonValue value2 = sublugar.value("Recursos");
                    if(value2.isArray()){
                        QJsonArray ja2=value2.toArray();
                        for(int i=0; i<ja2.count();i++){
                            QJsonObject subtree2=ja2.at(i).toObject();
                            string nombre = subtree2.value("Nombre").toString().toStdString();
                            int cantidad = subtree2.value("Cantidad").toInt();
                            tabla->agregar(categoria,nombre,cantidad);
                        }
                    }
                }
                acopio = true;
                limpiarAcopio();
                actualizarAcopio();
                verificarLogin();
            }
        } else {
            //failure
            //qDebug() << "Failure" <<reply->errorString();
            QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion.");
            delete reply;
        }
    }else{
        QMessageBox::warning(this,"Informacion","Solo centros de Acopio >:v");
    }
}

void MainWindow::on_btnSaveAcopio_clicked()
{
    string json = "[\n";
    if(tabla!=0){
        for(int a=0;a<tabla->size;a++){
            if(tabla->tablaHash[a]!=0){//si hay elementos en esa llave
                for(int b=0;b<tabla->tablaHash[a]->noElementos;b++){
                    json += "\t{\n\t\"Categoria\":\""+tabla->tablaHash[a]->elementos[b]->categoria+"\",\n\t\"Recursos\":[\n";

                    for(int c=0;c<tabla->tablaHash[a]->elementos[b]->noElementos;c++){
                        stringstream convert;
                        convert<<tabla->tablaHash[a]->elementos[b]->productos[c]->cantidad;
                        json+="\t\t{\n\t\t\"Nombre\":\""+tabla->tablaHash[a]->elementos[b]->productos[c]->nombre+"\",\n\t\t"+
                                "\"Cantidad\":"+convert.str().c_str()+"\n";
                        if(c==tabla->tablaHash[a]->elementos[b]->noElementos-1)
                            json+= "\t\t}\n";
                        else
                            json+= "\t\t},\n";
                    }
                    json+= "\t]\n\t},\n";
                }
            }
        }
    }
    if(tabla!=0)
        json.erase(json.length()-2,json.length());
    json += "\n]";

    if(acopioName!=""){

        QString envio = acopioName + "@@" +json.c_str();

        QEventLoop eventLoop;
        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/saveLocalidad") ) );
        req.setRawHeader("Content-Type", "text/plain");
        QNetworkReply *reply = mgr.post(req, envio.toUtf8());
        eventLoop.exec(); // blocks stack until "finished()" has been called

        if (reply->error() == QNetworkReply::NoError) {
            //success
            //qDebug() << "Success" <<reply->readAll();
            QString respuesta = reply->readAll();
            if(respuesta.compare("si")==0){
                QMessageBox::warning(this,"Informacion","Informacion guardada. :)");
            }else{
                QMessageBox::warning(this,"Informacion","No se pudo guardar la informacion :(");
            }
            delete reply;
        } else {
            //failure
            //qDebug() << "Failure" <<reply->errorString();
            QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion. :(");
            delete reply;
        }
    }else{
        QMessageBox::warning(this,"Informacion","No se pudo obtener el codigo del acopio. :(");
    }
    //cout<<json;
}

void MainWindow::on_btnCerrarAcopio_clicked()
{    
    QEventLoop eventLoop;
    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/freeLocalidad") ) );
    req.setRawHeader("Content-Type", "text/plain");
    QNetworkReply *reply = mgr.post(req, acopioName.toUtf8());
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        //qDebug() << "Success" <<reply->readAll();
        QString respuesta = reply->readAll();
        if(respuesta.compare("si")==0){
            acopioName="";
            acopio = false;
            verificarLogin();
            tabla = new Tabla();
            limpiarAcopio();
            actualizarAcopio();
        }else{
            QMessageBox::warning(this,"Informacion","No se pudo cerrar la sesion. :(");
        }
        delete reply;
    } else {
        //failure
        //qDebug() << "Failure" <<reply->errorString();
        QMessageBox::warning(this,"Informacion","El servidor no pudo resolver la peticion. :(");
        delete reply;
    }
}

void MainWindow::on_btnRegistrarProducto_clicked()
{
    string categoria = this->ui->lineEdit_12->text().toStdString().c_str();
    string nombre = this->ui->lineEdit_13->text().toStdString().c_str();
    int cantidad = 0;
    if(this->ui->lineEdit_14->text().toStdString().c_str()!=""){
        cantidad = atoi(this->ui->lineEdit_14->text().toStdString().c_str());
    }
    if(categoria!="" && nombre!="" && cantidad!=0 && tabla->agregar(categoria,nombre,cantidad)){
        this->actualizarAcopio();
        this->limpiarAcopio();
        QMessageBox::warning(this,"Informacion","Dato Agregado xD");
    }else
        QMessageBox::warning(this,"Informacion","No se pudo agregar el producto :(");
}

void MainWindow::on_btnModificarProducto_clicked()
{
    if(selCantidad!=0 && selCategoria!="" && selNombre!=""){
        string nuevaCat = this->ui->lineEdit_12->text().toStdString();
        string nuevoNom = this->ui->lineEdit_13->text().toStdString();
        int nuevaCant = 0;
        if(this->ui->lineEdit_14->text().toStdString()!="")
            nuevaCant = atoi(this->ui->lineEdit_14->text().toStdString().c_str());
        if(nuevaCat!=""&&nuevoNom!=""&&nuevaCant!=0){
            tabla->modificarRecurso(selCategoria,selNombre,nuevaCat,nuevoNom,nuevaCant);
            this->actualizarAcopio();
            this->limpiarAcopio();
            QMessageBox::warning(this,"Informacion","Datos modificados :)");
        }else{
            QMessageBox::warning(this,"Informacion","Nuevos datos invalidos >:v");
        }
    }else{
        QMessageBox::warning(this,"Informacion","Seleccione un elemento >:v");
    }
}

void MainWindow::on_btnEmtregaProducto_clicked()
{
    if(this->ui->lineEdit_15->text()!=""){
        int cantidad = atoi(this->ui->lineEdit_15->text().toStdString().c_str());
        if(selCantidad!=0 && selCategoria!="" && selNombre!=""){
            if(cantidad<=selCantidad){
                tabla->entregar(selCategoria,selNombre,cantidad);
                this->actualizarAcopio();
                this->limpiarAcopio();
                QMessageBox::warning(this,"Informacion","Recursos entregados :)");
            }else{
                QMessageBox::warning(this,"Informacion","Recursos insuficientes para la peticion >:v");
            }
        }else
            QMessageBox::warning(this,"Informacion","Seleccione un elemento >:v");
    }else
        QMessageBox::warning(this,"Informacion","Ingrese una cantidad >:v");
}

void MainWindow::on_btnReporteHash_clicked()
{
    if(tabla!=0){
        tabla->graficar();
        system("hash.png");
    }else
        QMessageBox::warning(this,"Informacion","No Hay tabla hash :(");
}

void MainWindow::on_btnEliminarCategoria_clicked()
{
    string categoria = this->ui->comboBox->currentText().toStdString();
    if(categoria!=""){

        QMessageBox eliminarRecurso;
        string texto = "Si elimina la categoria: " + categoria + ",\n los datos en ella seran eliminados.";
        eliminarRecurso.setText(texto.c_str());
        eliminarRecurso.setInformativeText("Desea seguir?");
        eliminarRecurso.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        eliminarRecurso.setDefaultButton(QMessageBox::Cancel);
        int decision = 0;

        decision = eliminarRecurso.exec();
        if(decision==QMessageBox::Ok){
            tabla->eliminarAcopio(categoria);
            limpiarAcopio();
            actualizarAcopio();
        }
    }else{
        QMessageBox::warning(this,"Informacion","No hay recursos :(");
    }
}

void MainWindow::on_btnEliminarCategoria_2_clicked()
{
    string categoria = this->ui->comboBox->currentText().toStdString();
    if(categoria!=""){
        string nuevaCategoria = this->ui->lineEdit_16->text().toStdString();
        if(nuevaCategoria!=""){
            tabla->modificarAcopio(categoria,nuevaCategoria);
            this->actualizarAcopio();
            this->limpiarAcopio();
            QMessageBox::warning(this,"Informacion","Categoria Modificada. :)");
        }else
            QMessageBox::warning(this,"Informacion","Ingrese un nuevo nombre. >:v");
    }else
        QMessageBox::warning(this,"Informacion","No hay recursos :(");
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    int b = this->ui->tableView_2->currentIndex().row();
    string categoria = model2->item(b,0)->text().toStdString();
    string nombre = model2->item(b,1)->text().toStdString();
    string cantidad = model2->item(b,2)->text().toStdString();

    if(categoria!=""&&nombre!=""&&cantidad!=""){
        this->ui->lineEdit_12->setText(categoria.c_str());
        this->ui->lineEdit_13->setText(nombre.c_str());
        this->ui->lineEdit_14->setText(cantidad.c_str());

        selCategoria = categoria;
        selNombre = nombre;
        selCantidad = atoi(cantidad.c_str());
    }

}


void MainWindow::on_pushButton_clicked()
{
    this->ui->lineEdit->setText("");
    this->ui->lineEdit_2->setText("");
    this->ui->lineEdit_3->setText("");
    this->ui->lineEdit_4->setText("");
    this->ui->lineEdit_5->setText("");
}

/*------------------------------------------------------------------------------------------------*/
void MainWindow::on_pushButton_2_clicked()
{
    grafo.clear();
    QEventLoop eventLoop;
    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("http://"+ip+"/P2Servidor/webresources/generic/getGrafo") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        QString respuesta = reply->readAll();
        qDebug() << "Success" <<respuesta;
        delete reply;

        this->ui->comboBox_6->clear();
        this->ui->comboBox_7->clear();

        QFile defaultTextFile("mapa.html");
        defaultTextFile.open(QIODevice::ReadOnly);
        QString html = defaultTextFile.readAll();
        defaultTextFile.close();
        QString markers = "";
        QJsonDocument documento = QJsonDocument::fromJson(respuesta.toStdString().c_str());
        QJsonArray ab = documento.array();
        for(int i=0; i<ab.count();i++){
            QJsonObject sublugar=ab.at(i).toObject();


            string codigo = sublugar.value("codigo").toString().toStdString();

            this->ui->comboBox_6->addItem(codigo.c_str());
            this->ui->comboBox_7->addItem(codigo.c_str());
            double lng = sublugar["lng"].toDouble();
            double lat = sublugar["lat"].toDouble();

            nodoG* nuevo = new nodoG(codigo,lat,lng);

            if(sublugar["codigo"].toString().startsWith("A")){
                markers.append(QString("var marker%1 = new google.maps.Marker({map: map,position: {lat:%2, lng:%3},title: '%4',icon: {path: google.maps.SymbolPath.CIRCLE,scale: 10,strokeColor: '#f00',strokeWeight: 5,fillColor: '#00f',fillOpacity:1}});\n")
                        .arg(
                            QString::number(i),
                            QString::number(sublugar["lat"].toDouble()),
                            QString::number(sublugar["lng"].toDouble()),
                            sublugar["codigo"].toString()
                        ));
            }else{
                markers.append(QString("var marker%1 = new google.maps.Marker({map: map,position: {lat:%2, lng:%3},title: '%4',icon: {path: google.maps.SymbolPath.CIRCLE,scale: 10,strokeColor: '#AAf',strokeWeight: 5,fillColor: '#fAA',fillOpacity:1}});\n")
                        .arg(
                            QString::number(i),
                            QString::number(sublugar["lat"].toDouble()),
                            QString::number(sublugar["lng"].toDouble()),
                            sublugar["codigo"].toString()
                        ));
            }



            QJsonValue value2 = sublugar.value("aristas");
            if(value2.isArray()){
                QJsonArray ja2=value2.toArray();
                for(int i=0; i<ja2.count();i++){
                    QJsonObject subtree2=ja2.at(i).toObject();
                        double mult = subtree2["multiplicador"].toDouble();
                        nuevo->aristas.push_back(new nodoAG(subtree2["destino"].toString().toStdString(),mult));
                }
            }
            grafo.push_back(nuevo);
        }

        QString conexion = "";
        int auxCont = 0;
        for(int a=0;a<grafo.size();a++){//recorrer grafo

            for(int b=0;b<grafo.at(a)->aristas.size();b++){//recorrer aristas de nodo
                for(int c=0;c<grafo.size();c++){//buscar el nodo que coincida con la arista
                    if(grafo.at(c)->codigo.compare(grafo.at(a)->aristas.at(b)->codigo)==0){
                        conexion.append(QString("var flightPlanCoordinates%1 = [{lat:%2,lng:%3},{lat:%4,lng:%5}];\nvar flightPath%6 = new google.maps.Polyline({path: flightPlanCoordinates%7,geodesic: true,strokeColor: '#15CF2A',strokeOpacity: 1.0,strokeWeight: 2});\nflightPath%8.setMap(map);")
                                .arg(
                                    QString::number(auxCont),
                                    QString::number(grafo.at(a)->latitud),
                                    QString::number(grafo.at(a)->longitud),
                                    QString::number(grafo.at(c)->latitud),
                                    QString::number(grafo.at(c)->longitud),
                                    QString::number(auxCont),
                                    QString::number(auxCont),
                                    QString::number(auxCont)
                                ));
                        auxCont++;
                        break;
                    }
                }
            }
        }

        /*
        string ruta = "var flightPlanCoordinates = [\n";
        for(int a=0;a<grafo.size();a++){
            int size11 = grafo.at(a)->aristas.size();
            stringstream convert1;
            convert1<<grafo.at(a)->latitud;
            stringstream convert2;
            convert2<<grafo.at(a)->longitud;

            for(int b=0;b<size11;b++){
                for(int c=0;c<grafo.size();c++){
                    if(grafo.at(a)->aristas.at(b)->codigo.compare(grafo.at(c)->codigo)==0){
                        stringstream convert3;
                        convert3<<grafo.at(c)->latitud;
                        stringstream convert4;
                        convert4<<grafo.at(c)->longitud;


                        ruta.append("\n{lat: ");
                        ruta.append(convert1.str().c_str());
                        ruta.append(",lng: ");
                        ruta.append(convert2.str().c_str());
                        ruta.append("},");
                        ruta.append("\n{lat: ");
                        ruta.append(convert3.str().c_str());
                        ruta.append(",lng: ");
                        ruta.append(convert4.str().c_str());
                        ruta.append("},");
                    }
                }
            }
        }
        ruta.erase(ruta.size()-1,1);
        ruta += "\n];\n  var flightPath = new google.maps.Polyline({path: flightPlanCoordinates,geodesic: true,strokeColor: '#15CF2A',strokeOpacity: 1.0,strokeWeight: 2});\n flightPath.setMap(map);";
        */


        //cout<<markers.toStdString().c_str()<<endl;
        //cout<<ruta<<endl;
        html.replace("###",markers);
        html.replace("@@@",conexion);

        mapaHtml = html;

        cout<<html.toStdString().c_str()<<endl;
        this->ui->webView->setContent(html.toUtf8());
    }else
        QMessageBox::warning(this,"Informacion","Error en peticion a servidor.");
}


void MainWindow::on_pushButton_3_clicked()
{
    string inicio = this->ui->comboBox_6->currentText().toStdString();
    string fin = this->ui->comboBox_7->currentText().toStdString();

    if(inicio.compare(fin)==0){
        QMessageBox::warning(this,"Informacion","Elija destinos distintos. >:v");
    }else{
        cout<<inicio<<"  "<<fin<<endl;
        //copia del grafo original
        QList<nodoG *> mejRuta;
        for(int a=0;a<grafo.size();a++){
            nodoG* copia = new nodoG(grafo.at(a)->codigo,grafo.at(a)->latitud,grafo.at(a)->longitud);
            for(int b=0;b<grafo.at(a)->aristas.size();b++){
                copia->aristas.push_back(new nodoAG(grafo.at(a)->aristas.at(b)->codigo,grafo.at(a)->aristas.at(b)->peso));
            }
            mejRuta.push_back(copia);
        }
        //Definir peso y mostrarlo
        model4->clear();
        model4->setHorizontalHeaderItem(0,new QStandardItem("ptoA"));
        model4->setHorizontalHeaderItem(1,new QStandardItem("Peso"));
        model4->setHorizontalHeaderItem(2,new QStandardItem("ptoB"));
        int contador =0;
        for(int a=0;a<mejRuta.size();a++){
            if(!mejRuta.at(a)->aristas.isEmpty()){
                for(int b=0;b<mejRuta.at(a)->aristas.size();b++){
                    for(int c=0;c<mejRuta.size();c++){
                        if(mejRuta.at(c)->codigo.compare(mejRuta.at(a)->aristas.at(b)->codigo)==0){
                            mejRuta.at(a)->aristas.at(b)->peso = this->obtenerPeso(mejRuta.at(a)->latitud,mejRuta.at(a)->longitud,mejRuta.at(c)->latitud,mejRuta.at(c)->longitud,mejRuta.at(a)->aristas.at(b)->peso);

                            QStandardItem *valBE = new QStandardItem(mejRuta.at(a)->codigo.c_str());
                            QStandardItem *valBF = new QStandardItem(mejRuta.at(c)->codigo.c_str());
                            stringstream convert;
                            convert<<mejRuta.at(a)->aristas.at(b)->peso;
                            QStandardItem *valBC = new QStandardItem(convert.str().c_str());

                            model4->setItem(contador,0,valBE);
                            model4->setItem(contador,1,valBC);
                            model4->setItem(contador,2,valBF);
                            contador++;
                            break;
                        }
                    }
                }
            }
        }
        // agregar aristas, doble via
        for(int a=0;a<mejRuta.size();a++){
            if(!mejRuta.at(a)->aristas.isEmpty()){
                for(int b=0;b<mejRuta.at(a)->aristas.size();b++){
                    for(int c=0;c<mejRuta.size();c++){
                        if(mejRuta.at(c)->codigo.compare(mejRuta.at(a)->aristas.at(b)->codigo)==0){

                            bool ya = false;
                            for(int n=0;n<mejRuta.at(c)->aristas.size();n++){
                                if(mejRuta.at(c)->aristas.at(n)->codigo.compare(mejRuta.at(a)->codigo)==0){
                                    ya = true;
                                    break;
                                }
                            }
                            if(ya==false)
                                mejRuta.at(c)->aristas.push_back(new nodoAG(mejRuta.at(a)->codigo,mejRuta.at(a)->aristas.at(b)->peso));
                            break;
                        }
                    }
                }
            }
        }
        //ordenar aristas
        for(int a=0;a<mejRuta.size();a++){
            if(!mejRuta.at(a)->aristas.isEmpty()){
                nodoAG* aux = 0;
                for(int i=0;i<mejRuta.at(a)->aristas.size()-1;i++){
                    for(int j=i+1;j<mejRuta.at(a)->aristas.size();j++){
                        if(mejRuta.at(a)->aristas.at(i)->peso>mejRuta.at(a)->aristas.at(j)->peso){
                            aux = mejRuta.at(a)->aristas.at(i);
                            mejRuta.at(a)->aristas.replace(i,mejRuta.at(a)->aristas.at(j));
                            mejRuta.at(a)->aristas.replace(j,aux);
                        }
                    }
                }

            }
        }
        //verificacion xD
        /*cout<<mejRuta.at(2)->codigo<<endl;
        for(int a=0;a<mejRuta.at(2)->aristas.size();a++){
            cout<<mejRuta.at(2)->aristas.at(a)->codigo<<endl;
        }

        //cout<<mejRuta.at(2)->codigo<<endl;
        for(int a=0;a<mejRuta.size();a++){
            cout<<mejRuta.at(a)->codigo<<endl;
        }

        */

        nodoG* esPrimero = 0;

        QList<nodoG *> pila;
        //buscar inicio y meter pila
        for(int a=0;a<mejRuta.size();a++){
            if(mejRuta.at(a)->codigo.compare(inicio)==0){
                pila.push_back(mejRuta.at(a));
                esPrimero = mejRuta.at(a);
                break;
            }
        }
        //empezar recorrido ingreso y salida de pila
        while(!pila.isEmpty()){
            //eliminacion doble via
            for(int a=0;a<pila.at(0)->aristas.size();a++){
                for(int b=0;b<mejRuta.size();b++){//recorrido nodos
                    if(mejRuta.at(b)->codigo.compare(pila.at(0)->aristas.at(a)->codigo)==0){//si nodo igual arista
                        //remover doble via
                        for(int c=0;c<mejRuta.at(b)->aristas.size();c++){
                            if(mejRuta.at(b)->aristas.at(c)->codigo.compare(pila.at(0)->codigo)==0){//arista igual a nodo
                                mejRuta.at(b)->aristas.removeAt(c);
                                break;
                            }
                        }
                        double valor = 0;
                        //agregar ruta corta
                        if(pila.at(0)->masCorto!=0){
                            valor = pila.at(0)->masCorto->peso + pila.at(0)->aristas.at(a)->peso;
                        }else{
                            valor = pila.at(0)->aristas.at(a)->peso;
                        }
                        //ver si destino tiene ya ruta corta
                        if(mejRuta.at(b)->masCorto!=0){//ya tiene
                            if(mejRuta.at(b)->masCorto->peso>valor){//si valor es mas corto
                                mejRuta.at(b)->masCorto->peso = valor;
                                mejRuta.at(b)->masCorto->codigo = pila.at(0)->codigo;
                            }
                        }else{//no tiene
                            mejRuta.at(b)->masCorto = new nodoAG(pila.at(0)->codigo,pila.at(0)->aristas.at(a)->peso);
                        }
                        //agregar a pila, verificar antes, si existe
                        bool existe = false;
                        for(int s=0;s<pila.size();s++){
                            if(pila.at(s)->codigo.compare(mejRuta.at(b)->codigo)==0){//ya existe
                                if(pila.at(s)->pesoPila>pila.at(0)->aristas.at(a)->peso){
                                    pila.at(s)->pesoPila = pila.at(0)->aristas.at(a)->peso;
                                    existe = true;
                                }
                                break;
                            }
                        }
                        if(existe==false){
                            mejRuta.at(b)->pesoPila = pila.at(0)->aristas.at(a)->peso;
                        }
                        pila.push_back(mejRuta.at(b));
                        break;
                    }
                }
            }
            //saco el primer elemento
            pila.pop_front();
            //ordenar pila
            nodoG* tempAux = 0;
            for(int a=0;a<pila.size()-1;a++){
                for(int b=a+1;b<pila.size();b++){
                    if(pila.at(a)->pesoPila>pila.at(b)->pesoPila){
                        tempAux = pila.at(a);
                        pila.replace(a,pila.at(b));
                        pila.replace(b,tempAux);
                    }
                }
            }

        }
        int ultimo = 0;
        //obtener ruta mas corta
        //buqueda de ultimo
        for(int a=0;a<mejRuta.size();a++){
            if(mejRuta.at(a)->codigo.compare(fin)==0){
                ultimo = a;
                break;
            }
        }
        //obtencion de ruta
        nodoG* recorrido = mejRuta.at(ultimo);
        string ruta = "var flightPlanCoordinatesAA = [\n";

        while(recorrido->masCorto!=0){
            //cout<<recorrido->codigo<<endl;
            stringstream convert3;
            convert3<<recorrido->latitud;
            stringstream convert4;
            convert4<<recorrido->longitud;
            ruta.append("\n{lat: ");
            ruta.append(convert3.str().c_str());
            ruta.append(",lng: ");
            ruta.append(convert4.str().c_str());
            ruta.append("},");

            for(int a=0;a<mejRuta.size();a++){
                if(mejRuta.at(a)->codigo.compare(recorrido->masCorto->codigo)==0){
                    recorrido = mejRuta.at(a);
                    break;
                }
            }
        }
        //cout<<esPrimero->codigo<<endl;
        stringstream convert3;
        convert3<<esPrimero->latitud;
        stringstream convert4;
        convert4<<esPrimero->longitud;
        ruta.append("\n{lat: ");
        ruta.append(convert3.str().c_str());
        ruta.append(",lng: ");
        ruta.append(convert4.str().c_str());
        ruta.append("}");

        ruta += "\n];\n  var flightPathAA = new google.maps.Polyline({path: flightPlanCoordinatesAA,geodesic: true,strokeColor: '#1D30DC',strokeOpacity: 1.0,strokeWeight: 2});\n flightPathAA.setMap(map);";

        //cout<<ruta<<endl;
        QString html = mapaHtml;
        html.replace("<!--&&&-->",ruta.c_str());
        this->ui->webView->setContent(html.toUtf8());
    }
}

double MainWindow::obtenerPeso(double latOri, double lngOri, double latDes, double lngDes, double mult){
    double respuesta = 0;

    double xC = pow((latDes - latOri),2);
    double yC = pow((lngDes - lngOri),2);
    respuesta = sqrt(xC+yC)*mult;

    return respuesta;
}
