#ifndef NASTAWYPID_H
#define NASTAWYPID_H
#include "stale.h"
#include "qkontrolersymulacji.h"
class NastawyPid
{
public:
    static NastawyRegulatora m_b(double b);
    static NastawyRegulatora m_Ti(double Ti);
    static NastawyRegulatora m_TD(double Td);
    static NastawyRegulatora m_N(double N);
    static NastawyRegulatora m_Tp(double Tp);
    static NastawyRegulatora m_kr(double kr);

    static QMapaDanych getM_b(NastawyRegulatora &n, QMapaDanych &m);
    static QMapaDanych getM_Ti(NastawyRegulatora &n, QMapaDanych &m);
    static QMapaDanych getM_TD(NastawyRegulatora &n, QMapaDanych &m);
    static QMapaDanych getM_N(NastawyRegulatora &n, QMapaDanych &m);
    static QMapaDanych getM_Tp(NastawyRegulatora &n, QMapaDanych &m);
    static QMapaDanych getM_kr(NastawyRegulatora &n, QMapaDanych &m);

private:
    NastawyPid();

};

#endif // NASTAWYPID_H
