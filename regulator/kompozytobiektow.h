#ifndef KOMPOZYTOBIEKTOW_H
#define KOMPOZYTOBIEKTOW_H
#include "obiektsiso.h"
#include <list>
class KompozytObiektow : public ObiektSiso
{
public:
    KompozytObiektow();
    double symuluj(double u, double *czas);
    void wczytajDane(std::string sciezka);
    void zapiszDane(std::string sciezka, std::string nazwa_obiektu);
    void resetujSymulacje();
    ///
    /// \brief dodajObiekt - wstawia nowy obiekt na listę
    /// \param obiekt - wskaźnik do obiektu do wstawienia
    /// \param gdzie - pozycja na którą należy wstawić nowy obiekt. Jeżeli podana zostanie liczba ujemna lub dodatnia, większa od długości wektora obiektów wskaźnik do obiektu zostanie wstawiony na końcu
    ///
    void dodajObiekt(ObiektSiso *obiekt,int gdzie=-1);

    ///
    /// \brief usunObiekt usówa element na pozycji "ktory" z listy. Jeżeli podana zostanie liczba ujemna lub większa od długości wektora obiektów zostanie usunięty ostatni element na liście. Jeżeli lista jest pusta nie robi nic.
    /// \param ktory - pozycja obiektu do usunięcia
    ///
    void usunObiekt(int ktory);


protected:
   std::vector<ObiektSiso*> m_obiekty;


};

#endif // KOMPOZYTOBIEKTOW_H
