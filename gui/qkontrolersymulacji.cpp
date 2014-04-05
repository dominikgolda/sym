#include "qkontrolersymulacji.h"
#include<QTimer>
#include<QMap>
#include<QString>
#include<QVector>
#include<memory>
#include"obiektdyskretny.h"
#include"regulator/petlaregulacji.h"
#include<iostream>
#include<QMessageBox>
#include<string>


QKontrolerSymulacji::QKontrolerSymulacji(ObiektDyskretny *kontrolowany,PetlaRegulacji *petla, QObject *parent) :
    QObject(parent),m_genWZadanej(new GeneratorWartosciZadanej)
{
    //timer kontrolujący symulację
    QObject::connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    m_petla = petla;
    m_ob = kontrolowany;

}


//s////////////////////////////////////////////////////////////
//s////////// Kontrola symulacji ////////////////////
//s////////////////////////////////////////////////////////////


//s////////// start ////////////////////////
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
//s////////// Stop ////////////////////////
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

//s////////// Symulacja Krokowa ////////////////////////
void QKontrolerSymulacji::symulacjaKrokowa(){
    //zatrzymanie symulacji ciągłej
    symulacjaCiaglaStop();
    m_symuluj();
    m_wyslijWynikiSymulacji();

}

//s////////// reset ////////////////////////
void QKontrolerSymulacji::resetSymulacji()
{
    std::vector<TypyWymuszen> a;
    a.push_back(TypyWymuszen::brak);
    std::vector<std::vector<double>> p{1};
    m_genWZadanej->setWartoscZadana(a,p);
    m_petla->setWartoscZadana(m_genWZadanej->getWartoscZadana());
    m_petla->resetujSymulacje();
}





//s////////////////////////////////////////////////////////////
//s////////// symulacja ////////////////////////
//s////////////////////////////////////////////////////////////

//s////////// SLOT TIMERA /////////////////////
void QKontrolerSymulacji::onTimeout(){
    m_symuluj();
}

//s////////// m_symuluj - właściwa funkcja symulująca ///////////////////
void QKontrolerSymulacji::m_symuluj(){
    if(m_petla!=NULL){
        //odpowiedź na skok jednostkowy
        if(m_u.size()==1){
            double czas;
            //symulacja
            double y = m_petla->symuluj(m_u.at(0));
            double wz = m_petla->getWartoscZadanaValue();
            double u = m_petla->getSterowanie();
            //odczytanie czasu
            czas = m_petla->getTime();
            //przechowywanie danych do przesłania
            m_histT.push_back(czas);
            m_histWZ.push_back(wz);
            m_histU.push_back(u);
            m_histY.push_back(y);
        }
        //odpowiedź na zadaną sekwencję wejść
        else if(m_u.size()>m_licznikProbek){
            double czas;
            //symulacja
            double y = m_petla->symuluj(m_u.at(m_licznikProbek));
            //odczytanie czasu
            czas = m_petla->getTime();
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


//s////////// wyślij wyniki ////////////////////
void QKontrolerSymulacji::m_wyslijWynikiSymulacji()
{
    if(!m_histT.empty()){
        emit wynikSymulacji(m_histY,m_histT,m_histU,m_histWZ);
        m_histY.clear();
        m_histU.clear();
        m_histT.clear();
        m_histWZ.clear();
    }
}


//s////////////////////////////////////////////////////////////
//s////////// Operacje na plikach //////////////////
//s////////////////////////////////////////////////////////////
/*!
* \brief QKontrolerSymulacji::openFile
* \param str - ścieżka pliku do otwarcia
*Funkcja odbiera od gui ścieżkę do pliku.
*/
void QKontrolerSymulacji::openFile(QString str)
{
    std::string stdStr(str.toUtf8());
    m_petla->wczytajDane(stdStr);
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
    m_petla->zapiszDane(stdStr,"test");
}


//s////////////////////////////////////////////////////////////
//s////////// SETERY ////////////////////////
//s////////////////////////////////////////////////////////////
void QKontrolerSymulacji::odbierzDaneObiektu(QMapaDanych m)
{
    DopuszczalneNazwyZmiennych pom;
    auto it = m.begin();

    //wektor wymuszeń
    pom = DopuszczalneNazwyZmiennych::m_wZadana;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_u = it.value();
        }
    }
    //wielomiany licznika
    pom = DopuszczalneNazwyZmiennych::m_licznik1;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setLicznik(it.value().toStdVector(),1);
        }
     }

    pom = DopuszczalneNazwyZmiennych::m_licznik2;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setLicznik(it.value().toStdVector(),2);
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_mianownik1;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setMianownik(it.value().toStdVector(),1);
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_mianownik2;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setMianownik(it.value().toStdVector(),2);
        }
    }
    pom = DopuszczalneNazwyZmiennych::m_wariancja;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setWariancja(it.value()[0]);
        }
    }

    //czas próbkowania
    pom = DopuszczalneNazwyZmiennych::m_dh;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            setDt(it.value()[0]);
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_delay;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setWariancja(it.value()[0]);
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_momentPrzelaczenia;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setMomentPrzelaczenia(it.value()[0]);
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_czas;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_ob->setCzas(it.value()[0]);
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_predkoscSymulacji;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            setPredkoscSymulacji(static_cast<int>(it.value()[0]));
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_wzmocnienie;
    it = m.find(pom);
    if(it!=m.end()){
        if(!(it.value().empty())){
            m_petla->setNastawyRegulatora(it.value().toStdVector());
        }
    }

}

void QKontrolerSymulacji::odbierzDaneObiektu(std::vector<TypyWymuszen> w, std::vector<std::vector<double> > param)
{
    m_genWZadanej->appendWartoscZadana(w,param);
    m_petla->setWartoscZadana(m_genWZadanej->getWartoscZadana());
}

//s////////// ////////////////////////
//s////////// SETERY specjalizowane ////////////////////////
//s////////// wywoływane przez ////////////////////////
//s///////// odbierzDaneObiektu ////////////////////////

//s////////// setDt ////////////////////////
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

//s////////// Prędkość symulacji //////////////////
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
//s////////// GETERY ////////////////////////
//s////////////////////////////////////////////////////////////
void QKontrolerSymulacji::zbierzDaneDoprzeslania()
{

    QMapaDanych m;
    QVector<double> pom;

    pom.push_back(m_u.at(static_cast<double>(m_licznikProbek)));
    m[DopuszczalneNazwyZmiennych::m_wZadana] = pom;
    pom.clear();

    pom.push_back(m_ob->getDt());
    m[DopuszczalneNazwyZmiennych::m_dh] = pom;
    pom.clear();

    pom.push_back(static_cast<double>(m_ob->getDelay()));
    m[DopuszczalneNazwyZmiennych::m_delay] = pom;
    pom.clear();

    pom.push_back(m_predkoscSymulacji);
    m[DopuszczalneNazwyZmiennych::m_predkoscSymulacji] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getLicznik(1));
    m[DopuszczalneNazwyZmiennych::m_licznik1] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getLicznik(2));
    m[DopuszczalneNazwyZmiennych::m_licznik2] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getMianownik(1));
    m[DopuszczalneNazwyZmiennych::m_mianownik1] = pom;
    pom.clear();

    pom = QVector<double>::fromStdVector(m_ob->getMianownik(2));
    m[DopuszczalneNazwyZmiennych::m_mianownik2] = pom;
    pom.clear();

    pom.push_back(m_ob->getWariancja());
    m[DopuszczalneNazwyZmiennych::m_wariancja] = pom;
    pom.clear();

    pom.push_back(m_ob->czas());
    m[DopuszczalneNazwyZmiennych::m_czas] = pom;
    pom.clear();

    emit wyslijDaneObiektu(m);
}

void QKontrolerSymulacji::getParameters()
{
    zbierzDaneDoprzeslania();
}




//s////////////////////////////////////////////////////////////
//s////////// DESTRUKTOR ////////////////////////
//s////////////////////////////////////////////////////////////
QKontrolerSymulacji::~QKontrolerSymulacji()
{
    std::cout<<"Jestem w destruktorze QKontrolerSymulacji"<<std::endl;
}

QMapaDanych QKontrolerSymulacji::stworzQMapeDanych(const DopuszczalneNazwyZmiennych klucz, QVector<double> vec)
{
        QMapaDanych m;
        m[klucz] = vec;
        return m;
}

QMapaDanych QKontrolerSymulacji::stworzQMapeDanych(const DopuszczalneNazwyZmiennych klucz, double wart)
{
    QVector<double> vec;
    vec.push_back(wart);
    QMapaDanych m;
    m[klucz] = vec;
    return m;
}



