#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QWidget>
#include <QTabBar>
#include <QMap>
#include <QVector>
#include "Container.h"

class CustomTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomTabWidget(QWidget *parent = nullptr);
    ~CustomTabWidget();

    // Tab management
    int addTab(const QString &tabName);
    int addSubTab(int parentIndex, const QString &tabName, QWidget *contentWidget = nullptr);

    // Text management
    void setTabText(int tabIndex, const QString &text);
    void setSubTabText(int tabIndex, int subTabIndex, const QString &text);

    // Content retrieval
    QWidget* getContentWidget(int tabIndex, int subTabIndex) const;

    // Selection (without emitting signals)
    void setCurrentIndices(int tabIndex, int subTabIndex);

    // Get tab bars (for positioning in MenuWidget)
    QTabBar* getLevel1TabBar() const { return m_level1TabBar; }
    Container* getLevel2Container() const { return m_level2Container; }

signals:
    void tabIndexChanged(int tabIndex, int subTabIndex);

private slots:
    void onLevel1TabChanged(int index);
    void onLevel2TabChanged(int index);

private:
    QWidget* createFakeContent();

    QTabBar *m_level1TabBar;
    Container *m_level2Container;

    // Map to store level 2 tab bars for each level 1 tab
    QMap<int, QTabBar*> m_level2TabBars;

    // Map to store content widgets for each level 2 tab
    // Key: level2TabBar, Value: Map of (sub tab index -> content widget)
    QMap<QTabBar*, QMap<int, QWidget*>> m_contentWidgets;

    // Fake content placeholders
    QVector<QWidget*> m_fakeContents;
};

#endif // CUSTOMTABWIDGET_H
