#include "klasaxml.h"

using namespace rapidxml;

KlasaXML::KlasaXML()
{
}
void KlasaXML::zapisz(std::list<std::string> lista_wyrazen, std::vector<std::vector<double>> lista_wartosci, std::string sciezka, std::string nazwa_obiektu){
    if(lista_wartosci.size() == lista_wartosci.size()){
        try{
            // utworzenie pliku
            std::ifstream file(sciezka);

            // utworzenie pustego xmla w pamieci
            xml_document<> doc;
            // przeniesienie obiektow które sa w pliku do dokumentu xml
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content(buffer.str());
            try{
                doc.parse<0>(&content[0]);
            }
            catch(...){
                doc.clear();
            }
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

            // tworzenie nowych node w obiekcie dla każdej nazwy.
            int numer=0;
            for(std::string& names : lista_wyrazen){
                //tworzenie nowego node w obiekcie
                xml_node<>* child = doc.allocate_node(node_element, names.c_str());
                // przypisanie node do odpowiedniego node (gałęzi do drzewa)
                root->append_node(child);
                // utworzenie kolejnych
                int i=0;
                for(double& value : lista_wartosci[numer]){
                    // Allocate string and copy name into it
                    char *node_name = doc.allocate_string(("a"+ std::to_string(i)).c_str());
                    // alokacja node
                    xml_node<>* child99 = doc.allocate_node(node_element, node_name);
                    // umieszczenie node w odpowiednim miejscu
                    child->append_node(child99);
                    char *node_name2 = doc.allocate_string(std::to_string(value).c_str());
                    // przypisanie wartosci
                    child99->value(node_name2);
                    ++i;
                }
                ++numer;
            }

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
    }else{
        //wyrzuc blad albo cos w tym stylu.
    }
}

std::vector<double> KlasaXML::wczytaj(std::string wyrazenie, std::string sciezka,std::string nazwa_obiektu){
    try{
        // utworzenie pliku
        std::ifstream file(sciezka);

        // utworzenie pustego xmla w pamieci
        xml_document<> doc;
        // przeniesienie obiektow które sa w pliku do dokumentu xml
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content(buffer.str());
        try{
            doc.parse<0>(&content[0]);
        }
        catch(...){
            doc.clear();
        }
        // wyjsciowy vector
        std::vector<double> wyjsciowy_vector;
        // sprawdzenie czy obiekt istnieje
        if (xml_node<> *objectnode = doc.first_node(nazwa_obiektu.c_str())){
            if (xml_node<> *correctnode = objectnode->first_node(wyrazenie.c_str())){
                for (xml_node<> *attr1 = correctnode->first_node(); attr1; attr1 = attr1->next_sibling())
                {
                    wyjsciowy_vector.push_back(std::stod( std::string(attr1->value()) ));
                }
            }
            return wyjsciowy_vector;
        }
        else{
            std::cout<<"nie ma takiego obiektu"<<std::endl;
            std::vector<double> a;
            return a;
        }
    }catch(...){
        std::cout << "nie mozna znalezc pliku" << std::endl;
        std::vector<double> a;
        return a;
    }
}
