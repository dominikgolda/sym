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
    Regulator(std::string nazwa="regulator");
    ///
    /// \brief Regulator - konstruktor pozwalający na ustawienie parametrów regulatora
    /// \param nazwa - nazwa stosowana przy odczycie z pliku
    /// \param nastawy - nastawy, nie implementowane w klasie regulator, do wykożystania w klasach pochodnych
    /// \param w - typy wartości zadanych jakie należy poskładać
    /// \param param - parametry dla wartości zadanych podanych w "w"
    ///
    Regulator(std::string nazwa, std::vector<TypyWymuszen> w, std::vector<std::vector<double>> param);
    ///
    /// \brief setWartoscZadana - tworzy generator wartości zadanej opisany przez zmienne funkcji
    /// \param w - typy wymuszeń jakie mają być użyte, pierwszy element w "w" jest najbardziej zewnętrznym jaki zostanie stworzony
    /// \param param - parametry dla tworzonych generatorów
    ///
    void setWartoscZadana(const std::vector<TypyWymuszen> &w, const std::vector<std::vector<double> > &param);

    ///
    /// \brief symuluj - funkcja wyznaczająca sterowanie w odpowiedzi na próbkę wejścia
    /// \param u - wejście do obiektu
    /// \param wartoscZadana - bieżąca wartość zadana - do wyświetlenia na wykresie.
    /// \return -wyjście z regulatora
    ///
    double symuluj(double y, double *wartoscZadana)=0;

    ///
    /// \brief getWartoscZadana zwraca do argumentów wywołania typ oraz parametry wartości zadanej
    /// \param w - typy wymuszeń
    /// \param param - parametry wymuszeń
    ///
    void getWartoscZadana(std::vector<TypyWymuszen> &w, std::vector<std::vector<double> > &param);

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
protected:
   std::shared_ptr<Komponent> m_WartoscZadana;
    ///
    /// \brief stworzGenerator - tworzy obiekt typu wynikającego ze zmiennej jaki
    /// \param jaki - decyduje o tym jakiego typu obiekt zostanie stworzony
    /// \param param - wektor parametrów obiektu, jeżeli podane zostanie mniej elementów niż potrzebuje dany konstruktor wówczas użyte zostaną wartości domyślne. Jeżeli podane zostanie więcej parametrów dalsze parametry zostaną zignorowane.
    /// \return - shared_ptr do poprzedniego w kolejności składania generatora
    ///
    std::shared_ptr<Komponent>  stworzGenerator(TypyWymuszen jaki, std::vector<double> param, std::shared_ptr<Komponent> poprzedni);

    //zmienna wykorzystywana w generatorze
    int m_nrProbki = 0;
    //parametr stosowany przy zapisie do pliku
    string m_nazwaRegulatora;
    //wektor generatorów, jakie mają być złożone
    std::vector<TypyWymuszen> m_typyWymuszen;
    //wektor z wektorami parametrów dla generatorów
    std::vector<std::vector<double>> m_parametryWymuszenia;
};

#endif // REGULATOR_H
