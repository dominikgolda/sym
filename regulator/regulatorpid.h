#ifndef REGULATORPID_H
#define REGULATORPID_H
#include "regulator/regulator.h"
#include "klasaxml/klasaxml.h"
#include<limits>
class RegulatorPID : public Regulator
{
public:
    ///
    /// \brief RegulatorPID
    /// Domyślne nastawy regulatora dobrane są tak, żeby ich wpływ na obiekt był jak najmniejszy. Część całkująca i różniczkująca są wyłączone. Stała czasowa całkowania jest ustawiona na największą wartość jaką może przyjąć double. Stała czasowa różniczkowania równa 0. Wzmocnienie regulatora równa 1.
    RegulatorPID();
    ///
    /// \brief symuluj - oblicza wartość sterowania na podstawie próbki wyjścia
    /// \param y - próbka wyjścia z obiektu
    /// \return sterowanie dla obiektu
    ///
    double symuluj(double y);
    ///
    /// \brief setNastawyRegulatora pozwala ustawić nastawy regulatora PID
    /// \param nastawy - NastawyRegulatora typ zdefiniowany w stale.h jako std::map<std::string,std::vector<double>>. Wartości indeksów zaimplementowanych w RegulatorPID:
    /// -"m_kr" - wzmocnienie regulatora
    /// -"m_TD" - stała zasowa różniczkowania
    /// -"m_N"  - stosunek stałej czasowej różniczkowania do stałej czasowej filtru
    /// - "m_Ti" - stała czasowa całkowania
    /// - "m_b" - stała dla wzmocnienia wartości zadanej części proporcjonalnej P = m_kr*(m_b*w - y); P - wyjście części proporcjonalnej, w - wartość zadana, y - wyjście z obiektu
    /// - "m_Tp" - okres próbkowania
    void setNastawyRegulatora(NastawyRegulatora nastawy);

    ///
    /// \brief getNastawyRegulatora
    /// \return
    ///
    NastawyRegulatora getNastawyRegulatora();
    ///
    /// \brief wczytajDane - wczytuje nastawy regulatora z pliku
    /// \param sciezka - ścieżka do pliku
    ///
    void wczytajDane(string sciezka);

    ///
    /// \brief zapiszDane - zapisuje nastawy regulatora do pliku
    /// \param sciezka - ścieżka do pliku
    ///
    void zapiszDane(string sciezka, string);

    ///
    /// \brief resetujSymulacje - resetuje wartości przechowywane w regulatorze i wartość zadaną. Pozostawia nastawy regulatora be zmian.
    ///
    void resetujSymulacje();
private:

    //c/////////////////////////////
    //c/    NASTAWY REGULATORA    //
    //c////////////////////////////
    //stała czasowa różniczkowania
    double m_TD=0;
    //stosunek stałej czasowej różniczkowania do stałej czasowej filtru
    double m_N = 8;
    //stała czasowa całkowania
    double m_Ti=std::numeric_limits<double>::max();
    //wzmocnienie regulatora
    double m_kr=1;
    //stała dla wzmocnienia wartości zadanej części proporcjonalnej P = m_kr*(m_b*w - y); P - wyjście części proporcjonalnej, w - wartość zadana, y - wyjście z obiektu
    double m_b = 1;
    //okres próbkowania
    double m_Tp=0.1;
    //c///////////////////////////////////////////
    //c/    ZMIENNE PAMIĘTAJĄCE STAN REGULATORA //
    //c///////////////////////////////////////////

    //poprzednie wyjście z części różniczkującej
    double m_poprzednieD = 0;
    //poprzedni uchyb
    double m_poprzedniUchyb = 0;
    //poprzenia wartość wejścia do regulatora
    double m_poprzednieWejscie = 0;
    //
    double m_wartoscCalki =0;
    //c///////////////////////////////////////////
    //c/    ODCZYT/ZAPIS DO PLIKU       //////////
    //c///////////////////////////////////////////
    KlasaXML m_kXml;
};

#endif // REGULATORPID_H
