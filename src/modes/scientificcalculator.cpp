#include "scientificcalculator.h"
#include "ui_scientificcalculator.h"
#include "StringUtils.h"
#include "UiHelper.h"

#include <QPushButton>
#include <QFont>
#include <QKeyEvent>
#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include "clcengine.h"

ScientificCalculator::ScientificCalculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScientificCalculator)
{
    ui->setupUi(this);

    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    // ui->lblDisplay->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    ui->lblDisplay->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->lblExpression->setTextInteractionFlags(Qt::NoTextInteraction);

    ui->lblDisplay->setMaximumHeight(80);
    ui->lblExpression->setMaximumHeight(30);
    ui->lblMemoryIndicator->setVisible(false);

    ui->lblDisplay->setAlignment(Qt::AlignRight);
    ui->lblDisplay->setAlignment(Qt::AlignRight);

    m_engine = std::make_shared<ClcEngine>();
    init();
    setGridLayout();
    updateDisplay();
}

ScientificCalculator::~ScientificCalculator()
{
    delete ui;
}

void ScientificCalculator::add()
{
    m_engine->setOperator(ClcOperator::Add);
    updateDisplay();
}

void ScientificCalculator::subtract()
{
    m_engine->setOperator(ClcOperator::Subtract);
    updateDisplay();
}

void ScientificCalculator::multiply()
{
    m_engine->setOperator(ClcOperator::Multiply);
    updateDisplay();
}

void ScientificCalculator::divide()
{
    m_engine->setOperator(ClcOperator::Divide);
    updateDisplay();
}

void ScientificCalculator::power()
{
    m_engine->setOperator(ClcOperator::Power);
    updateDisplay();
}

void ScientificCalculator::equals()
{
    m_engine->calculate();
    QString expr = m_engine->getExpressionStr();
    if (!expr.isEmpty())
    {
        QString history = expr + " " + m_engine->getDisplayStr(); // "30 + 30 = 60"
        emit calculationCompleted(history);
    }
    updateDisplay();
}

void ScientificCalculator::negate()
{
    m_engine->negate();
    updateDisplay();
}

void ScientificCalculator::sqrt()
{
    m_engine->squareRoot();
    updateDisplay();
}

void ScientificCalculator::percent()
{
    m_engine->percent();
    updateDisplay();
}

void ScientificCalculator::reciprocal()
{
    m_engine->reciprocal();
    updateDisplay();
}

void ScientificCalculator::square()
{
    m_engine->square();
    updateDisplay();
}

void ScientificCalculator::clearEntry()
{
    m_engine->clearEntry();
    updateDisplay();
}

void ScientificCalculator::clearAll()
{
    m_engine->clear();
    updateDisplay();
}

void ScientificCalculator::backspace()
{
    m_engine->backSpace();
    updateDisplay();
}

void ScientificCalculator::digitPressed()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if(!btn)
    {
        return;
    }

    m_engine->inputDigit(btn->text());
    updateDisplay();
}

void ScientificCalculator::sine()
{
    m_engine->sine();
    updateDisplay();
}

void ScientificCalculator::cosine()
{
    m_engine->cosine();
    updateDisplay();
}

void ScientificCalculator::tangent()
{
    m_engine->tangent();
    updateDisplay();
}

void ScientificCalculator::log10()
{
    m_engine->log10();
    updateDisplay();
}

void ScientificCalculator::ln()
{
    m_engine->ln();
    updateDisplay();
}

void ScientificCalculator::pi()
{
    m_engine->pi();
    updateDisplay();
}

void ScientificCalculator::euler()
{
    m_engine->euler();
    updateDisplay();
}

void ScientificCalculator::exponental()
{
    m_engine->exponental();
    updateDisplay();
}

void ScientificCalculator::absoluteValue()
{
    m_engine->absoluteValue();
    updateDisplay();
}

void ScientificCalculator::factorial()
{
    m_engine->factorial();
    updateDisplay();
}

void ScientificCalculator::init()
{
    const QList<QList<QPair<QString, QString>>> layout = {
        { {"sin", "btnSin"}, {"cos", "btnCos"}, {"tan", "btnTan"}, {"C", "btnC"},   {"CE", "btnCE"} },
        { {"log", "btnLog"}, {"ln", "btnLn"},   {"x^y", "btnPow"}, {"⌫", "btnBack"}, {"÷", "btnDivide"} },
        { {"x²", "btnSquare"}, {"√x", "btnSqrt"}, {"1/x", "btnReciprocal"}, {"%", "btnPercent"}, {"×", "btnMultiply"} },
        { {"7", "btn7"},     {"8", "btn8"},     {"9", "btn9"},     {"π", "btnPi"},  {"−", "btnMinus"} },
        { {"4", "btn4"},     {"5", "btn5"},     {"6", "btn6"},     {"e", "btnE"},   {"+", "btnPlus"} },
        { {"1", "btn1"},     {"2", "btn2"},     {"3", "btn3"},     {"|x|", "btnAbs"}, {"=", "btnEquals"} },
        { {"±", "btnPlusMinus"}, {"0", "btn0"}, {".", "btnDot"},   {"10^x", "btnExp"}, {"n!", "btnFact"} },
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

    // Digits
    for (int i = 0; i <= 9; ++i)
    {
        connect(m_buttons.value(QString("btn%1").arg(i)), &QPushButton::clicked, this, &ScientificCalculator::digitPressed);
    }
    connect(m_buttons.value("btnDot"), &QPushButton::clicked, this, &ScientificCalculator::digitPressed);

    // Basic Operators
    connect(m_buttons.value("btnPlus"), &QPushButton::clicked, this, &ScientificCalculator::add);
    connect(m_buttons.value("btnMinus"), &QPushButton::clicked, this, &ScientificCalculator::subtract);
    connect(m_buttons.value("btnMultiply"), &QPushButton::clicked, this, &ScientificCalculator::multiply);
    connect(m_buttons.value("btnDivide"), &QPushButton::clicked, this, &ScientificCalculator::divide);
    connect(m_buttons.value("btnPow"), &QPushButton::clicked, this, &ScientificCalculator::power);

    // Unary Operators
    connect(m_buttons.value("btnPercent"), &QPushButton::clicked, this, &ScientificCalculator::percent);
    connect(m_buttons.value("btnSquare"), &QPushButton::clicked, this, &ScientificCalculator::square);
    connect(m_buttons.value("btnSqrt"), &QPushButton::clicked, this, &ScientificCalculator::sqrt);
    connect(m_buttons.value("btnReciprocal"), &QPushButton::clicked, this, &ScientificCalculator::reciprocal);
    connect(m_buttons.value("btnPlusMinus"), &QPushButton::clicked, this, &ScientificCalculator::negate);

    // Actions
    connect(m_buttons.value("btnEquals"), &QPushButton::clicked, this, &ScientificCalculator::equals);
    connect(m_buttons.value("btnC"), &QPushButton::clicked, this, &ScientificCalculator::clearAll);
    connect(m_buttons.value("btnCE"), &QPushButton::clicked, this, &ScientificCalculator::clearEntry);
    connect(m_buttons.value("btnBack"), &QPushButton::clicked, this, &ScientificCalculator::backspace);

    // Scientific
    connect(m_buttons.value("btnSin"), &QPushButton::clicked, this, &ScientificCalculator::sine);
    connect(m_buttons.value("btnCos"), &QPushButton::clicked, this, &ScientificCalculator::cosine);
    connect(m_buttons.value("btnTan"), &QPushButton::clicked, this, &ScientificCalculator::tangent);
    connect(m_buttons.value("btnLog"), &QPushButton::clicked, this, &ScientificCalculator::log10);

    connect(m_buttons.value("btnLn"), &QPushButton::clicked, this, &ScientificCalculator::ln);
    connect(m_buttons.value("btnPi"), &QPushButton::clicked, this, &ScientificCalculator::pi);
    connect(m_buttons.value("btnE"), &QPushButton::clicked, this, &ScientificCalculator::euler);
    connect(m_buttons.value("btnAbs"), &QPushButton::clicked, this, &ScientificCalculator::absoluteValue);
    connect(m_buttons.value("btnExp"), &QPushButton::clicked, this, &ScientificCalculator::exponental);
    connect(m_buttons.value("btnFact"), &QPushButton::clicked, this, &ScientificCalculator::factorial);

    // Memory
    connect(m_buttons.value("btnMC"), &QPushButton::clicked, this, [this](){m_engine->memoryClear(); ui->lblMemoryIndicator->setVisible(false);});
    connect(m_buttons.value("btnMR"), &QPushButton::clicked, this, [this](){m_engine->memoryRecall(); updateDisplay();});
    connect(m_buttons.value("btnMPlus"), &QPushButton::clicked, this, [this](){m_engine->memoryAdd(); ui->lblMemoryIndicator->setVisible(true);});
    connect(m_buttons.value("btnM_Minus"), &QPushButton::clicked, this, [this](){m_engine->memorySubtract(); ui->lblMemoryIndicator->setVisible(true);});
    connect(m_buttons.value("btnMS"), &QPushButton::clicked, this, [this](){m_engine->memoryStore(); ui->lblMemoryIndicator->setVisible(true);});
}

void ScientificCalculator::setGridLayout()
{
    // Give the main grid the priority to expand (stretch factor 1)
    ui->verticalLayout->setStretch(0, 0); // Expression label (no stretch)
    ui->verticalLayout->setStretch(1, 0); // Display label (no stretch)
    ui->verticalLayout->setStretch(2, 0); // Memory row (no stretch)
    ui->verticalLayout->setStretch(3, 1); // Grid layout (takes all remaining space)
}

void ScientificCalculator::updateDisplay()
{
    QString rawText = m_engine->getDisplayStr();
    QString formattedText = formatWithCommas(rawText);

    ui->lblDisplay->setText(formattedText);
    ui->lblExpression->setText(m_engine->getExpressionStr());

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

void ScientificCalculator::keyPressEvent(QKeyEvent *event)
{
    // Shifted shorcuts
    // '+' and '*' require Shift on a normal keyboard, so they are
    // not in this block. They will safely fall through to the main switch.
    if (event->modifiers() & Qt::ShiftModifier)
    {
        switch (event->key())
        {
        case Qt::Key_E:
            euler();
            return;
        case Qt::Key_Backslash:
            absoluteValue();
            return;
        case Qt::Key_Exclam:
            factorial();
            return;
        case Qt::Key_At:
            sqrt();
            return;
        }
    }

    // Unmodified letter shorcuts
    // This ensures Ctrl+P, Alt+X, etc., don't trigger the calculator math.
    if (event->modifiers() == Qt::NoModifier)
    {
        switch (event->key())
        {
        case Qt::Key_L:
            log10();
            return;
        case Qt::Key_N:
            ln();
            return;
        case Qt::Key_P:
            pi();
            return;
        case Qt::Key_X:
            exponental();
            return;
        case Qt::Key_C:
            cosine();
            return;
        case Qt::Key_S:
            sine();
            return;
        case Qt::Key_T:
            tangent();
            return;
        }
    }

    // Numbers and decimals
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
    {
        m_engine->inputDigit(event->text());
        updateDisplay();
        return;
    }
    if (event->key() == Qt::Key_Period)
    {
        m_engine->inputDigit(".");
        updateDisplay();
        return;
    }

    // Standard operator and actions
    // + and * naturally use Shift so no need to check.
    switch (event->key())
    {
    case Qt::Key_Plus:
        add();
        break;
    case Qt::Key_Minus:
        subtract();
        break;
    case Qt::Key_Asterisk:
        multiply();
        return;
    case Qt::Key_Slash:
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

    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
