#include "qkontrolersymulacji.h"
#include<QTimer>
#include<memory>
#include"obiektdyskretny.h"
#include<iostream>
#include<QMessageBox>
#include<string>
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
void QKontrolerSymulacji::symulacjaKrokowa(){
    symulacjaCiaglaStop();
    m_symuluj();
    m_wyslijWynikiSymulacji();
}

void QKontrolerSymulacji::resetSymulacji()
{
    m_ob->resetujSymulacje();
}

//s////////////////////////////////////////////////////////////
//s//////////      symulacja           ////////////////////////
//s////////////////////////////////////////////////////////////

void QKontrolerSymulacji::onTimeout(){
    m_symuluj();
}

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
//s//////////      SETERY          ////////////////////////
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

//s////////////////////////////////////////////////////////////
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

//s////////////////////////////////////////////////////////////
//s//////////      setPredkoscSymulacji      //////////////////
void QKontrolerSymulacji::setPredkoscSymulacji(int prSym)
{
    if(prSym>=0){
        m_predkoscSymulacji = prSym;
        if(m_symulacjaCiagla){
            if(m_timer.isActive()){
                m_timer.stop();
            }
            if(prSym==0){
                m_timer.start(0);
            }else{
                m_timer.start(m_dt*1000/static_cast<double>(m_predkoscSymulacji));
            }
        }
    }else{
        QMessageBox::critical(0,"Błąd","Podano ujemną prędkość symulacji");
    }
}


//s////////////////////////////////////////////////////////////
//s//////////      DESTRUKTOR          ////////////////////////
//s////////////////////////////////////////////////////////////
QKontrolerSymulacji::~QKontrolerSymulacji()
{
    std::cout<<"Jestem w destruktorze QKontrolerSymulacji"<<std::endl;
}



