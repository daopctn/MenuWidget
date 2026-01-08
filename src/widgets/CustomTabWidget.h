#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QMap>
#include <QVector>

/**
 * CustomTabWidget - QTabWidget with dual mapping (index + name)
 *
 * Features:
 * - Inherits QTabWidget (built-in tab bar + content management)
 * - Dual mapping: access by index OR by name
 * - Fake content support for placeholder tabs
 * - Name-based signals in addition to index-based
 *
 * Usage:
 *   CustomTabWidget *tabs = new CustomTabWidget();
 *   tabs->addTab("Tab 1", widget1);
 *   tabs->addTab("Tab 2");  // Auto fake content
 *
 *   // Access by name
 *   QWidget *w = tabs->getContent("Tab 1");
 *
 *   // Nesting: each tab can contain another CustomTabWidget
 *   CustomTabWidget *subTabs = new CustomTabWidget();
 *   subTabs->addTab("Sub 1", subWidget1);
 *   tabs->addTab("Parent", subTabs);
 */
class CustomTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit CustomTabWidget(QWidget *parent = nullptr);
    ~CustomTabWidget();

    // ========================================
    // Tab Management (Override + Dual Mapping)
    // ========================================

    /**
     * Add a tab with label and optional content
     * @param label Tab text (must be unique)
     * @param content Widget to show (nullptr = auto fake content)
     * @return Tab index, or -1 on error
     */
    int addTab(const QString &label, QWidget *content = nullptr);

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

private slots:
    void onCurrentChanged(int index);

private:
    QWidget* createFakeContent();
    void updateDualMapping(int index, const QString &oldLabel, const QString &newLabel);

    // Dual mapping
    QMap<QString, int> m_nameToIndex;  // "Tab 1" → 0
    QMap<int, QString> m_indexToName;  // 0 → "Tab 1"

    // Fake content placeholders
    QVector<QWidget*> m_fakeContents;
};

#endif // CUSTOMTABWIDGET_H
