#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QColor>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addHistory(const QString &entry);
    void addScientificHistory(const QString &entry);
    void addProgrammerHistory(const QString &entry);
    void applyTheme(int themeIndex);

private:
    Ui::MainWindow *ui;

    void init();
    QString themeStyleSheet(int themeIndex) const;
    void applyModeIcons(int themeIndex);
    QColor modeIconColor(int themeIndex) const;
};
#endif // MAINWINDOW_H

