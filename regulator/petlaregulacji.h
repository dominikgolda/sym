#ifndef PETLAREGULACJI_H
#define PETLAREGULACJI_H
#include "regulator/polaczenieszeregowe.h"
#include "regulator/regulator.h"
#include <memory>
#include "stale.h"



#include <iostream>
class PetlaRegulacji : private PolaczenieSzeregowe
{
public:
    PetlaRegulacji();
    ///
    /// \brief symuluj - jeden krok symulacji układu w pętli zamkniętej
    /// \return
    ///
    double symuluj(double);
    ///
    /// \brief inline getWartoscZadanaValue - zwraca wartość zadaną jaka była stosowana przez regulator przy ostatniej symulacji. Jeżeli nie
    /// \return
    ///
    double getWartoscZadanaValue();
    /// \brief getWartoscZadana Jeżeli obecny jest regulator zwraca do argumentów wywołania typ oraz parametry wartości zadanej. Jeżeli regulator nie został przypisany do pętli regulacji wektory są czyszczne.
    /// \param w - typy wymuszeń
    /// \param param - parametry wymuszeń
    void dodajObiekt(ObiektSiso *obiekt,int gdzie=-1);
    ///
    /// \brief dodajRegulator - dodaje regulator na pierwsze miejsce listy obiektów. Jeżeli PętlaRegulacji nie zawierała wcześniej regulatora jest on wstawiany na pierwsze miejsce. Jeżeli regulator był obecny jest on zastępowany nowym regulatorem.
    /// \param reg - regulator odziedziczony z klasy Regulator
    ///
    void dodajRegulator(Regulator *reg);
    ///
    /// \brief setNastawyRegulatora - ustawia parametry regulatora
    /// \param nastawy - zależne od regulatora, dla którego jest wywoływane
    ///
    void setNastawyRegulatora(NastawyRegulatora nastawy);

    ///
    /// \brief getNastawyRegulatora - zwraca nastawy regulatora o ile jakiś znajduje się w pętli
    /// \return - nastawy regulaotra
    ///
    NastawyRegulatora getNastawyRegulatora();
    ///
    /// \brief setWartoscZadana - tworzy generator wartości zadanej opisany przez zmienne funkcji
    /// \param w - typy wymuszeń jakie mają być użyte, pierwszy element w "w" jest najbardziej zewnętrznym jaki zostanie stworzony
    /// \param param - parametry dla tworzonych generatorów
    ///
    void setWartoscZadana(shared_ptr<Komponent> wartoscZadana);

    ///
    /// \brief getSterowanie
    /// \return - wartość sterowania w danej iteracji. Jeżeli w pętli nie ma regulatora zwraca 0
    ///
    double getSterowanie() const;
    ///
    /// \brief getTime
    /// \return - czas symulacji. Jeżeli w pętli jest obiekt czas pobierany jest z obiektu. Jeżeli w pętli jest tylko regulator czas pobierany jest z regulatora. Jeżeli w pętli nie ma ani obiektów, ani regulatorów wówczas zwracane jeset 0.
    ///
    double getTime() const;

    void resetujSymulacje();

    //funkcje z KompozytuObiektow, które stosujemy
    using PolaczenieSzeregowe::wczytajDane;
    using PolaczenieSzeregowe::zapiszDane;
    using PolaczenieSzeregowe::resetujSymulacje;
protected:
    double m_poprzednieWyjscie = 0;
    //przechowuje wartość wartości zadaną jaka była stosowana przez regulator
    double m_wZadana=0;
    //wartosc na wyjsciu z regulatora
    double m_sterowanie = 0;
    //ile regulatorów jest w pętli
    unsigned short m_jestRegulator = 0;

};

#endif // PETLAREGULACJI_H
