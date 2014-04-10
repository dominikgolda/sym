#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include<QLineEdit>
#include "qkontrolersymulacji.h"
#include<vector>
#include<string>
#include<QMap>
#include "stale.h"
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
//    void setWymuszenie(std::vector<double> u);
//    void setDt(double dt);
//    void setPredkoscSymulacji(int prSym);
    void openFile(QString str);
    void saveFile(QString str);
    void resetSymulacji();
//    void setWielomianMianownika(std::vector<double>,int ktory, std::string id);
//    void setWielomianLicznika(std::vector<double>,int ktory, std::string id);
//    void setWariancja(double wariancja);
    void getParameters();
    void setParameters(QMapaDanych m);
    void setParameters(std::vector<TypyWymuszen> w, std::vector<std::vector<double> > param);
public slots:
    void odbierzWyniki(QVector<double> y, QVector<double> t, QVector<double> u,QVector<double> wz);
    void symulacjaZakonczona();
    /*!
     * \brief ustawDaneDlaUzytkownika
     *Funkcja pozwala przedstawić dane obiektu na formatce
     */
    void ustawDaneDlaUzytkownika(QMapaDanych m);


private:
    Ui::Gui *m_ui;
    //timer do odświeżania wykresu
    QTimer m_refreshTimer;
    bool m_czyOdswiezac = false;
    void ustawWykres();
    void ustawKontrolki();
    void setTextDlaKontrolekDouble(QLineEdit* edit, QString tekst);
    unsigned int m_liczbaWidocznychProbek = 200;
    QVector<double> convretQStringToDoubleVector(QString str);
    QString stworzQStringWielomianu(QVector<double> &wielomian, int delay=0);

//c//////////////////////////////////
private slots:
    void przeskalujOsie();
    void on_actionSymulacja_ciagla_triggered();

    void on_symCiagla_clicked();

    void on_actionZatrzymaj_symulacje_triggered();

    void on_symKrokowa_clicked();

    void on_actionSymulacja_krokowa_triggered();



    //void on_setWZadana_returnPressed();

    void on_setDh_returnPressed();

    void on_setPrSym_valueChanged(int arg1);

    void on_resetujSymulacje_clicked();
    void on_actionZapi_sz_triggered();
    void on_actionWczytaj_plik_triggered();
    void on_setWielomianLicznka_returnPressed();
    void on_setWielomianMianownika_returnPressed();
    void on_resetujWykresy_clicked();
    void on_setWariancja_returnPressed();
    void on_wyborLicznika_valueChanged(int);
    void on_wyborMianownika_valueChanged(int);
    void on_setWzmocnienie_returnPressed();

    void on_aktualizujSinusaButton_clicked();
    void on_aktualizujWartoscSinu_clicked();
    void on_aktualizujWartoscTrojkat_clicked();
    void on_aktualizujWartoscStala_clicked();
    void on_setLiczbaWidocznychProbek_returnPressed();
    void on_setTi_returnPressed();
    void on_setTD_returnPressed();
    void on_setb_returnPressed();
    void on_setN_returnPressed();
};

#endif // GUI_H
