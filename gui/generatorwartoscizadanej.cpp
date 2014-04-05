#include "generatorwartoscizadanej.h"
#include "stale.h"
#include "dekorator/dekorator.h"
#include "dekorator/dekorujprostokatem.h"
#include "dekorator/dekorujsinusem.h"
#include "dekorator/dekorujstala.h"
#include "dekorator/dekorujstala.h"
#include "dekorator/dekorujtrojkatem.h"
#include "dekorator/komponent.h"
#include "dekorator/komponentkonkretny.h"
#include <vector>
#include <memory>
GeneratorWartosciZadanej::GeneratorWartosciZadanej(){}
GeneratorWartosciZadanej::GeneratorWartosciZadanej(const std::vector<TypyWymuszen> &w,const std::vector<std::vector<double>> &param)
{
    setWartoscZadana(w,param);
}

void GeneratorWartosciZadanej::setWartoscZadana(const std::vector<TypyWymuszen> &w,const std::vector<std::vector<double>> &param)
{
//    //sprawdzam, czy wektor nie jest pusty
//        int i = w.size()-1;
//        //najbardziej zewnętrzny element
//        m_WartoscZadana = std::shared_ptr<Komponent>(new KomponentKonkretny());
//        m_WartoscZadana = stworzGenerator(w.at(i),param.at(i),m_WartoscZadana);
//        --i;
//        for(;i>=0;--i){
//            m_WartoscZadana = stworzGenerator(w.at(i),param.at(i),m_WartoscZadana);
//        }
        m_typyWymuszen = w;
        m_parametryWymuszenia = param;

}

void GeneratorWartosciZadanej::appendWartoscZadana(const std::vector<TypyWymuszen> &w, const std::vector<std::vector<double> > &param)
{
//    //sprawdzam, czy wektor nie jest pusty
//    if(w.size()>0){
//        int i = w.size()-1;
//        //najbardziej zewnętrzny element
//        m_WartoscZadana = std::shared_ptr<Komponent>(new KomponentKonkretny());
//        m_WartoscZadana = stworzGenerator(w.at(i),param.at(i),m_WartoscZadana);
//        --i;
//        for(;i>=0;--i){
//            m_WartoscZadana = stworzGenerator(w.at(i),param.at(i),m_WartoscZadana);
//        }
    m_typyWymuszen.reserve(m_typyWymuszen.size()+w.size());
    m_typyWymuszen.insert(m_typyWymuszen.end(),w.begin(),w.end());

    m_parametryWymuszenia.reserve(m_parametryWymuszenia.size() + param.size());
    m_parametryWymuszenia.insert(m_parametryWymuszenia.begin(),param.begin(),param.end());

}

std::shared_ptr<Komponent> GeneratorWartosciZadanej::getWartoscZadana() const
{
    std::shared_ptr<Komponent> wartoscZadana;
    wartoscZadana = std::shared_ptr<Komponent>(new KomponentKonkretny());
    for(int i= 0;i<m_typyWymuszen.size();++i){
        wartoscZadana = stworzGenerator(m_typyWymuszen.at(i),m_parametryWymuszenia.at(i),wartoscZadana);
    }
    return wartoscZadana;
}


std::shared_ptr<Komponent> GeneratorWartosciZadanej::stworzGenerator(TypyWymuszen jaki, std::vector<double> param, std::shared_ptr<Komponent> poprzedni) const
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

//c//////////////////////////////////////////////////////////////////////////////////
//c///////////                      GETERY I SETERY PROSTE                     //////
//c//////////////////////////////////////////////////////////////////////////////////

std::vector<std::vector<double> > GeneratorWartosciZadanej::parametryWymuszenia() const
{
    return m_parametryWymuszenia;
}


std::vector<TypyWymuszen> GeneratorWartosciZadanej::typyWymuszen() const
{
    return m_typyWymuszen;
}

