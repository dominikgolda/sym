#ifndef KOMPONENTKONKRETNY_H
#define KOMPONENTKONKRETNY_H

#include <dekorator/komponent.h>

//!
//! \brief The KomponentKonkretny class
//! Klasa służąca do zapoczątkowania składania szeregowego komponentów.
//!
class KomponentKonkretny : public Komponent
{
public:
    KomponentKonkretny();
    // daje stala wartosc = 0
    //!
    //! \brief generuj
    //! \param nr_probki - Numer aktualnej probki przebiegu dla obiektow dyskretnych. Nie używany.
    //! \return zwraca wartość = 0
    //! Funkcja służy do zapoczątkowania składania przebiegów wartości zadanej.
    double generuj(int){
        return 0;
    }
};

#endif // KOMPONENTKONKRETNY_H
