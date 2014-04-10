#include "nastawypid.h"
#include <string>
NastawyPid::NastawyPid()
{
}


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
