#ifndef GENERATORWARTOSCIZADANEJ_H
#define GENERATORWARTOSCIZADANEJ_H
#include "dekorator/dekorator.h"
#include <vector>
#include "stale.h"
class GeneratorWartosciZadanej
{
public:
    GeneratorWartosciZadanej();
    GeneratorWartosciZadanej(const std::vector<TypyWymuszen> &w, const std::vector<std::vector<double> > &param);

    void setWartoscZadana(const std::vector<TypyWymuszen> &w,const std::vector<std::vector<double>> &param);
    void appendWartoscZadana(const std::vector<TypyWymuszen> &w,const std::vector<std::vector<double>> &param);

    std::shared_ptr<Komponent> getWartoscZadana() const;

    //getery
    std::vector<TypyWymuszen> typyWymuszen() const;
    std::vector<std::vector<double> > parametryWymuszenia() const;

private:
    //wektor generatorów, jakie mają być złożone
    std::vector<TypyWymuszen> m_typyWymuszen;
    //wektor z wektorami parametrów dla generatorów
    std::vector<std::vector<double>> m_parametryWymuszenia;
    ///
    /// \brief stworzGenerator - tworzy obiekt typu wynikającego ze zmiennej jaki
    /// \param jaki - decyduje o tym jakiego typu obiekt zostanie stworzony
    /// \param param - wektor parametrów obiektu, jeżeli podane zostanie mniej elementów niż potrzebuje dany konstruktor wówczas użyte zostaną wartości domyślne. Jeżeli podane zostanie więcej parametrów dalsze parametry zostaną zignorowane.
    /// \return - shared_ptr do poprzedniego w kolejności składania generatora
    ///
    std::shared_ptr<Komponent>  stworzGenerator(TypyWymuszen jaki, std::vector<double> param, std::shared_ptr<Komponent> poprzedni) const;


};

#endif // GENERATORWARTOSCIZADANEJ_H
