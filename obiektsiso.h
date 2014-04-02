#ifndef OBIEKTSISO_H
#define OBIEKTSISO_H

#include<string>
#include<vector>

class ObiektSiso
{

public:

    ObiektSiso();
    //czysto wirtualne
    virtual double symuluj(double u, double *czas) =0;
    virtual void wczytajDane(std::string sciezka)=0;
    virtual void resetujSymulacje()=0;

//    virtual double getDt()=0;
//    virtual std::string getId()=0;
    virtual void zapiszDane(std::string sciezka, std::string nazwa_obiektu)=0;
    //wirtualne
    virtual ~ObiektSiso();
};

#endif // OBIEKTSISO_H
