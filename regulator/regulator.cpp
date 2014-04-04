#include "regulator.h"
#include "dekorator/dekorujprostokatem.h"
#include "dekorator/dekorujsinusem.h"
#include "dekorator/dekorujstala.h"
#include "dekorator/dekorujtrojkatem.h"
#include "dekorator/komponentkonkretny.h"
Regulator::Regulator(string nazwa):
    m_WartoscZadana(new KomponentKonkretny)
{
    m_nazwaRegulatora = nazwa;
    m_typyWymuszen[0] = TypyWymuszen::brak;
}

Regulator::Regulator(string nazwa,std::vector<TypyWymuszen> w, std::vector<std::vector<double>> param)
{
    m_nazwaRegulatora = nazwa;
    m_typyWymuszen = w;
    m_parametryWymuszenia = param;
    setWartoscZadana(m_typyWymuszen,m_parametryWymuszenia);

}


void Regulator::setWartoscZadana(const std::vector<TypyWymuszen> &w,const std::vector<std::vector<double>> &param)
{
    //sprawdzam, czy wektor nie jest pusty i zy wektor typów jest tej samej długości co wektor parametrów
    if(w.size()>0){
        int i = w.size()-1;
        //najbardziej zewnętrzny element
        m_WartoscZadana = std::shared_ptr<Komponent>(new KomponentKonkretny());
        m_WartoscZadana = stworzGenerator(w.at(i),param.at(i),m_WartoscZadana);
        --i;
        for(;i>=0;--i){
            m_WartoscZadana = stworzGenerator(w.at(i),param.at(i),m_WartoscZadana);
        }
    }
    if(&m_typyWymuszen!=&w){
        m_typyWymuszen = w;
    }
    if(&m_parametryWymuszenia!=&param){
        m_parametryWymuszenia = param;
    }
}



std::shared_ptr<Komponent> Regulator::stworzGenerator(TypyWymuszen jaki, std::vector<double> param, std::shared_ptr<Komponent> poprzedni)
{
    std::shared_ptr<Komponent> pom = poprzedni;
    if(jaki==TypyWymuszen::prostokat){
        if(param.size()>=2){
            pom = std::shared_ptr<Komponent>(new DekorujProstokatem(poprzedni,param.at(0),param.at(1)));
        }else if(param.size()==1){
            pom = std::shared_ptr<Komponent>(new DekorujProstokatem(poprzedni,param.at(0)));
        }else{
            pom = std::shared_ptr<Komponent>(new DekorujProstokatem(poprzedni));
        }
    }else if(jaki == TypyWymuszen::sinus){
        if(param.size()>=2){
            pom = std::shared_ptr<Komponent>(new DekorujSinusem(poprzedni,param.at(0),param.at(1)));
        }else if(param.size()==1){
            pom = std::shared_ptr<Komponent>(new DekorujSinusem(poprzedni,param.at(0)));
        }else{
            pom = std::shared_ptr<Komponent>(new DekorujSinusem(poprzedni));
        }
    }else if(jaki == TypyWymuszen::stala){
        if(param.size() >=1){
            pom = std::shared_ptr<Komponent>(new DekorujStala(poprzedni,param.at(0)));
        }else{
            pom = std::shared_ptr<Komponent>(new DekorujStala(poprzedni));
        }
    }else if(jaki == TypyWymuszen::trojkat){
        if(param.size()>=3){
            pom = std::shared_ptr<Komponent>(new DekorujTrojkatem(poprzedni,param.at(0),param.at(1),param.at(2)));
        }else if(param.size() == 2){
            pom = std::shared_ptr<Komponent>(new DekorujTrojkatem(poprzedni,param.at(0),param.at(1)));
        }else if(param.size() ==1){
            pom = std::shared_ptr<Komponent>(new DekorujTrojkatem(poprzedni,param.at(0)));
        }else{
            pom = std::shared_ptr<Komponent>(new DekorujTrojkatem(poprzedni));
        }
    }
    return pom;
}


void Regulator::getWartoscZadana(std::vector<TypyWymuszen> &w, std::vector<std::vector<double> > &param)
{
    w = m_typyWymuszen;
    param = m_parametryWymuszenia;
}


double Regulator::getWartoscZadanaValue()
{
    return m_WartoscZadana->generuj(m_nrProbki);
}


void Regulator::resetujSymulacje()
{
    m_nrProbki = 0;
}

