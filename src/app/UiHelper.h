#ifndef UIHELPER_H
#define UIHELPER_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QFont>
#include <QHash>
#include <QWidget>

// Simple helper only for memory buttons

class QWidget;
class QPushButton;

namespace UiHelper {

inline QHash<QString, QPushButton*> createMemoryButtons(QWidget *parent, QBoxLayout *layout, int insertIndex = 2)
{
    QHash<QString, QPushButton*> buttons;
    if (!layout)
    {
        return buttons;
    }

    QStringList names = {"MC", "MR", "M+", "M-", "MS"};
    QStringList objNames = {"btnMC", "btnMR", "btnMPlus", "btnM_Minus", "btnMS"};

    auto *memoryLayout = new QHBoxLayout();

    for (int i = 0; i < names.size(); ++i)
    {
        auto *btn = new QPushButton(names[i], parent);
        btn->setObjectName(objNames[i]);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        btn->setMinimumHeight(30);

        QFont font = btn->font();
        font.setPointSize(10);
        btn->setFont(font);

        memoryLayout->addWidget(btn);
        buttons.insert(objNames[i], btn);
    }

    layout->insertLayout(insertIndex, memoryLayout);
    return buttons;
}

}

#endif // UIHELPER_H
