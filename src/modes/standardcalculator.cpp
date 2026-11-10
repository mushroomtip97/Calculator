#include "standardcalculator.h"
#include "ui_standardcalculator.h"

#include "clcengine.h"
#include "StringUtils.h"
#include "UiHelper.h"

#include <QPushButton>
#include <QFont>
#include <QKeyEvent>
#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>

StandardCalculator::StandardCalculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StandardCalculator)
{
    ui->setupUi(this);

    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    // ui->lblDisplay->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    ui->lblDisplay->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->lblExpression->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->lblMemoryIndicator->setVisible(false);

    ui->lblDisplay->setMaximumHeight(80);
    ui->lblExpression->setMaximumHeight(30);

    ui->lblDisplay->setAlignment(Qt::AlignRight);
    ui->lblDisplay->setAlignment(Qt::AlignRight);


    m_engine = std::make_shared<ClcEngine>();
    init();
    setGridLayout();
    updateDisplay();
}

StandardCalculator::~StandardCalculator()
{
    delete ui;
}

void StandardCalculator::add()
{
    m_engine->setOperator(ClcOperator::Add);
    updateDisplay();
}

void StandardCalculator::subtract()
{
    m_engine->setOperator(ClcOperator::Subtract);
    updateDisplay();
}

void StandardCalculator::multiply()
{
    m_engine->setOperator(ClcOperator::Multiply);
    updateDisplay();
}

void StandardCalculator::divide()
{
    m_engine->setOperator(ClcOperator::Divide);
    updateDisplay();
}

void StandardCalculator::equals()
{
    m_engine->calculate();
    QString expr = m_engine->getExpressionStr();
    if (!expr.isEmpty())
    {
        QString history = expr + " " + m_engine->getDisplayStr();
        emit calculationCompleted(history);
    }
    updateDisplay();
}

void StandardCalculator::negate()
{
    m_engine->negate();
    updateDisplay();
}

void StandardCalculator::sqrt()
{
    m_engine->squareRoot();
    updateDisplay();
}

void StandardCalculator::percent()
{
    m_engine->percent();
    updateDisplay();
}

void StandardCalculator::reciprocal()
{
    m_engine->reciprocal();
    updateDisplay();
}

void StandardCalculator::square()
{
    m_engine->square();
    updateDisplay();
}


void StandardCalculator::clearEntry()
{
    m_engine->clearEntry();
    updateDisplay();
}

void StandardCalculator::clearAll()
{
    m_engine->clear();
    updateDisplay();
}

void StandardCalculator::backspace()
{
    m_engine->backSpace();
    updateDisplay();
}

void StandardCalculator::digitPressed()
{
    auto *btn = qobject_cast<QPushButton*>(sender());
    if(!btn)
    {
        return;
    }
    m_engine->inputDigit(btn->text());
    updateDisplay();
}

void StandardCalculator::init()
{

    const QList<QList<QPair<QString, QString>>> layout = {
        { {"%", "btnPercent"}, {"CE", "btnCE"}, {"C", "btnC"}, {"⌫", "btnBack"} },
        { {"1/x", "btnReciprocal"}, {"x²", "btnSquare"}, {"√x", "btnSqrt"}, {"÷", "btnDivide"} },
        { {"7", "btn7"}, {"8", "btn8"}, {"9", "btn9"}, {"×", "btnMultiply"} },
        { {"4", "btn4"}, {"5", "btn5"}, {"6", "btn6"}, {"−", "btnMinus"} },
        { {"1", "btn1"}, {"2", "btn2"}, {"3", "btn3"}, {"+", "btnPlus"} },
        { {"±", "btnPlusMinus"}, {"0", "btn0"}, {".", "btnDot"}, {"=", "btnEquals"} },
    };

    for (int row = 0; row < layout.size(); ++row)
    {
        for (int col = 0; col < layout[row].size(); ++col)
        {
            QString text = layout[row][col].first;
            QString objName = layout[row][col].second;

            if (text.isEmpty()) continue;

            auto *btn = new QPushButton(text);
            btn->setObjectName(objName);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            ui->gridLayout->addWidget(btn, row, col);

            m_buttons.insert(objName, btn);
        }
    }

    auto memoryBtns = UiHelper::createMemoryButtons(this, ui->verticalLayout, 2);

    for (auto it = memoryBtns.begin(); it != memoryBtns.end(); ++it)
    {
        m_buttons.insert(it.key(), it.value());
    }

    for (int i = 0; i <= 9; ++i)
    {
        connect(m_buttons.value(QString("btn%1").arg(i)), &QPushButton::clicked, this, &StandardCalculator::digitPressed);
    }

    connect(m_buttons.value("btnDot"), &QPushButton::clicked, this, &StandardCalculator::digitPressed);

    // Basic Operators
    connect(m_buttons.value("btnPlus"), &QPushButton::clicked, this, &StandardCalculator::add);
    connect(m_buttons.value("btnMinus"), &QPushButton::clicked, this, &StandardCalculator::subtract);
    connect(m_buttons.value("btnMultiply"), &QPushButton::clicked, this, &StandardCalculator::multiply);
    connect(m_buttons.value("btnDivide"), &QPushButton::clicked, this, &StandardCalculator::divide);

    // Unary Operators
    connect(m_buttons.value("btnPercent"), &QPushButton::clicked, this, &StandardCalculator::percent);
    connect(m_buttons.value("btnSquare"), &QPushButton::clicked, this, &StandardCalculator::square);
    connect(m_buttons.value("btnSqrt"), &QPushButton::clicked, this, &StandardCalculator::sqrt);
    connect(m_buttons.value("btnReciprocal"), &QPushButton::clicked, this, &StandardCalculator::reciprocal);
    connect(m_buttons.value("btnPlusMinus"), &QPushButton::clicked, this, &StandardCalculator::negate);

    // Actions
    connect(m_buttons.value("btnEquals"), &QPushButton::clicked, this, &StandardCalculator::equals);
    connect(m_buttons.value("btnC"), &QPushButton::clicked, this, &StandardCalculator::clearAll);
    connect(m_buttons.value("btnCE"), &QPushButton::clicked, this, &StandardCalculator::clearEntry);
    connect(m_buttons.value("btnBack"), &QPushButton::clicked, this, &StandardCalculator::backspace);

    // Memory
    connect(m_buttons.value("btnMC"), &QPushButton::clicked, this, [this](){m_engine->memoryClear(); ui->lblMemoryIndicator->setVisible(false);});
    connect(m_buttons.value("btnMR"), &QPushButton::clicked, this, [this](){m_engine->memoryRecall(); updateDisplay();});
    connect(m_buttons.value("btnMPlus"), &QPushButton::clicked, this, [this](){m_engine->memoryAdd(); ui->lblMemoryIndicator->setVisible(true);});
    connect(m_buttons.value("btnM_Minus"), &QPushButton::clicked, this, [this](){m_engine->memorySubtract(); ui->lblMemoryIndicator->setVisible(true);});
    connect(m_buttons.value("btnMS"), &QPushButton::clicked, this, [this](){m_engine->memoryStore(); ui->lblMemoryIndicator->setVisible(true);});

}

void StandardCalculator::setGridLayout()
{
    // Give the main grid the priority to expand
    ui->verticalLayout->setStretch(0, 0); // Expression label (no stretch)
    ui->verticalLayout->setStretch(1, 0); // Display label (no stretch)
    ui->verticalLayout->setStretch(2, 0); // Memory row (no stretch)
    ui->verticalLayout->setStretch(3, 1); // Grid layout (takes all remaining space)
}

void StandardCalculator::updateDisplay()
{

    // Same comma formating will be included in scientific
    // For programmer its gonna be a bit different

    QString rawText = m_engine->getDisplayStr();

    // Format it with commas
    QString formattedText = formatWithCommas(rawText);

    ui->lblDisplay->setText(formattedText);
    ui->lblExpression->setText(m_engine->getExpressionStr());

    // Auto-shrink based on the RAW length (ignoring commas)
    QFont font = ui->lblDisplay->font();
    int length = rawText.length();

    if (length > 16)
    {
        font.setPointSize(20);
    }
    else if (length > 10)
    {
        font.setPointSize(28);
    }
    else
    {
        font.setPointSize(36);
    }
    ui->lblDisplay->setFont(font);


    if(length > 16)
    {
        font.setPointSize(20);
    }
    else if (length > 10)
    {
        font.setPointSize(28);
    }
    else
    {
        font.setPointSize(36);
    }

    ui->lblDisplay->setFont(font);
}

void StandardCalculator::keyPressEvent(QKeyEvent *event)
{
    if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
    {
        m_engine->inputDigit(event->text());
        updateDisplay();
        return;
    }

    switch(event->key())
    {
    case Qt::Key_Plus:
        add();
        break;

    case Qt::Key_Minus:
        subtract();
        break;

    case Qt::Key_Asterisk:
        multiply();
        break;

    case Qt::Key_Slash: // Shift+8 ussualy
        divide();
        break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        equals();
        break;

    case Qt::Key_Backspace:
        backspace();
        break;

    case Qt::Key_Escape:
    case Qt::Key_Delete:
        clearAll();
        break;

    case Qt::Key_Period:
        m_engine->inputDigit(".");
        updateDisplay();
        break;

    default:
        QWidget::keyPressEvent(event); // Passes to parent if not handled
        break;
    }
}



