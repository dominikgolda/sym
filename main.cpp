#include <QApplication>
#include <iostream>
#include <vector>
#include <initializer_list>
#include "qkontrolersymulacji.h"
#include "obiektdyskretny.h"
#include "gui.h"
#include "regulator/regulatorp.h"
#include "regulator/petlaregulacji.h"
#include "stale.h"
#include<memory>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::vector<double> L1{0.01, 0};
    std::vector<double> M1{1,-0.99};
    std::vector<double> L{0.1, 0};
    std::vector<double> M{1,-0.9};


    std::vector<std::vector<double>> wymuszenie({{50}});
    std::vector<TypyWymuszen> wym{TypyWymuszen::prostokat};
    PetlaRegulacji petla;
    RegulatorP reg(std::string("mojRegulator"),wym,wymuszenie);
    std::shared_ptr<ObiektDyskretny>inercja(new ObiektDyskretny(std::move(L),std::move(M),0,std::move(L1),std::move(M1),0));//(0,0.1,1,10,2);
    petla.dodajObiekt(inercja.get());
    petla.dodajRegulator(&reg);
    std::shared_ptr<QKontrolerSymulacji>kontroler(new QKontrolerSymulacji(inercja.get(),&petla));
    Gui okno(kontroler.get());

    okno.show();

    app.exec();
    return 0;
}
