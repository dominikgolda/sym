#ifndef PETLAREGULACJI_H
#define PETLAREGULACJI_H
#include "regulator/kompozytobiektow.h"
#include "regulator/regulator.h"
#include <memory>
#include "stale.h"



#include <iostream>
class PetlaRegulacji : private KompozytObiektow
{
public:
    PetlaRegulacji();
    ///
    /// \brief symuluj - jeden krok symulacji układu w pętli zamkniętej
    /// \param czas
    /// \return
    ///
    double symuluj(double, double *czas);
    ///
    /// \brief inline getWartoscZadanaValue - zwraca wartość zadaną jaka była stosowana przez regulator przy ostatniej symulacji. Jeżeli nie
    /// \return
    ///
    double getWartoscZadanaValue();
    void zagwizdz(){std::cout<<"test"<<std::endl;}
    /// \brief getWartoscZadana Jeżeli obecny jest regulator zwraca do argumentów wywołania typ oraz parametry wartości zadanej. Jeżeli regulator nie został przypisany do pętli regulacji wektory są czyszczne.
    /// \param w - typy wymuszeń
    /// \param param - parametry wymuszeń
    void getWartoscZadana(std::vector<TypyWymuszen> &w, std::vector<std::vector<double> > &param);
    ///
    /// \brief dodajObiekt - wstawia nowy obiekt na listę. Nie należy używać tej funkcji do dodawania regulatorów, do tego służy funkcja dodajRegulator.
    /// \param obiekt - wskaźnik do obiektu do wstawienia
    /// \param gdzie - pozycja na którą należy wstawić nowy obiekt. Jeżeli podana zostanie liczba ujemna lub dodatnia, większa od długości wektora obiektów wskaźnik do obiektu zostanie wstawiony na końcu. Funcja nie wstawi obiektu na pozycję 0, jeżeli w pętli jest regulator.
    ///
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
    void setNastawyRegulatora(std::vector<double> nastawy);

    ///
    /// \brief setWartoscZadana - tworzy generator wartości zadanej opisany przez zmienne funkcji
    /// \param w - typy wymuszeń jakie mają być użyte, pierwszy element w "w" jest najbardziej zewnętrznym jaki zostanie stworzony
    /// \param param - parametry dla tworzonych generatorów
    ///
    void setWartoscZadana(const std::vector<TypyWymuszen> &w, const std::vector<std::vector<double> > &param);

    ///
    /// \brief getSterowanie
    /// \return - wartość sterowania w danej iteracji. Jeżeli w pętli nie ma regulatora zwraca 0
    ///
    double getSterowanie();
    //funkcje z KompozytuObiektow, które stosujemy
    using KompozytObiektow::wczytajDane;
    using KompozytObiektow::zapiszDane;
    using KompozytObiektow::resetujSymulacje;
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
