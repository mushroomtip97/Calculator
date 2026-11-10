#ifndef TINTCOLOR_H
#define TINTCOLOR_H

#include <QPainter>
#include <QPixmap>

namespace TintColor {

// Tints an icon while preserving its transparency.
// The color is painted first, then the original icon is used as a mask
// through CompositionMode_DestinationIn.
inline const QPixmap tintIcon(const QString& imagePath, const QColor& targetColor)
{
    QPixmap original(imagePath);
    if (original.isNull())
    {
        return original;
    }

    QPixmap tinted(original.size());
    tinted.fill(Qt::transparent);

    QPainter painter(&tinted);
    painter.fillRect(tinted.rect(), targetColor);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap(0, 0, original);
    painter.end();

    return tinted;
}

} // namespace TintColor

#endif // TINTCOLOR_H


