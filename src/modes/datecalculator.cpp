#include "datecalculator.h"
#include "ui_datecalculator.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QLocale>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>

DateCalculator::DateCalculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DateCalculator)
{
    ui->setupUi(this);

    init();
}

DateCalculator::~DateCalculator()
{
    delete ui;
}

void DateCalculator::init()
{
    connect(ui->cbDateMode, &QComboBox::currentIndexChanged, ui->swDateMode, &QStackedWidget::setCurrentIndex);
    connect(ui->btnDateCalculate, &QPushButton::clicked, this, &DateCalculator::calculate);

    const QDate today = QDate::currentDate();
    ui->deDateFrom->setDate(today.addYears(-1));
    ui->deDateTo->setDate(today);
    ui->deBaseDate->setDate(today);

    calculate();
}

void DateCalculator::calculate()
{
    if(ui->cbDateMode->currentIndex() == 0)
    {
        calculateDifference();
    }
    else
    {
        calculateAddSubtract();
    }
}

void DateCalculator::calculateDifference()
{
    QDate from = ui->deDateFrom->date();
    QDate to = ui->deDateTo->date();

    const bool swapped = from > to;

    if (swapped)
    {
        std::swap(from, to);
    }

    if (ui->chkIncludeEndDay->isChecked())
    {
        to = to.addDays(1);
    }

    qint64 totalDays = from.daysTo(to);


    QDate current = from;
    int years = 0;
    int months = 0;


    while (current.addYears(1) <= to)
    {
        current = current.addYears(1);
        years++;
    }

    while (current.addMonths(1) <= to)
    {
        current = current.addMonths(1);
        months++;
    }

    int days = current.daysTo(to);


    qint64 weeks = totalDays / 7;
    qint64 leftoverDays = totalDays % 7;

    QString result = QStringLiteral("%1 years, %2 months, %3 days\n").arg(years).arg(months).arg(days);

    result += QStringLiteral("Total: %1 days (%2 weeks and %3 days)").arg(totalDays).arg(weeks).arg(leftoverDays);

    if (swapped)
    {
        result.prepend(QStringLiteral("From date was after To date; the dates were swapped.\n"));
    }

    ui->lblDateResult->setText(result);
}

void DateCalculator::calculateAddSubtract()
{
    QDate base = ui->deBaseDate->date();

    int years = ui->spDateYears->value();
    int months = ui->spDateMonths->value();
    int days = ui->spDateDays->value();

    if (ui->cbDateOperation->currentIndex() == 1)
    {
        years = -years;
        months = -months;
        days = -days;
    }

    // Order: years / months / days
    // Same as Date calc windows has
    QDate result = base;
    result = result.addYears(years);
    result = result.addMonths(months);
    result = result.addDays(days);

    ui->lblDateResult->setText(QStringLiteral("Result: %1").arg(QLocale::system().toString(result, QLocale::LongFormat)));
}
