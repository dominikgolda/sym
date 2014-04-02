#ifndef DEKORUJPROSTOKATEM_H
#define DEKORUJPROSTOKATEM_H

#include <dekorator/dekorator.h>


class DekorujProstokatem : public Dekorator
{
public:
    DekorujProstokatem(int kokres=2, double kmax=1, double kmin=0);
    DekorujProstokatem(std::shared_ptr<Komponent> komponencik, int kokres=2, double kmax=1, double kmin=0);
    double generuj(int nr_probki);
private:
    int okres = 2;
    double max = 1;
    double min = 0;

};

#endif // DEKORUJPROSTOKATEM_H
