#include "obiektdyskretny.h"
#include <string>
#include <iostream>
#include <utility>
#include <functional>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include <math.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml;

ObiektDyskretny::ObiektDyskretny()
{

}

ObiektDyskretny::ObiektDyskretny(const std::vector<double> &licznik1,
                                 const std::vector<double> &mianownik1,
                                 int delay,
                                 const std::vector<double> &licznik2,
                                 const std::vector<double> &mianownik2,
                                 int probkaPrzelaczenia, std::string id)
    :m_histU(licznik1.size()+delay),m_histY(mianownik1.size()-1)
{
    using namespace std::placeholders;
    m_id = id;
    m_licznik1 =licznik1;
    m_mianownik1 = mianownik1;
    if(m_mianownik1.at(0)!=1){
        //od m_mianownik1.begin() + 1 ponieważ mianownik[0] jest wykożystywany jako argument do mnożenia
        std::transform(m_mianownik1.begin()+1, m_mianownik1.end(), m_mianownik1.begin()+1,std::bind(std::divides<double>(),_1,m_mianownik1.at(0)));
    }
    m_mianownik1.erase(m_mianownik1.begin(),m_mianownik1.begin()+1);
    m_delay = delay;
    m_momentPrzelaczenia = probkaPrzelaczenia;
    m_licznik2 = licznik2;
    m_mianownik2 = mianownik2;
    if(m_mianownik2.at(0)!=1){
        //od m_mianownik2.begin() + 1 ponieważ mianownik[0] jest wykożystywany jako argument do mnożenia
        std::transform(m_mianownik2.begin()+1, m_mianownik2.end(), m_mianownik2.begin()+1,std::bind(std::divides<double>(),_1,m_mianownik2.at(0)));
    }
    m_mianownik2.erase(m_mianownik2.begin(),m_mianownik2.begin()+1);


    wypisz_wielomiany();
}
double mod(double a, double N){
    double ret = a-N*floor(a/N);
    return ret;
}

ObiektDyskretny::~ObiektDyskretny()
{
    std::cout<<"destruktor obiektu dyskretnego"<<std::endl;
}
//s//////////////////             symuluj         ///////////////////////////////////////////////
double ObiektDyskretny::symuluj(double u){
    m_czas+=m_dh;
    double yNowe;

    //przełączenie parametrów nie występuje, w ogóle lub jeszcze nie występuje
    if(m_momentPrzelaczenia<m_epsilon || mod(m_czas,2*m_momentPrzelaczenia)<m_momentPrzelaczenia){
        if(m_zmianaNaWielomian1){
            m_zmianaNaWielomian1 = false;
            dopasujDlugoscKolejkiU(m_licznik1);
            dopasujDlugoscKolejkiY(m_mianownik1);
        }
        m_histU.pop_back();
        m_histU.push_front(u);
        yNowe =std::inner_product(m_licznik1.begin(),m_licznik1.end(),m_histU.begin(),0.0);
        yNowe -=std::inner_product(m_mianownik1.begin(),m_mianownik1.end(),m_histY.begin(),0.0);
    }
    else{
        //moment przełączenia parametrów. Sprawdzam, czy parametry nie wymagają zmiany długości kolejek
        if(m_zmianaNaWielomian2){
            m_zmianaNaWielomian2 = false;
            dopasujDlugoscKolejkiU(m_licznik2);
            dopasujDlugoscKolejkiY(m_mianownik2);
        }// Koniec sprawdzania długości kolejek.

        m_histU.pop_back();
        m_histU.push_front(u);
        //        yNowe = std::inner_product(m_licznik2.begin(),m_licznik2.end(),m_histU.begin(),0);
        //                std::inner_product(m_mianownik2.begin(),m_mianownik2.end(),m_histY.begin(),0);
        yNowe =std::inner_product(m_licznik2.begin(),m_licznik2.end(),m_histU.begin(),0.0);
        yNowe -=std::inner_product(m_mianownik2.begin(),m_mianownik2.end(),m_histY.begin(),0.0);
        wypisz_wielomiany();
    }
    //generacja szumu
    std::normal_distribution<> d(0,m_wariancja);
    //dodawanie szumu
    yNowe +=d(gen);
    //włożenie do kolejki
    m_histY.pop_back();
    m_histY.push_front(yNowe);
    return yNowe;
}


//s//////////////////       setLicznik         ///////////////////////////////////////////////
void ObiektDyskretny::setLicznik(const std::vector<double> &dane, short ktory){
    //czy należy zmienić m_licznik1, czy m_licznik2
    if(ktory==1){
        //sprawdzam, czy licznik1 jest aktualnie wykorzysywany do wyznaczania wartości wyjścia.
        if(m_momentPrzelaczenia<m_epsilon || m_momentPrzelaczenia>m_czas)
        {
            dopasujDlugoscKolejkiU(dane);
        }
        m_licznik1 = dane;
    }else{
        //sprawdzam, czy licznik2 jest aktualnie wykorzysywany do wyznaczania wartości wyjścia.
        if(m_momentPrzelaczenia>m_epsilon && m_momentPrzelaczenia<=m_czas)
        {
            dopasujDlugoscKolejkiU(dane);
        }
        m_licznik2 = dane;
    }
    wypisz_wielomiany();
}
//s//////////////////       setMianownik       ///////////////////////////////////////////////
void ObiektDyskretny::setMianownik(const std::vector<double> &dane, short ktory){
    using namespace std::placeholders;
    //czy należy zmienić m_mianownik1, czy m_mianownik2
    if(ktory==1){
        m_mianownik1 = dane;
        if(m_mianownik1.at(0)!=1){
            //od m_mianownik1.begin() + 1 ponieważ mianownik[0] jest wykożystywany jako argument do mnożenia
            std::transform(m_mianownik1.begin()+1, m_mianownik1.end(), m_mianownik1.begin()+1,std::bind(std::divides<double>(),_1,m_mianownik1.at(0)));
        }
        m_mianownik1.erase(m_mianownik1.begin(),m_mianownik1.begin()+1);
        //sprawdzam, czy licznik1 jest aktualnie wykorzysywany do wyznaczania wartości wyjścia.
        if(m_momentPrzelaczenia<m_epsilon || m_momentPrzelaczenia>m_czas){
            dopasujDlugoscKolejkiY(m_mianownik1);
        }
    }else{
        //sprawdzam, czy licznik2 jest aktualnie wykorzysywany do wyznaczania wartości wyjścia.
        m_mianownik2 = dane;
        if(m_mianownik2.at(0)!=1){
            //od m_mianownik1.begin() + 1 ponieważ mianownik[0] jest wykożystywany jako argument do mnożenia
            std::transform(m_mianownik2.begin()+1, m_mianownik2.end(), m_mianownik2.begin()+1,std::bind(std::divides<double>(),_1,m_mianownik2.at(0)));
        }
        m_mianownik2.erase(m_mianownik2.begin(),m_mianownik2.begin()+1);
        if(m_momentPrzelaczenia>m_epsilon && m_momentPrzelaczenia<=m_czas){
            dopasujDlugoscKolejkiY(m_mianownik2);
        }
    }
    wypisz_wielomiany();
}

//double zeruj(double){
//   return static_cast<double>(0);
//}

void ObiektDyskretny::resetujSymulacje()
{
    for(auto it=m_histU.begin();it!=m_histU.end();it++){
        *it = 0;
    }
    for(auto it=m_histY.begin();it!=m_histY.end();it++){
        *it = 0;
    }
}

std::vector<double> ObiektDyskretny::getLicznik(short ktory)
{
    std::vector<double> a;
    if(ktory==1){
        a = m_licznik1;
    }else{
        a = m_licznik2;
    }
    wypisz_wielomiany();
    return a;

}

std::vector<double> ObiektDyskretny::getMianownik(short ktory)
{
    std::vector<double> a;
    if(ktory==1){
        a = m_mianownik1;
    }else{

        a= m_mianownik2;
    }
    auto it = a.begin();
    a.insert(it,1.0);
    return a;
}

void ObiektDyskretny::setMomentPrzelaczenia(double t)
{
    m_momentPrzelaczenia = t;
    m_zmianaNaWielomian1 = true;
    m_zmianaNaWielomian2 = true;
}
double ObiektDyskretny::czas() const
{
    return m_czas;
}

void ObiektDyskretny::setCzas(double czas)
{
    m_czas = czas;
}

std::string ObiektDyskretny::id() const
{
    return m_id;
}

void ObiektDyskretny::setId(const std::string &id)
{
    m_id = id;
}

//##############################Maciek
// Convert string values into type T results.
// Returns false in case the conversion fails.
template <typename T>
bool getValueFromString( const char * value, T & result )
{
    std::istringstream iss( value );
    return !( iss >> result ).fail();
}
void ObiektDyskretny::wczytajDane(std::string sciezka, std::string nazwa_obiektu){
    try{
        // utworzenie pliku
        std::ifstream file(sciezka);

        // utworzenie pustego xmla w pamieci
        xml_document<> doc;
        // przeniesienie obiektow które sa w pliku do dokumentu xml
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content(buffer.str());
        doc.parse<0>(&content[0]);
        // sprawdzenie czy obiekt istnieje
        if (xml_node<> *rootnode1 = doc.first_node(nazwa_obiektu.c_str())){
            int i =0;
            for (xml_node<> *attr = rootnode1->first_node(); attr; attr = attr->next_sibling())
            {
                ++i;
                switch (i){
                case 1:
                    m_licznik1.clear();
                    for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                    {
                        m_licznik1.push_back(std::stod( std::string(attr1->value()) ));
                    }
                    break;
                case 2:
                    m_mianownik1.clear();
                    for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                    {
                        m_mianownik1.push_back(std::stod( std::string(attr1->value()) ));
                    }
                    break;
                case 3:
                    m_licznik2.clear();
                    for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                    {
                        m_licznik2.push_back(std::stod( std::string(attr1->value()) ));
                    }
                    break;
                case 4:
                    m_mianownik2.clear();
                    for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                    {
                        m_mianownik2.push_back(std::stod( std::string(attr1->value()) ));
                    }
                    break;
                case 5:
                    m_histU.clear();
                    for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                    {
                        m_histU.push_back(std::stod( std::string(attr1->value()) ));
                    }
                    break;
                case 6:
                    m_histY.clear();
                    for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                    {
                        m_histY.push_back(std::stod( std::string(attr1->value()) ));
                    }
                    break;
                case 7:
                    m_momentPrzelaczenia = std::stod( std::string(attr->value()));
                    break;
                case 8:
                    m_delay = std::stod( std::string(attr->value()));
                    break;
                case 9:
                    m_dh = std::stod( std::string(attr->value()));
                    break;
                case 10:
                    m_czas = std::stod( std::string(attr->value()));
                    break;
                case 11:
                    m_wariancja = std::stod( std::string(attr->value()));
                    break;
                case 12:
                    //m_epsilon = std::stod( std::string(attr->value()));
                    break;
                default:
                    std::cout<<"chyba to nie ten plik"  << std::endl;
                }

            }
        }
        // jeżeli obiekt nie istnieje oinformuj o tym
        else{
            std::cout << "obiekt nie istnieje" << std::endl;
        }
        doc.clear();
    }catch(...){
        std::cout << "nie mozna znalezc ani utworzyc pliku" << std::endl;
    }

    this->wypisz_wielomiany();
    this->wypisz_kolejki();
}

void ObiektDyskretny::wczytajDane(std::string sciezka){
    try{

        // utworzenie pustego xmla w pamieci
        xml_document<> doc;
        // utworzenie pliku
        std::ifstream file(sciezka);
        // przeniesienie obiektow które sa w pliku do dokumentu xml
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content(buffer.str());
        doc.parse<0>(&content[0]);
        // pobieranie adresu pierwszego obiektu
        xml_node<> *rootnode1 = doc.first_node();
        // licznik przy ktorym jestesmy node
        int i =0;
        for (xml_node<> *attr = rootnode1->first_node(); attr; attr = attr->next_sibling())
        {
            ++i;
            switch (i){
            case 1:
                m_licznik1.clear();
                for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                {
                    m_licznik1.push_back(std::stod( std::string(attr1->value()) ));
                }
                break;
            case 2:
                m_mianownik1.clear();
                for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                {
                    m_mianownik1.push_back(std::stod( std::string(attr1->value()) ));
                }
                break;
            case 3:
                m_licznik2.clear();
                for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                {
                    m_licznik2.push_back(std::stod( std::string(attr1->value()) ));
                }
                break;
            case 4:
                m_mianownik2.clear();
                for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                {
                    m_mianownik2.push_back(std::stod( std::string(attr1->value()) ));
                }
                break;
            case 5:
                m_histU.clear();
                for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                {
                    m_histU.push_back(std::stod( std::string(attr1->value()) ));
                }
                break;
            case 6:
                m_histY.clear();
                for (xml_node<> *attr1 = attr->first_node(); attr1; attr1 = attr1->next_sibling())
                {
                    m_histY.push_back(std::stod( std::string(attr1->value()) ));
                }
                break;
            case 7:
                m_momentPrzelaczenia = std::stod( std::string(attr->value()));
                break;
            case 8:
                m_delay = std::stod( std::string(attr->value()));
                break;
            case 9:
                m_czas = std::stod( std::string(attr->value()));
                break;
            case 10:
                m_dh = std::stod( std::string(attr->value()));
                break;
            case 11:
                m_wariancja = std::stod( std::string(attr->value()));
                break;
            case 12:
                //m_epsilon = std::stod( std::string(attr->value()));
                break;
            default:
                std::cout<<"chyba to nie ten plik"  << std::endl;
                //}

            }
        }
        doc.clear();
        //###########
        //        Dopisane przez Dominika
        m_zmianaNaWielomian1 = true;
        m_zmianaNaWielomian2 = true;
        //#############
    }catch(...){
        std::cout << "nie mozna znalezc ani utworzyc pliku" << std::endl;
    }

}

void ObiektDyskretny::zapiszDane(std::string sciezka, std::string nazwa_obiektu = "ObiektDyskretny"){
    try{

        // utworzenie pustego xmla w pamieci
        xml_document<> doc;
        // utworzenie pliku
        std::fstream file(sciezka);
        // przeniesienie obiektow które sa w pliku do dokumentu xml
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content(buffer.str());
        doc.parse<0>(&content[0]);

        // sprawdzenie czy obiekt istnieje
        if (xml_node<> *rootnode1 = doc.first_node(nazwa_obiektu.c_str())){
            // jeżeli istnieje to go usun
            rootnode1->remove_all_nodes();
            doc.remove_node(rootnode1);
        }

        // deklaracja zgodnosci
        xml_node<>* decl = doc.allocate_node(node_declaration);
        decl->append_attribute(doc.allocate_attribute("version", "1.0"));
        decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
        doc.append_node(decl);
        //tworzenie nowego obiektu
        xml_node<>* root = doc.allocate_node(node_element, nazwa_obiektu.c_str());
        //dodanie go do dokumentu
        doc.append_node(root);

        //tworzenie nowego node w obiekcie
        xml_node<>* child = doc.allocate_node(node_element, "m_licznik");
        // przypisanie node do odpowiedniego node (gałęzi do drzewa)
        root->append_node(child);
        // utworzenie kolejnych
        for(unsigned int i=0; i < m_licznik1.size(); ++i){
            // Allocate string and copy name into it
            char *node_name = doc.allocate_string(("a"+ std::to_string(i)).c_str());
            // alokacja node
            xml_node<>* child99 = doc.allocate_node(node_element, node_name,0,0,0);
            // umieszczenie node w odpowiednim miejscu
            child->append_node(child99);
            char *node_name2 = doc.allocate_string(std::to_string(m_licznik1[i]).c_str());
            // przypisanie wartosci
            child99->value(node_name2);
        }
        xml_node<>* child1 = doc.allocate_node(node_element, "m_mianownik");
        root->append_node(child1);
        for(unsigned int i=0; i < m_mianownik1.size(); ++i){
            char *node_name = doc.allocate_string(("b"+ std::to_string(i)).c_str());
            xml_node<>* child99 = doc.allocate_node(node_element, node_name);
            child1->append_node(child99);
            char *node_name2 = doc.allocate_string(std::to_string(m_mianownik1[i]).c_str());
            child99->value(node_name2);
        }
        xml_node<>* child2 = doc.allocate_node(node_element, "m_licznik2");
        root->append_node(child2);
        for(unsigned int i=0; i < m_licznik2.size(); ++i){
            char *node_name = doc.allocate_string(("a"+ std::to_string(i)).c_str());
            xml_node<>* child99 = doc.allocate_node(node_element, node_name);
            child2->append_node(child99);
            char *node_name2 = doc.allocate_string(std::to_string(m_licznik2[i]).c_str());
            child99->value(node_name2);
        }
        xml_node<>* child3 = doc.allocate_node(node_element, "m_mianownik2");
        root->append_node(child3);
        for(unsigned int i=0; i < m_mianownik2.size(); ++i){
            char *node_name = doc.allocate_string(("b"+ std::to_string(i)).c_str());
            xml_node<>* child99 = doc.allocate_node(node_element, node_name);
            child3->append_node(child99);
            char *node_name2 = doc.allocate_string(std::to_string(m_mianownik2[i]).c_str());
            child99->value(node_name2);
        }
        xml_node<>* child4 = doc.allocate_node(node_element, "m_histU");
        root->append_node(child4);
        for(unsigned int i=0; i < m_histU.size(); ++i){
            char *node_name = doc.allocate_string(("U"+ std::to_string(i)).c_str());
            xml_node<>* child99 = doc.allocate_node(node_element, node_name);
            child4->append_node(child99);
            char *node_name2 = doc.allocate_string(std::to_string(m_histU[i]).c_str());
            child99->value(node_name2);
        }
        xml_node<>* child5 = doc.allocate_node(node_element, "m_histY");
        root->append_node(child5);
        for(unsigned int i=0; i < m_histY.size(); ++i){
            char *node_name = doc.allocate_string(("Y"+ std::to_string(i)).c_str());
            xml_node<>* child99 = doc.allocate_node(node_element, node_name);
            child5->append_node(child99);
            char *node_name2 = doc.allocate_string(std::to_string(m_histY[i]).c_str());
            child99->value(node_name2);
        }
        // tworzenie nowego node i nadanie wartosci, po uprzednim zaalokowaniu nazwy
        char *node_name6 = doc.allocate_string(std::to_string(m_momentPrzelaczenia).c_str());
        xml_node<>* child6 = doc.allocate_node(node_element, "mPrzel", node_name6);
        //przypisanie do obiektu
        root->append_node(child6);
        char *node_name7 = doc.allocate_string(std::to_string(m_delay).c_str());
        xml_node<>* child7 = doc.allocate_node(node_element, "delay", node_name7);
        root->append_node(child7);
        char *node_name8 = doc.allocate_string(std::to_string(m_czas).c_str());
        xml_node<>* child8 = doc.allocate_node(node_element, "czas", node_name8);
        root->append_node(child8);
        char *node_name9 = doc.allocate_string(std::to_string(m_dh).c_str());
        xml_node<>* child9 = doc.allocate_node(node_element, "dh", node_name9);
        root->append_node(child9);
        char *node_name10 = doc.allocate_string(std::to_string(m_wariancja).c_str());
        xml_node<>* child10 = doc.allocate_node(node_element, "war", node_name10);
        root->append_node(child10);
        char *node_name11 = doc.allocate_string(std::to_string(m_epsilon).c_str());
        xml_node<>* child11 = doc.allocate_node(node_element, "epsilon", node_name11);
        root->append_node(child11);

        // Convert doc to string if needed
        std::string xml_as_string;
        rapidxml::print(std::back_inserter(xml_as_string), doc);

        // Save to file ==================================
        std::ofstream file_stored(sciezka);
        file_stored << doc;
        file_stored.close();
        doc.clear();
    }catch(...){
        std::cout << "nie mozna znalezc ani utworzyc pliku" << std::endl;
    }
}










//###########################koniec jego kompomitacji


//k//////////////////////////////////////////////////////////////////////////////////////////
//k//////////////////////                                                ////////////////////
//k/////////////////////           FUNKCJE PRYWATNE                     /////////////////////
//k////////////////////                                                //////////////////////
//k//////////////////////////////////////////////////////////////////////////////////////////
void ObiektDyskretny::dopasujDlugoscKolejkiY(const std::vector<double> &mianownik)
{
    //sprawdzam, czy kolejka wyjść jest odpowiedniej długości
    if( mianownik.size()!=m_histY.size()){
        //czy jest za krótka - dodaję zera na końcu
        if( mianownik.size()>m_histY.size()){
            while(mianownik.size()!=m_histY.size()){
                m_histY.push_back(0);
            }
        }
        //czy jest dłuższa -usówam koniec
        else{
            while(mianownik.size()!=m_histY.size()){
                m_histY.pop_back();
            }
        }
    }
}

void ObiektDyskretny::dopasujDlugoscKolejkiU(const std::vector<double> &licznik)
{
    //sprawdzam, czy kolejka wejść jest odpowiedniej długości
    if( (licznik.size()+m_delay)!=m_histU.size()){
        //czy jest za krótka - dodaję zera na końcu
        if( (licznik.size()+m_delay)>m_histU.size()){
            while((licznik.size()+m_delay)!=m_histU.size()){
                m_histU.push_back(0);
            }
        }
        //czy jest dłuższa -usówam koniec
        else{
            while((licznik.size()+m_delay)!=m_histU.size()){
                m_histU.pop_back();
            }
        }
    }
}



















//do debugowania
void ObiektDyskretny::wypisz_wielomiany(){
    std::cout<<"Licznik"<<std::endl;
    for(auto it =m_licznik1.begin();it!=m_licznik1.end();it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl<<"Mianownik"<<std::endl;
    for(auto it =m_mianownik1.begin();it!=m_mianownik1.end();it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;

    std::cout<<std::endl<<"Probki U"<<std::endl;
    for(auto it =m_histU.begin();it!=m_histU.end();it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
    std::cout<<std::endl<<"Probki Y"<<std::endl;
    for(auto it =m_histY.begin();it!=m_histY.end();it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;
}

void ObiektDyskretny::wypisz_kolejki()
{
    double pom=1;
    std::cout<<"Kolejka wyjść: ";
    for(unsigned int i =0;i<m_histY.size();i++){
        pom =m_histY.at(i);
        std::cout<<pom<<" ";
    }
    std::cout<<std::endl<<"Kolejka wejść: ";
    for(auto it=m_histU.begin();it!=m_histU.end();it++){
        pom =*it;
        std::cout<<pom<<" ";
    }
    std::cout<<std::endl;
}

