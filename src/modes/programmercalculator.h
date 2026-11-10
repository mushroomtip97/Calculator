#ifndef PROGRAMMERCALCULATOR_H
#define PROGRAMMERCALCULATOR_H

#include <QWidget>
#include <QtTypes>
#include <memory>
#include <QHash>




class ProgrammerEngine;
class QPushButton;
class QLabel;

namespace Ui {
class ProgrammerCalculator;
}

class ProgrammerCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit ProgrammerCalculator(QWidget *parent = nullptr);
    ~ProgrammerCalculator();


signals:
    void calculationCompleted(const QString &historyEntry);

private slots:
    void digitPressed();
    void add();
    void subtract();
    void multiply();
    void divide();
    void mod();
    void equals();

    // Bitwise
    void andOp();
    void orOp();
    void xorOp();
    void notOp();
    void lshOp();
    void rshOp();
    void nandOp();
    void norOp();

    // Radix / UI
    void setRadixHex();
    void setRadixDec();
    void setRadixOct();
    void setRadixBin();
    void toggleSign();

    // Deletion
    void backspace();
    void clearEntry();
    void clearAll();


private:
    Ui::ProgrammerCalculator *ui;
    std::shared_ptr<ProgrammerEngine> m_engine;
    QHash<QString, QPushButton *> m_buttons;
    QLabel *m_hexLabel = nullptr;
    QLabel *m_decLabel = nullptr;
    QLabel *m_octLabel = nullptr;
    QLabel *m_binLabel = nullptr;

    void init();
    void setGridLayout();
    void updateDisplay();


    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // PROGRAMMERCALCULATOR_H

