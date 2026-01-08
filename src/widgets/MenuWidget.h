#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QTabBar>
#include <QMap>
#include "CustomWidget.h"
#include "Container.h"

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

    // Add a level 1 tab
    void addLevel1Tab(const QString &tabName);

    // Add a level 2 tab with one content widget
    void addLevel2Tab(int level1Index, const QString &tabName, CustomWidget *contentWidget);

    // Get content widget for given indices
    CustomWidget* getContentWidget(int level1Index, int level2Index) const;

    // Set current tab indices (without emitting signals)
    void setCurrentTabs(int level1Index, int level2Index);

    // Rename a level 1 tab (category)
    void setLevel1TabText(int level1Index, const QString &newText);

    // Rename a level 2 tab (item)
    void setLevel2TabText(int level1Index, int level2Index, const QString &newText);

signals:
    // Emitted when tab selection changes
    void tabSelectionChanged(int level1Index, int level2Index);

private slots:
    void onLevel1TabChanged(int index);
    void onLevel2TabChanged(int index);

private:
    void updateWidgetPositions();

    Ui::MenuWidget *ui;
    QTabBar *m_level1TabBar;
    Container *m_level2Container;

    // Map to store level 2 tab bars for each level 1 tab
    QMap<int, QTabBar*> m_level2TabBars;

    // Map to store content widgets for each level 2 tab
    // Key: level2TabBar, Value: Map of (tab index -> widget)
    QMap<QTabBar*, QMap<int, CustomWidget*>> m_contentWidgets;
};

#endif // MENUWIDGET_H
