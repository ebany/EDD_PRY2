#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString ip = "192.168.184.56:8080";
    void actualizarAcopio();
    void verificarLogin();
    void limpiarAcopio();
    void actualizarAlbergue();
    void limpiarAlbergue();
    double obtenerPeso(double latOri,double lngOri,double latDes,double lngDes,double mult);

private slots:
    void on_btnConsultaArbol_clicked();

    void on_btnDonar_clicked();

    void on_btnLoginAlbergue_clicked();

    void on_btnSaveAlbergue_clicked();

    void on_btnCerrarAlbergue_clicked();

    void on_btnRegistroIndividual_clicked();

    void on_btnRegistroFamiliar_clicked();

    void on_btnModificarRegistro_clicked();

    void on_btnEliminarRegistro_clicked();

    void on_btnReporteAlbergue_clicked();

    void on_btnLoginAcopio_clicked();

    void on_btnSaveAcopio_clicked();

    void on_btnCerrarAcopio_clicked();

    void on_btnRegistrarProducto_clicked();

    void on_btnModificarProducto_clicked();

    void on_btnEmtregaProducto_clicked();

    void on_btnReporteHash_clicked();

    void on_btnEliminarCategoria_clicked();

    void on_btnEliminarCategoria_2_clicked();

    void on_tableView_2_clicked(const QModelIndex &index);

    void on_tableView_3_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
