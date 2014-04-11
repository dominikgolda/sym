#include "regulatorpid.h"
#include <list>
RegulatorPID::RegulatorPID()
{
}

double RegulatorPID::symuluj(double y)
{
    double wZadana;
    double P=0,I=0,D = 0;
    //licznik próbek - potrzebny do generacji wartości zadanej
    m_nrProbki++;
    wZadana = m_WartoscZadana->generuj(m_nrProbki);
    //część proporcjonalna - z uwzględnieniem mniejszej od 1 wagi wartości zadanej
    P = m_kr*(m_b*wZadana - y);
    //część całkująca
    if(m_Ti>epsilon){
        I = m_kr/m_Ti/2.0*(wZadana-y +m_poprzedniUchyb);
    }
    //część różniczkująca
    if(m_TD+m_N*m_Tp>epsilon){
        D = -m_TD*m_poprzednieD/(m_TD+m_N*m_Tp) + m_kr*m_N*m_TD*(y - m_poprzednieWejscie)/(m_TD+m_N*m_Tp);
    }
    m_wartoscCalki +=I;

    m_poprzednieWejscie = y;
    m_poprzednieD = D;
    m_poprzedniUchyb = wZadana - y;
    return P+m_wartoscCalki+D;
}

void RegulatorPID::setNastawyRegulatora(NastawyRegulatora nastawy)
{
    //przypisujemy do zmiennych tymczasowych poprzednie nastawy. Dzięki temu nie trzeba zmieniać wszystkich nastaw.
    double kr = m_kr,TD = m_TD,N = m_N,Ti = m_Ti,b = m_b,Tp = m_Tp;
    bool nastawyPoprawne = true;
    //szukamy w mapie m_kr
    auto pom = nastawy.find("m_kr");
    //sprawdzamy, czy element "m_kr" istnieje
    if(nastawy.end()!=pom){
        //sprawdzamy, czy wektor z parametrami nie jest pusty
        if(pom->second.size()>=1){
            if(pom->second.at(0)<-epsilon || pom->second.at(0)>epsilon){
                kr = pom->second.at(0);
            }else{
                nastawyPoprawne = false;
            }
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = nastawy.find("m_TD");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            //sprawdzamy, czy nie podano ujemnej stałej czasowej
            if(pom->second.at(0)>=epsilon){
                TD = pom->second.at(0);
            }else{
                nastawyPoprawne = false;
            }
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = nastawy.find("m_N");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            //sprawdzamy, czy N nie jet ujemne
            if(pom->second.at(0)>=epsilon){
                N  = pom->second.at(0);
            }else{
                nastawyPoprawne = false;
            }
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = nastawy.find("m_Ti");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            //sprawdzamy czy stała czasowa nie jest ujemna
            if(pom->second.at(0)>epsilon){
                Ti = pom->second.at(0);
            }else{
                nastawyPoprawne = false;
            }
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = nastawy.find("m_b");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            //sprawdzamy, czy m_b jest w zakresie (0,1>
            if(pom->second.at(0)>epsilon&&pom->second.at(0)<=1){
                b = pom->second.at(0);
            }else{
                nastawyPoprawne = false;
            }
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = nastawy.find("m_Tp");
    if(nastawy.end()!=pom){
        if(pom->second.size()>=1){
            if(pom->second.at(0)>epsilon){
                Tp = pom->second.at(0);
            }else{ //ponieważ tak samo jest w obiekcie dyskretnym. Nie ma sensu dawać zerowego okersu próbkowania
                nastawyPoprawne = false;
            }
        }else{
            nastawyPoprawne = false;
        }
    }
    //przepisujemy dane ze zmiennych tymczasowych
    if(nastawyPoprawne){
        m_kr=kr;
        m_TD= TD;
        m_N = N;
        m_Ti =Ti;
        m_b = b;
        m_Tp = Tp;
    }


}

NastawyRegulatora RegulatorPID::getNastawyRegulatora()
{
    NastawyRegulatora pom;
    std::vector<double> vec;

    vec.push_back(m_kr);
    pom["m_kr"] = vec;

    vec.clear();
    vec.push_back(m_TD);
    pom["m_TD"] = vec;

    vec.clear();
    vec.push_back(m_N);
    pom["m_N"] = vec;

    vec.clear();
    vec.push_back(m_Ti);
    pom["m_Ti"] = vec;

    vec.clear();
    vec.push_back( m_b);
    pom["m_b"] = vec;

    vec.clear();
    vec.push_back( m_Tp);
    pom["m_Tp"] = vec;

    return pom;

}

void RegulatorPID::wczytajDane(string sciezka)
{
    std::vector<double> pom;
    double kr = m_kr,TD = m_TD,N = m_N,Ti = m_Ti,b = m_b,Tp = m_Tp;
    bool nastawyPoprawne = true;

    pom = m_kXml.wczytaj("m_kr",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        if(pom.at(0)>epsilon||pom.at(0)<-epsilon){
            kr = pom.at(0);
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = m_kXml.wczytaj("m_TD",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        if(pom.at(0)>epsilon){
            TD= pom.at(0);
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = m_kXml.wczytaj("m_Ti",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        if(pom.at(0)>epsilon){
            Ti= pom.at(0);
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = m_kXml.wczytaj("m_N",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        if(pom.at(0)>epsilon){
            N = pom.at(0);
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = m_kXml.wczytaj("m_b",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        if(pom.at(0)>epsilon && pom.at(0)<=1){
             b= pom.at(0);
        }else{
            nastawyPoprawne = false;
        }
    }

    pom = m_kXml.wczytaj("m_Tp",sciezka,m_nazwaRegulatora);
    if(pom.size()>=1){
        if(pom.at(0)>epsilon){
            Tp= pom.at(0);
        }else{
            nastawyPoprawne = false;
        }
    }

    //przepisujemy dane ze zmiennych tymczasowych
    if(nastawyPoprawne){
        m_kr=kr;
        m_TD= TD;
        m_N = N;
        m_Ti =Ti;
        m_b = b;
        m_Tp = Tp;
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
