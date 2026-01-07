#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWidget;
class MenuWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MainWidget *m_mainWidget;
    MenuWidget *m_menuWidget;

    void setupMenuWidget();
};

#endif // MAINWINDOW_H
