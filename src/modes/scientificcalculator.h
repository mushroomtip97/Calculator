#ifndef SCIENTIFICCALCULATOR_H
#define SCIENTIFICCALCULATOR_H

#include <QWidget>
#include <memory>
#include <QHash>

class ClcEngine;
class QPushButton;

namespace Ui {
class ScientificCalculator;
}

class ScientificCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit ScientificCalculator(QWidget *parent = nullptr);
    ~ScientificCalculator();

signals:
    void calculationCompleted(const QString &historyEntry);


private slots:
    void add();
    void subtract();
    void multiply();
    void divide();
    void power();
    void equals();
    void negate();
    void sqrt();
    void percent();
    void reciprocal();
    void square();
    void clearEntry();
    void clearAll();
    void backspace();
    void digitPressed();
    void sine();
    void cosine();
    void tangent();
    void log10();
    void ln();
    void pi();
    void euler();
    void exponental();
    void absoluteValue();
    void factorial();


private:
    Ui::ScientificCalculator *ui;
    std::shared_ptr<ClcEngine> m_engine;
    QHash<QString, QPushButton*> m_buttons;

    void init();
    void setGridLayout();
    void updateDisplay();


    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // SCIENTIFICCALCULATOR_H

