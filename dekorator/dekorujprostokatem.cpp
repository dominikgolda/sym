#include "dekorujprostokatem.h"

DekorujProstokatem::DekorujProstokatem(int kokres, double kmax, double kmin):
    Dekorator() //wywolanie konstruktora domyslnego, komponenty zawarte to KomponentKonkretny
{
    okres = kokres;
    min= kmin;
    max= kmax;
}
DekorujProstokatem::DekorujProstokatem(std::shared_ptr<Komponent> komponencik, int kokres, double kmax, double kmin):
    Dekorator(komponencik)// wywolanie konstruktora Dekorator by zawieral poprzednie komponenty
{
    okres=kokres;
    min=kmin;
    max=kmax;
}

double DekorujProstokatem::generuj(int nr_probki){
    // wartosc pomocnicza
    double WartoscWyjsciowa;
    // modelowanie przebiegu prostokątnego
    if(nr_probki%(okres) < okres/2){
        WartoscWyjsciowa = max;
    }
    else{
        WartoscWyjsciowa = min;
    }
    // zwracana wartosc przebiegu prostokatnego i poprzednich przebiegow
    return WartoscWyjsciowa + Dekorator::generuj(nr_probki);
}
