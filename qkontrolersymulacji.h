#ifndef QKONTROLERSYMULACJI_H
#define QKONTROLERSYMULACJI_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <vector>
#include <QVector>
#include <QMap>

class QString;
class ObiektDyskretny;
class QKontrolerSymulacji : public QObject
{
    Q_OBJECT
public:
    explicit QKontrolerSymulacji(ObiektDyskretny* kontrolowany,QObject *parent = 0);
    ~QKontrolerSymulacji();
signals:
    void symulacjaZakonczona();
    void wynikSymulacji(QVector<double> y, QVector<double> t,QVector<double> u);
    void wyslijDaneObiektu(QMap<QString,QVector<double>> m);
public slots:
    /*!
     * \brief setWymuszenie
     * \param u - wymusznie dla którego należy przeprowadzić symulację. Jeżeli przekazany zostanie wektor o długości 1 wówczas potraktowany on zostanie jako wartość skoku jednostkowego. Jeżeli przekazany zostanie dłuższy wektor wówczas zostanie on uznany za komplet próbek, dla których należy przeprowadzić symulację, po przeprowadzeniu symulacji dla wszystkich próbek symulator zatrzyma się. W razie prób uruchomienia dalszej symulacji wyemitowany zostanie sygnał symulacjaZakonczona();
     *
     */
    void setWymuszenie(std::vector<double> u);
    void setWielomianLicznika(std::vector<double> licz,int ktory,std::string id);
    void setWielomianMianownik(std::vector<double> licz,int ktory,std::string id);
    void setDt(double dt);
    void setPredkoscSymulacji(int prSym);
    void setWariancja(double wariancja);
    void symulacjaCiaglaStart();
    void symulacjaCiaglaStop();
    void symulacjaKrokowa();
    void resetSymulacji();
    void openFile(QString str);
    void saveFile(QString str);
    void getParameters();
private slots:
    void onTimeout();
    //wysyła sygnał wynikiSymulacji z m_histU, m_histY, m_histT oraz czyści te bufory
    void m_wyslijWynikiSymulacji();

private:
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

    //wektory do przechowywania punktów pomiędzy odświeżeniami wykresu/wysłaniami sygnału  wynikiSymulacji
    QVector<double> m_histU;
    QVector<double> m_histY;
    QVector<double> m_histT;
    //wymuszenie, jeżeli u.size = 1 przyjmujemy, że ma być podtrzymywane - nieskończenie wiele próbek
    std::vector<double> m_u;
    //jeżeli m_u ma więcej niż jeden element wówczas zmienna m_licznikProbek służy do określania która probka wejścia ma być podana na obiekt
    unsigned int m_licznikProbek=0;
    //włąściwa funkcja wykonująca symulację
    void m_symuluj();
    //tworzenie mapy danych do przesłania do gui
    void zbierzDaneDoprzeslania();
};

#endif // QKONTROLERSYMULACJI_H
