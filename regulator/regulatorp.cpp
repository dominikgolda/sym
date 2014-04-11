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

void RegulatorP::setNastawyRegulatora(NastawyRegulatora nastawy)
{
    auto pom = nastawy.find("m_k");
    if(nastawy.end()!=pom){
        m_k = nastawy[std::string("m_k")].at(0);
    }
}

NastawyRegulatora RegulatorP::getNastawyRegulatora()
{
    NastawyRegulatora p;
    std::vector<double> wektor;
    wektor.push_back(m_k);
    p["m_k"] = wektor;
}

void RegulatorP::wczytajDane(string)
{

}

void RegulatorP::zapiszDane(string, string)
{

}
