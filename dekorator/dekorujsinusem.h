#ifndef DEKORUJSINUSEM_H
#define DEKORUJSINUSEM_H

#include <dekorator/dekorator.h>
#include <math.h>

class DekorujSinusem : public Dekorator
{
public:
    DekorujSinusem(int kokres=4, double kamplituda=1);
    DekorujSinusem(std::shared_ptr<Komponent> komponencik,int kokres=4, double kamplituda=1);
    double generuj(int nr_probki);
private:
    int okres = 4;
    double amp = 1;
};

#endif // DEKORUJSINUSEM_H
