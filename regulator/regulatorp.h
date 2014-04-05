#ifndef REGULATORP_H
#define REGULATORP_H
#include "regulator/regulator.h"
class RegulatorP : public Regulator
{
public:
    RegulatorP(const std::string &nazwa="regulatorP");

    RegulatorP(const std::string &nazwa, std::shared_ptr<Komponent> wartoscZadana);

    ///
    /// \brief symuluj - funkcja wyznaczająca sterowanie w odpowiedzi na próbkę wejścia
    /// \param u - wejście do obiektu
    /// \param wartoscZadana - bieżąca wartość zadana - do wyświetlenia na wykresie.
    /// \return -wyjście z regulatora
    ///
    double symuluj(double y);
    ///
    /// \brief setNastawyRegulatora - pozwala zmienić wzmocnienie regulatora
    /// \param nastawy - brana jest pod uwagę tylko pierwsza wartość w pierwszym wektorze
    ///
    void setNastawyRegulatora(std::vector<double> nastawy);
    void wczytajDane(string);
    void zapiszDane(string, string);
private:
    //wzmocnienie
    double m_k = 1;
};

#endif // REGULATORP_H
