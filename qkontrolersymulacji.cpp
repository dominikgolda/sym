#include "qkontrolersymulacji.h"
#include<QTimer>
#include<memory>
#include"obiektdyskretny.h"
#include<iostream>
#include<QMessageBox>
#include<string>
#define DEBUG
QKontrolerSymulacji::QKontrolerSymulacji(ObiektDyskretny *kontrolowany, QObject *parent) :
    QObject(parent)
{
    //timer kontrolujący symulację
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimeout()));

    m_ob = kontrolowany;
}


//s////////////////////////////////////////////////////////////
//s//////////      Kontrola symulacji      ////////////////////
//s////////////////////////////////////////////////////////////


//s//////////         start           ////////////////////////
void QKontrolerSymulacji::symulacjaCiaglaStart(){
    if(!m_symulacjaCiagla){
        if(m_predkoscSymulacji==0){
            m_timer.start(0);
            QObject::connect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
            m_refreshTimer.start(m_okresOdswiezania);
            m_symulacjaCiagla = true;
        }else{
            m_timer.start(m_dt*1000/static_cast<double>(m_predkoscSymulacji));
            if(m_dt*1000/static_cast<double>(m_predkoscSymulacji)<m_okresOdswiezania){
                QObject::connect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
                m_refreshTimer.start(m_okresOdswiezania);
            }else{
                QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
            }
            m_symulacjaCiagla = true;
        }
    }
}
//s//////////         Stop           ////////////////////////
void QKontrolerSymulacji::symulacjaCiaglaStop()
{
    m_symulacjaCiagla=false;
    //zatrzymanie timerów
    m_timer.stop();
    m_refreshTimer.stop();
    //odłączenie sygnałów od funkcji m_wyslijWynikiSymulacji
    QObject::disconnect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
    QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));

    //wysłanie wyników i wyczyszczenie buforów
    m_wyslijWynikiSymulacji();
}

//s//////////         Symulacja Krokowa           ////////////////////////
void QKontrolerSymulacji::symulacjaKrokowa(){
    symulacjaCiaglaStop();
    m_symuluj();
    m_wyslijWynikiSymulacji();

#ifdef DEBUG
    zbierzDaneDoprzeslania();
#endif
}

//s//////////          reset           ////////////////////////
void QKontrolerSymulacji::resetSymulacji()
{
    m_ob->resetujSymulacje();
}



//s//////////      Prędkość symulacji      //////////////////
void QKontrolerSymulacji::setPredkoscSymulacji(int prSym)
{
    //nie może być ujemnego czasu
    if(prSym>=0){
        m_predkoscSymulacji = prSym;
        //jeżeli symulator działa trzeba zmienić ustawienia timerów
        if(m_symulacjaCiagla){
            //zatrzymuję timer, jeśli działa (chyba nie trzeba)
            if(m_timer.isActive()){
                m_timer.stop();
            }
            if(prSym==0){
                //odłączenie m_timera od odświeżania
                QObject::disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
                m_timer.start(0);
                //podłączenie timera odświeżającego obraz do funkcji odświeżającej
                QObject::connect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
                //włączenie timera odświeżającego obraz
                m_refreshTimer.start(m_okresOdswiezania);
            }else{
                m_timer.start(m_dt*1000/static_cast<double>(m_predkoscSymulacji));
                if(m_dt*1000/static_cast<double>(m_predkoscSymulacji)<m_okresOdswiezania){
                    QObject::connect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
                    m_refreshTimer.start(m_okresOdswiezania);
                }else{
                    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
                }
            }
        }
    }else{
        QMessageBox::critical(0,"Błąd","Podano ujemną prędkość symulacji");
    }
}

//s////////////////////////////////////////////////////////////
//s//////////      symulacja           ////////////////////////
//s////////////////////////////////////////////////////////////

void QKontrolerSymulacji::onTimeout(){
    m_symuluj();
}

//s//////////          m_symuluj           ////////////////////////
void QKontrolerSymulacji::m_symuluj(){
    if(m_ob!=NULL){
        if(m_u.size()==1){
            double czas;
            double y = m_ob->symuluj(m_u.at(0),&czas);
            m_histT.push_back(czas);
            m_histU.push_back(m_u.at(0));
            m_histY.push_back(y);
        }else if(m_u.size()>m_licznikProbek){
            double czas;
            double y = m_ob->symuluj(m_u.at(m_licznikProbek),&czas);
            ++m_licznikProbek;
            m_histT.push_back(czas);
            m_histU.push_back(m_u.at(m_licznikProbek));
            m_histY.push_back(y);
         }else{
            m_symulacjaCiagla = false;
            m_timer.stop();
            emit symulacjaZakonczona();
        }
    }
}


//s//////////    wyślij wyniki    ////////////////////
void QKontrolerSymulacji::m_wyslijWynikiSymulacji()
{
    if(!m_histT.empty()){
        emit wynikSymulacji(m_histY,m_histT,m_histU);
        m_histY.clear();
        m_histU.clear();
        m_histT.clear();
    }
}


//s////////////////////////////////////////////////////////////
//s//////////      Operacje na plikach       //////////////////
//s////////////////////////////////////////////////////////////
/*!
 * \brief QKontrolerSymulacji::openFile
 * \param str - ścieżka pliku do otwarcia
 *Funkcja odbiera od gui ścieżkę do pliku.
 */
void QKontrolerSymulacji::openFile(QString str)
{
    std::string stdStr(str.toUtf8());
    m_ob->wczytajDane(stdStr);
}
/*!
 * \brief QKontrolerSymulacji::saveFile
 * \param str -ścieżka pliku do zapisu
 *Funkcja odbiera od gui ścieżkę do pliku.
 */
void QKontrolerSymulacji::saveFile(QString str)
{
    std::string stdStr(str.toUtf8());
    m_ob->zapiszDane(stdStr,"test");
}


//s////////////////////////////////////////////////////////////
//s//////////      SETERY              ////////////////////////
//s////////////////////////////////////////////////////////////

void QKontrolerSymulacji::setWymuszenie(std::vector<double> u)
{
    m_u = u;
}
void QKontrolerSymulacji::setWielomianLicznika(std::vector<double> licz, int ktory, std::string id)
{
    m_ob->setLicznik(licz,ktory,id);
}
void QKontrolerSymulacji::setWielomianMianownik(std::vector<double> licz, int ktory, std::string id)
{
    m_ob->setMianownik(licz,ktory,id);
}

//s//////////          setDt           ////////////////////////
void QKontrolerSymulacji::setDt(double dt)
{
    if(dt<0){
        QMessageBox::critical(0,"Błąd","Podano niedodatni okres próbkowania");
    }else{
        m_dt = dt;
        m_ob->setDt(m_dt);
        if(m_symulacjaCiagla){
            if(m_predkoscSymulacji>=0){
                if(m_timer.isActive()){
                    m_timer.stop();
                }
                m_timer.start(m_dt*1000/static_cast<double>(m_predkoscSymulacji));
            }else{
                //tutaj nigdy nie powinniśmy dojść, bo nie ograniczamy wartości zadane do dotdatnich
                QMessageBox::critical(0,"Błąd","Podano ujemną prędkość symulacji");
            }
        }
    }
}
void QKontrolerSymulacji::setWariancja(double wariancja)
{
    m_ob->setWariancja(wariancja,"");
}

//s////////////////////////////////////////////////////////////
//s//////////      GETERY          ////////////////////////
//s////////////////////////////////////////////////////////////
void QKontrolerSymulacji::zbierzDaneDoprzeslania()
{

    QMap<QString, QVector<double> > m;
    QVector<double> pom;

    pom.push_back(m_u.at(m_licznikProbek));
    m[QString("m_wZadana")] = pom;
    pom.clear();

    pom.push_back(m_ob->getDt());
    m[QString("m_dh")] = pom;
    pom.clear();

    pom.push_back(static_cast<double>(m_ob->getDelay()));
    m[QString("m_delay")] = pom;
    pom.clear();

    pom.push_back(m_predkoscSymulacji);
    m[QString("m_predkoscSymulacji")] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getLicznik(1,""));
    m[QString("m_licznik1")] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getLicznik(2,""));
    m[QString("m_licznik2")] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getMianownik(1,""));
    m[QString("m_mianownik1")] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getMianownik(2,""));
    m[QString("m_mianownik2")] = pom;
    pom.clear();

    pom.push_back(m_ob->getWariancja());
    m[QString("m_wZadana")] = pom;
    pom.clear();




    emit wyslijDaneObiektu(m);
}
void QKontrolerSymulacji::getParameters()
{
    zbierzDaneDoprzeslania();
}


//s////////////////////////////////////////////////////////////
//s//////////      DESTRUKTOR          ////////////////////////
//s////////////////////////////////////////////////////////////
QKontrolerSymulacji::~QKontrolerSymulacji()
{
    std::cout<<"Jestem w destruktorze QKontrolerSymulacji"<<std::endl;
}




