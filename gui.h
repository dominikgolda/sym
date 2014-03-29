#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include<QLineEdit>
#include "qkontrolersymulacji.h"
#include<vector>
#include<string>
class QString;

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Gui(QKontrolerSymulacji *kontroler, QWidget *parent = 0);
    ~Gui();
signals:
    void symulacjaCiagla();
    void symulacjaKrokowa();
    void symulacjaStop();
    void setWymuszenie(std::vector<double> u);
    void setDt(double dt);
    void setPredkoscSymulacji(int prSym);
    void openFile(QString str);
    void saveFile(QString str);
    void resetSymulacji();
    void setWielomianMianownika(std::vector<double>,int ktory, std::string id);
    void setWielomianLicznika(std::vector<double>,int ktory, std::string id);
public slots:
    void odbierzWyniki(QVector<double> y, QVector<double> t, QVector<double> u);
    void symulacjaZakonczona();


private:
    Ui::Gui *m_ui;
    //timer do odświeżania wykresu
    QTimer m_refreshTimer;
    bool m_czyOdswiezac = false;
    void ustawWykres();
    void ustawKontrolki();
    void setTextDlaKontrolekDouble(QLineEdit* edit, QString tekst);
    std::vector<double> convretQStringToDoubleVector(QString str);

//c//////////////////////////////////
private slots:
    void on_actionSymulacja_ciagla_triggered();

    void on_symCiagla_clicked();

    void on_actionZatrzymaj_symulacje_triggered();

    void on_symKrokowa_clicked();

    void on_actionSymulacja_krokowa_triggered();



    void on_setWZadana_returnPressed();

    void on_setDh_returnPressed();

    void on_setPrSym_valueChanged(int arg1);

    void on_resetujSymulacje_clicked();
    void on_actionZapi_sz_triggered();
    void on_actionWczytaj_plik_triggered();
    void on_setWielomianLicznka_returnPressed();
    void on_setWielomianMianownika_returnPressed();
};

#endif // GUI_H
