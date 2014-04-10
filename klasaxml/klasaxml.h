#ifndef KLASAXML_H
#define KLASAXML_H

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

//!
//! \brief The KlasaXML class
//! Klasa przechowuje funkcje służące do zapisu oraz odczytu danych do pliku XML
class KlasaXML
{
public:
    KlasaXML();
    //!
    //! \brief zapisz
    //! \param lista_wyrazen lista nazw kolejnych wyrazen.
    //! \param lista_wartosci lista vectorów wartości dla konkretnych wyrażeń. Np. vector posiada wartość 10 ostatnich wyjść symulacji.
    //! \param sciezka sciezka dostępu do pliku
    //! \param nazwa_obiektu nazwa obiektu który ma być zapisany
    //!
    void zapisz(std::list<std::string> lista_wyrazen, std::vector<std::vector<double>> lista_wartosci, std::string sciezka, std::string nazwa_obiektu);
    //!
    //! \brief wczytaj
    //! \param wyrazenie prosba o dostep do konkretnego
    //! \param sciezka sciezka dostępu do pliku
    //! \param nazwa_obiektu nazwa obiektu który ma być zapisany
    //! \return vector wartosci przypisanych do danego wyrazenia
    //!
    std::vector<double> wczytaj(std::string wyrazenie, std::string sciezka, std::string nazwa_obiektu);

};

#endif // KLASAXML_H
