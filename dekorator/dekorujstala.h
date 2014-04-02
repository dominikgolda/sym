#ifndef DEKORUJSTALA_H
#define DEKORUJSTALA_H

#include <dekorator/dekorator.h>
#include <dekorator/komponent.h>
#include <dekorator/komponentkonkretny.h>

class DekorujStala : public Dekorator
{
public:
    DekorujStala(double kstala =1);
    DekorujStala(std::shared_ptr<Komponent> komponencik, double kstala=1);
    double generuj(int nr_probki);
private:
    double stala = 1;
};

#endif // DEKORUJSTALA_H
