#include "dekorujstala.h"

DekorujStala::DekorujStala(double kstala):
    Dekorator() //wywolanie konstruktora domyslnego, komponenty zawarte to KomponentKonkretny
{
    stala=kstala;
}

DekorujStala::DekorujStala(std::shared_ptr<Komponent> komponencik, double kstala):
    Dekorator(komponencik)// wywolanie konstruktora Dekorator by zawieral poprzednie komponenty
{
    stala=kstala;
}

double DekorujStala::generuj(int nr_probki){
    // zwraca sumę stałej i reszty przebiegow
    return stala+Dekorator::generuj(nr_probki);
}
