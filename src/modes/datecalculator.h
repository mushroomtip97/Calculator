#ifndef DATECALCULATOR_H
#define DATECALCULATOR_H

#include <QWidget>

namespace Ui {
class DateCalculator;
}

class DateCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit DateCalculator(QWidget *parent = nullptr);
    ~DateCalculator();

private slots:
    void calculate();

private:
    Ui::DateCalculator *ui;

    void init();
    void calculateDifference();
    void calculateAddSubtract();
};

#endif // DATECALCULATOR_H
