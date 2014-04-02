#ifndef REGULATOR_H
#define REGULATOR_H
#include "dekorator/dekorator.h"
#include "obiektsiso.h"
#include <memory>

class Regulator : public ObiektSiso
{
public:
    Regulator();
    void ustawWartoscZadana();
private:
    //zmienna wykorzystywana w generatorze
    int m_NrProbki = 0;
    std::shared_ptr<Dekorator> m_WartoscZadana;
};

#endif // REGULATOR_H
