#include "regulatorp.h"
#include <string>
RegulatorP::RegulatorP(std::string nazwa):
    Regulator(nazwa)
{
}

RegulatorP::RegulatorP(std::string nazwa, std::vector<TypyWymuszen> w, std::vector<std::vector<double>> param):
    Regulator(nazwa,w,param)
{

}

double RegulatorP::symuluj(double y, double *wartoscZadana)
{
    *wartoscZadana = m_WartoscZadana->generuj(m_nrProbki);
    ++m_nrProbki;
    return m_k*(*wartoscZadana - y);
}

void RegulatorP::setNastawyRegulatora(std::vector<double> nastawy)
{
    if(nastawy.end()!=nastawy.begin()){
        m_k = nastawy[0];
    }
}
