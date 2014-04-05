#ifndef REGULATOR_H
#define REGULATOR_H
#include "dekorator/dekorator.h"
#include "obiektsiso.h"
#include <memory>
#include <vector>
#include <string>
#include "stale.h"
class Regulator : public ObiektSiso
{
public:
    Regulator(const std::string &nazwa="regulator");
    ///
    /// \brief Regulator - konstruktor pozwalający na ustawienie parametrów regulatora
    /// \param nazwa - nazwa stosowana przy odczycie z pliku
    /// \param nastawy - nastawy, nie implementowane w klasie regulator, do wykożystania w klasach pochodnych
    /// \param wartoscZadana - wskaźnik na element klasy Komponent, generujący przebieg
    Regulator(const std::string &nazwa,shared_ptr<Komponent> wartoscZadana);
    ///
    /// \brief setWartoscZadana - tworzy generator wartości zadanej opisany przez zmienne funkcji
    /// \param w - typy wymuszeń jakie mają być użyte, pierwszy element w "w" jest najbardziej zewnętrznym jaki zostanie stworzony
    /// \param param - parametry dla tworzonych generatorów
    ///
    void setWartoscZadana(shared_ptr<Komponent> &wartoscZadana);

    ///
    /// \brief symuluj - funkcja wyznaczająca sterowanie w odpowiedzi na próbkę wejścia
    /// \param u - wejście do obiektu
    /// \param wartoscZadana - bieżąca wartość zadana - do wyświetlenia na wykresie.
    /// \return -wyjście z regulatora
    ///
    double symuluj(double y)=0;

    ///
    /// \brief wczytajDane - odczytuje dane regulatora z pliku
    /// \param sciezka - ścieżka do pliku
    ///
    void wczytajDane(std::string sciezka)=0;

    ///
    /// \brief zapiszDane - zapisuje dane do pliku
    /// \param sciezka - ścieżka do pliku
    /// \param nazwa_obiektu -
    ///
    void zapiszDane(std::string sciezka, std::string nazwa_obiektu)=0;
    ///
    /// \brief resetujSymulacje - zeruje licznik próbek.
    ///
    void resetujSymulacje();

    ///
    /// \brief getWartoscZadanaValue - zwraca wartość jaką ma w danym momencie wartość zadana
    /// \return
    ///
    double getWartoscZadanaValue();

    ///
    /// \brief setNastawyRegulatora - ustawia parametry regulatora. Funkcja czysto wirtualna.
    /// \param nastawy
    ///
    virtual void setNastawyRegulatora(std::vector<double> nastawy)=0;
    ///
    /// \brief getTime
    /// \return zwraca czas symulacji w próbkach
    ///
    double getTime() const {return m_nrProbki;}
protected:
   std::shared_ptr<Komponent> m_WartoscZadana;

    //zmienna wykorzystywana w generatorze
    int m_nrProbki = 0;
    //parametr stosowany przy zapisie do pliku
    string m_nazwaRegulatora;
};

#endif // REGULATOR_H
