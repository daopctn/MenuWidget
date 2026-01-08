#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QMap>
#include "CustomWidget.h"
#include "CustomTabWidget.h"

namespace Ui {
class MenuWidget;
}

/**
 * MenuWidget - UI container for hierarchical tab navigation
 *
 * Structure:
 * - Level 1: CustomTabWidget (categories)
 *   - Each tab contains:
 *     - Level 2: CustomTabWidget (items)
 *       - Each tab contains: content widget
 *
 * Features:
 * - Two-level nested CustomTabWidget hierarchy
 * - Manual positioning via resizeEvent()
 * - Dual mapping support (index + name)
 * - Backward compatible API
 */
class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

    // ========================================
    // Level 1 Tab Management
    // ========================================

    /**
     * Add a level 1 tab (category)
     * @param tabName Category name
     * @return Level 1 index
     */
    int addLevel1Tab(const QString &tabName);

    /**
     * Get level 1 tab index by name
     */
    int getLevel1Index(const QString &tabName) const;

    // ========================================
    // Level 2 Tab Management
    // ========================================

    /**
     * Add a level 2 tab (item) by level 1 index
     * @param level1Index Parent category index
     * @param tabName Item name
     * @param contentWidget Content widget (nullptr = fake content)
     * @return Level 2 index within parent
     */
    int addLevel2Tab(int level1Index, const QString &tabName, CustomWidget *contentWidget = nullptr);

    /**
     * Add a level 2 tab by level 1 name
     * @param level1Name Parent category name
     * @param tabName Item name
     * @param contentWidget Content widget
     * @return Level 2 index within parent
     */
    int addLevel2Tab(const QString &level1Name, const QString &tabName, CustomWidget *contentWidget = nullptr);

    /**
     * Get level 2 tab index by names
     */
    int getLevel2Index(const QString &level1Name, const QString &level2Name) const;

    // ========================================
    // Content Access (Dual Mapping)
    // ========================================

    /**
     * Get content widget by indices
     */
    CustomWidget* getContentWidget(int level1Index, int level2Index) const;

    /**
     * Get content widget by names (name-based access)
     */
    CustomWidget* getContentWidget(const QString &level1Name, const QString &level2Name) const;

    // ========================================
    // Current Tab Selection
    // ========================================

    /**
     * Set current tabs by indices (without emitting signals)
     */
    void setCurrentTabs(int level1Index, int level2Index);

    /**
     * Set current tabs by names (without emitting signals)
     */
    void setCurrentTabs(const QString &level1Name, const QString &level2Name);

    // ========================================
    // Text Management
    // ========================================

    /**
     * Rename a level 1 tab (category) by index
     */
    void setLevel1TabText(int level1Index, const QString &newText);

    /**
     * Rename a level 1 tab by current name
     */
    void setLevel1TabText(const QString &oldText, const QString &newText);

    /**
     * Rename a level 2 tab (item) by indices
     */
    void setLevel2TabText(int level1Index, int level2Index, const QString &newText);

    /**
     * Rename a level 2 tab by names
     */
    void setLevel2TabText(const QString &level1Name, const QString &oldLevel2Name, const QString &newText);

    // ========================================
    // Direct Access
    // ========================================

    /**
     * Get level 1 CustomTabWidget
     */
    CustomTabWidget* getLevel1TabWidget() const { return m_level1TabWidget; }

    /**
     * Get level 2 CustomTabWidget by level 1 index
     */
    CustomTabWidget* getLevel2TabWidget(int level1Index) const;

    /**
     * Get level 2 CustomTabWidget by level 1 name
     */
    CustomTabWidget* getLevel2TabWidget(const QString &level1Name) const;

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    /**
     * Emitted when tab selection changes (index-based)
     */
    void tabSelectionChanged(int level1Index, int level2Index);

    /**
     * Emitted when tab selection changes (name-based)
     */
    void tabSelectionChanged(const QString &level1Name, const QString &level2Name);

private slots:
    void onLevel1TabChanged(int index);
    void onLevel1TabChanged(const QString &label);
    void onLevel2TabChanged(int level1Index, int level2Index);

private:
    void updateWidgetPositions();
    CustomTabWidget* ensureLevel2TabWidget(int level1Index);

    Ui::MenuWidget *ui;
    CustomTabWidget *m_level1TabWidget;

    // Map level 1 index → level 2 CustomTabWidget
    QMap<int, CustomTabWidget*> m_level2TabWidgets;
};

#endif // MENUWIDGET_H
