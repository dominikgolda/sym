#include "dekorujtrojkatem.h"


DekorujTrojkatem::DekorujTrojkatem(int kokres, double kmax, double kmin):
    Dekorator() //wywolanie konstruktora domyslnego, komponenty zawarte to KomponentKonkretny
{
    okres=kokres;
    min=kmin;
    max=kmax;
}
DekorujTrojkatem::DekorujTrojkatem(std::shared_ptr<Komponent> komponencik, int kokres, double kmax, double kmin):
    Dekorator(komponencik) // wywolanie konstruktora Dekorator by zawieral poprzednie komponenty
{
    okres=kokres;
    min=kmin;
    max=kmax;
}

double DekorujTrojkatem::generuj(int nr_probki){
    // wartosc pomocnicza do obliczen.
    double WartZwracana;
    // jezeli okres>1 czyli jest sens przeprowadzac przebieg trojkatny
    if(okres>1){
        // definiowanie przebiegu trojkatnego na podstawie nr probki
        WartZwracana = static_cast<double>(((nr_probki+okres/2)%okres))-okres/2.0;
        // wartosc bezwzgledna
        WartZwracana = fabs(WartZwracana);
        // noralizacja przebiegu
        WartZwracana = WartZwracana/(static_cast<double>(okres/2.0));
        // nadanie wartosci max
        WartZwracana *= max - min;
        // nadanie wartosci min
        WartZwracana += min;
    }
    else{
        // jezeli nie moze generowac przebiegu trojkatnego zwraca wartosc stala rowna max
        WartZwracana = max;
    }
    // dodaje przebieg trojkatny do reszty przebiegow
    return WartZwracana + Dekorator::generuj(nr_probki);
}
