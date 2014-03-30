#include "qkontrolersymulacji.h"
#include<QTimer>
#include<QMap>
#include<QString>
#include<QVector>
#include<memory>
#include"obiektdyskretny.h"
#include<iostream>
#include<QMessageBox>
#include<string>
#include "badvariablenameexception.h"
#define DEBUG
const std::set<QString> QKontrolerSymulacji::m_dopuszczalneNazwyZmiennych = {"m_wZadana","m_licznik1","m_licznik2","m_mianownik1","m_mianownik2","m_wariancja","m_dh","m_delay","m_momentPrzelaczenia","m_czas","m_predkoscSymulacji"};

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
    //jeżeli symulacja ciągła jest już włącona nic nie robi
    if(!m_symulacjaCiagla){
        //prędkość symulacji ustawiona na maksymalną
        if(m_predkoscSymulacji==0){
            m_timer.start(0);
            //stosujemy m_refreshTimer, żeby ograniczyć częstość odświeżania ekranu - przyśpieszyć symulację
            QObject::connect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
            m_refreshTimer.start(m_okresOdswiezania);
        }
        //prędkość symulacji ustawiona na wartość mniejszą niż maksymalną
        else{
            m_timer.start(m_dt*1000/static_cast<double>(m_predkoscSymulacji));
            //jeżeli okres m_timer jest mniejszy niż okres odświeżania stosujemy do odświeżania m_refreshTimer
            //inaczej stosujemy m_timer
            if(m_dt*1000/static_cast<double>(m_predkoscSymulacji)<m_okresOdswiezania){
                QObject::connect(&m_refreshTimer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
                m_refreshTimer.start(m_okresOdswiezania);
            }else{
                QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(m_wyslijWynikiSymulacji()));
            }
        }
        //zabezpieczenie przed ponownym wejściem do if'a i włączaniem timerów/podłączeniem sygnałów
        m_symulacjaCiagla = true;
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
    //zatrzymanie symulacji ciągłej
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





//s////////////////////////////////////////////////////////////
//s//////////      symulacja           ////////////////////////
//s////////////////////////////////////////////////////////////

//s//////////          SLOT TIMERA         /////////////////////
void QKontrolerSymulacji::onTimeout(){
    m_symuluj();
}

//s//////////          m_symuluj - właściwa funkcja symulująca ///////////////////
void QKontrolerSymulacji::m_symuluj(){
    if(m_ob!=NULL){
        //odpowiedź na skok jednostkowy
        if(m_u.size()==1){
            double czas;
            //symulacja
            double y = m_ob->symuluj(m_u.at(0),&czas);
            //przechowywanie danych do przesłania
            m_histT.push_back(czas);
            m_histU.push_back(m_u.at(0));
            m_histY.push_back(y);
        }
        //odpowiedź na zadaną sekwencję wejść
        else if(m_u.size()>m_licznikProbek){
            double czas;
            //symulacja
            double y = m_ob->symuluj(m_u.at(m_licznikProbek),&czas);
            //przechowywanie danych do przesłania
            m_histT.push_back(czas);
            m_histU.push_back(m_u.at(m_licznikProbek));
            m_histY.push_back(y);
            //przejście do następnej próbki
            ++m_licznikProbek;
        }
        //zadana sekwencja wejść skończyła sie, lub przesłano pusty wektor
        else{
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
    zbierzDaneDoprzeslania();
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
void QKontrolerSymulacji::odbierzDaneObiektu(QMapaDanych m)
{
    QString pom;
    auto it = m.begin();

    //wektor wymuszeń
    pom = "m_wZadana";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_u = it.value();
        }
    }
    //wielomiany licznika
    pom = "m_licznik1";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setLicznik(it.value().toStdVector(),1);
        }
     }

    pom = "m_licznik2";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setLicznik(it.value().toStdVector(),2);
        }
    }

    pom = "m_mianownik1";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setMianownik(it.value().toStdVector(),1);
        }
    }

    pom = "m_mianownik2";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setMianownik(it.value().toStdVector(),2);
        }
    }
    pom = "m_wariancja";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setWariancja(it.value()[0]);
        }
    }

    //czas próbkowania
    pom = "m_dh";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            setDt(it.value()[0]);
        }
    }

    pom = "m_delay";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setWariancja(it.value()[0]);
        }
    }

    pom = "m_momentPrzelaczenia";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setMomentPrzelaczenia(it.value()[0]);
        }
    }

    pom = "m_czas";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setCzas(it.value()[0]);
        }
    }

    pom = "m_predkoscSymulacji";
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            setPredkoscSymulacji(static_cast<int>(it.value()[0]));
        }
    }

}

//s//////////                          ////////////////////////
//s////////// SETERY  specjalizowane   ////////////////////////
//s//////////  wywoływane przez        ////////////////////////
//s/////////  odbierzDaneObiektu       ////////////////////////

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
//s//////////      GETERY          ////////////////////////
//s////////////////////////////////////////////////////////////
void QKontrolerSymulacji::zbierzDaneDoprzeslania()
{

    QMapaDanych m;
    QVector<double> pom;

    pom.push_back(m_u.at(static_cast<double>(m_licznikProbek)));
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

    pom = QVector<double>::fromStdVector(m_ob->getLicznik(1));
    m[QString("m_licznik1")] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getLicznik(2));
    m[QString("m_licznik2")] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getMianownik(1));
    m[QString("m_mianownik1")] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getMianownik(2));
    m[QString("m_mianownik2")] = pom;
    pom.clear();

    pom.push_back(m_ob->getWariancja());
    m[QString("m_wariancja")] = pom;
    pom.clear();

    pom.push_back(m_ob->czas());
    m[QString("m_czas")] = pom;
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

QMapaDanych QKontrolerSymulacji::stworzQMapeDanych(const char *tab, QVector<double> vec)
{
    QMapaDanych m;
    QString str(tab);
    auto it = m_dopuszczalneNazwyZmiennych.find(str);
    if(it!=m_dopuszczalneNazwyZmiennych.end()){
        m[str] = vec;
        return m;
    }else{
        QString komunikatBledu;
        komunikatBledu = QString("Zmienna \"")+str+QString(" \"nie istnieje");
        throw BadVariableNameException(komunikatBledu);
    }
}

QMapaDanych QKontrolerSymulacji::stworzQMapeDanych(const char *tab, double wart)
{
    QVector<double> vec;
    vec.push_back(wart);
    return stworzQMapeDanych(tab,vec);
}




