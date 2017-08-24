#ifndef QSPINBOXINCREMENTAL_H
#define QSPINBOXINCREMENTAL_H

#include <QSpinBox>

class QSpinBoxIncremental : public QSpinBox
{
    Q_OBJECT
public:
    QSpinBoxIncremental(QWidget *parent = 0);

public slots:
    void increment();
    void decrement();
};

#endif // QSPINBOXINCREMENTAL_H
