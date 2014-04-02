#ifndef KOMPONENT_H
#define KOMPONENT_H

//!
//! \brief The Komponent class
//! Wirtualna abstrakcyjna klasa posiadająca wirtualną funkcję generuj. Dzięki zastosowaniu dziedziczenia oraz polimorfizmu pozwala łączyć przebiegi z wielu generowanych przebiegów.
//!
class Komponent
{
public:
    Komponent();
    //!
    //! \brief generuj
    //! \param nr_probki - Numer aktualnej probki przebiegu dla obiektow dyskretnych.
    //! \return aktualna wartość przebiegu
    //!
    virtual double generuj(int nr_probki) = 0;
};

#endif // KOMPONENT_H
