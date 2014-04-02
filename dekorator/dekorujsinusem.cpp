#include "dekorujsinusem.h"


DekorujSinusem::DekorujSinusem(int kokres, double kamplituda):
    Dekorator() //wywolanie konstruktora domyslnego, komponenty zawarte to KomponentKonkretny
{
    okres = kokres;
    amp = kamplituda;
}

DekorujSinusem::DekorujSinusem(std::shared_ptr<Komponent> komponencik,int kokres, double kamplituda):
    Dekorator(komponencik)// wywolanie konstruktora Dekorator by zawieral poprzednie komponenty
{
    okres = kokres;
    amp = kamplituda;
}
double DekorujSinusem::generuj(int nr_probki){
    // wartosc pomocnicza
    double WartZwracana;
    // jezeli jest jakis okres
    if(okres>1){
        // tworzenie przebiegu piloksztaltnego i normalizacja
        WartZwracana = (nr_probki%okres)/(static_cast<double>(okres));
        // przebieg piloksztaltny w wartosciach 0-2*pi
        WartZwracana *= 2*atan(1)*4;
        // wyznaczanie wartosci sinusa i mnozenie razy amplituda
        WartZwracana = sin(WartZwracana)*amp;
    }
    else{ // dla okresu <1
        WartZwracana = amp;
    }
    //zwracanie sumy wartosci przebiegu sinusa i poprzednich przebiegow
    return WartZwracana + Dekorator::generuj(nr_probki);
}


