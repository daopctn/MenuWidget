#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMap>

namespace Ui {
class MainWidget;
}

class MenuWidget;
class Container;
class CustomWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    // Set the MenuWidget to be displayed in the menu container
    void setMenuWidget(MenuWidget *menuWidget);

    // Initialize both areas with their default widgets
    void initializeAreas();

private slots:
    void onArea1ButtonClicked();
    void onArea2ButtonClicked();
    void onMenuTabSelectionChanged(int level1Index, int level2Index);

private:
    void setupAreaButtons();
    void switchToArea(int areaIndex);
    void updateAreaDisplay(int areaIndex);

    Ui::MainWidget *ui;
    MenuWidget *m_menuWidget;

    // Area management
    QPushButton *m_area1Button;
    QPushButton *m_area2Button;
    QLabel *m_area1Label;
    QLabel *m_area2Label;

    Container *m_areaContainers[2];  // Containers for subWidget1 and subWidget2
    int m_currentArea;

    // Remember indices for each area
    int m_areaLevel1Index[2];
    int m_areaLevel2Index[2];
};

#endif // MAINWIDGET_H
