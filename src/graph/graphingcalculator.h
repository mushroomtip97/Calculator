#ifndef GRAPHINGCALCULATOR_H
#define GRAPHINGCALCULATOR_H

#include <QWidget>

class GraphScene;
class QEvent;
class QResizeEvent;
class QLabel;

namespace Ui {
class GraphingCalculator;
}

class GraphingCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit GraphingCalculator(QWidget *parent = nullptr);
    ~GraphingCalculator();

private slots:
    void addEquation();
    void removeSelected();
    void clearAll();
    void chooseLineColor();
    void updateZoomLabel(qreal pixelsPerUnit);
    void updateLineColorButton(int row);
private:
    Ui::GraphingCalculator *ui;
    GraphScene *m_scene = nullptr;
    QLabel *m_coordLabel = nullptr;
    bool m_isPanning = false;
    QPoint m_lastPanPos;

    void init();
    void positionCoordLabel();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;


};

#endif // GRAPHINGCALCULATOR_H
