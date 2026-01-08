#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QMap>
#include <QVector>

class Container;

/**
 * CustomTabWidget - QTabWidget with dual mapping and container integration
 *
 * Features:
 * - Inherits QTabWidget (built-in tab bar + content management)
 * - Dual mapping: access by index OR by name
 * - Container integration: attach widgets to containers by label
 * - Fake content support for placeholder tabs
 * - Name-based signals in addition to index-based
 * - Nested tab support via addSubTabWidget()
 *
 * Usage:
 *   CustomTabWidget *tabs = new CustomTabWidget();
 *   Container *container = new Container();
 *
 *   // Add tab with container
 *   tabs->addTab("Tab 1", widget1, container);
 *
 *   // Add nested tab widget
 *   CustomTabWidget *subTabs = new CustomTabWidget();
 *   tabs->addSubTabWidget("Parent", subTabs, container);
 */
class CustomTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit CustomTabWidget(QWidget *parent = nullptr);
    ~CustomTabWidget();

    // ========================================
    // Tab Management
    // ========================================

    /**
     * Add a tab with label, content, and optional container
     * @param label Tab text (must be unique)
     * @param content Widget to show (nullptr = auto fake content)
     * @param container Container to attach widget to (optional)
     * @return Tab index, or -1 on error
     */
    int addTab(const QString &label, QWidget *content = nullptr, Container *container = nullptr);

    /**
     * Add a nested CustomTabWidget as a tab
     * @param label Tab text (must be unique)
     * @param subTabWidget Nested CustomTabWidget
     * @param container Container for the nested widget (optional)
     * @return Tab index, or -1 on error
     */
    int addSubTabWidget(const QString &label, CustomTabWidget *subTabWidget, Container *container = nullptr);

    /**
     * Set fixed width for all tabs
     * @param singleTabWidth Width in pixels for each tab
     */
    void setTabWidth(int singleTabWidth);

    // ========================================
    // Dual Mapping - Access by Index OR Name
    // ========================================

    /**
     * Get tab index by label
     * @return Index, or -1 if not found
     */
    int getTabIndex(const QString &label) const;

    /**
     * Get tab label by index
     * @return Label, or empty string if invalid
     */
    QString getTabLabel(int index) const;

    /**
     * Get content widget by label
     */
    QWidget* getContent(const QString &label) const;

    /**
     * Get container by label
     */
    Container* getContainer(const QString &label) const;

    /**
     * Get nested CustomTabWidget by label
     */
    CustomTabWidget* getSubTabWidget(const QString &label) const;

    // ========================================
    // Current Tab (Name-Based)
    // ========================================

    /**
     * Get current tab label
     */
    QString currentLabel() const;

    /**
     * Set current tab by label
     */
    void setCurrentTab(const QString &label);

    // ========================================
    // Text Management (Name-Based)
    // ========================================

    /**
     * Change tab text (updates dual mapping)
     */
    void setTabText(int index, const QString &newText);

    /**
     * Change tab text by current label
     */
    void setTabText(const QString &oldLabel, const QString &newText);

signals:
    /**
     * Emitted when current tab changes (name-based)
     * Note: QTabWidget already emits currentChanged(int)
     */
    void currentTabChanged(const QString &label);

    /**
     * Emitted when tab index changes (for compatibility with STabWidget)
     */
    void tabIndexChanged(int index);

public slots:
    /**
     * Slot for handling index view changes (for compatibility with STabWidget)
     */
    void onIndexViewChanged(int index);

private slots:
    void onCurrentChanged(int index);

private:
    QWidget* createFakeContent();
    void updateDualMapping(int index, const QString &oldLabel, const QString &newLabel);

    // Dual mapping (index ↔ name)
    QMap<QString, int> m_nameToIndex;  // "Tab 1" → 0
    QMap<int, QString> m_indexToName;  // 0 → "Tab 1"

    // Content mappings (name → objects)
    QMap<QString, QWidget*> m_mapWgContents;           // label → content widget
    QMap<QString, Container*> m_mapWgContainer;        // label → container
    QMap<QString, CustomTabWidget*> m_mapSubTabContents;  // label → nested tab widget
    QMap<QString, Container*> m_mapSubTabContainer;    // label → container for nested widget

    // Index mappings (for compatibility)
    QMap<int, QString> m_tabIndexMapping;     // index → label (same as m_indexToName)
    QMap<int, QString> m_subTabIndexMapping;  // index → label for sub tabs

    // Fake content placeholders
    QVector<QWidget*> m_fakeContents;
};

#endif // CUSTOMTABWIDGET_H
