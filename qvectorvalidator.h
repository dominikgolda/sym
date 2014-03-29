#ifndef QVECTORVALIDATOR_H
#define QVECTORVALIDATOR_H

#include <QValidator>

class QVectorValidator : public QValidator
{
    Q_OBJECT
public:
    explicit QVectorValidator(QObject *parent = 0);
    State validate(QString &input, int &) const;
signals:
    
public slots:
    
};

#endif // QVECTORVALIDATOR_H
