#include "gui.h"
#include "ui_gui.h"
#include "qkontrolersymulacji.h"
#include <vector>
#include <string>
#include<iostream>
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
    connect(m_ui->actionZakoncz,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(this,SIGNAL(symulacjaCiagla()), kontroler,SLOT(symulacjaCiaglaStart()));
    connect(this,SIGNAL(symulacjaKrokowa()), kontroler,SLOT(symulacjaKrokowa()));
    connect(this,SIGNAL(symulacjaStop()),kontroler,SLOT(symulacjaCiaglaStop()));
    connect(this,SIGNAL(setWymuszenie(std::vector<double>)),kontroler,SLOT(setWymuszenie(std::vector<double>)));
    connect(this,SIGNAL(setDt(double)),kontroler,SLOT(setDt(double)));
    connect(this,SIGNAL(setPredkoscSymulacji(int)),kontroler,SLOT(setPredkoscSymulacji(int)));
    connect(this,SIGNAL(resetSymulacji()),kontroler,SLOT(resetSymulacji()));
    connect(this,SIGNAL(openFile(QString)),kontroler,SLOT(openFile(QString)));
    connect(this,SIGNAL(saveFile(QString)),kontroler,SLOT(saveFile(QString)));
    connect(this,SIGNAL(setWielomianLicznika(std::vector<double>,int,std::string)),kontroler,SLOT(setWielomianLicznika(std::vector<double>,int,std::string)));
    connect(this,SIGNAL(setWielomianMianownika(std::vector<double>,int,std::string)),kontroler,SLOT(setWielomianMianownik(std::vector<double>,int,std::string)));

    connect(kontroler,SIGNAL(wynikSymulacji(QVector<double>,QVector<double>,QVector<double>)),this,SLOT(odbierzWyniki(QVector<double>,QVector<double>,QVector<double>)));
    connect(kontroler,SIGNAL(symulacjaZakonczona()),this,SLOT(symulacjaZakonczona()));

    std::vector<double>a={1};
    emit setWymuszenie(a);
    m_ui->labelSterowanie->setText(QString::number(1));
    setTextDlaKontrolekDouble(m_ui->setWZadana,QString::number(1));
    setTextDlaKontrolekDouble(m_ui->setDh,QString::number(0.1));
    m_ui->setPrSym->setValue(1);

}
//s////////////////////////////////////////////////////////////
//s//////////      Destruktor                //////////////////
//s////////////////////////////////////////////////////////////

Gui::~Gui()
{
    delete m_ui;
}

//s////////////////////////////
// symulacja ciągła
//s////////////////////////
void Gui::on_actionSymulacja_ciagla_triggered()
{
    emit symulacjaCiagla();
}
void Gui::on_symCiagla_clicked()
{
    if(m_ui->symCiagla->isChecked()){
        on_actionSymulacja_ciagla_triggered();
    }else{
        on_actionZatrzymaj_symulacje_triggered();
    }
}
//s////////////////////////////
// symulacja krokowa
//s////////////////////////
void Gui::on_symKrokowa_clicked()
{
    on_actionSymulacja_krokowa_triggered();
}

void Gui::on_actionSymulacja_krokowa_triggered()
{
    emit symulacjaKrokowa();
}
//s////////////////////////////
// symulacja zatrzymanie symulacji
//s////////////////////////

void Gui::on_actionZatrzymaj_symulacje_triggered()
{
    emit symulacjaStop();
}


//s////////////////////////////////////////////////////////////
//s//////////      wykres           ////////////////////////
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
////
/// \brief Gui::symulacjaZakonczona - slot przez który QKontrolerSymulacji informuje Gui, że nie ma już więcej próbek
///
void Gui::symulacjaZakonczona()
{
    QMessageBox::critical(this,tr(""),tr("Symulacja zakończona lub nie podano wymuszenia"));
}

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



//ustawia właściwości kontrolek
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

    m_ui->setWielomianLicznka->setValidator(new QVectorValidator);
    m_ui->setWielomianMianownika->setValidator(new QVectorValidator);

}

void Gui::setTextDlaKontrolekDouble(QLineEdit *edit,QString tekst)
{
    edit->setText(tekst);
    QDoubleValidator *pom = new QDoubleValidator;
    pom->setLocale(QLocale::English);
    m_ui->setWZadana->setValidator(pom);

}

std::vector<double> Gui::convretQStringToDoubleVector(QString str)
{
    QTextStream strumien(&str);
    std::vector<double> vec;
    double pom;
    while(!strumien.atEnd()){
        strumien>>pom;
        vec.push_back(pom);
    }
    return vec;
}



void Gui::on_setWZadana_returnPressed()
{
    std::vector<double> vec;
    vec.push_back(m_ui->setWZadana->text().toDouble());
    emit setWymuszenie(vec);
}

void Gui::on_setDh_returnPressed()
{
    double pom;
    pom = m_ui->setDh->text().toDouble();
    emit setDt(pom);
}

void Gui::on_setPrSym_valueChanged(int arg1)
{
    emit setPredkoscSymulacji(arg1);
}

void Gui::on_resetujSymulacje_clicked()
{
    resetSymulacji();
}
/*!
 * \brief Gui::on_actionZapi_sz_triggered - sprawdza, czy istnieje plik i czy da się go otworzyć do odczytu. Jeżeli nie
 *informuje o tym użytkownika. Jeżeli da się otworzyć plik, to wysyła sygnał
 */
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

void Gui::on_setWielomianLicznka_returnPressed()
{
    std::vector<double> a = convretQStringToDoubleVector(m_ui->setWielomianLicznka->text());
    for(auto it=a.begin();it!=a.end();it++){
        std::cout<<(*it)<<std::endl;
    }
    emit setWielomianLicznika(a,m_ui->wyborLicznika->value(),"");
}



void Gui::on_setWielomianMianownika_returnPressed()
{
    std::vector<double> a = convretQStringToDoubleVector(m_ui->setWielomianMianownika->text());
    emit setWielomianMianownika(a,m_ui->wyborMianownika->value(),"");
}



