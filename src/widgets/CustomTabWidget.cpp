#include "CustomTabWidget.h"
#include <QLabel>
#include <QVBoxLayout>

CustomTabWidget::CustomTabWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create level 1 tab bar
    m_level1TabBar = new QTabBar(this);

    // Create container for level 2 tab bars
    m_level2Container = new Container(this);

    // Connect level 1 tab change signal
    connect(m_level1TabBar, &QTabBar::currentChanged,
            this, &CustomTabWidget::onLevel1TabChanged);
}

CustomTabWidget::~CustomTabWidget()
{
    // Clean up fake content widgets
    qDeleteAll(m_fakeContents);
    m_fakeContents.clear();
}

int CustomTabWidget::addTab(const QString &tabName)
{
    // Get the index for the new level 1 tab
    int tabIndex = m_level1TabBar->count();

    // Create a new level 2 tab bar for this level 1 tab
    QTabBar *level2TabBar = new QTabBar(this);
    m_level2TabBars[tabIndex] = level2TabBar;

    // Attach the level 2 tab bar to the container
    m_level2Container->attach(level2TabBar);

    // Initialize the content widgets map for this level 2 tab bar
    m_contentWidgets[level2TabBar] = QMap<int, QWidget*>();

    // Connect level 2 tab change signal
    connect(level2TabBar, &QTabBar::currentChanged,
            this, &CustomTabWidget::onLevel2TabChanged);

    // Add tab to level 1 tab bar
    m_level1TabBar->addTab(tabName);

    // If this is the first tab, show it
    if (tabIndex == 0) {
        m_level1TabBar->setCurrentIndex(0);
        m_level2Container->show(level2TabBar);
    }

    return tabIndex;
}

int CustomTabWidget::addSubTab(int parentIndex, const QString &tabName, QWidget *contentWidget)
{
    // Check if the parent index is valid
    if (!m_level2TabBars.contains(parentIndex)) {
        return -1;
    }

    // Get the corresponding level 2 tab bar
    QTabBar *level2TabBar = m_level2TabBars[parentIndex];

    // Get the index for the new sub tab
    int subTabIndex = level2TabBar->count();

    // If no content widget provided, create fake content
    if (!contentWidget) {
        contentWidget = createFakeContent();
    }

    // Add tab to level 2 tab bar
    level2TabBar->addTab(tabName);

    // Store the mapping of sub tab index to content widget
    m_contentWidgets[level2TabBar][subTabIndex] = contentWidget;

    // If this is the first sub tab for the current level 1 tab, set it as current
    if (subTabIndex == 0) {
        level2TabBar->setCurrentIndex(0);
        // Emit signal
        emit tabIndexChanged(parentIndex, 0);
    }

    return subTabIndex;
}

void CustomTabWidget::setTabText(int tabIndex, const QString &text)
{
    // Validate tab index
    if (tabIndex < 0 || tabIndex >= m_level1TabBar->count()) {
        return;
    }

    // Set the new text for the tab
    m_level1TabBar->setTabText(tabIndex, text);
}

void CustomTabWidget::setSubTabText(int tabIndex, int subTabIndex, const QString &text)
{
    // Validate tab index and check if level 2 tab bar exists
    if (!m_level2TabBars.contains(tabIndex)) {
        return;
    }

    QTabBar *level2TabBar = m_level2TabBars[tabIndex];

    // Validate sub tab index
    if (subTabIndex < 0 || subTabIndex >= level2TabBar->count()) {
        return;
    }

    // Set the new text for the sub tab
    level2TabBar->setTabText(subTabIndex, text);
}

QWidget* CustomTabWidget::getContentWidget(int tabIndex, int subTabIndex) const
{
    if (!m_level2TabBars.contains(tabIndex)) {
        return nullptr;
    }

    QTabBar *level2TabBar = m_level2TabBars[tabIndex];
    if (!m_contentWidgets.contains(level2TabBar)) {
        return nullptr;
    }

    if (!m_contentWidgets[level2TabBar].contains(subTabIndex)) {
        return nullptr;
    }

    return m_contentWidgets[level2TabBar][subTabIndex];
}

void CustomTabWidget::setCurrentIndices(int tabIndex, int subTabIndex)
{
    // Validate level 1 index
    if (tabIndex < 0 || tabIndex >= m_level1TabBar->count()) {
        return;
    }

    // Block signals to avoid triggering tabIndexChanged
    m_level1TabBar->blockSignals(true);
    m_level1TabBar->setCurrentIndex(tabIndex);
    m_level1TabBar->blockSignals(false);

    // Show the corresponding level 2 tab bar
    if (m_level2TabBars.contains(tabIndex)) {
        QTabBar *level2TabBar = m_level2TabBars[tabIndex];
        m_level2Container->show(level2TabBar);

        // Validate and set level 2 index
        if (subTabIndex >= 0 && subTabIndex < level2TabBar->count()) {
            level2TabBar->blockSignals(true);
            level2TabBar->setCurrentIndex(subTabIndex);
            level2TabBar->blockSignals(false);
        }
    }
}

void CustomTabWidget::onLevel1TabChanged(int index)
{
    // Switch to the corresponding level 2 tab bar
    if (m_level2TabBars.contains(index)) {
        QTabBar *level2TabBar = m_level2TabBars[index];
        m_level2Container->show(level2TabBar);

        // Get current level 2 index and emit signal
        int currentSubTabIndex = level2TabBar->currentIndex();
        if (currentSubTabIndex >= 0) {
            emit tabIndexChanged(index, currentSubTabIndex);
        }
    }
}

void CustomTabWidget::onLevel2TabChanged(int index)
{
    // Get the current level 2 tab bar
    QTabBar *level2TabBar = qobject_cast<QTabBar*>(sender());
    if (!level2TabBar) {
        return;
    }

    // Find the level 1 index for this level 2 tab bar
    int tabIndex = -1;
    for (auto it = m_level2TabBars.begin(); it != m_level2TabBars.end(); ++it) {
        if (it.value() == level2TabBar) {
            tabIndex = it.key();
            break;
        }
    }

    if (tabIndex >= 0) {
        emit tabIndexChanged(tabIndex, index);
    }
}

QWidget* CustomTabWidget::createFakeContent()
{
    // Create a simple placeholder widget
    QWidget *fakeWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(fakeWidget);
    QLabel *label = new QLabel("Fake Content - Not Initialized", fakeWidget);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: gray; font-style: italic;");
    layout->addWidget(label);
    fakeWidget->setLayout(layout);

    // Store it so we can delete it later
    m_fakeContents.append(fakeWidget);

    return fakeWidget;
}
