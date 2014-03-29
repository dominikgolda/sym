#include "qvectorvalidator.h"

QVectorValidator::QVectorValidator(QObject *parent) :
    QValidator(parent)
{
}

QValidator::State QVectorValidator::validate(QString &input, int &) const
{
    QChar pom('-');
    for(auto it=input.begin();it!=input.end();++it){
        if( !(it->isNumber()) && !(it->isSpace()) && *it!=pom){
            return QValidator::Invalid;
        }
    }
    return QValidator::Acceptable;
}

