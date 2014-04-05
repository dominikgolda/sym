#include "regulatorp.h"
#include <string>
RegulatorP::RegulatorP(const std::string &nazwa):
    Regulator(nazwa)
{
}

RegulatorP::RegulatorP(const std::string &nazwa, std::shared_ptr<Komponent> wartoscZadana):
    Regulator(nazwa,wartoscZadana)
{

}

double RegulatorP::symuluj(double y)
{
    double wartoscZadana = m_WartoscZadana->generuj(m_nrProbki);
    ++m_nrProbki;
    return m_k*(wartoscZadana - y);
}

void RegulatorP::setNastawyRegulatora(std::vector<double> nastawy)
{
    if(nastawy.end()!=nastawy.begin()){
        m_k = nastawy[0];
    }
}

void RegulatorP::wczytajDane(string)
{

}

void RegulatorP::zapiszDane(string, string)
{

}
