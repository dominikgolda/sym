#ifndef DEKORUJTROJKATEM_H
#define DEKORUJTROJKATEM_H

#include <dekorator/dekorator.h>
#include <math.h>

class DekorujTrojkatem : public Dekorator
{
public:
    DekorujTrojkatem(int kokres=4, double kmax=4, double kmin=0);
    DekorujTrojkatem(std::shared_ptr<Komponent> komponencik, int kokres=4, double kmax=4, double kmin=0);
    double generuj(int nr_probki);
private:
    int okres = 4;
    double min = 0;
    double max = 1;
};

#endif // DEKORUJTROJKATEM_H
