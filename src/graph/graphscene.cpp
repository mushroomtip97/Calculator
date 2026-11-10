#include "graphscene.h"

#include <QFont>
#include <QGraphicsTextItem>
#include <QJSEngine>
#include <QJSValue>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QRegularExpression>
#include <algorithm>
#include <cmath>
#include <utility>

GraphScene::GraphScene(QObject *parent)
    : QGraphicsScene{parent}
{}

namespace{

constexpr qreal kDefaultPixelsPerUnit = 50.0;
constexpr qreal kMinPixelsPerUnit = 5.0;
constexpr qreal kMaxPixelsPerUnit = 200.0;
const QList<QColor> kEquationPalette = {
    QColor(31, 111, 214),   // blue
    QColor(214, 69, 69),    // red
    QColor(46, 160, 67),    // green
    QColor(230, 145, 30),   // orange
    QColor(150, 80, 200),   // purple
    QColor(0, 150, 150),    // teal
    QColor(180, 90, 40),    // brown
    QColor(200, 60, 140),    // magenta
    QColor(0, 170, 220),    // cyan
    QColor(120, 170, 20),   // lime
    QColor(225, 105, 165),  // pink
    QColor(190, 145, 0),    // gold
    QColor(95, 105, 120),   // slate
    QColor(60, 90, 170)     // indigo
};

    QString toJavaScript(const QString &raw)
    {
        QString str = raw.trimmed();
        str.remove(QRegularExpression(QStringLiteral("^\\s*y\\s*=\\s*"), QRegularExpression::CaseInsensitiveOption));

        if(str.isEmpty())
        {
            return str;
        }

        str.replace(QStringLiteral("X"), QStringLiteral("x"));
        str.replace(QStringLiteral("Ã—"), QStringLiteral("*"));
        str.replace(QStringLiteral("Ã·"), QStringLiteral("/"));

        // Imlicit multiplication like: 2x, 2sin(x), 2(x+1), x(x+1), (x+1)(x-1)
        str.replace(QRegularExpression(QStringLiteral("(\\d)(?=[A-Za-z(])")), QStringLiteral("\\1*"));
        str.replace(QRegularExpression(QStringLiteral("(x)(?=\\()")), QStringLiteral("x*"));
        str.replace(QRegularExpression(QStringLiteral("(\\))(?=[A-Za-z0-9(])")), QStringLiteral("\\1*"));

        // The lookbehind will stop re-matching words
        // that were alreay translated
        const auto replaceWord = [&str](const QString &word, const QString &repl){
            const QString pattern = QStringLiteral("(?<![A-Za-z0-9_.])(%1)(?![A-Za-z0-9_])").arg(QRegularExpression::escape(word));
            str.replace(QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption), repl);
        };

        replaceWord(QStringLiteral("log10"), QStringLiteral("Math.log10"));
        replaceWord(QStringLiteral("asin"),  QStringLiteral("Math.asin"));
        replaceWord(QStringLiteral("acos"),  QStringLiteral("Math.acos"));
        replaceWord(QStringLiteral("atan"),  QStringLiteral("Math.atan"));
        replaceWord(QStringLiteral("sqrt"),  QStringLiteral("Math.sqrt"));
        replaceWord(QStringLiteral("cbrt"),  QStringLiteral("Math.cbrt"));
        replaceWord(QStringLiteral("abs"),   QStringLiteral("Math.abs"));
        replaceWord(QStringLiteral("sin"),   QStringLiteral("Math.sin"));
        replaceWord(QStringLiteral("cos"),   QStringLiteral("Math.cos"));
        replaceWord(QStringLiteral("tan"),   QStringLiteral("Math.tan"));
        replaceWord(QStringLiteral("exp"),   QStringLiteral("Math.exp"));
        replaceWord(QStringLiteral("floor"), QStringLiteral("Math.floor"));
        replaceWord(QStringLiteral("ceil"),  QStringLiteral("Math.ceil"));
        replaceWord(QStringLiteral("round"), QStringLiteral("Math.round"));
        replaceWord(QStringLiteral("ln"),    QStringLiteral("Math.log"));
        replaceWord(QStringLiteral("log"),   QStringLiteral("Math.log10"));
        replaceWord(QStringLiteral("pi"),    QStringLiteral("Math.PI"));
        replaceWord(QStringLiteral("e"),     QStringLiteral("Math.E"));

        str.replace(QStringLiteral("^"), QStringLiteral("**"));
        return str;
    }

    qreal majorGridUnits(qreal pixelsPerUnit)
    {
        const qreal targetPixels = 50.0;
        qreal neededUnits = targetPixels / pixelsPerUnit;
        const qreal niceSteps[] = {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0};

        for(qreal step : niceSteps)
        {
            if(step >= neededUnits - 1e-9)
            {
                return step;
            }
        }
        return 100.0;
    }

    QString axisLabel(qreal val)
    {
        const qreal rounded = std::round(val * 100.0) / 100.0;
        QString text = QString::number(rounded, 'f', 2);
        while (text.endsWith(QLatin1Char('0')))
        {
            text.chop(1);
        }
        if (text.endsWith(QLatin1Char('.')))
        {
            text.chop(1);
        }
        return text;
    }

}

void GraphScene::setViewportSize(const QSize &viewPortSize)
{
    m_viewportSize = QSize(std::max(200, viewPortSize.width()), std::max(200, viewPortSize.height()));
    updateSceneRect();
}

void GraphScene::updateSceneRect()
{
    const qreal width = m_viewportSize.width();
    const qreal height = m_viewportSize.height();

    setSceneRect(m_center.x() - width / 2.0, m_center.y() - height / 2.0, width, height);
}

// TODO: Cache the compiled QJSValue function for each equation.
// Recompiling on every pan/zoom frame seem fine for a few equations,
// but caching would probably make dragging and zooming smoother.
bool GraphScene::buildPath(const QString &equation, QPainterPath &path, QString *errorMessage)
{

    if (errorMessage)
    {
        errorMessage->clear();
    }

    const QString js = toJavaScript(equation);
    if (js.isEmpty())
    {
        if (errorMessage)
        {
            *errorMessage = QStringLiteral("The equation is empty.");
        }
        return false;
    }

    // Compile once, then call the function for every x sample.
    QJSEngine engine;
    const QJSValue function = engine.evaluate(QStringLiteral("(x) => (%1)").arg(js));
    if (function.isError())
    {
        if (errorMessage)
        {
            *errorMessage = function.toString();
        }
        return false;
    }
    if (!function.isCallable())
    {
        if (errorMessage)
        {
            *errorMessage = QStringLiteral("Could not build a function from that expression.");
        }
        return false;
    }

    bool firstPoint = true;
    bool drewAnyPoint = false;
    QString firstRuntimeError;

    const qreal unit = m_pixelsPerUnit;
    const qreal xMin = sceneRect().left() / unit;
    const qreal xMax = sceneRect().right() / unit;
    const qreal step = 1.0 / unit;
    const qreal top = sceneRect().top();
    const qreal bottom = sceneRect().bottom();
    const qreal offscreenMargin = unit * 2.0; //points may overshoot a little
    const qreal discontinuityJump = unit * 4.0;

    for (qreal mathX = xMin; mathX <= xMax + 1e-9; mathX += step)
    {
        QJSValueList args;
        args.append(QJSValue(static_cast<double>(mathX)));
        const QJSValue result = function.call(args);

        if (result.isError())
        {
            if (firstRuntimeError.isEmpty())
            {
                firstRuntimeError = result.toString();
            }
            firstPoint = true;
            continue;
        }
        if (!result.isNumber())
        {
            firstPoint = true;
            continue;
        }

        const qreal mathY = result.toNumber();
        if (!std::isfinite(mathY))
        {
            firstPoint = true;
            continue;
        }

        const qreal pixelX = mathX * unit;
        const qreal pixelY = -mathY * unit;

        if (pixelY < top - offscreenMargin || pixelY > bottom + offscreenMargin)
        {
            firstPoint = true;
            continue;
        }

        if (!firstPoint)
        {
            const QPointF lastPoint = path.currentPosition();
            if (std::abs(pixelY - lastPoint.y()) > discontinuityJump)
            {
                firstPoint = true;
            }
        }

        if (firstPoint)
        {
            path.moveTo(pixelX, pixelY);
        }
        else
        {
            path.lineTo(pixelX, pixelY);
        }

        firstPoint = false;
        drewAnyPoint = true;
    }

    if (!drewAnyPoint)
    {
        if (errorMessage)
        {
            *errorMessage = firstRuntimeError.isEmpty() ? QStringLiteral("No points could be plotted in the visible range.") : firstRuntimeError;
        }
        return false;
    }


    return true;
}

QString GraphScene::equationAt(int index) const
{
    if(index < 0 || index >= m_equations.size())
    {
        return QString();
    }
    return m_equations.at(index).expression;
}

QColor GraphScene::equationColorAt(int index) const
{
    if(index < 0 || index >= m_equations.size())
    {
        return QColor();
    }
    return m_equations.at(index).color;
}

QColor GraphScene::nextAutoColor() const
{
    for(const auto &c : kEquationPalette)
    {
        bool used = false;
        for(const auto &e : m_equations)
        {
            if(e.color == c)
            {
                used = true;
                break;
            }
        }
        if(!used)
        {
            return c;
        }
    }

    return kEquationPalette.at(m_equations.size() % kEquationPalette.size());
}

void GraphScene::redrawAll()
{
    clear();

    qreal legendY = sceneRect().top() + 8.0;

    for (const EquationEntry &e : std::as_const(m_equations))
    {
        QPainterPath path;
        QString error;
        if (!buildPath(e.expression, path, &error))
        {
            continue;
        }

        if (!path.isEmpty())
        {
            QPen curvePen(e.color);
            curvePen.setWidthF(2.4);
            curvePen.setCapStyle(Qt::RoundCap);
            curvePen.setJoinStyle(Qt::RoundJoin);
            curvePen.setCosmetic(true);
            addPath(path, curvePen);
        }

        // Legend entry in the equation color.
        QColor lgColor = e.color;
        if (lgColor.lightness() > 200)
        {
            lgColor = QColor(60, 60, 60);
        }

        auto *equationLabel = addText(QStringLiteral("y = ") + e.expression, QFont(QStringLiteral("Segoe UI"), 9, QFont::Bold));
        equationLabel->setDefaultTextColor(lgColor);
        equationLabel->setPos(sceneRect().left() + 12.0, legendY);
        equationLabel->setZValue(10.0);
        legendY += 18.0;
    }
}

bool GraphScene::addEquation(const QString &rawEquation, QString *errorMessage)
{
    QPainterPath path;
    if(!buildPath(rawEquation, path, errorMessage))
    {
        return false;
    }

    EquationEntry e;
    e.expression = rawEquation;
    e.color = nextAutoColor();
    m_equations.append(e);

    redrawAll();
    return true;
}

void GraphScene::removeEquation(int index)
{
    if(index < 0 || index >= m_equations.size())
    {
        return;
    }
    m_equations.removeAt(index);
    redrawAll();
}

void GraphScene::clearEquations()
{
    m_equations.clear();
    clear();
}

void GraphScene::setEquationColor(int index, const QColor &color)
{
    if(index < 0 || index >= m_equations.size() || !color.isValid())
    {
        return;
    }
    m_equations[index].color = color;
    redrawAll();
}

void GraphScene::replotAll()
{
    redrawAll();
}

void GraphScene::panBy(const QPointF &sceneDelta)
{
    m_center += sceneDelta;
    updateSceneRect();
    redrawAll();
}

void GraphScene::recenter()
{
    m_center = QPointF(0.0, 0.0);
    updateSceneRect();
    redrawAll();
}

void GraphScene::setPixelsPerUnit(qreal pixelsPerUnit)
{

    if (m_pixelsPerUnit > 0.0)
    {
        zoomAt(m_center, pixelsPerUnit / m_pixelsPerUnit);
    }
}

void GraphScene::zoomIn()
{
    zoomAt(m_center, kZoomFactor);
}

void GraphScene::zoomOut()
{
    zoomAt(m_center, 1.0 / kZoomFactor);
}

void GraphScene::resetZoom()
{
    setPixelsPerUnit(kDefaultPixelsPerUnit);
}

void GraphScene::zoomAt(const QPointF &sceneAnchor, qreal factor)
{
    if (factor <= 0.0)
    {
        return;
    }

    const qreal newPixelsPerUnit = std::clamp(m_pixelsPerUnit * factor, kMinPixelsPerUnit,kMaxPixelsPerUnit);
    const qreal actualFactor = newPixelsPerUnit / m_pixelsPerUnit;

    if (std::abs(actualFactor - 1.0) < 1e-6)
    {
        return;
    }

    m_center += (actualFactor - 1.0) * sceneAnchor;
    m_pixelsPerUnit = newPixelsPerUnit;

    updateSceneRect();
    redrawAll();

    emit pixelsPerUnitChanged(m_pixelsPerUnit);
}

void GraphScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QColor(255, 255, 255));

    const qreal unit = m_pixelsPerUnit;
    const qreal majorUnits = majorGridUnits(unit);
    const qreal minorUnits = majorUnits / 2.0;
    const qreal majorPx = unit * majorUnits;
    const qreal minorPx = unit * minorUnits;

    // Minor grid
    if (minorPx >= 8.0)
    {
        QPen minorPen(QColor(242, 242, 242));
        minorPen.setCosmetic(true);
        painter->setPen(minorPen);

        const int firstColumn = std::floor(rect.left() / minorPx);
        const int lastColumn = std::ceil(rect.right() / minorPx);
        for (int column = firstColumn; column <= lastColumn; ++column)
        {
            const qreal x = column * minorPx;
            painter->drawLine(QLineF(x, rect.top(), x, rect.bottom()));
        }

        const int firstRow = std::floor(rect.top() / minorPx);
        const int lastRow = std::ceil(rect.bottom() / minorPx);
        for (int row = firstRow; row <= lastRow; ++row)
        {
            const qreal y = row * minorPx;
            painter->drawLine(QLineF(rect.left(), y, rect.right(), y));
        }
    }

    // Major grid
    QPen majorPen(QColor(214, 214, 214));
    majorPen.setCosmetic(true);
    painter->setPen(majorPen);

    const int firstMajorColumn = std::floor(rect.left() / majorPx);
    const int lastMajorColumn = std::ceil(rect.right() / majorPx);
    for (int column = firstMajorColumn; column <= lastMajorColumn; ++column)
    {
        const qreal x = column * majorPx;
        if (std::abs(x) < 0.5)
        {
            continue;
        }
        painter->drawLine(QLineF(x, rect.top(), x, rect.bottom()));
    }

    const int firstMajorRow = std::floor(rect.top() / majorPx);
    const int lastMajorRow = std::ceil(rect.bottom() / majorPx);
    for (int row = firstMajorRow; row <= lastMajorRow; ++row)
    {
        const qreal y = row * majorPx;
        if (std::abs(y) < 0.5)
        {
            continue;
        }
        painter->drawLine(QLineF(rect.left(), y, rect.right(), y));
    }

    // Axes
    QPen axisPen(QColor(90, 90, 90));
    axisPen.setWidthF(1.4);
    axisPen.setCosmetic(true);
    painter->setPen(axisPen);
    painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));
    painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0));

    // Axis labels
    QFont labelFont(QStringLiteral("Segoe UI"), 8);
    painter->setFont(labelFont);
    painter->setPen(QColor(120, 120, 120));

    for (int column = firstMajorColumn; column <= lastMajorColumn; ++column)
    {
        const qreal x = column * majorPx;
        if (std::abs(x) < 0.5)
        {
            continue;
        }
        painter->drawText(QRectF(x + 5.0, 5.0, 70.0, 14.0),Qt::AlignLeft | Qt::AlignVCenter, axisLabel(x / unit));
    }

    for (int row = firstMajorRow; row <= lastMajorRow; ++row)
    {
        const qreal y = row * majorPx;
        if (std::abs(y) < 0.5)
        {
            continue;
        }
        painter->drawText(QRectF(-74.0, y - 7.0, 66.0, 14.0),Qt::AlignRight | Qt::AlignVCenter, axisLabel(-y / unit));
    }

    painter->drawText(QRectF(5.0, 5.0, 30.0, 14.0), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("0"));
}


