#ifndef OBIEKTDYSKRETNY_H
#define OBIEKTDYSKRETNY_H
#include "obiektsiso.h"
#include <vector>
#include <deque>
#include <random>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"


class ObiektDyskretny : public ObiektSiso
{

public:
    ObiektDyskretny();
    ObiektDyskretny(
            const std::vector<double> &licznik1,
            const std::vector<double> &mianownik1,
            int delay=0,
            const std::vector<double> &licznik2 ={1,1},
            const std::vector<double> &mianownik2 = {1,1},
            int probkaPrzelaczenia=0,
            std::string id="");

    ~ObiektDyskretny();
    double symuluj(double u, double *czas=NULL);

    //################moje
        /*!
         * \brief Wczytuje dane obiektu z pliku.
         * \param sciezka - sciezka do obiektu lub nazwa pliku.
         * \param nazwa_obiektu - nazwa obiektu ktory chcemy wczytac, brak oznacza wczytanie pierwszego obiektu
         */
        void wczytajDane(std::string sciezka, std::string nazwa_obiektu);
        void wczytajDane(std::string sciezka);
        /*!
         * \brief Zapisuje dane obiektu do pliku.
         * \param sciezka - sciezka do obiektu lub nazwa pliku.
         * \param nazwa_obiektu - nazwa obiektu ktory chcemy zapisac, brak oznacza wczytanie pierwszego obiektu
         */
        void zapiszDane(std::string sciezka, std::string nazwa_obiektu);
    //########nie moje
    /*!
     * \brief setLicznik
     * \param dane -wektor z danymi dane(0) - b0 dane(N) - bN
     * \param ktory - wybiera, pomiędzy mianownikiem1, a mianownikiem2. Jeżeli ktory=1
     *wybrany jest mianownik 1, jeżeli ktory!=1 wybrany jest mianownik2.`
     * \param i nie używane w ObiektDyskretny, będzie implementowane w obiekcie
     *obsługującym połączenia ObiektówDyskretnych w celu wyboru obiektu, do którego
     *należy przypisać dane.
     *
     *Jeżeli funkcja setLicznik jest używana dla licznika, który jest obecnie stosowany do wyznaczaniaFunkcja setLicznik w razie wywołania z vektorem o długości innej niż obecnie używany zmienia długość kolejki próbek wejściowych m_histU. W przypadku skracania wektora usówane są ostatnie próbki. W przypadku wydłużania wektora na końcu dopisywane są zera.
     */
    void setLicznik(const std::vector<double> &dane, short ktory=1,std::string id=""/*unused*/);
    /*!
     * \brief setMianownik
     * \param dane -wektor z danymi dane(0) - b0 dane(N) - bN, jest przesówany
     *(move) do wektora wybranego przez ktory
     * \param ktory - wybiera, pomiędzy mianownikiem1, a mianownikiem2. Jeżeli ktory=1
     *wybrany jest mianownik 1, jeżeli ktory=2 wybrany jest mianownik2.`
     * \param i nie używane w ObiektDyskretny, będzie implementowane w obiekcie
     *obsługującym połączenia ObiektówDyskretnych w celu wyboru obiektu, do którego
     *należy przypisać dane.
     */
    void setMianownik(const std::vector<double> &dane, short ktory=1, std::string ="");
    /*!
     * \brief setVariancja
     * \param w - wartość wariancji, jaką należy ustawić dla obiektu
     * \param ktory_obiekt - nie wykorzystywany w klasie ObiektDyskretny, ale w
     *kompozycie @@@ jakim @@@, żeby określić który obiekt ma zostać zmieniony
     */
    void setWariancja(double w,std::string /*unused*/){m_wariancja=w;}

    virtual void resetujSymulacje();


    std::vector<double> getLicznik(short ktory, std::string);
    std::vector<double>  getMianownik(short ktory, std::string);
    double getWariancja(){return m_wariancja;}
    void setMomentPrzelaczenia(double t);
    void setDelay(int d){m_delay = d;}
    void setDt(double dt){m_dh = dt;}
    double getDt(){return m_dh;}
    std::string getId(){return m_id;}
    int getDelay() const {return m_delay;}
    //signals:
    //    void symulacjaZakonczona(std::pair<double,double> wynik);


    double czas() const;
    void setCzas(double czas);

    std::string id() const;
    void setId(const std::string &id);

private:
    //pierwszy zestaw parametrów
    std::vector<double> m_licznik1;
    std::vector<double> m_mianownik1;
    //drugi zestaw parametrów
    std::vector<double> m_licznik2;
    std::vector<double> m_mianownik2;
    //pojemniki na poprzednie wartości wyjścia i wejścia obiektu
    std::deque<double>m_histU;
    std::deque<double>m_histY;
    //moment zmiany z współczynników wielomianu. Drugi zestaw parametrów jest wykorzystywany od momentu kiedy m_probka==m_probkaPrzelaczenia
    int m_momentPrzelaczenia = 0;
    //z^-k z modelu obiektu
    int m_delay = 0;
    //czas symulacji
    double m_czas = 0;
    //krok symulacji
    double m_dh = 0.1;
    //wariancja białego szumu
    double m_wariancja =0;
    //wartość dla której uznajemy, że zmienna double ==0
    const double m_epsilon =0.000001;
    //zmienna służąca do wykrywania momentu przłączenia pomiędzy wielomianami 1, a wielomianami 2
    //tylko wtedy zachodzi konieczność sprawdzenia w funkcji symuluj, czy kolejki mają odpowiedznią długość
    bool m_zmianaNaWielomian2 = true;
    bool m_zmianaNaWielomian1 = true;
    std::string m_id = "";
    void dopasujDlugoscKolejkiY(const std::vector<double> &mianownik);
    void dopasujDlugoscKolejkiU(const std::vector<double> &licznik);
    //funkcje do debugowania
    void wypisz_wielomiany();
    void wypisz_kolejki();

    //losowanie
    std::random_device rd;
    std::mt19937 gen;

};

#endif // OBIEKTDYSKRETNY_H
