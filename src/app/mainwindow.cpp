#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "standardcalculator.h"
#include "scientificcalculator.h"
#include "programmercalculator.h"
#include "graphingcalculator.h"
#include "datecalculator.h"
#include "Themes.h"
#include "Icons.h"
#include "TintColor.h"

#include <QApplication>
#include <QIcon>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addHistory(const QString &entry)
{
    ui->listHistory->addItem(entry);
    ui->listHistory->scrollToBottom();
}

void MainWindow::addScientificHistory(const QString &entry)
{
    ui->listHistory->addItem(entry);
    ui->listHistory->scrollToBottom();
}

void MainWindow::addProgrammerHistory(const QString &entry)
{
    ui->listHistory->addItem(entry);
    ui->listHistory->scrollToBottom();
}

void MainWindow::applyTheme(int themeIndex)
{
    qApp->setStyleSheet(themeStyleSheet(themeIndex));
    applyModeIcons(themeIndex);
}

void MainWindow::init()
{
    this->setMinimumSize(600, 600);

    auto *standardCalc = new StandardCalculator(this);
    auto *scientificCalc = new ScientificCalculator(this);
    auto *programmerCalc = new ProgrammerCalculator(this);
    auto *graphinCalc = new GraphingCalculator(this);
    auto *dateCalc = new DateCalculator(this);

    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes({420, 180});

    ui->stackedWidget->addWidget(standardCalc);
    ui->stackedWidget->addWidget(scientificCalc);
    ui->stackedWidget->addWidget(programmerCalc);
    ui->stackedWidget->addWidget(graphinCalc);
    ui->stackedWidget->addWidget(dateCalc);


    ui->cbMode->addItem("Standard");
    ui->cbMode->addItem("Scientific");
    ui->cbMode->addItem("Programmer");
    ui->cbMode->addItem("Graphing");
    ui->cbMode->addItem("Date calculation");

    applyModeIcons(ui->cbTheme->currentIndex());


    ui->cbMode->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->cbMode, &QComboBox::currentIndexChanged, this, [this](int index)
    {
        ui->stackedWidget->setCurrentIndex(index);
    });

    // Themes
    ui->cbTheme->addItems({
        QStringLiteral("Light"),
        QStringLiteral("Dark"),
        QStringLiteral("Blue"),
        QStringLiteral("High contrast"),
        QStringLiteral("Silver"),
        QStringLiteral("Gold"),
        QStringLiteral("Ink light"),
        QStringLiteral("Ink blue"),
        QStringLiteral("Ink pure")

    });

    ui->cbTheme->setCurrentIndex(0);

    connect(ui->cbTheme, &QComboBox::currentIndexChanged, this, &MainWindow::applyTheme);

    applyTheme(ui->cbTheme->currentIndex());

    connect(standardCalc, &StandardCalculator::calculationCompleted, this, &MainWindow::addHistory);
    connect(scientificCalc, &ScientificCalculator::calculationCompleted, this, &MainWindow::addScientificHistory);
    connect(programmerCalc, &ProgrammerCalculator::calculationCompleted, this, &MainWindow::addProgrammerHistory);
}

QString MainWindow::themeStyleSheet(int themeIndex) const
{
    switch (themeIndex)
    {
    case 0:
        return Themes::light;
    case 1:
        return Themes::dark;
    case 2:
        return Themes::blue;
    case 4:
        return Themes::metallicSilver;
    case 5:
        return Themes::metallicGold;
    case 6:
        return Themes::inkLight;
    case 7:
        return Themes::inkBlue;
    case 8:
        return Themes::pureInk;
    case 3:
    default:
        return Themes::highContrast;
    }
}

void MainWindow::applyModeIcons(int themeIndex)
{
    const QColor iconColor = modeIconColor(themeIndex);

    ui->cbMode->setItemIcon(0, QIcon(TintColor::tintIcon(Icons::Standard, iconColor)));
    ui->cbMode->setItemIcon(1, QIcon(TintColor::tintIcon(Icons::Scientific, iconColor)));
    ui->cbMode->setItemIcon(2, QIcon(TintColor::tintIcon(Icons::Programmer, iconColor)));
    ui->cbMode->setItemIcon(3, QIcon(TintColor::tintIcon(Icons::Graphing, iconColor)));
    ui->cbMode->setItemIcon(4, QIcon(TintColor::tintIcon(Icons::Date, iconColor)));
}

QColor MainWindow::modeIconColor(int themeIndex) const
{
    switch (themeIndex)
    {
    case 0:  // Light
        return QColor(QStringLiteral("#2d3748"));
    case 1:  // Dark
        return QColor(QStringLiteral("#f0f0f0"));
    case 2:  // Blue
        return QColor(QStringLiteral("#2e69b1"));
    case 3:  // High contrast
        return QColor(QStringLiteral("#ffff00"));
    case 4:  // Silver
        return QColor(QStringLiteral("#6b7280")); //#6b7280 //#4d6588 //677688
    case 5:  // Gold
        return QColor(QStringLiteral("#4a3718"));
    case 6:  // Ink light
        return QColor(QStringLiteral("#111111"));
    case 7:  // Ink blue
        return QColor(QStringLiteral("#74b9ff"));
    case 8:  // Ink pure (black background)
        return QColor(QStringLiteral("#f0f0f0"));
    default:
        return QColor(QStringLiteral("#111111"));
    }
}
