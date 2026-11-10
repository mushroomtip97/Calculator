#include "clcengine.h"
#include <cmath>



namespace MathConstants
{
    constexpr double PI = 3.14159265358979323846;
    constexpr double E  = 2.71828182845904523536;
}

ClcEngine::ClcEngine() {}

void ClcEngine::inputDigit(const QString &digit)
{
    // Stops after 16 digits
    if (!m_startNewInput && m_currentInput.length() > 16)
    {
        return;
    }
    if(m_currentInput == "0" || m_startNewInput)
    {
        m_currentInput = digit;
        m_startNewInput = false;
    }
    else
    {
        if(digit == "." && m_currentInput.contains("."))
        {
            return;
        }
        m_currentInput += digit;
    }
}

void ClcEngine::setOperator(ClcOperator op)
{
    double currentVal = m_currentInput.toDouble();

    // If there is already a pending operator, calculate the intermediate result
    if (m_pendingOperator != ClcOperator::None && !m_startNewInput)
    {
        m_currentVal = evaluate(m_storedVal, currentVal, m_pendingOperator);
        m_currentInput = QString::number(m_currentVal, 'g', 15);
    }
    else
    {
        m_currentVal = currentVal;
    }

    m_storedVal = m_currentVal;
    m_pendingOperator = op;
    m_startNewInput = true;

    // Build the expression string for the top label
    QString symbol;
    switch(op)
    {
    case ClcOperator::Add:
        symbol = "+";
        break;
    case ClcOperator::Subtract:
        symbol = "-";
        break;
    case ClcOperator::Multiply:
        symbol = "x";
        break;
    case ClcOperator::Divide:
        symbol = "Ã·";
        break;
    default:
        break;
    }

    m_expressionString = QString::number(m_storedVal, 'g', 15) + " " + symbol;
}

void ClcEngine::calculate()
{
    if(m_pendingOperator == ClcOperator::None)
    {
        return;
    }

    double currentVal = m_currentInput.toDouble();

    // Save the full equation before calculation
    QString fullEquation = m_expressionString + " " + m_currentInput + " =";
    double result = evaluate(m_storedVal, currentVal, m_pendingOperator);

    m_currentInput = QString::number(result, 'g', 15);
    m_storedVal = result;
    m_pendingOperator = ClcOperator::None;
    m_startNewInput = true;
    m_expressionString = fullEquation;
}

void ClcEngine::clear()
{
    m_currentVal = 0.0;
    m_storedVal = 0.0;
    m_pendingOperator = ClcOperator::None;
    m_currentInput = "0";
    m_expressionString = "";
    m_startNewInput = true;
}

void ClcEngine::clearEntry()
{
    m_currentInput = "0";
    m_startNewInput = true;
}

void ClcEngine::backSpace()
{
    // Does not backspace on a calculated result.
    if(m_startNewInput)
    {
        return;
    }

    m_currentInput.chop(1);
    if(m_currentInput.isEmpty() || m_currentInput == "-")
    {
        m_currentInput = "0";
    }
}

void ClcEngine::square()
{
    applyUnary(m_currentInput.toDouble() * m_currentInput.toDouble());
}

void ClcEngine::squareRoot()
{
    double val = m_currentInput.toDouble();
    if(val < 0)
    {
        m_currentInput = "Invalid Input";
        m_startNewInput = true;
    }
    else
    {
        applyUnary(std::sqrt(val));
    }
}

void ClcEngine::reciprocal()
{
    double val = m_currentInput.toDouble();
    if(val == 0.0)
    {
        m_currentInput = "Cannot divide by zero";
        m_startNewInput = true;
    }
    else
    {
        applyUnary(1.0 / val);
    }
}

void ClcEngine::negate()
{
    double val = m_currentInput.toDouble();
    applyUnary(-val);
}

void ClcEngine::percent()
{
    double val = m_currentInput.toDouble();
    if(m_pendingOperator != ClcOperator::None)
    {
        applyUnary(m_storedVal * (val / 100.0));
    }
    else
    {
        applyUnary(val / 100.0);
    }
}

void ClcEngine::sine()
{
    double degrees = m_currentInput.toDouble();
    double radians = degrees * M_PI / 180.0;
    applyUnary(std::sin(radians));

}

void ClcEngine::cosine()
{
    double degrees = m_currentInput.toDouble();
    double radians = degrees * M_PI / 180.0;
    applyUnary(std::cos(radians));
}

void ClcEngine::tangent()
{
    double degrees = m_currentInput.toDouble();
    double radians = degrees * M_PI / 180.0;
    applyUnary(std::tan(radians));
}

void ClcEngine::log10()
{
    double val = m_currentInput.toDouble();
    if(val <= 0)
    {
        m_currentInput = "Invalid Input";
        m_startNewInput = true;

    }
    else
    {
        applyUnary(std::log10(val));
    }
}

void ClcEngine::ln()
{
    double val = m_currentInput.toDouble();
    if(val <= 0)
    {
        m_currentInput = "Invalid Input";
        m_startNewInput = true;
    }
    else
    {
        applyUnary(std::log(val));
    }

}

void ClcEngine::pi()
{
    applyUnary(MathConstants::PI);
}

void ClcEngine::euler()
{
    applyUnary(MathConstants::E);
}

void ClcEngine::exponental()
{
    //On most scientific calculators, the exp button means 10^x(Scientific Notation).
    //So this makes more sense
    applyUnary(std::pow(10.0,m_currentInput.toDouble()));
}

void ClcEngine::absoluteValue()
{
    applyUnary(std::abs(m_currentInput.toDouble()));
}

void ClcEngine::factorial()
{
    double val = m_currentInput.toDouble();
    if(val < 0)
    {
        m_currentInput = "Invalid Input";
        m_startNewInput = true;
        return;
    }

    if(val >= 170)
    {
        m_currentInput = "Overflow";
        m_startNewInput = true;
        return;
    }

    applyUnary(std::tgamma(val + 1));

}

void ClcEngine::memoryStore()
{
    m_memory = m_currentInput.toDouble();
}

void ClcEngine::memoryClear()
{
    m_memory = defMemory;
}

void ClcEngine::memoryRecall()
{
    applyUnary(m_memory);
}

void ClcEngine::memoryAdd()
{
    m_memory += m_currentInput.toDouble();
}

void ClcEngine::memorySubtract()
{
    m_memory -= m_currentInput.toDouble();
}

double ClcEngine::evaluate(double a, double b, ClcOperator op)
{
    switch(op)
    {
    case ClcOperator::Add:
        return a + b;
    case ClcOperator::Subtract:
        return a - b;
    case ClcOperator::Multiply:
        return a * b;
    case ClcOperator::Divide:
        if (b == 0.0)
        {
            return 0.0;
        }
        return a / b;
    case ClcOperator::Power:
        return std::pow(a, b);
        break;
    case ClcOperator::None:
        return b;
    default:
        return 0.0;
    }
}

void ClcEngine::applyUnary(double result)
{
    m_currentInput = QString::number(result, 'g', 15);
    m_startNewInput = true;
}

