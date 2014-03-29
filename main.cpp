#include <QApplication>
#include <iostream>
#include <vector>
#include <initializer_list>
#include "qkontrolersymulacji.h"
#include "obiektdyskretny.h"
#include "gui.h"
#include<memory>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::vector<double> L1{0.01, 0};
    std::vector<double> M1{1,-0.99};
    std::vector<double> L{0.1, 0};
    std::vector<double> M{1,-0.9};
    std::shared_ptr<ObiektDyskretny>inercja(new ObiektDyskretny(std::move(L),std::move(M),0,std::move(L1),std::move(M1),0));//(0,0.1,1,10,2);
       std::shared_ptr<QKontrolerSymulacji>kontroler(new QKontrolerSymulacji(inercja.get()));
       Gui okno(kontroler.get());

//       std::vector<double> L{0.001, 0};
//       std::vector<double> M{1,-0.999};
//       ObiektDyskretny *inercja = new ObiektDyskretny(std::move(L),std::move(M));//(0,0.1,1,10,2);
//       delete inercja;
       //QKontrolerSymulacji* kontroler= new QKontrolerSymulacji(inercja);
       //Gui okno(kontroler);
    okno.show();

    app.exec();

}
