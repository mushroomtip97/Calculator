#include "programmercalculator.h"
#include "ui_programmercalculator.h"

#include "programmerengine.h"
#include "StringUtils.h"
#include "UiHelper.h"

#include <QPushButton>
#include <QComboBox>
#include <QFont>
#include <QKeyEvent>
#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>


ProgrammerCalculator::ProgrammerCalculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProgrammerCalculator)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);
    // ui->lblDisplay->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    // Same setup as standard/scientific
    ui->lblDisplay->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->lblExpression->setTextInteractionFlags(Qt::NoTextInteraction);

    ui->lblDisplay->setMaximumHeight(80);
    ui->lblExpression->setMaximumHeight(30);
    ui->lblMemoryIndicator->setVisible(false);

    ui->lblDisplay->setAlignment(Qt::AlignRight);
    ui->lblDisplay->setAlignment(Qt::AlignRight);

    m_engine = std::make_shared<ProgrammerEngine>();
    init();
    setGridLayout();
    updateDisplay();
}

ProgrammerCalculator::~ProgrammerCalculator()
{
    delete ui;
}

void ProgrammerCalculator::digitPressed()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if(!btn)
    {
        return;
    }
    m_engine->inputDigit(btn->text());
    updateDisplay();
}

void ProgrammerCalculator::add()
{
    m_engine->setOperator(ProgrammerOperator::Add);
    updateDisplay();
}

void ProgrammerCalculator::subtract()
{
    m_engine->setOperator(ProgrammerOperator::Subtract);
    updateDisplay();
}

void ProgrammerCalculator::multiply()
{
    m_engine->setOperator(ProgrammerOperator::Multiply);
    updateDisplay();
}

void ProgrammerCalculator::divide()
{
    m_engine->setOperator(ProgrammerOperator::Divide);
    updateDisplay();
}

void ProgrammerCalculator::mod()
{
    m_engine->setOperator(ProgrammerOperator::Modulus);
    updateDisplay();
}

void ProgrammerCalculator::equals()
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

void ProgrammerCalculator::andOp()
{
    m_engine->setOperator(ProgrammerOperator::And);
    updateDisplay();
}

void ProgrammerCalculator::orOp()
{
    m_engine->setOperator(ProgrammerOperator::Or);
    updateDisplay();
}

void ProgrammerCalculator::xorOp()
{
    m_engine->setOperator(ProgrammerOperator::Xor);
    updateDisplay();
}

void ProgrammerCalculator::notOp()
{
    m_engine->setOperator(ProgrammerOperator::Not);
    updateDisplay();
}

void ProgrammerCalculator::lshOp()
{
    m_engine->setOperator(ProgrammerOperator::Shl);
    updateDisplay();
}

void ProgrammerCalculator::rshOp()
{
    m_engine->setOperator(ProgrammerOperator::Shr);
    updateDisplay();
}

void ProgrammerCalculator::nandOp()
{
    m_engine->setOperator(ProgrammerOperator::Nand);
    updateDisplay();
}

void ProgrammerCalculator::norOp()
{
    m_engine->setOperator(ProgrammerOperator::Nor);
    updateDisplay();
}

void ProgrammerCalculator::setRadixHex()
{
    m_engine->setBase(ProgrammerEngine::Base::Hex);
    updateDisplay();

    // Enable A-F
    for (const QString &name : QStringList{"btnHexA", "btnHexB", "btnHexC", "btnHexD", "btnHexE", "btnHexF"})
    {
        m_buttons.value(name)->setEnabled(true);
    }
    // Enable 0-9
    for (int i = 0; i <= 9; ++i)
    {
        m_buttons.value(QString("btn%1").arg(i))->setEnabled(true);
    }
}

void ProgrammerCalculator::setRadixDec()
{
    m_engine->setBase(ProgrammerEngine::Base::Dec);
    updateDisplay();


    for (const QString &name : QStringList{"btnHexA", "btnHexB", "btnHexC", "btnHexD", "btnHexE", "btnHexF"})
    {
        m_buttons.value(name)->setEnabled(false);
    }

    for (int i = 0; i <= 9; ++i)
    {
        m_buttons.value(QString("btn%1").arg(i))->setEnabled(true);
    }
}

void ProgrammerCalculator::setRadixBin()
{
    m_engine->setBase(ProgrammerEngine::Base::Bin);
    updateDisplay();

    for (const QString &name : QStringList{"btnHexA", "btnHexB", "btnHexC", "btnHexD", "btnHexE", "btnHexF"})
    {
        m_buttons.value(name)->setEnabled(false);
    }

    for (int i = 0; i <= 9; ++i)
    {
        m_buttons.value(QString("btn%1").arg(i))->setEnabled(i <= 1);
    }
}

void ProgrammerCalculator::setRadixOct()
{
    m_engine->setBase(ProgrammerEngine::Base::Oct);
    updateDisplay();

    for (const QString &name : QStringList{"btnHexA", "btnHexB", "btnHexC", "btnHexD", "btnHexE", "btnHexF"})
    {
        m_buttons.value(name)->setEnabled(false);
    }

    for (int i = 0; i <= 9; ++i)
    {
        m_buttons.value(QString("btn%1").arg(i))->setEnabled(i <= 7);
    }
}

void ProgrammerCalculator::toggleSign()
{
    m_engine->applyNegate();
    updateDisplay();
}

void ProgrammerCalculator::backspace()
{
    m_engine->backspace();
    updateDisplay();
}

void ProgrammerCalculator::clearEntry()
{
    m_engine->clearEntry();
    updateDisplay();
}

void ProgrammerCalculator::clearAll()
{
    m_engine->clear();
    updateDisplay();
}

void ProgrammerCalculator::init()
{
    const QList<QList<QPair<QString, QString>>> layout = {
        { {"A", "btnHexA"},   {"B", "btnHexB"},   {"C", "btnHexC"},   {"D", "btnHexD"},   {"E", "btnHexE"},   {"F", "btnHexF"} },
        { {"<<", "btnLsh"},   {">>", "btnRsh"},   {"Or", "btnOr"},    {"Xor", "btnXor"},  {"Not", "btnNot"},  {"And", "btnAnd"} },
        { {"CE", "btnCE"},    {"C", "btnC"},      {"\u232B", "btnBack"},   {"\u00F7", "btnDivide"}, {"Mod", "btnMod"},  {"", "btnSpacer"} },
        { {"HEX", "btnHex"},  {"7", "btn7"},      {"8", "btn8"},      {"9", "btn9"},      {"\u00D7", "btnMultiply"}, {"NAND", "btnNand"} },
        { {"DEC", "btnDec"},  {"4", "btn4"},      {"5", "btn5"},      {"6", "btn6"},      {"\u2212", "btnMinus"},  {"NOR", "btnNor"} },
        { {"OCT", "btnOct"},  {"1", "btn1"},      {"2", "btn2"},      {"3", "btn3"},      {"+", "btnPlus"},   {"\u00B1", "btnPlusMinus"} },
        { {"BIN", "btnBin"},  {"0", "btn0"},      {".", "btnDot"},    {"=", "btnEquals"}, {"", "btnSpacer"},  {"", "btnSpacer"} }
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
        connect(m_buttons.value(QString("btn%1").arg(i)), &QPushButton::clicked, this, &ProgrammerCalculator::digitPressed);
    }
    connect(m_buttons.value("btnDot"), &QPushButton::clicked, this, &ProgrammerCalculator::digitPressed);
    // By default it should be disabled, integer only
    // Still many calculators still have it even in programmer mode
    m_buttons.value("btnDot")->setDisabled(true);

    // Basic Arithmetic Operators
    connect(m_buttons.value("btnPlus"), &QPushButton::clicked, this, &ProgrammerCalculator::add);
    connect(m_buttons.value("btnMinus"), &QPushButton::clicked, this, &ProgrammerCalculator::subtract);
    connect(m_buttons.value("btnMultiply"), &QPushButton::clicked, this, &ProgrammerCalculator::multiply);
    connect(m_buttons.value("btnDivide"), &QPushButton::clicked, this, &ProgrammerCalculator::divide);
    connect(m_buttons.value("btnMod"), &QPushButton::clicked, this, &ProgrammerCalculator::mod);

    // Bitwise Operators
    connect(m_buttons.value("btnAnd"), &QPushButton::clicked, this, &ProgrammerCalculator::andOp);
    connect(m_buttons.value("btnOr"), &QPushButton::clicked, this, &ProgrammerCalculator::orOp);
    connect(m_buttons.value("btnXor"), &QPushButton::clicked, this, &ProgrammerCalculator::xorOp);
    connect(m_buttons.value("btnLsh"), &QPushButton::clicked, this, &ProgrammerCalculator::lshOp);
    connect(m_buttons.value("btnRsh"), &QPushButton::clicked, this, &ProgrammerCalculator::rshOp);
    connect(m_buttons.value("btnNand"), &QPushButton::clicked, this, &ProgrammerCalculator::nandOp);
    connect(m_buttons.value("btnNor"), &QPushButton::clicked, this, &ProgrammerCalculator::norOp);

    // Unary Operators
    connect(m_buttons.value("btnNot"), &QPushButton::clicked, this, [this](){
        QString before = m_engine->getDisplayStr();
        m_engine->applyNot();
        QString after = m_engine->getDisplayStr();
        emit calculationCompleted("NOT " + before + " = " + after);
        updateDisplay();
    });
    connect(m_buttons.value("btnPlusMinus"), &QPushButton::clicked, this, [this](){
        QString before = m_engine->getDisplayStr();
        m_engine->applyNegate();
        QString after = m_engine->getDisplayStr();
        emit calculationCompleted("Â± " + before + " = " + after);
        updateDisplay();
    });

    // Radix Modes
    connect(m_buttons.value("btnHex"), &QPushButton::clicked, this, &ProgrammerCalculator::setRadixHex);
    connect(m_buttons.value("btnDec"), &QPushButton::clicked, this, &ProgrammerCalculator::setRadixDec);
    connect(m_buttons.value("btnOct"), &QPushButton::clicked, this, &ProgrammerCalculator::setRadixOct);
    connect(m_buttons.value("btnBin"), &QPushButton::clicked, this, &ProgrammerCalculator::setRadixBin);

    // Memory - Same implementation as the other two
    connect(m_buttons.value("btnMC"), &QPushButton::clicked, this, [this](){m_engine->memoryClear(); ui->lblMemoryIndicator->setVisible(false);});
    connect(m_buttons.value("btnMR"), &QPushButton::clicked, this, [this](){m_engine->memoryRecall(); updateDisplay();});
    connect(m_buttons.value("btnMPlus"), &QPushButton::clicked, this, [this](){m_engine->memoryAdd(); ui->lblMemoryIndicator->setVisible(true);});
    connect(m_buttons.value("btnM_Minus"), &QPushButton::clicked, this, [this](){m_engine->memorySubtract(); ui->lblMemoryIndicator->setVisible(true);});
    connect(m_buttons.value("btnMS"), &QPushButton::clicked, this, [this](){m_engine->memoryStore(); ui->lblMemoryIndicator->setVisible(true);});

    // Actions
    connect(m_buttons.value("btnEquals"), &QPushButton::clicked, this, &ProgrammerCalculator::equals);
    connect(m_buttons.value("btnC"), &QPushButton::clicked, this, &ProgrammerCalculator::clearAll);
    connect(m_buttons.value("btnCE"), &QPushButton::clicked, this, &ProgrammerCalculator::clearEntry);
    connect(m_buttons.value("btnBack"), &QPushButton::clicked, this, &ProgrammerCalculator::backspace);

    // Setting up Word Sizes combo box
    auto *cbWordSize = new QComboBox(this);
    QStringList words{"QWORD", "DWORD", "WORD", "BYTE"};
    cbWordSize->addItems(words);

    cbWordSize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cbWordSize->setFixedWidth(100);
    ui->verticalLayout->insertWidget(0, cbWordSize);

    connect(cbWordSize, &QComboBox::currentIndexChanged, this, [this](int index){
        switch(index)
        {
        case 0:
            m_engine->setWordSize(ProgrammerEngine::WordSize::Qword);
            break;
        case 1:
            m_engine->setWordSize(ProgrammerEngine::WordSize::Dword);
            break;
        case 2:
            m_engine->setWordSize(ProgrammerEngine::WordSize::Word);
            break;
        case 3:
            m_engine->setWordSize(ProgrammerEngine::WordSize::Byte);
            break;
        }
        updateDisplay();
    });

    // Live representations
    m_hexLabel = new QLabel(this);
    m_decLabel = new QLabel(this);
    m_octLabel = new QLabel(this);
    m_binLabel = new QLabel(this);

    m_hexLabel->setObjectName("lblHex");
    m_decLabel->setObjectName("lblDec");
    m_octLabel->setObjectName("lblOct");
    m_binLabel->setObjectName("lblBin");

    auto *radixLayout = new QVBoxLayout;
    radixLayout->addWidget(m_hexLabel);
    radixLayout->addWidget(m_decLabel);
    radixLayout->addWidget(m_octLabel);
    radixLayout->addWidget(m_binLabel);

    ui->verticalLayout->insertLayout(1, radixLayout);

    // Start in decimal mode
    setRadixDec();
}

void ProgrammerCalculator::setGridLayout()
{
    // Same as for the other two calcs
    ui->verticalLayout->setStretch(0, 0);
    ui->verticalLayout->setStretch(1, 0);
    ui->verticalLayout->setStretch(2, 0);
    ui->verticalLayout->setStretch(3, 1);
}

void ProgrammerCalculator::updateDisplay()
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

    // Live Radix Displays
    m_hexLabel->setText("HEX   " + m_engine->getStrInBase(ProgrammerEngine::Base::Hex));
    // The DEC line will always show the true decimal value with commas,
    // regardless of the currently selected input base.
    m_decLabel->setText("DEC   " + formatWithCommas(m_engine->getStrInBase(ProgrammerEngine::Base::Dec)));
    m_octLabel->setText("OCT   " + m_engine->getStrInBase(ProgrammerEngine::Base::Oct));
    m_binLabel->setText("BIN   " + m_engine->getStrInBase(ProgrammerEngine::Base::Bin));

}

void ProgrammerCalculator::keyPressEvent(QKeyEvent *event)
{
    // Numbers 0 - 9
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
    {
        m_engine->inputDigit(event->text());
        updateDisplay();
        return;
    }

    // Hex Letters A-F (Only if in Hex mode, the engine will ignore them otherwise)
    if (event->key() >= Qt::Key_A && event->key() <= Qt::Key_F)
    {
        m_engine->inputDigit(event->text());
        updateDisplay();
        return;
    }

    switch (event->key())
    {
    // Math
    case Qt::Key_Plus:
        add();
        updateDisplay();
        break;
    case Qt::Key_Minus:
        subtract();
        updateDisplay();
        break;
    case Qt::Key_Asterisk:
        multiply();
        updateDisplay();
        break;
    case Qt::Key_Slash:
        divide();
        updateDisplay();
        break;
    case Qt::Key_Percent:
        m_engine->setOperator(ProgrammerOperator::Modulus);
        updateDisplay();
        break;

    // Bitwise
    case Qt::Key_Ampersand:
        andOp();
        updateDisplay();
        break; // '&'
    case Qt::Key_Bar:
        orOp();
        updateDisplay();
        break;  // '|'
    case Qt::Key_AsciiCircum:
        xorOp();
        updateDisplay();
        break; // '^'
    case Qt::Key_Less:
        lshOp();
        updateDisplay();
        break; // '<'
    case Qt::Key_Greater:
        rshOp();
        updateDisplay();
        break; // '>'

    // Actions
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



