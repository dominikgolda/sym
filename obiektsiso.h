#ifndef OBIEKTSISO_H
#define OBIEKTSISO_H

#include<string>
#include<vector>

class ObiektSiso
{

public:

    ObiektSiso();
    //czysto wirtualne
    virtual double symuluj(double u, double *czas) =0;
    virtual void wczytajDane(std::string sciezka)=0;
 //   virtual void setLicznik(const std::vector<double> &dane,short ktory,std::string id)=0;
  //  virtual void setMianownik(const std::vector<double> &dane,short ktory,std::string id)=0;
  //  virtual void setDt(double dt)=0;
    virtual void resetujSymulacje()=0;

    virtual double getDt()=0;
  //  virtual std::vector<double> getLicznik(short ktory,std::string id)=0;
  //  virtual std::vector<double>  getMianownik(short ktory,std::string id)=0;
    virtual std::string getId()=0;
    virtual void zapiszDane(std::string sciezka, std::string nazwa_obiektu)=0;
    //wirtualne
    virtual ~ObiektSiso();
 //   virtual void setWariancja(double w=0,std::string id="");
 //   virtual void setMomentPrzelaczenia(double t);

 //   virtual double getMomentPrzelaczenia(){return 0;}
 //   virtual double getTime(){return 0;}
};

#endif // OBIEKTSISO_H
