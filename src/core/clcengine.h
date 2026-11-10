#ifndef CLCENGINE_H
#define CLCENGINE_H

#include <QString>


//Engine for Standard and Scientific calculators

enum class ClcOperator{
    None,
    Add,
    Subtract,
    Multiply,
    Divide,
    Power
};

class ClcEngine
{
public:
    ClcEngine();

    void inputDigit(const QString &digit);
    void setOperator(ClcOperator op);
    void calculate();
    void clear();
    void clearEntry();
    void backSpace();
    void square();
    void squareRoot();
    void reciprocal();
    void negate();
    void percent();
    void sine();
    void cosine();
    void tangent();
    void log10();
    void ln();
    void pi();
    void euler();
    void exponental();
    void absoluteValue();
    void factorial();

    // Memory functions
    void memoryStore();
    void memoryClear();
    void memoryRecall();

    void memoryAdd();
    void memorySubtract();

    QString getDisplayStr()const {return m_currentInput;};
    QString getExpressionStr()const {return m_expressionString;};

    // Defaults
    static constexpr QUtf8StringView defExpressionStr = "";
    static constexpr double defCurrentVal = 0.0;
    static constexpr double defStoredVal = 0.0;
    static constexpr double defMemory = 0.0;
    static constexpr QUtf8StringView defCurrentInput = "0";

private:
    QString m_expressionString = defExpressionStr.toString(); // Gets the small top string
    double  m_currentVal = defCurrentVal;
    double m_storedVal = defStoredVal;
    double m_memory = defMemory; // For multi memory slots, replace it with a dynamic container
    ClcOperator m_pendingOperator = ClcOperator::None;
    bool m_startNewInput = true;
    QString m_currentInput = defCurrentInput.toString(); // Gets the big bottom number



    double evaluate(double a, double b, ClcOperator op);
    void applyUnary(double result);
};

#endif // CLCENGINE_H
