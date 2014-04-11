#include "petlaregulacji.h"

PetlaRegulacji::PetlaRegulacji()
{
}

double PetlaRegulacji::symuluj(double)
{
    auto it = m_obiekty.begin();
    //symulacja regulatora
    if(m_obiekty.begin()!=m_obiekty.end()){
        //czy do w pętli regulacji znajduje się regulator
        if(m_jestRegulator){
            m_sterowanie = (*it)->symuluj(m_poprzednieWyjscie);
            ++it;
        }else{
            m_sterowanie = (*it)->symuluj(m_poprzednieWyjscie);
        }

    }
    //symulacja innych obiektów
    for(;it!=m_obiekty.end();++it){
        m_poprzednieWyjscie = (*it)->symuluj(m_sterowanie);
    }
    return m_poprzednieWyjscie;
//    m_poprzednieWyjscie= KompozytObiektow::symuluj(m_poprzednieWyjscie,czas);
}

double PetlaRegulacji::getWartoscZadanaValue()
{
    //jeżeli w pętli jest regulator zwracana jest wartość zadana dla obecnego numeru próbki
    if(m_jestRegulator){
        return dynamic_cast<Regulator*>(m_obiekty.at(0))->getWartoscZadanaValue();
    }else{
        //jeżeli w pętli nie ma regulatora pętla działa tak jakby był w niej regulator p o wzmocnieniu 1 - zwracana jest poprzednia wartość wyjścia
        return m_poprzednieWyjscie;
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

void PetlaRegulacji::setNastawyRegulatora(NastawyRegulatora nastawy)
{
    if(m_jestRegulator){
        dynamic_cast<Regulator*>(m_obiekty.at(0))->setNastawyRegulatora(nastawy);
    }
}

NastawyRegulatora PetlaRegulacji::getNastawyRegulatora()
{
    if(m_jestRegulator){
        return dynamic_cast<Regulator*>(m_obiekty.at(0))->getNastawyRegulatora();
    }
}

void PetlaRegulacji::setWartoscZadana(shared_ptr<Komponent> wartoscZadana)
{
    if(m_jestRegulator){
        dynamic_cast<Regulator*>(m_obiekty.at(0))->setWartoscZadana(wartoscZadana);
    }
}

double PetlaRegulacji::getSterowanie() const
{
    if(m_jestRegulator){
        return m_sterowanie;
    }else{
        return m_poprzednieWyjscie;
    }
}

double PetlaRegulacji::getTime() const
{
    if(m_jestRegulator){
        if(m_obiekty.size()>1){
            return m_obiekty.at(1)->getTime();
        }else if(m_obiekty.size()==1){
            return m_obiekty.at(0)->getTime();
        }else{
            return 0;
        }
    }else{
        if(m_obiekty.size()>0){
            return m_obiekty.at(0)->getTime();
        }else{
            return 0;
        }
    }
}

void PetlaRegulacji::resetujSymulacje()
{
    m_sterowanie = 0;
    m_poprzednieWyjscie=0;
    PolaczenieSzeregowe::resetujSymulacje();
}
