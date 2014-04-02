#include <list>
#include "regulator/kompozytobiektow.h"

KompozytObiektow::KompozytObiektow()
{
}

double KompozytObiektow::symuluj(double u, double *czas)
{
    //*trash potrzebujemy tylko jednej wartości czasu, inne wyrzucamy
    double *trash;
    auto it = m_obiekty.begin();
    if(m_obiekty.begin()!=m_obiekty.end()){
        u = symuluj(u,czas);
        ++it;
    }
    for(;it!=m_obiekty.end();++it){
        u = (*it)->symuluj(u,trash);
    }
    return u;
}

void KompozytObiektow::wczytajDane(std::string sciezka)
{
}

void KompozytObiektow::zapiszDane(std::string sciezka, std::string nazwa_obiektu)
{
}

void KompozytObiektow::resetujSymulacje()
{
}
