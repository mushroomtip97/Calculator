#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QColor>
#include <QList>
#include <QSize>

class QPainter;
class QRectF;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = nullptr);

    int equationCount() const { return m_equations.size(); }
    QString equationAt(int index) const;
    QColor equationColorAt(int index) const;

    bool addEquation(const QString &rawEquation, QString *errorMessage = nullptr);
    void removeEquation(int index);
    void clearEquations();
    void setEquationColor(int index, const QColor &color);
    void replotAll();

    // View/Navigation
    void setViewportSize(const QSize &viewPortSize);
    qreal pixelsPerUnit()const {return m_pixelsPerUnit;}

    void zoomIn();
    void zoomOut();
    void resetZoom();
    void zoomAt(const QPointF &sceneAnchor, qreal factor);

    void panBy(const QPointF &sceneDelta);
    void recenter();

    static constexpr const qreal kZoomFactor = 1.25;

signals:
    void pixelsPerUnitChanged(qreal pixelsPerUnit);

private:
    struct EquationEntry
    {
        QString expression;
        QColor color;
    };

    QList<EquationEntry> m_equations;
    qreal m_pixelsPerUnit = 50.0;
    QSize m_viewportSize;
    QPointF m_center;

    void setPixelsPerUnit(qreal pixelsPerUnit);
    void updateSceneRect();
    bool buildPath(const QString &equation, QPainterPath &path, QString *errorMessage);
    QColor nextAutoColor()const;
    void redrawAll();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
};

#endif // GRAPHSCENE_H
