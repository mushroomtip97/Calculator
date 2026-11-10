#ifndef PROGRAMMERENGINE_H
#define PROGRAMMERENGINE_H

#include <QtTypes>
#include <QString>



enum class ProgrammerOperator : quint8
{
    None,       // No pending operation
    Add,        // +
    Subtract,   // -
    Multiply,   // *
    Divide,     // /
    Modulus,    // % (or Mod)

    And,        // &
    Or,         // |
    Nand,       // ~(a & b)
    Nor,        // ~(a | b)
    Xor,        // ^
    Not,        // ~ (Unary - executes immediately)

    Shl,        // << (Shift Left)
    Shr,        // >> (Shift Right)
    Rol,        // Rotate Left
    Ror         // Rotate Right
};

class ProgrammerEngine
{
public:
    ProgrammerEngine();

    // Defaults
    static constexpr quint64 defCurrentInput = Q_UINT64_C(0); //or 0ULL
    static constexpr quint64 defStoredVal = Q_UINT64_C(0);
    static constexpr const char *defExpressionString = "";
    static constexpr quint64 defMemory = Q_UINT64_C(0);

    // Unary
    static quint64 notOp(quint64 a);
    static quint64 twos_complement(quint64 a);
    static quint64 swap_endianness(quint64 a);

    // State and Interaction
    void inputDigit(const QString &digit);
    void setOperator(ProgrammerOperator op);
    void calculate();
    void clear();
    void clearEntry();
    void backspace();

    enum class Base {Bin = 2, Oct = 8, Dec = 10, Hex = 16};
    enum class WordSize {Byte = 8, Word = 16, Dword = 32, Qword = 64};

    void setBase(Base b) { m_base = b; }
    void setWordSize(WordSize size);

    QString getDisplayStr()const {return QString::number(m_currentInput, static_cast<int>(m_base)).toUpper();};
    QString getExpressionStr()const {return m_expressionString;};
    static QString getOperatorSymbol(ProgrammerOperator op);
    QString getStrInBase(Base b) const{ return QString::number(m_currentInput, static_cast<int>(b)).toUpper();}

    static quint64 add(quint64 a, quint64 b);
    static quint64 subtract(quint64 a, quint64 b);
    static quint64 multiply(quint64 a, quint64 b);
    static quint64 divide(quint64 a, quint64 b);
    static quint64 andOp(quint64 a, quint64 b);
    static quint64 orOp(quint64 a, quint64 b);
    static quint64 nanOp(quint64 a, quint64 b);
    static quint64 norOp(quint64 a, quint64 b);
    static quint64 xorOp(quint64 a, quint64 b);
    static quint64 shlOp(quint64 val, quint64 shiftAmount);
    static quint64 rolOp(quint64 val, quint64 shiftAmount);
    static quint64 modulus(quint64 a, quint64 b);
    static quint64 shrOp(quint64 val, quint64 shiftAmount);
    static quint64 rorOp(quint64 val, quint64 shiftAmount);

    // Unary operations
    void applyNot();
    void applyNegate();

    // Memory functions
    void memoryStore();
    void memoryClear();
    void memoryRecall();

    void memoryAdd();
    void memorySubtract();


private:
    quint64 m_currentInput = defCurrentInput;
    quint64 m_storedVal = defStoredVal;
    QString m_expressionString = defExpressionString;
    quint64 m_memory = defMemory;  // Same as for ClcEngine should be replaced with dynamic container for multiple memory slots

    ProgrammerOperator m_pendingOperator = ProgrammerOperator::None;
    bool m_startNewInput = true;

    Base m_base = Base::Dec;
    WordSize m_wordSize = WordSize::Qword;

    // Keeps numbers within the selected bit limit
    quint64 applyMask(quint64 val) const;
    quint64 evaluate(quint64 a, quint64 b, ProgrammerOperator op);

};

#endif // PROGRAMMERENGINE_H
