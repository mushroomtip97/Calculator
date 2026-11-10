#include "programmerengine.h"

#include <QtEndian>

ProgrammerEngine::ProgrammerEngine() {}


void ProgrammerEngine::inputDigit(const QString &digit)
{

    if(!m_startNewInput)
    {
        int maxLength = 16;
        if(m_base == Base::Dec)
        {
            maxLength = 20;
        }
        else if(m_base == Base::Oct)
        {
            maxLength = 22;
        }
        else if(m_base == Base::Bin)
        {
            maxLength = 64;
        }

        if(getDisplayStr().length() > maxLength)
        {
            return;
        }
    }

    if(m_startNewInput)
    {
        m_currentInput = defCurrentInput;
        m_startNewInput = 0;
    }

    bool ok;

    // Convert the typed string
    quint64 val = digit.toULongLong(&ok, static_cast<int>(m_base));

    if(!ok)
    {
        return;
    }

    quint64 baseVal = static_cast<quint64>(m_base);

    // Shift left and add new digit
    m_currentInput = (m_currentInput * baseVal) + val;

    // This prevents it from exceeding the word size
    m_currentInput = applyMask(m_currentInput);
}

void ProgrammerEngine::setOperator(ProgrammerOperator op)
{

    if (m_pendingOperator != ProgrammerOperator::None && !m_startNewInput)
    {
        m_currentInput = evaluate(m_storedVal, m_currentInput, m_pendingOperator);
    }

    m_storedVal = m_currentInput;
    m_pendingOperator = op;
    m_startNewInput = true;

    m_expressionString = getDisplayStr() + " " + getOperatorSymbol(op);
}

void ProgrammerEngine::calculate()
{
    if(m_pendingOperator == ProgrammerOperator::None)
    {
        return;
    }

    m_currentInput = evaluate(m_storedVal, m_currentInput, m_pendingOperator);

    m_pendingOperator = ProgrammerOperator::None;
    m_startNewInput = true;
    m_expressionString += " " + getDisplayStr();
}

void ProgrammerEngine::clear()
{
    m_currentInput = defCurrentInput;
    m_storedVal = defStoredVal;
    m_expressionString.clear();
    m_pendingOperator = ProgrammerOperator::None;
    m_base = Base::Dec;
    m_startNewInput = true;
}

void ProgrammerEngine::clearEntry()
{
    m_currentInput = defCurrentInput;
    m_startNewInput = true;
}

void ProgrammerEngine::backspace()
{
    // Does not backspace on a calculated result.
    if(m_startNewInput)
    {
        return;
    }

    quint64 baseVal = static_cast<quint64>(m_base);

    // Remove the lowest digit by dividing by the current base.
    m_currentInput /= baseVal;
}

void ProgrammerEngine::setWordSize(WordSize size)
{
    m_wordSize = size;
    // Truncate
    m_currentInput = applyMask(m_currentInput);
    m_storedVal = applyMask(m_storedVal);
}

QString ProgrammerEngine::getOperatorSymbol(ProgrammerOperator op)
{
    switch (op)
    {
    case ProgrammerOperator::Add:
        return "+";
    case ProgrammerOperator::Subtract:
        return "-";
    case ProgrammerOperator::Multiply:
        return "Ã—";
    case ProgrammerOperator::Divide:
        return "Ã·";
    case ProgrammerOperator::Modulus:
        return "MOD";

    // Bitwise operations
    case ProgrammerOperator::And:
        return "AND";
    case ProgrammerOperator::Or:
        return "OR";
    case ProgrammerOperator::Nand:
        return "NAND";
    case ProgrammerOperator::Nor:
        return "NOR";
    case ProgrammerOperator::Xor:
        return "XOR";
    case ProgrammerOperator::Not:
        return "NOT"; // Unary

    // Shifts and Rotates
    case ProgrammerOperator::Shl:
        return "<<"; // Or "Lsh"
    case ProgrammerOperator::Shr:
        return ">>"; // Or "Rsh"
    case ProgrammerOperator::Rol:
        return "ROL";
    case ProgrammerOperator::Ror:
        return "ROR";
    case ProgrammerOperator::None:
    default:
        return "";
    }

}

quint64 ProgrammerEngine::add(quint64 a, quint64 b)
{
    return a + b;
}

quint64 ProgrammerEngine::subtract(quint64 a, quint64 b)
{
    return a - b;
}

quint64 ProgrammerEngine::multiply(quint64 a, quint64 b)
{
    return a * b;
}

quint64 ProgrammerEngine::divide(quint64 a, quint64 b)
{
    if (b == 0)
    {
        return 0;
    }
    return a / b;
}

quint64 ProgrammerEngine::andOp(quint64 a, quint64 b)
{
    return a & b;
}

quint64 ProgrammerEngine::orOp(quint64 a, quint64 b)
{
    return a | b;
}

quint64 ProgrammerEngine::nanOp(quint64 a, quint64 b)
{
    return ~(a & b);
}

quint64 ProgrammerEngine::norOp(quint64 a, quint64 b)
{
    return ~(a | b);
}

quint64 ProgrammerEngine::xorOp(quint64 a, quint64 b)
{
    return a ^ b;
}

quint64 ProgrammerEngine::shlOp(quint64 val, quint64 shiftAmount)
{
    if(shiftAmount >= 64)
    {
        return 0;
    }

    return val << shiftAmount;

}

quint64 ProgrammerEngine::shrOp(quint64 val, quint64 shiftAmount)
{
    if(shiftAmount >= 64)
    {
        return 0;
    }

    return val >> shiftAmount;
}

quint64 ProgrammerEngine::rolOp(quint64 val, quint64 shiftAmount)
{
    shiftAmount %= 64;
    if(shiftAmount == 0)
    {
        return val;
    }

    return (val << shiftAmount) | (val >> (64 - shiftAmount));

}

quint64 ProgrammerEngine::rorOp(quint64 val, quint64 shiftAmount)
{
    shiftAmount %= 64;
    if(shiftAmount == 0)
    {
        return val;
    }

    return (val >> shiftAmount) | (val << (64 - shiftAmount));
}

quint64 ProgrammerEngine::notOp(quint64 a)
{
    return ~a;
}

quint64 ProgrammerEngine::modulus(quint64 a, quint64 b)
{
    if(b == 0)
    {
        return 0;
    }
    return a % b;
}

quint64 ProgrammerEngine::twos_complement(quint64 a)
{
    return (~a) + 1;
}

quint64 ProgrammerEngine::swap_endianness(quint64 a)
{
    return qbswap(a);
}

quint64 ProgrammerEngine::applyMask(quint64 val) const
{
    // If in 64 mode, no mask is needed
    // Also prevents undefined behavior by shifting
    if(m_wordSize == WordSize::Qword)
    {
        return val;
    }

    quint64 bits = static_cast<quint64>(m_wordSize);
    quint64 mask = (1ULL << bits) - 1;

    // Apply the mask to keep only the lower bits
    return val & mask;
}

quint64 ProgrammerEngine::evaluate(quint64 a, quint64 b, ProgrammerOperator op)
{
    quint64 result = 0;

    switch(op)
    {
    case ProgrammerOperator::Add:
        result = add(a, b);
        break;
    case ProgrammerOperator::Subtract:
        result = subtract(a, b);
        break;
    case ProgrammerOperator::Multiply:
        result = multiply(a, b);
        break;
    case ProgrammerOperator::Divide:
        result = divide(a, b);
        break;
    case ProgrammerOperator::Modulus:
        result = modulus(a, b);
        break;
    case ProgrammerOperator::And:
        result = andOp(a, b);
        break;
    case ProgrammerOperator::Nor:
        result = norOp(a, b);
        break;
    case ProgrammerOperator::Or:
        result = orOp(a, b);
        break;
    case ProgrammerOperator::Nand:
        result = nanOp(a, b);
        break;
    case ProgrammerOperator::Xor:
        result = xorOp(a, b);
        break;
    case ProgrammerOperator::Not:
        result = notOp(a);
        break;
    case ProgrammerOperator::Shl:
        result = shlOp(a, b);
        break;
    case ProgrammerOperator::Shr:
        result = shrOp(a, b);
        break;
    case ProgrammerOperator::Rol:
        result = rolOp(a, b);
        break;
    case ProgrammerOperator::Ror:
        result = rorOp(a, b);
        break;
    case ProgrammerOperator::None:
        return b;
    default:
        return 0;
    }

    return applyMask(result);
}

void ProgrammerEngine::applyNot()
{
    m_currentInput = applyMask(notOp(m_currentInput));
    m_startNewInput = true;
}

void ProgrammerEngine::applyNegate()
{
    m_currentInput = applyMask(twos_complement(m_currentInput));
    m_startNewInput = true;
}

void ProgrammerEngine::memoryStore()
{
    m_memory = applyMask(m_currentInput);
}

void ProgrammerEngine::memoryClear()
{
    m_memory = defCurrentInput;
}

void ProgrammerEngine::memoryRecall()
{
    m_currentInput = applyMask(m_memory);
    m_startNewInput = true;
}

void ProgrammerEngine::memoryAdd()
{
    m_memory = applyMask(m_memory + m_currentInput);
}

void ProgrammerEngine::memorySubtract()
{
    m_memory = applyMask(m_memory - m_currentInput);
}


