#include <list>
#include "regulator/polaczenieszeregowe.h"

PolaczenieSzeregowe::PolaczenieSzeregowe()
{
}

double PolaczenieSzeregowe::symuluj(double u)
{
    auto it = m_obiekty.begin();
    if(m_obiekty.begin()!=m_obiekty.end()){
        u = (*it)->symuluj(u);
        ++it;
    }
    for(;it!=m_obiekty.end();++it){
        u = (*it)->symuluj(u);
    }
    return u;
}

void PolaczenieSzeregowe::wczytajDane(std::string sciezka)
{
    auto it = m_obiekty.begin();
    if(m_obiekty.begin()!=m_obiekty.end()){
        (*it)->wczytajDane(sciezka);
        ++it;
    }
    for(;it!=m_obiekty.end();++it){
        (*it)->wczytajDane(sciezka);
    }
}


void PolaczenieSzeregowe::zapiszDane(std::string sciezka, std::string nazwa_obiektu)
{
//    auto it = m_obiekty.begin();
//    if(m_obiekty.begin()!=m_obiekty.end()){
//        (*it)->zapiszDane(sciezka,nazwa_obiektu);
//        ++it;
//    }
//    for(;it!=m_obiekty.end();++it){
//        (*it)->zapiszDane(sciezka,nazwa_obiektu);
//    }
    auto it = m_obiekty.begin();
    if(m_obiekty.begin()!=m_obiekty.end()){
        it = m_obiekty.end();
        --it;
        (*it)->zapiszDane(sciezka,nazwa_obiektu);
        --it;
    }
    for(;it!=m_obiekty.begin();--it){
        (*it)->zapiszDane(sciezka,nazwa_obiektu);
    }
    (*it)->zapiszDane(sciezka,nazwa_obiektu);

}
void PolaczenieSzeregowe::resetujSymulacje()
{
    {
        auto it = m_obiekty.begin();
        if(m_obiekty.begin()!=m_obiekty.end()){
            (*it)->resetujSymulacje();
            ++it;
        }
        for(;it!=m_obiekty.end();++it){
            (*it)->resetujSymulacje();
        }
    }
}

void PolaczenieSzeregowe::dodajObiekt(ObiektSiso *obiekt, int gdzie)
{
    if(gdzie>=0){
        if(static_cast<int>(m_obiekty.size())>gdzie){
            m_obiekty.insert(m_obiekty.begin()+gdzie,obiekt);
        }else{
            m_obiekty.push_back(obiekt);
        }
    }else{
        m_obiekty.push_back(obiekt);
    }
}

void PolaczenieSzeregowe::usunObiekt(int ktory)
{
    //czy listan nie jest pusta
    if(m_obiekty.begin()!=m_obiekty.end()){
        if(ktory<0||ktory>static_cast<int>(m_obiekty.size())){
            m_obiekty.erase(m_obiekty.end()-1);
        }else{
            m_obiekty.erase(m_obiekty.begin()+ktory);
        }
    }
}
