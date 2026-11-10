#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <QString>


// Simple helper for including commas
inline QString formatWithCommas(const QString &str)
{
    // Return as is if its decimal or too short
    if(str.length() <= 3 || str.contains("."))
    {
        return str;
    }

    QString result = str;
    int insertPosition = result.length() - 3;
    while(insertPosition > 0)
    {
        result.insert(insertPosition, ",");
        insertPosition -= 3;
    }

    return result;
}

#endif // STRINGUTILS_H
