#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include "CustomWidget.h"
#include "CustomTabWidget.h"

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

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

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    // Emitted when tab selection changes
    void tabSelectionChanged(int level1Index, int level2Index);

private:
    void updateWidgetPositions();

    Ui::MenuWidget *ui;
    CustomTabWidget *m_customTabWidget;
};

#endif // MENUWIDGET_H
