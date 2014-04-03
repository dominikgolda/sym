#ifndef REGULATORP_H
#define REGULATORP_H
#include "regulator/regulator.h"
class RegulatorP : public Regulator
{
public:
    RegulatorP(std::string nazwa="regulatorP");

    RegulatorP(std::string nazwa, std::vector<TypyWymuszen> w, std::vector<std::vector<double>> param);

    double symuluj(double y, double *wartoscZadana);
    void setNastawyRegulatora(std::vector<double> nastawy);
    void wczytajDane(string sciezka){}
    void zapiszDane(string sciezka, string nazwa_obiektu){}
private:
    //wzmocnienie
    double m_k = 10;
};

#endif // REGULATORP_H
