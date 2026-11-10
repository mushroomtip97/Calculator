#include <QtTest>

#include <cmath>

#include "clcengine.h"

// Tests for ClcEngine, used by both the Standard and
// Scientific calculator
class TestClcEngine : public QObject
{
    Q_OBJECT

private slots:
    void initialState();
    void inputBuildsNumber();
    void duplicateDecimalIsIgnored();
    void addition();
    void subtraction();
    void multiplication();
    void division();
    void divisionByZeroReturnsZero();
    void chainedOperators();
    void power();
    void percentWithPendingOperator();
    void clearAll();
    void clearEntry();
    void backspace();
    void square();
    void squareRoot();
    void squareRootNegativeIsInvalid();
    void reciprocal();
    void reciprocalZeroIsInvalid();
    void negate();
    void absoluteValue();
    void sine();
    void cosine();
    void tangent();
    void log10();
    void ln();
    void logInvalidInput();
    void pi();
    void euler();
    void exponental();
    void factorial();
    void factorialNegativeIsInvalid();
    void memoryStoreRecall();
    void memoryAddSubtractClear();
};

void TestClcEngine::initialState()
{
    ClcEngine engine;
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("0"));
    QCOMPARE(engine.getExpressionStr(), QString());
}

void TestClcEngine::inputBuildsNumber()
{
    ClcEngine engine;
    engine.inputDigit("1");
    engine.inputDigit("2");
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("12"));
}

void TestClcEngine::duplicateDecimalIsIgnored()
{
    ClcEngine engine;
    engine.inputDigit("1");
    engine.inputDigit(".");
    engine.inputDigit("2");
    engine.inputDigit(".");
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("1.2"));
}

void TestClcEngine::addition()
{
    ClcEngine engine;
    engine.inputDigit("20");
    engine.setOperator(ClcOperator::Add);
    engine.inputDigit("22");
    engine.calculate();
    QCOMPARE(engine.getDisplayStr().toDouble(), 42.0);
}

void TestClcEngine::subtraction()
{
    ClcEngine engine;
    engine.inputDigit("50");
    engine.setOperator(ClcOperator::Subtract);
    engine.inputDigit("8");
    engine.calculate();
    QCOMPARE(engine.getDisplayStr().toDouble(), 42.0);
}

void TestClcEngine::multiplication()
{
    ClcEngine engine;
    engine.inputDigit("6");
    engine.setOperator(ClcOperator::Multiply);
    engine.inputDigit("7");
    engine.calculate();
    QCOMPARE(engine.getDisplayStr().toDouble(), 42.0);
}

void TestClcEngine::division()
{
    ClcEngine engine;
    engine.inputDigit("84");
    engine.setOperator(ClcOperator::Divide);
    engine.inputDigit("2");
    engine.calculate();
    QCOMPARE(engine.getDisplayStr().toDouble(), 42.0);
}

void TestClcEngine::divisionByZeroReturnsZero()
{
    ClcEngine engine;
    engine.inputDigit("8");
    engine.setOperator(ClcOperator::Divide);
    engine.inputDigit("0");
    engine.calculate();

    // This is current, intentional behavior; keep the test in sync if
    // the engine is changed to report an error instead.
    QCOMPARE(engine.getDisplayStr().toDouble(), 0.0);
}

void TestClcEngine::chainedOperators()
{
    ClcEngine engine;
    engine.inputDigit("2");
    engine.setOperator(ClcOperator::Add);
    engine.inputDigit("3");
    engine.setOperator(ClcOperator::Multiply);
    engine.inputDigit("4");
    engine.calculate();

    QCOMPARE(engine.getDisplayStr().toDouble(), 20.0);
}

void TestClcEngine::power()
{
    ClcEngine engine;
    engine.inputDigit("2");
    engine.setOperator(ClcOperator::Power);
    engine.inputDigit("10");
    engine.calculate();

    QCOMPARE(engine.getDisplayStr().toDouble(), 1024.0);
}

void TestClcEngine::percentWithPendingOperator()
{
    ClcEngine engine;
    engine.inputDigit("200");
    engine.setOperator(ClcOperator::Add);
    engine.inputDigit("10");
    engine.percent();          // 10% of 200 = 20
    QCOMPARE(engine.getDisplayStr().toDouble(), 20.0);
    engine.calculate();        // 200 + 20 = 220
    QCOMPARE(engine.getDisplayStr().toDouble(), 220.0);
}

void TestClcEngine::clearAll()
{
    ClcEngine engine;
    engine.inputDigit("99");
    engine.setOperator(ClcOperator::Add);
    engine.clear();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("0"));
    QCOMPARE(engine.getExpressionStr(), QString());
}

void TestClcEngine::clearEntry()
{
    ClcEngine engine;
    engine.inputDigit("99");
    engine.clearEntry();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("0"));
}

void TestClcEngine::backspace()
{
    ClcEngine engine;
    engine.inputDigit("1");
    engine.inputDigit("2");
    engine.inputDigit("3");
    engine.backSpace();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("12"));

    engine.backSpace();
    engine.backSpace();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("0"));
}

void TestClcEngine::square()
{
    ClcEngine engine;
    engine.inputDigit("9");
    engine.square();
    QCOMPARE(engine.getDisplayStr().toDouble(), 81.0);
}

void TestClcEngine::squareRoot()
{
    ClcEngine engine;
    engine.inputDigit("81");
    engine.squareRoot();
    QCOMPARE(engine.getDisplayStr().toDouble(), 9.0);
}

void TestClcEngine::squareRootNegativeIsInvalid()
{
    ClcEngine engine;
    engine.inputDigit("9");
    engine.negate();
    engine.squareRoot();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("Invalid Input"));
}

void TestClcEngine::reciprocal()
{
    ClcEngine engine;
    engine.inputDigit("4");
    engine.reciprocal();
    QCOMPARE(engine.getDisplayStr().toDouble(), 0.25);
}

void TestClcEngine::reciprocalZeroIsInvalid()
{
    ClcEngine engine;
    engine.reciprocal();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("Cannot divide by zero"));
}

void TestClcEngine::negate()
{
    ClcEngine engine;
    engine.inputDigit("7");
    engine.negate();
    QCOMPARE(engine.getDisplayStr().toDouble(), -7.0);
}

void TestClcEngine::absoluteValue()
{
    ClcEngine engine;
    engine.inputDigit("12");
    engine.negate();
    engine.absoluteValue();
    QCOMPARE(engine.getDisplayStr().toDouble(), 12.0);
}

void TestClcEngine::sine()
{
    ClcEngine engine;
    engine.inputDigit("90");
    engine.sine();
    QVERIFY(std::abs(engine.getDisplayStr().toDouble() - 1.0) < 1e-9);
}

void TestClcEngine::cosine()
{
    ClcEngine engine;
    engine.inputDigit("0");
    engine.cosine();
    QVERIFY(std::abs(engine.getDisplayStr().toDouble() - 1.0) < 1e-9);
}

void TestClcEngine::tangent()
{
    ClcEngine engine;
    engine.inputDigit("45");
    engine.tangent();
    QVERIFY(std::abs(engine.getDisplayStr().toDouble() - 1.0) < 1e-9);
}

void TestClcEngine::log10()
{
    ClcEngine engine;
    engine.inputDigit("100");
    engine.log10();
    QCOMPARE(engine.getDisplayStr().toDouble(), 2.0);
}

void TestClcEngine::ln()
{
    ClcEngine engine;
    engine.euler(); // e
    engine.ln();
    QVERIFY(std::abs(engine.getDisplayStr().toDouble() - 1.0) < 1e-9);
}

void TestClcEngine::logInvalidInput()
{
    ClcEngine engine;
    engine.inputDigit("0");
    engine.log10();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("Invalid Input"));

    engine.clear();
    engine.inputDigit("5");
    engine.negate();
    engine.ln();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("Invalid Input"));
}

void TestClcEngine::pi()
{
    ClcEngine engine;
    engine.pi();
    QVERIFY(std::abs(engine.getDisplayStr().toDouble() - 3.14159265358979) < 1e-9);
}

void TestClcEngine::euler()
{
    ClcEngine engine;
    engine.euler();
    QVERIFY(std::abs(engine.getDisplayStr().toDouble() - 2.71828182845905) < 1e-9);
}

void TestClcEngine::exponental()
{
    ClcEngine engine;
    engine.inputDigit("2");
    engine.exponental(); // 10^2
    QCOMPARE(engine.getDisplayStr().toDouble(), 100.0);
}

void TestClcEngine::factorial()
{
    ClcEngine engine;
    engine.inputDigit("5");
    engine.factorial();
    QCOMPARE(engine.getDisplayStr().toDouble(), 120.0);
}

void TestClcEngine::factorialNegativeIsInvalid()
{
    ClcEngine engine;
    engine.inputDigit("5");
    engine.negate();
    engine.factorial();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("Invalid Input"));
}

void TestClcEngine::memoryStoreRecall()
{
    ClcEngine engine;
    engine.inputDigit("42");
    engine.memoryStore();
    engine.clear();
    engine.memoryRecall();
    QCOMPARE(engine.getDisplayStr().toDouble(), 42.0);
}

void TestClcEngine::memoryAddSubtractClear()
{
    ClcEngine engine;

    engine.inputDigit("10");
    engine.memoryStore();   // memory = 10
    engine.clearEntry();    // start a fresh input for the next number
    engine.inputDigit("5");
    engine.memoryAdd();     // memory = 15
    engine.clearEntry();
    engine.inputDigit("2");
    engine.memorySubtract(); // memory = 13
    engine.clear();
    engine.memoryRecall();
    QCOMPARE(engine.getDisplayStr().toDouble(), 13.0);

    engine.memoryClear();
    engine.clear();
    engine.memoryRecall();
    QCOMPARE(engine.getDisplayStr().toDouble(), 0.0);
}

QTEST_APPLESS_MAIN(TestClcEngine)
#include "tst_clcengine.moc"


