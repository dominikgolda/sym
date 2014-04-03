#include "petlaregulacji.h"

PetlaRegulacji::PetlaRegulacji()
{
}

double PetlaRegulacji::symuluj(double, double *czas)
{
    auto it = m_obiekty.begin();
    //symulacja regulatora
    if(m_obiekty.begin()!=m_obiekty.end()){
        //czy do w pętli regulacji znajduje się regulator
        if(m_jestRegulator){
            m_poprzednieWyjscie = (*it)->symuluj(m_poprzednieWyjscie,&m_wZadana);
            ++it;
        }else{
            m_poprzednieWyjscie = (*it)->symuluj(m_poprzednieWyjscie,czas);
        }

    }
    //symulacja innych obiektów
    for(;it!=m_obiekty.end();++it){
        m_poprzednieWyjscie = (*it)->symuluj(m_poprzednieWyjscie,czas);
    }
    return m_poprzednieWyjscie;
//    m_poprzednieWyjscie= KompozytObiektow::symuluj(m_poprzednieWyjscie,czas);
}

double PetlaRegulacji::getWartoscZadanaValue()
{
    return m_wZadana;
}

void PetlaRegulacji::getWartoscZadana(std::vector<TypyWymuszen> &w, std::vector<std::vector<double> > &param)
{
    if(m_jestRegulator){
        //jeżeli jest regulator zwracane są wartości z regulatora
        dynamic_cast<Regulator*>(m_obiekty[0])->getWartoscZadana(w,param);
    }else{
        //jeżeli nie ma regulatora wektory są opróżniane
        w.erase(w.begin(),w.end());
        param.erase(param.begin(),param.end());
    }

}

void PetlaRegulacji::dodajObiekt(ObiektSiso *obiekt, int gdzie)
{
    //dodajemy obiekt zawsze za regulatorami
    if(gdzie>=1){
        if(m_obiekty.size()>gdzie){
            m_obiekty.insert(m_obiekty.begin()+gdzie,obiekt);
        }else{
            m_obiekty.push_back(obiekt);
        }
    }else{
        m_obiekty.push_back(obiekt);
    }
}

void PetlaRegulacji::dodajRegulator(Regulator *reg)
{
    if(m_jestRegulator){
        m_obiekty[0] = reg;
    }else{
        m_obiekty.insert(m_obiekty.begin(),reg);
        m_jestRegulator = true;
    }
}

void PetlaRegulacji::setNastawyRegulatora(std::vector<double> nastawy)
{
    if(m_jestRegulator){
        dynamic_cast<Regulator*>(m_obiekty.at(0))->setNastawyRegulatora(nastawy);
    }
}
