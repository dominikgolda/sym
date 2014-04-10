#ifndef NASTAWYPID_H
#define NASTAWYPID_H
#include "stale.h"
class NastawyPid
{
public:
    static NastawyRegulatora m_b(double b);
    static NastawyRegulatora m_Ti(double Ti);
    static NastawyRegulatora m_TD(double Td);
    static NastawyRegulatora m_N(double N);
    static NastawyRegulatora m_Tp(double Tp);
    static NastawyRegulatora m_kr(double kr);
private:
    NastawyPid();

};

#endif // NASTAWYPID_H
