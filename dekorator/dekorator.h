#ifndef DEKORATOR_H
#define DEKORATOR_H

// pliki z zalaczonymi klasami wzorca dekorator
#include <dekorator/komponentkonkretny.h>
// biblioteki standardowe
#include <memory>
using namespace std;

//!
//! \brief The Dekorator class
//! Klasa zawierająca wskaźnik na Komponent.
//!
class Dekorator : public Komponent
{
public:
    //!
    //! \brief Dekorator
    //! Tworzy komponent Konkretny zwracajacy wartosc 0 w funkcji generuj.
    Dekorator();
    // konstruktor pozwala zawierać komponent dowolnego rodzaju o ile ma przeladowana funkcje generuj()
    //!
    //! \brief Dekorator
    //! \param komponencik - Wskaznik do Komponentu zawierajacego inne przebiegi
    //!
    Dekorator(std::shared_ptr<Komponent> komponencik);

    // funkcja generuj przeznaczona do dziedziczenia lub wywolania komponentu konkretnego
    //!
    //! \brief generuj
    //! \param nr_probki - Numer aktualnej probki przebiegu dla obiektow dyskretnych.
    //! \return aktualna wartość przebiegu
    //!
    double generuj(int nr_probki);

private:
    // nasz wskaźnik na Komponent
    //!
    //! \brief komp
    //! Wskaźnik przechowujący Komponent. Pozwala zawierać szeregowo wiele komponentów.
    std::shared_ptr<Komponent> komp;
};

#endif // DEKORATOR_H
