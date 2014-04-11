#include "nastawypid.h"
#include <string>
NastawyPid::NastawyPid()
{
}

//c/////////////////////////////////////////////////
//          TOWRZENIE MAPY PARAMETROW            ///
//c/////////////////////////////////////////////////

NastawyRegulatora NastawyPid::m_b(double b)
{
    std::vector<double> pom;
    pom.push_back(b);
    NastawyRegulatora m;
    m["m_b"] = pom;
    return m;
}

NastawyRegulatora NastawyPid::m_Ti(double Ti)
{
    std::vector<double> pom;
    pom.push_back(Ti);
    NastawyRegulatora m;
    m["m_Ti"] = pom;
    return m;

}

NastawyRegulatora NastawyPid::m_TD(double Td)
{
    std::vector<double> pom;
    pom.push_back(Td);
    NastawyRegulatora m;
    m["m_TD"] = pom;
    return m;
}

NastawyRegulatora NastawyPid::m_N(double N)
{
    std::vector<double> pom;
    pom.push_back(N);
    NastawyRegulatora m;
    m["m_N"] = pom;
    return m;
}

NastawyRegulatora NastawyPid::m_Tp(double Tp)
{
    std::vector<double> pom;
    pom.push_back(Tp);
    NastawyRegulatora m;
    m["m_Tp"] = pom;
    return m;
}

NastawyRegulatora NastawyPid::m_kr(double kr)
{
    std::vector<double> pom;
    pom.push_back(kr);
    NastawyRegulatora m;
    m["m_kr"] = pom;
    return m;
}

//c/////////////////////////////////////////////////
//          DEKODOWANIE MAPY                     ///
//c/////////////////////////////////////////////////

QMapaDanych NastawyPid::getM_b(NastawyRegulatora &n, QMapaDanych &m)
{
    QVector<double> nowyVec;
    std::vector<double> pom;
    NastawyRegulatora::iterator it = n.find("m_b");
    if(it!=n.end()){
        pom = it->second;
        nowyVec.push_back(pom.at(0));
        m[DopuszczalneNazwyZmiennych::m_b] = nowyVec;
    }
    return m;
}

QMapaDanych NastawyPid::getM_Ti(NastawyRegulatora& n, QMapaDanych& m)
{
    QVector<double> nowyVec;
    std::vector<double> pom;
    auto it = n.find("m_Ti");
    if(it!=n.end()){
        pom = it->second;
        nowyVec.push_back(pom.at(0));
        m[DopuszczalneNazwyZmiennych::m_Ti] = nowyVec;
    }
    return m;
}

QMapaDanych NastawyPid::getM_TD(NastawyRegulatora &n, QMapaDanych &m)
{
    QVector<double> nowyVec;
    std::vector<double> pom;
    auto it = n.find("m_TD");
    if(it!=n.end()){
        pom = it->second;
        nowyVec.push_back(pom.at(0));
        m[DopuszczalneNazwyZmiennych::m_TD] = nowyVec;
    }
    return m;

}

QMapaDanych NastawyPid::getM_N(NastawyRegulatora &n, QMapaDanych &m)
{
    QVector<double> nowyVec;
    std::vector<double> pom;
    auto it = n.find("m_N");
    if(it!=n.end()){
        pom = it->second;
        nowyVec.push_back(pom.at(0));
        m[DopuszczalneNazwyZmiennych::m_N] = nowyVec;
    }
    return m;
}

QMapaDanych NastawyPid::getM_Tp(NastawyRegulatora &n, QMapaDanych &m)
{
    QVector<double> nowyVec;
    std::vector<double> pom;
    auto it = n.find("m_Tp");
    if(it!=n.end()){
        pom = it->second;
        nowyVec.push_back(pom.at(0));
        m[DopuszczalneNazwyZmiennych::m_dh] = nowyVec;
    }
    return m;
}

QMapaDanych NastawyPid::getM_kr(NastawyRegulatora &n, QMapaDanych &m)
{
    QVector<double> nowyVec;
    auto it = n.find("m_kr");
    std::vector<double> pom;
    if(it!=n.end()){
        pom = it->second;
        nowyVec.push_back(pom.at(0));
        m[DopuszczalneNazwyZmiennych::m_wzmocnienie] = nowyVec;
    }
    return m;
}
