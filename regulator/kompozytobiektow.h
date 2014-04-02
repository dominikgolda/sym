#ifndef KOMPOZYTOBIEKTOW_H
#define KOMPOZYTOBIEKTOW_H
#include "obiektsiso.h"
#include <list>
class KompozytObiektow : public ObiektSiso
{
public:
    KompozytObiektow();
    double symuluj(double u, double *czas);
    void wczytajDane(std::string sciezka);
    void zapiszDane(std::string sciezka, std::string nazwa_obiektu);
    void resetujSymulacje();


private:
   std::list<ObiektSiso*> m_obiekty;


};

#endif // KOMPOZYTOBIEKTOW_H
