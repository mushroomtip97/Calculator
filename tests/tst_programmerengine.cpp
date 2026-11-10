#include <QtTest>

#include <limits>

#include "programmerengine.h"

class TestProgrammerEngine : public QObject
{
    Q_OBJECT

private slots:
    void initialState();
    void decimalInput();
    void hexInput();
    void binaryInput();
    void invalidDigitIsIgnored();
    void backspace();
    void decimalAddition();
    void hexAddition();
    void chainedOperators();
    void wordSizeByteTruncates();
    void wordSizeWordTruncates();
    void applyNotByte();
    void applyNegateByte();
    void staticBitwiseOperations();
    void staticShiftOperations();
    void staticRotateOperations();
    void divideAndModulusByZero();
    void memoryOperations();
};

void TestProgrammerEngine::initialState()
{
    ProgrammerEngine engine;
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("0"));
}

void TestProgrammerEngine::decimalInput()
{
    ProgrammerEngine engine;
    engine.inputDigit("1");
    engine.inputDigit("2");
    engine.inputDigit("3");
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("123"));
}

void TestProgrammerEngine::hexInput()
{
    ProgrammerEngine engine;
    engine.setBase(ProgrammerEngine::Base::Hex);
    engine.inputDigit("F");
    engine.inputDigit("F");
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("FF"));
}

void TestProgrammerEngine::binaryInput()
{
    ProgrammerEngine engine;
    engine.setBase(ProgrammerEngine::Base::Bin);
    engine.inputDigit("1");
    engine.inputDigit("0");
    engine.inputDigit("1");
    engine.inputDigit("0");
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("1010"));
}

void TestProgrammerEngine::invalidDigitIsIgnored()
{
    ProgrammerEngine engine;
    engine.setBase(ProgrammerEngine::Base::Bin);
    engine.inputDigit("1");
    engine.inputDigit("2"); // invalid in binary
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("1"));
}

void TestProgrammerEngine::backspace()
{
    ProgrammerEngine engine;
    engine.setBase(ProgrammerEngine::Base::Hex);
    engine.inputDigit("A");
    engine.inputDigit("B");
    engine.backspace();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("A"));
}

void TestProgrammerEngine::decimalAddition()
{
    ProgrammerEngine engine;
    engine.inputDigit("20");
    engine.setOperator(ProgrammerOperator::Add);
    engine.inputDigit("22");
    engine.calculate();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("42"));
}

void TestProgrammerEngine::hexAddition()
{
    ProgrammerEngine engine;
    engine.setBase(ProgrammerEngine::Base::Hex);
    engine.inputDigit("F");
    engine.setOperator(ProgrammerOperator::Add);
    engine.inputDigit("1");
    engine.calculate();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("10"));
}

void TestProgrammerEngine::chainedOperators()
{
    ProgrammerEngine engine;
    engine.inputDigit("2");
    engine.setOperator(ProgrammerOperator::Add);
    engine.inputDigit("3");
    engine.setOperator(ProgrammerOperator::Multiply); // no precedence: (2 + 3) * 4
    engine.inputDigit("4");
    engine.calculate();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("20"));
}

void TestProgrammerEngine::wordSizeByteTruncates()
{
    ProgrammerEngine engine;
    engine.setWordSize(ProgrammerEngine::WordSize::Byte);
    engine.inputDigit("3");
    engine.inputDigit("0");
    engine.inputDigit("0");
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("44")); // 300 & 0xFF
}

void TestProgrammerEngine::wordSizeWordTruncates()
{
    ProgrammerEngine engine;
    engine.setWordSize(ProgrammerEngine::WordSize::Word);
    engine.setBase(ProgrammerEngine::Base::Hex);
    engine.inputDigit("1");
    engine.inputDigit("0");
    engine.inputDigit("0");
    engine.inputDigit("0");
    engine.inputDigit("0");
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("0")); // 0x10000 & 0xFFFF
}

void TestProgrammerEngine::applyNotByte()
{
    ProgrammerEngine engine;
    engine.setWordSize(ProgrammerEngine::WordSize::Byte);
    engine.setBase(ProgrammerEngine::Base::Hex);
    engine.applyNot(); // ~0 & 0xFF
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("FF"));
}

void TestProgrammerEngine::applyNegateByte()
{
    ProgrammerEngine engine;
    engine.setWordSize(ProgrammerEngine::WordSize::Byte);
    engine.inputDigit("1");
    engine.applyNegate();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("255"));
}

void TestProgrammerEngine::staticBitwiseOperations()
{
    QCOMPARE(ProgrammerEngine::andOp(0xF0, 0x0F), quint64(0));
    QCOMPARE(ProgrammerEngine::orOp(0xF0, 0x0F), quint64(0xFF));
    QCOMPARE(ProgrammerEngine::xorOp(0xFF, 0x0F), quint64(0xF0));
    QCOMPARE(ProgrammerEngine::nanOp(0xF0, 0x0F), std::numeric_limits<quint64>::max());
    QCOMPARE(ProgrammerEngine::norOp(0x00, 0x00), std::numeric_limits<quint64>::max());
    QCOMPARE(ProgrammerEngine::notOp(0x00), std::numeric_limits<quint64>::max());
}

void TestProgrammerEngine::staticShiftOperations()
{
    QCOMPARE(ProgrammerEngine::shlOp(quint64(1), quint64(4)), quint64(16));
    QCOMPARE(ProgrammerEngine::shrOp(quint64(16), quint64(4)), quint64(1));
    QCOMPARE(ProgrammerEngine::shlOp(quint64(1), quint64(64)), quint64(0));
    QCOMPARE(ProgrammerEngine::shrOp(quint64(1), quint64(64)), quint64(0));
}

void TestProgrammerEngine::staticRotateOperations()
{
    QCOMPARE(ProgrammerEngine::rolOp(quint64(1), quint64(1)), quint64(2));
    QCOMPARE(ProgrammerEngine::rorOp(quint64(2), quint64(1)), quint64(1));
    QCOMPARE(ProgrammerEngine::rolOp(quint64(1), quint64(65)), quint64(2));
}

void TestProgrammerEngine::divideAndModulusByZero()
{
    QCOMPARE(ProgrammerEngine::divide(quint64(10), quint64(0)), quint64(0));
    QCOMPARE(ProgrammerEngine::modulus(quint64(10), quint64(0)), quint64(0));
}

void TestProgrammerEngine::memoryOperations()
{
    ProgrammerEngine engine;
    engine.setWordSize(ProgrammerEngine::WordSize::Byte);

    engine.inputDigit("10");
    engine.memoryStore();    // memory = 10
    engine.clearEntry();     // start a fresh input for the next number
    engine.inputDigit("5");
    engine.memoryAdd();      // memory = 15
    engine.clearEntry();
    engine.inputDigit("2");
    engine.memorySubtract(); // memory = 13
    engine.clear();
    engine.memoryRecall();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("13"));

    engine.memoryClear();
    engine.clear();
    engine.memoryRecall();
    QCOMPARE(engine.getDisplayStr(), QStringLiteral("0"));
}

QTEST_APPLESS_MAIN(TestProgrammerEngine)
#include "tst_programmerengine.moc"

