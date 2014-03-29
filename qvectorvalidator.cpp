#include "qvectorvalidator.h"

QVectorValidator::QVectorValidator(QObject *parent) :
    QValidator(parent)
{
}

QValidator::State QVectorValidator::validate(QString &input, int &) const
{
    QChar pom('-');
    QChar pom1('.');
    for(auto it=input.begin();it!=input.end();++it){
        if( !(it->isNumber()) && !(it->isSpace()) && *it!=pom &&  *it!=pom1){
            return QValidator::Invalid;
        }
    }
    return QValidator::Acceptable;
}

