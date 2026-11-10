#ifndef STANDARDCALCULATOR_H
#define STANDARDCALCULATOR_H

#include <QWidget>
#include <memory>
#include <QHash>

class ClcEngine;
class QPushButton;

namespace Ui {
class StandardCalculator;
}

class StandardCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit StandardCalculator(QWidget *parent = nullptr);
    ~StandardCalculator();

signals:
    void calculationCompleted(const QString &historyEntry);

private slots:
    void add();
    void subtract();
    void multiply();
    void divide();
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

private:
    Ui::StandardCalculator *ui;

    void init();
    void setGridLayout();
    void updateDisplay();

    std::shared_ptr<ClcEngine> m_engine;
    QHash<QString, QPushButton*> m_buttons;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // STANDARDCALCULATOR_H

