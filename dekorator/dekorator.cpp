#include "dekorator.h"

Dekorator::Dekorator():
    komp(new KomponentKonkretny) // tworzenie Komponentu Konkretnego ktory zwraca 0 w funkcji generuj
{
}

Dekorator::Dekorator(std::shared_ptr<Komponent> komponencik){
    komp = komponencik; // zawieranie poprzednich komponentow
}

double Dekorator::generuj(int nr_probki){
    // zwracanie wartosci ktora generuje zawarty komponent
    return komp->generuj(nr_probki);
}
