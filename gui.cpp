#include "gui.h"
#include "ui_gui.h"
#include "qkontrolersymulacji.h"
#include <vector>
#include <string>
#include<iostream>
#include<map>
#include "stale.h"
#include "qvectorvalidator.h"


//s////////////////////////////////////////////////////////////
//s//////////      Konstruktor               //////////////////
//s////////////////////////////////////////////////////////////

Gui::Gui(QKontrolerSymulacji *kontroler, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::Gui)
{
    m_ui->setupUi(this);
    ustawWykres();
    ustawKontrolki();
    //zamykanie programu
    connect(m_ui->actionZakoncz,SIGNAL(triggered()),qApp,SLOT(quit()));
    //kontrola symulacji
    connect(this,SIGNAL(symulacjaCiagla()), kontroler,SLOT(symulacjaCiaglaStart()));
    connect(this,SIGNAL(symulacjaKrokowa()), kontroler,SLOT(symulacjaKrokowa()));
    connect(this,SIGNAL(symulacjaStop()),kontroler,SLOT(symulacjaCiaglaStop()));
    connect(this,SIGNAL(resetSymulacji()),kontroler,SLOT(resetSymulacji()));

    //odczyt i zapis
    connect(this,SIGNAL(openFile(QString)),kontroler,SLOT(openFile(QString)));
    connect(this,SIGNAL(saveFile(QString)),kontroler,SLOT(saveFile(QString)));

    //odbiór danych
    connect(kontroler,SIGNAL(wynikSymulacji(QVector<double>,QVector<double>,QVector<double>)),this,SLOT(odbierzWyniki(QVector<double>,QVector<double>,QVector<double>)));
    connect(kontroler,SIGNAL(wyslijDaneObiektu(QMapaDanych)),this,SLOT(ustawDaneDlaUzytkownika(QMapaDanych)));
    //informacje z obiektu
    connect(kontroler,SIGNAL(symulacjaZakonczona()),this,SLOT(symulacjaZakonczona()));

    //rządanie wysłania danych
    connect(this,SIGNAL(getParameters()),kontroler,SLOT(getParameters()));
    //wysyłanie ustawień do obiektu
    connect(this,SIGNAL(setParameters(QMapaDanych)),kontroler,SLOT(odbierzDaneObiektu(QMapaDanych)));

    //wysyłanie ustawień do obiektu
//    connect(this,SIGNAL(setWymuszenie(std::vector<double>)),kontroler,SLOT(setWymuszenie(std::vector<double>)));
//    connect(this,SIGNAL(setDt(double)),kontroler,SLOT(setDt(double)));
//    connect(this,SIGNAL(setWielomianLicznika(std::vector<double>,int,std::string)),kontroler,SLOT(setWielomianLicznika(std::vector<double>,int,std::string)));
//    connect(this,SIGNAL(setWielomianMianownika(std::vector<double>,int,std::string)),kontroler,SLOT(setWielomianMianownik(std::vector<double>,int,std::string)));
//    connect(this,SIGNAL(setWariancja(double)),kontroler,SLOT(setWariancja(double)));


    //wymuszenie skok jednostkowy
    emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_wZadana,1));
    //ustawienie tekstów w kontrolkach zgodnie z zawartością obiektu
    emit getParameters();
}

//s////////////////////////////////////////////////////////////
//s//////////      Destruktor                //////////////////
//s////////////////////////////////////////////////////////////
Gui::~Gui()
{
    delete m_ui;
}




//s////////////////////////////////////////////////////////////
//s//////////      odbieranie wyników symulacji        ////////
//s////////////////////////////////////////////////////////////
void Gui::odbierzWyniki(QVector<double> y, QVector<double> t, QVector<double> u)
{

    m_ui->m_wykrWyjWartZad->graph(0)->addData(t,y);
    m_ui->m_wykrSter->graph(0)->addData(t,u);
    m_ui->m_wykrSter->graph(0)->rescaleAxes();
   m_ui->m_wykrWyjWartZad->graph(0)->rescaleAxes();
    m_ui->m_wykrSter->replot();
    m_ui->m_wykrWyjWartZad->replot();
    //ustawianie wartości w labelach
    m_ui->labelSterowanie->setText(QString::number(u.last()));
    m_ui->labelWyjscie->setText(QString::number(y.last()));
    m_ui->labelWZadana->setText(QString::number(u.last()));

}

//c/////////            symulacja zakończona        ////////////
////
/// \brief Gui::symulacjaZakonczona - slot przez który QKontrolerSymulacji informuje Gui, że nie ma już więcej próbek
///
void Gui::symulacjaZakonczona()
{
    QMessageBox::critical(this,tr(""),tr("Symulacja zakończona lub nie podano wymuszenia"));
}

//s////////////////////////////////////////////////////////////
//s//////////      ZMIANA DANYCH NA FORMATCE           ////////
//s////////////////////////////////////////////////////////////
/// \brief Gui::ustawDaneDlaUzytkownika - ustawia wartości na formatce
///
/// \param m - mapa postaci klucz parametru <=> vector doubli. Klucz parametru odpowiada nazwie pól
///klasy QObiektDyskretny. W przyszłości można będzie rozbudować mapę o parametry regulatorów.
void Gui::ustawDaneDlaUzytkownika(QMapaDanych m)
{

    DopuszczalneNazwyZmiennych pom;
    auto it = m.begin();

    pom = DopuszczalneNazwyZmiennych::m_wZadana;
    it = m.find(pom);
    if(it!=m.end()){
        setTextDlaKontrolekDouble(m_ui->setWZadana,QString::number(it.value()[0]));
    }

    pom = DopuszczalneNazwyZmiennych::m_dh;
    it = m.find(pom);
    if(it!=m.end()){
        setTextDlaKontrolekDouble(m_ui->setDh,QString::number(it.value()[0]));
    }

    //do przechowywania opóźnienia - potrzebne w kilku miejscach
    int localDelay=0;
    pom = DopuszczalneNazwyZmiennych::m_delay;
    it = m.find(pom);
    if(it!=m.end()){
        localDelay = static_cast<int>(it.value().at(0));
    }

    pom = DopuszczalneNazwyZmiennych::m_predkoscSymulacji;
    it = m.find(pom);
    if(it!=m.end()){
        m_ui->setPrSym->setValue(it.value().at(0));
    }

    pom = DopuszczalneNazwyZmiennych::m_licznik1;
    it = m.find(pom);
    if(it!=m.end()){
        if(m_ui->wyborLicznika->value()==1){
            m_ui->labelWielomianLicznika->setText(stworzQStringWielomianu(it.value(),localDelay));
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_licznik2;
    it = m.find(pom);
    if(it!=m.end()){
        if(m_ui->wyborLicznika->value()==2){
            m_ui->labelWielomianLicznika->setText(stworzQStringWielomianu(it.value(),localDelay));
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_mianownik1;
    it = m.find(pom);
    if(it!=m.end()){
        if(m_ui->wyborMianownika->value()==1){
            m_ui->labelWielomianMianownika->setText(stworzQStringWielomianu(it.value(),0));
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_mianownik2;
    it = m.find(pom);
    if(it!=m.end()){
        if(m_ui->wyborMianownika->value()==2){
            m_ui->labelWielomianMianownika->setText(stworzQStringWielomianu(it.value(),0));
        }
    }

    pom = DopuszczalneNazwyZmiennych::m_wariancja;
    it = m.find(pom);
    if(it!=m.end()){
        setTextDlaKontrolekDouble(m_ui->setWariancja,QString::number(it.value()[0]));
    }



}

//s////////////////////////////////////////////////////////////
//s//////////      FUNKCJE POMOCNICZE             /////////////
//s////////////////////////////////////////////////////////////
/// \brief Gui::stworzQStringWielomianu - przerabia wielomian na QString z wartościami typu a0 + a1 z^-1 + a2 z^-2 + ... + an z^-N+1
///Jeżeli ustawimy delay na wartość różną od 0 wówczas wielomian będzie przesunięty o -delay, czyli a0 z^-delay+ a1 z^-delay-1...
/// \param wielomian - wielomian dla którego chcemy zmienić
/// \param delay - wartość opóźnienia pozwala na zmianę wyświetlanie wielomianu nie od z^0, ale od z^-delay
/// \return QString z wielomianem zapisanym tak jak opisano wcześniej
///
QString Gui::stworzQStringWielomianu(QVector<double> &wielomian,int delay)
{
    QString pom("");
    int i=0;
    for( QVector<double>::iterator it = wielomian.begin(); it!=wielomian.end();++it,--i){
        if(*it>=0&&it!=wielomian.begin()){
            pom+=" + ";
        }else if(*it<0){
            pom+=" - ";
            *it = -*it;
        }
        pom+=QString::number(*it,'g',3);

        if((i-delay)<0){
            pom+= "z";
            pom+= QString::number(i-delay);
        }
    }
    return pom;
}

///
/// \brief Gui::ustawWykres - wstępnie ustawia parametry wykresu
///
void Gui::ustawWykres()
{
    //wykres dolny
    m_ui->m_wykrSter->addGraph();
    m_ui->m_wykrSter->xAxis->setLabel("t[s]");
    m_ui->m_wykrSter->yAxis->setLabel("y");
    m_ui->m_wykrSter->graph(0)->rescaleAxes();
    m_ui->m_wykrSter->replot();

    m_ui->m_wykrWyjWartZad->addGraph();
    m_ui->m_wykrWyjWartZad->addGraph();
    m_ui->m_wykrWyjWartZad->xAxis->setLabel("t[s]");
    m_ui->m_wykrWyjWartZad->yAxis->setLabel("y");
    m_ui->m_wykrWyjWartZad->graph(0)->rescaleAxes();
    m_ui->m_wykrWyjWartZad->replot();

}



//c//////////    ustaw kontrolki   //////////////////
///
/// \brief Gui::ustawKontrolki - inicjalizuje lineEdity odpowiednimi validatorami
///
void Gui::ustawKontrolki()
{
    QDoubleValidator *pom;
    //QDoubleValidator musi być skonfigurowane na angielskie locale, żeby przyjmował '.' a nie ',' jako
    //separator dziesiętny
    pom= new QDoubleValidator;
    pom->setLocale(QLocale::English);
    m_ui->setWZadana->setValidator(pom);

    pom= new QDoubleValidator;
    pom->setLocale(QLocale::English);
    //dt musi być dodatnie, zakładam, że min to epsilon zdefiniowane w stale.h
    pom->setBottom(epsilon);
    m_ui->setDh->setValidator(pom);

    pom= new QDoubleValidator;
    pom->setLocale(QLocale::English);
    m_ui->setWariancja->setValidator(pom);

    m_ui->setWielomianLicznka->setValidator(new QVectorValidator);
    m_ui->setWielomianMianownika->setValidator(new QVectorValidator);

}

///
/// \brief Gui::setTextDlaKontrolekDouble - ustawia tekst w kontrolce, która ma przyjmować wartości typu double.
///Po ustawieniu tekstu podpina na nowo do kontrolki validator.
/// \param edit - wskaźnik na QLineEdit
/// \param tekst - tekst, jaki ma być w nim wyświetlony
///
void Gui::setTextDlaKontrolekDouble(QLineEdit *edit,QString tekst)
{
    edit->setText(tekst);
    QDoubleValidator *pom = new QDoubleValidator;
    pom->setLocale(QLocale::English);
    m_ui->setWZadana->setValidator(pom);

}

///
/// \brief Gui::convretQStringToDoubleVector - zmienia QString na std::vector<double>. Liczby w wektorze są w takiej samej kolejności jak w stringu
/// \param str - QString który ma być przetworzony
/// \return std::vector<double> odpowiadający stringowi
///
QVector<double> Gui::convretQStringToDoubleVector(QString str)
{
    QTextStream strumien(&str);
    QVector<double> vec;
    double pom;
    while(!strumien.atEnd()){
        strumien>>pom;
        vec.push_back(pom);
    }
    return vec;
}


///
/// \brief Gui::on_actionZapi_sz_triggered - sprawdza, czy istnieje plik i czy da się go otworzyć do odczytu. Jeżeli nie
/// informuje o tym użytkownika. Jeżeli da się otworzyć plik, to wysyła sygnał saveFile
///
void Gui::on_actionZapi_sz_triggered()
{
    QString nazwaPliku = QFileDialog::getSaveFileName(this,tr("Wybierz plik"),"");

    if(nazwaPliku != ""){
        QFile plik(nazwaPliku);
        if(!plik.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this,tr("Błąd"),tr("Nie można otworzyć pliku"));
        }else{
            plik.close();
            emit saveFile(nazwaPliku);
        }
    }
}


///
/// \brief Gui::on_actionWczytaj_plik_triggered - sprawdza czy plik istnieje. Jeżeli tak wysyła sygnał openFile
///
void Gui::on_actionWczytaj_plik_triggered()
{
    QString nazwaPliku = QFileDialog::getOpenFileName(this,tr("Wybierz plik"),"");

    if(nazwaPliku != ""){
        QFile plik(nazwaPliku);
        if(!plik.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,tr("Błąd"),tr("Nie można otworzyć pliku"));
        }else{
            plik.close();
            emit openFile(nazwaPliku);
        }
    }
}

//s////////////////////////////////////////////////////////////
//s////////////////////////////////////////////////////////////
//s////////////////////////////////////////////////////////////
//                                                           //
//            Obsługa przycisków i editów                    //
//                                                           //
//s////////////////////////////////////////////////////////////
//s////////////////////////////////////////////////////////////
//s////////////////////////////////////////////////////////////



//s////////////////////////////////////////////////////////////
//s//////////      zakładka pierwsz - z wykresami     /////////
//s////////////////////////////////////////////////////////////


//s////////////////////////////////////////////////////////////
//s//////////      setery                             /////////
//s////////////////////////////////////////////////////////////


//s//////////         wartość zadana                  /////////

void Gui::on_setWZadana_returnPressed()
{
    QVector<double> vec;
    vec.push_back(m_ui->setWZadana->text().toDouble());
    emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_wZadana,vec));

}

//s//////////         krok symulacji <=> Dh           /////////
void Gui::on_setDh_returnPressed()
{
    double pom = m_ui->setDh->text().toDouble();
    emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_dh,pom));
}

//s//////////        prędkość symulacji               /////////
void Gui::on_setPrSym_valueChanged(int arg1)
{
    emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_predkoscSymulacji,static_cast<double>(arg1)));
}


//s////////////////////////////////////////////////////////////
//s//////////         Kontrola symulacji              /////////
//s////////////////////////////////////////////////////////////

//s//////////        resetuj symulację                /////////
void Gui::on_resetujSymulacje_clicked()
{
    //usunięcie danych z obiektu
    resetSymulacji();
    //zresetowanie wykresu
    on_resetujWykresy_clicked();
    //wyzerowanie czasu symulacji
    emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_czas,0));

}

//s//////////        symulacja ciągła                 /////////
void Gui::on_symCiagla_clicked()
{
    if(m_ui->symCiagla->isChecked()){
        on_actionSymulacja_ciagla_triggered();
    }else{
        on_actionZatrzymaj_symulacje_triggered();
    }
}

void Gui::on_resetujWykresy_clicked()
{
    m_ui->m_wykrSter->graph(0)->clearData();
    m_ui->m_wykrWyjWartZad->graph(0)->clearData();
}

//s//////////        symulacja krokowa                 /////////
void Gui::on_symKrokowa_clicked()
{
    on_actionSymulacja_krokowa_triggered();
}

//s////////////////////////////////////////////////////////////
//s//////////      zakładka druga - parametry obiektu  ////////
//s////////////////////////////////////////////////////////////

void Gui::on_setWielomianLicznka_returnPressed()
{
    QVector<double>  a = convretQStringToDoubleVector(m_ui->setWielomianLicznka->text());
    if(!a.empty()){
        if(m_ui->wyborMianownika->value()==1){
            emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_licznik1,a));
        }else if(m_ui->wyborMianownika->value()==2){
            emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_licznik2,a));
        }
    }
    emit getParameters();
}


void Gui::on_setWielomianMianownika_returnPressed()
{
    QVector<double> a = convretQStringToDoubleVector(m_ui->setWielomianMianownika->text());
    //czy wektor nie jest pusty
    if(!a.empty()){
        //który licznik należy ustawić
        if(m_ui->wyborMianownika->value()==1){
            emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_mianownik1,a));
        }else if(m_ui->wyborMianownika->value()==2){
            emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_mianownik2,a));
        }
    }
    emit getParameters();
}


void Gui::on_setWariancja_returnPressed()
{
    double pom = m_ui->setWariancja->text().toDouble();
    emit setParameters(QKontrolerSymulacji::stworzQMapeDanych(DopuszczalneNazwyZmiennych::m_wariancja,pom));
}


void Gui::on_wyborLicznika_valueChanged(int)
{
   emit getParameters();
}

void Gui::on_wyborMianownika_valueChanged(int)
{
    emit getParameters();
}


//s////////////////////////////////////////////////////////////
//s//////////         Akcje                           /////////
//s////////////////////////////////////////////////////////////

//s//////////        symulacja ciągła                 /////////
void Gui::on_actionSymulacja_ciagla_triggered()
{
    emit symulacjaCiagla();
}

//s//////////        symulacja krokowa                 /////////
void Gui::on_actionSymulacja_krokowa_triggered()
{
    emit symulacjaKrokowa();
}

//s//////////        STOP symulacji                    /////////
void Gui::on_actionZatrzymaj_symulacje_triggered()
{
    emit symulacjaStop();
}

void Gui::on_setWzmocnienie_returnPressed()
{

}
