#include "regulator.h"
#include "dekorator/komponentkonkretny.h"
Regulator::Regulator(const string &nazwa):
    m_WartoscZadana(new KomponentKonkretny)
{
    m_nazwaRegulatora = nazwa;
}

Regulator::Regulator(const string &nazwa, shared_ptr<Komponent> wartoscZadana)
{
    m_nazwaRegulatora = nazwa;

    setWartoscZadana(wartoscZadana);

}

void Regulator::setWartoscZadana(shared_ptr<Komponent> &wartoscZadana)
{
    m_WartoscZadana = wartoscZadana;
}







double Regulator::getWartoscZadanaValue()
{
    return m_WartoscZadana->generuj(m_nrProbki);
}


void Regulator::resetujSymulacje()
{
    m_nrProbki = 0;
}

