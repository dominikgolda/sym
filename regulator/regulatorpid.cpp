#include "regulatorpid.h"
#include <list>
RegulatorPID::RegulatorPID()
{
}

double RegulatorPID::symuluj(double y)
{
    double wZadana;
    double P,I,D;
    m_nrProbki++;
    wZadana = m_WartoscZadana->generuj(m_nrProbki);
    P = m_kr*(m_b*wZadana - y);
    I = m_kr/m_Ti/2.0*(wZadana-y +m_poprzedniUchyb);
    D = m_TD/(m_TD+m_N*m_Tp)*(m_poprzednieD-m_kr*m_N*(y - m_poprzednieWejscie) );

    m_poprzednieWejscie = y;
    m_poprzednieD = D;
    m_poprzedniUchyb = wZadana - y;
    m_wartoscCalki +=I;
    return P+m_wartoscCalki+D;
}

void RegulatorPID::setNastawyRegulatora(NastawyRegulatora nastawy)
{
    //szukamy w mapie m_kr
    auto pom = nastawy.find("m_kr");
    //sprawdzamy, czy element "m_kr" istnieje
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            m_kr = pom->second.at(0);
        }
    }

    pom = nastawy.find("m_TD");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            m_TD = pom->second.at(0);
        }
    }

    pom = nastawy.find("m_N");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
           m_N  = pom->second.at(0);
        }
    }

    pom = nastawy.find("m_Ti");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            m_Ti = pom->second.at(0);
        }
    }

    pom = nastawy.find("m_b");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            m_b = pom->second.at(0);
        }
    }

    pom = nastawy.find("m_Tp");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            m_Tp = pom->second.at(0);
        }
    }

//    pom = nastawy.find("");
//    if(nastawy.end()!=pom){
//        if(pom->second.size()>=1){
//             = pom->second.at(0);
//        }
//    }


}

void RegulatorPID::wczytajDane(string sciezka)
{
    std::vector<double> pom;

    pom = m_kXml.wczytaj("m_kr",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        m_kr = pom.at(0);
    }

    pom = m_kXml.wczytaj("m_TD",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
         m_TD= pom.at(0);
    }

    pom = m_kXml.wczytaj("m_Ti",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
         m_Ti= pom.at(0);
    }

    pom = m_kXml.wczytaj("m_N",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        m_N = pom.at(0);
    }

    pom = m_kXml.wczytaj("m_b",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
         m_b= pom.at(0);
    }

    pom = m_kXml.wczytaj("m_Tp",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
         m_Tp= pom.at(0);
    }

}

void RegulatorPID::zapiszDane(string sciezka, string)
{
    std::list<std::string> nazwy;
    std::vector<std::vector<double>> wartosci;
    std::vector<double> pom;

    nazwy.push_back(std::string("m_kr"));
    nazwy.push_back(std::string("m_TD"));
    nazwy.push_back(std::string("m_Ti"));
    nazwy.push_back(std::string("m_N"));
    nazwy.push_back(std::string("m_b"));
    nazwy.push_back(std::string("m_Tp"));

    pom.push_back(m_kr);
    wartosci.push_back(pom);

    pom[0] = m_TD;
    wartosci.push_back(pom);

    pom[0] = m_Ti;
    wartosci.push_back(pom);

    pom[0] = m_N;
    wartosci.push_back(pom);

    pom[0] = m_b;
    wartosci.push_back(pom);

    pom[0] = m_Tp;
    wartosci.push_back(pom);

    m_kXml.zapisz(nazwy,wartosci,sciezka,m_nazwaRegulatora);
}

void RegulatorPID::resetujSymulacje()
{
    m_poprzednieD = 0;
    //poprzedni uchyb
    m_poprzedniUchyb = 0;
    //poprzenia wartość wejścia do regulatora
    m_poprzednieWejscie = 0;
    //
    m_wartoscCalki =0;

    Regulator::resetujSymulacje();
}
