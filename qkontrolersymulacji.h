#ifndef QKONTROLERSYMULACJI_H
#define QKONTROLERSYMULACJI_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <vector>
#include <QVector>
#include <QMap>
#include <QSet>
#include <set>
#include "stale.h"

//enum class globalNazwyZmiennych  {m_wZadana,m_licznik1,m_licznik2,m_mianownik1,m_mianownik2,m_wariancja,m_dh,m_delay,m_momentPrzelaczenia,m_czas,m_przedkoscSymulacji};
enum class DopuszczalneNazwyZmiennych{m_wZadana,m_licznik1,m_licznik2,m_mianownik1,m_mianownik2,m_wariancja,m_dh,m_delay,m_momentPrzelaczenia,m_czas,m_predkoscSymulacji,m_wzmocnienie};

typedef QMap<DopuszczalneNazwyZmiennych,QVector<double>> QMapaDanych;

class PetlaRegulacji;
class QString;
class ObiektDyskretny;
class QKontrolerSymulacji : public QObject
{
    Q_OBJECT
public:
    //przechowuje nazwy akceptowane przez wyslijDaneObiektu i odbierzDaneObiektu
    //c/////////////////////////////////////////////////////////
    //c//////////      konstruktor     ////////////////////
    //c/////////////////////////////////////////////////////////

    explicit QKontrolerSymulacji(ObiektDyskretny* kontrolowany, PetlaRegulacji *petla, QObject *parent = 0);
    //c/////////////////////////////////////////////////////////
    //c//////////      destruktor     ////////////////////
    //c/////////////////////////////////////////////////////////
    ~QKontrolerSymulacji();

    //c/////////////////////////////////////////////////////////
    //c//////////      SYGNAŁY              ////////////////////
    //c/////////////////////////////////////////////////////////
    static QMapaDanych stworzQMapeDanych(const DopuszczalneNazwyZmiennych klucz, QVector<double> vec);
    static QMapaDanych stworzQMapeDanych(const DopuszczalneNazwyZmiennych klucz, double wart);
 signals:
    ///
    /// \brief symulacjaZakonczona - wysyłany jeżeli skończyła się sekwencja próbek podanych na wejście, lub jeżeli była ona pusta
    ///
    void symulacjaZakonczona();
    ///
    /// \brief wynikSymulacji - zwraca wyniki symulacji
    /// \param y - wyjścia z obiektu
    /// \param t - czas odpowiadający tym wyjściom
    /// \param u - wejścia które podano do układu
    ///
    void wynikSymulacji(QVector<double> y, QVector<double> t,QVector<double> u);

    ///
    /// \brief wyslijDaneObiektu
    /// \param m  - mapa indeksowana nazwami zmiennych. Parametry zwracane są w postaci QVectora double - jeżeli parametr jest pojedyńczą wartością, wówczas ustawiana jest tylko pierwsza wartość. Obecnie zaimplementowane nazwy to:
    /// -"m_wZadana" obecne wymuszenie.
    /// -"m_licznik1" - pierwszy licznik transmitancji obiektu
    /// -"m_licznik2" - drugi licznik transmitancji obiektu
    /// -"m_mianownik1" - pierwszy mianowinik transmitancji obiektu
    /// -"m_mianownik2" - drugi mianowinik transmitancji obiektu
    /// -"m_wariancja" - wariancja białego szumu obiektu
    /// -"m_dh" - okrs próbkowania obiektu
    /// -"m_delay" - opóźnienie z^-k w liczniku obiektu
    /// -"m_momentPrzelaczenia" - moment zmiany parametrów obiektu
    /// -"m_czas" - czas symulacji
    /// -"m_predkoscSymulacji" - o ile szybciej ma wykonywać się symulacja
    ///
    void wyslijDaneObiektu(QMapaDanych m);

    //c/////////////////////////////////////////////////////////
    //c//////////      SLOTY PUBLICZNE      ////////////////////
    //c/////////////////////////////////////////////////////////
public slots: 
    //kontrola symulacji
    ///
    /// \brief symulacjaCiaglaStart - rozpoczyna symulację ciągłą
    ///
    void symulacjaCiaglaStart();
    ///
    /// \brief symulacjaCiaglaStop - kończy symulacje ciągłą
    ///
    void symulacjaCiaglaStop();
    ///
    /// \brief symulacjaKrokowa - zatrzymuje symulację ciągłą, jeżeli była włączona i robi jeden krok symulacji
    ///
    void symulacjaKrokowa();
    ///
    /// \brief resetSymulacji - resetuje symulację - czyści historię wejść i stanów - NIE zmienia czasu symulacji.
    ///
    void resetSymulacji();

    //operacje na plikach
    ///
    /// \brief openFile - wczytuje dane obiektu/obiektów z pliku
    /// \param str - ścieżka do pliku
    ///
    void openFile(QString str);
    ///
    /// \brief saveFile - zapisuje dane obiektu/obiektów do pliku
    /// \param str - ścieżka do pliku
    ///
    void saveFile(QString str);

    ///
    /// \brief getParameters - slot pozwalający widokowi rządać wysłania parametrów obiektu i kontrolera symulacji. W przyszłości również regulatorów.
    ///
    void getParameters();
    ///
    /// \brief odbierzDaneObiektu
    /// \param m - mapa indeksowana nazwami zmiennych. Parametry należy przekazać w postaci QVectora double - jeżeli parametr jest pojedyńczą wartością, wówczas brana jest tylko pierwsza wartość. Obecnie zaimplementowane nazwy to:
    /// -"m_wZadana" - wymuszenie. Jeżeli przekazany jest wektor dłuższy niz 1 element trakotwany jest on jako pełny zestaw próbek do symulacji. Jeżeli podana jest tylko jedna wartość traktowana jest ona jako skok jednostkowy.
    /// -"m_licznik1" - pierwszy licznik transmitancji obiektu
    /// -"m_licznik2" - drugi licznik transmitancji obiektu
    /// -"m_mianownik1" - pierwszy mianowinik transmitancji obiektu
    /// -"m_mianownik2" - drugi mianowinik transmitancji obiektu
    /// -"m_wariancja" - wariancja białego szumu obiektu
    /// -"m_dh" - okrs próbkowania obiektu
    /// -"m_delay" - opóźnienie z^-k w liczniku obiektu
    /// -"m_momentPrzelaczenia" - moment zmiany parametrów obiektu
    /// -"m_czas" - czas symulacji
    /// -"m_predkoscSymulacji" - o ile szybciej ma wykonywać się symulacja
    void odbierzDaneObiektu(QMapaDanych m);
private slots:
    void onTimeout();
    //wysyła sygnał wynikiSymulacji z m_histU, m_histY, m_histT oraz czyści te bufory
    void m_wyslijWynikiSymulacji();

private:
    void setDt(double dt);
    void setPredkoscSymulacji(int prSym);
    QKontrolerSymulacji(){}
    //wymuszenie
    //timer do odmierzania czasu symulacji
    QTimer m_timer;
    //timer do odświerzania obrazu
    QTimer m_refreshTimer;
    //okres odświeżania w milisekundach
    int m_okresOdswiezania = 100;
    //czy symulator jest w trybie pracy ciągłej, czy w trybie pracy krokowej
    bool m_symulacjaCiagla = false;
    double m_dt=0.1;
    int m_predkoscSymulacji = 1;

    ObiektDyskretny *m_ob=NULL;

    PetlaRegulacji *m_petla = NULL;

    //wektory do przechowywania punktów pomiędzy odświeżeniami wykresu/wysłaniami sygnału  wynikiSymulacji
    QVector<double> m_histU;
    QVector<double> m_histY;
    QVector<double> m_histT;
    //wymuszenie, jeżeli u.size = 1 przyjmujemy, że ma być podtrzymywane - nieskończenie wiele próbek
    QVector<double> m_u;
    //jeżeli m_u ma więcej niż jeden element wówczas zmienna m_licznikProbek służy do określania która probka wejścia ma być podana na obiekt
    int m_licznikProbek=0;

    //włąściwa funkcja wykonująca symulację
    void m_symuluj();

    //tworzenie mapy danych do przesłania do gui
    void zbierzDaneDoprzeslania();

    //const static std::set<QString> m_dopuszczalneNazwyZmiennych = {"m_wZadana","m_licznik1","m_licznik2","m_mianownik1","m_mianownik2","m_wariancja","m_dh","m_delay","m_momentPrzelaczenia","m_czas","m_przedkoscSymulacji"};
};
#endif // QKONTROLERSYMULACJI_H
