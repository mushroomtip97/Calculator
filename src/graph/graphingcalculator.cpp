#include "graphingcalculator.h"
#include "ui_graphingcalculator.h"

#include "graphscene.h"
#include <QFrame>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QColorDialog>


GraphingCalculator::GraphingCalculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GraphingCalculator)
{
    ui->setupUi(this);

    init();
}

GraphingCalculator::~GraphingCalculator()
{
    delete ui;
}

void GraphingCalculator::init()
{
    m_scene = new GraphScene(this);

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setRenderHint(QPainter::TextAntialiasing);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setBackgroundBrush(Qt::white);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    m_scene->setViewportSize(ui->graphicsView->viewport()->size());

    // Small coordinate label.
    m_coordLabel = new QLabel(ui->graphicsView->viewport());
    m_coordLabel->setStyleSheet(QStringLiteral(
        "background-color: rgba(255, 255, 255, 220);"
        "color: #333333;"
        "border: 1px solid #c0c0c0;"
        "border-radius: 4px;"
        "padding: 3px 8px;"));
    m_coordLabel->setText(QStringLiteral("x: --, y: --"));
    m_coordLabel->setFixedWidth(150);
    m_coordLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_coordLabel->show();

    ui->graphicsView->viewport()->setMouseTracking(true);
    ui->graphicsView->viewport()->setCursor(Qt::OpenHandCursor);
    ui->graphicsView->viewport()->installEventFilter(this);

    // Equations
    connect(ui->lnEquation, &QLineEdit::returnPressed, this, &GraphingCalculator::addEquation);
    connect(ui->btnPlot, &QPushButton::clicked, this, &GraphingCalculator::addEquation);
    connect(ui->btnRemove, &QPushButton::clicked, this, &GraphingCalculator::removeSelected);
    connect(ui->btnClear, &QPushButton::clicked, this, &GraphingCalculator::clearAll);
    connect(ui->btnLineColor, &QPushButton::clicked, this, &GraphingCalculator::chooseLineColor);
    connect(ui->listEquations, &QListWidget::currentRowChanged,this, &GraphingCalculator::updateLineColorButton);

    // Zoom
    connect(ui->btnZoomIn, &QPushButton::clicked, m_scene, &GraphScene::zoomIn);
    connect(ui->btnZoomOut, &QPushButton::clicked, m_scene, &GraphScene::zoomOut);
    connect(ui->btnZoomReset, &QPushButton::clicked, m_scene, &GraphScene::resetZoom);
    connect(m_scene, &GraphScene::pixelsPerUnitChanged, this, &GraphingCalculator::updateZoomLabel);


    ui->lnEquation->setText(QStringLiteral("x^2"));
    addEquation();
    ui->lnEquation->clear();

    updateZoomLabel(m_scene->pixelsPerUnit());
    updateLineColorButton(ui->listEquations->currentRow());
    positionCoordLabel();

}

void GraphingCalculator::positionCoordLabel()
{
    if (!m_coordLabel || !ui->graphicsView->viewport())
    {
        return;
    }

    const QSize viewportSize = ui->graphicsView->viewport()->size();
    const int margin = 8;

    const int x = viewportSize.width() - m_coordLabel->width() - margin;
    const int y = viewportSize.height() - m_coordLabel->height() - margin;

    m_coordLabel->move(x, y);
    m_coordLabel->raise();
}

void GraphingCalculator::addEquation()
{
    const QString equation = ui->lnEquation->text().trimmed();

    if (equation.isEmpty())
    {
        ui->lblGraphStatus->setText(QStringLiteral("Type an equation first."));
        setStyleSheet(QString());
        return;
    }

    QString error;
    if (!m_scene->addEquation(equation, &error))
    {
        ui->lblGraphStatus->setText(QStringLiteral("Error: ") + error);
        setStyleSheet(QString());
        return;
    }

    const int row = m_scene->equationCount() - 1;
    auto *item = new QListWidgetItem(equation, ui->listEquations);
    item->setForeground(QBrush(m_scene->equationColorAt(row)));
    ui->listEquations->setCurrentRow(row);

    ui->lnEquation->clear();
    ui->lnEquation->setFocus();

    ui->lblGraphStatus->setText(QStringLiteral("%1 equation(s) plotted").arg(m_scene->equationCount()));
    setStyleSheet(QString());
}

void GraphingCalculator::removeSelected()
{
    const int row = ui->listEquations->currentRow();
    if (row < 0)
    {
        ui->lblGraphStatus->setText(QStringLiteral("Select an equation to remove."));
        setStyleSheet(QString());
        return;
    }

    m_scene->removeEquation(row);
    delete ui->listEquations->takeItem(row);

    ui->lblGraphStatus->setText(QStringLiteral("%1 equation(s) plotted").arg(m_scene->equationCount()));
    setStyleSheet(QString());

    updateLineColorButton(ui->listEquations->currentRow());
}

void GraphingCalculator::clearAll()
{
    m_scene->clearEquations();
    ui->listEquations->clear();

    ui->lblGraphStatus->setText(QStringLiteral("All equations cleared."));
    setStyleSheet(QString());

    updateLineColorButton(-1);
}

void GraphingCalculator::chooseLineColor()
{
    const int row = ui->listEquations->currentRow();
    if (row < 0 || row >= m_scene->equationCount())
    {
        ui->lblGraphStatus->setText(QStringLiteral("Select an equation first."));
        setStyleSheet(QString());
        return;
    }

    const QColor initial = m_scene->equationColorAt(row);
    const QColor chosen = QColorDialog::getColor(initial, this, QStringLiteral("Choose line color"));

    if (!chosen.isValid())
    {
        return;
    }

    m_scene->setEquationColor(row, chosen);

    if (QListWidgetItem *item = ui->listEquations->item(row))
    {
        item->setForeground(QBrush(chosen));
    }

    updateLineColorButton(row);
}

void GraphingCalculator::updateZoomLabel(qreal pixelsPerUnit)
{
    const int percent = qRound(pixelsPerUnit / 50.0 * 100.0);
    ui->lblZoom->setText(QStringLiteral("Zoom: %1%").arg(percent));
}

void GraphingCalculator::updateLineColorButton(int row)
{
    // Gray = no selection.
    QColor color(200, 200, 200);

    if (row >= 0 && row < m_scene->equationCount())
    {
        color = m_scene->equationColorAt(row);
    }

    const QString textColor = color.lightness() < 128 ? QStringLiteral("white") : QStringLiteral("#202020");

    ui->btnLineColor->setStyleSheet(QStringLiteral("background-color: %1;""color: %2;""border: 1px solid #808080;""padding: 4px 10px;").arg(color.name(), textColor));
}

void GraphingCalculator::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (!m_scene || !ui->graphicsView->viewport())
    {
        return;
    }

    m_scene->setViewportSize(ui->graphicsView->viewport()->size());
    m_scene->replotAll();
    positionCoordLabel();
}

bool GraphingCalculator::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != ui->graphicsView->viewport())
    {
        return QWidget::eventFilter(watched, event);
    }

    switch (event->type())
    {
    case QEvent::Wheel:
    {
        auto *wheelEvent = static_cast<QWheelEvent *>(event);

        const int delta = wheelEvent->angleDelta().y();
        if (delta == 0)
        {
            return false;
        }

        const QPoint viewportPos = wheelEvent->position().toPoint();
        const QPointF sceneAnchor = ui->graphicsView->mapToScene(viewportPos);

        if (delta > 0)
        {
            m_scene->zoomAt(sceneAnchor, GraphScene::kZoomFactor);
        }
        else
        {
            m_scene->zoomAt(sceneAnchor, 1.0 / GraphScene::kZoomFactor);
        }

        return true;
    }

    case QEvent::MouseButtonPress:
    {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            m_isPanning = true;
            m_lastPanPos = mouseEvent->position().toPoint();

            ui->graphicsView->viewport()->setCursor(Qt::ClosedHandCursor);
            ui->graphicsView->viewport()->grabMouse();
            return true;
        }

        return false;
    }

    case QEvent::MouseMove:
    {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        const QPoint current = mouseEvent->position().toPoint();

        const QPointF scenePos = ui->graphicsView->mapToScene(current);
        const qreal mathX = scenePos.x() / m_scene->pixelsPerUnit();
        const qreal mathY = -scenePos.y() / m_scene->pixelsPerUnit();

        m_coordLabel->setText(QStringLiteral("x: %1, y: %2").arg(QString::number(mathX, 'f', 2), QString::number(mathY, 'f', 2)));

        if (m_isPanning)
        {
            const QPointF sceneDelta =ui->graphicsView->mapToScene(current) - ui->graphicsView->mapToScene(m_lastPanPos);

            m_scene->panBy(QPointF(-sceneDelta.x(), -sceneDelta.y()));
            m_lastPanPos = current;
            return true;
        }

        return false;
    }

    case QEvent::MouseButtonRelease:
    {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton && m_isPanning)
        {
            m_isPanning = false;
            ui->graphicsView->viewport()->releaseMouse();
            ui->graphicsView->viewport()->setCursor(Qt::OpenHandCursor);
            return true;
        }

        return false;
    }

    case QEvent::MouseButtonDblClick:
    {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            m_scene->recenter();
            return true;
        }

        return false;
    }

    case QEvent::Leave:
    {
        m_coordLabel->setText(QStringLiteral("x: --, y: --"));
        return false;
    }

    default:
        break;
    }

    return QWidget::eventFilter(watched, event);
}