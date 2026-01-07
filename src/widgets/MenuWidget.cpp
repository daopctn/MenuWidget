#include "MenuWidget.h"

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mainLayout = new QVBoxLayout(this);

    // Create level 1 tab bar
    m_level1TabBar = new QTabBar(this);

    // Create container for level 2 tab bars
    m_level2Container = new Container(this);

    // Add widgets to layout
    m_mainLayout->addWidget(m_level1TabBar);
    m_mainLayout->addWidget(m_level2Container);

    setLayout(m_mainLayout);

    // Connect level 1 tab change signal
    connect(m_level1TabBar, &QTabBar::currentChanged,
            this, &MenuWidget::onLevel1TabChanged);
}

MenuWidget::~MenuWidget()
{
}

void MenuWidget::addLevel1Tab(const QString &tabName)
{
    // Get the index for the new level 1 tab
    int level1Index = m_level1TabBar->count();

    // Create a new level 2 tab bar for this level 1 tab
    QTabBar *level2TabBar = new QTabBar(this);
    m_level2TabBars[level1Index] = level2TabBar;

    // Attach the level 2 tab bar to the container
    m_level2Container->attach(level2TabBar);

    // Initialize the content widgets map for this level 2 tab bar
    m_contentWidgets[level2TabBar] = QMap<int, CustomWidget*>();

    // Connect level 2 tab change signal
    connect(level2TabBar, &QTabBar::currentChanged,
            this, &MenuWidget::onLevel2TabChanged);

    // Add tab to level 1 tab bar
    m_level1TabBar->addTab(tabName);

    // If this is the first tab, show it
    if (level1Index == 0) {
        m_level1TabBar->setCurrentIndex(0);
        m_level2Container->show(level2TabBar);
    }
}

void MenuWidget::addLevel2Tab(int level1Index, const QString &tabName, CustomWidget *contentWidget)
{
    // Check if the level 1 index is valid
    if (!m_level2TabBars.contains(level1Index)) {
        return;
    }

    // Get the corresponding level 2 tab bar
    QTabBar *level2TabBar = m_level2TabBars[level1Index];

    // Get the index for the new level 2 tab
    int level2Index = level2TabBar->count();

    // Add tab to level 2 tab bar
    level2TabBar->addTab(tabName);

    // Store the mapping of level 2 tab index to content widget
    m_contentWidgets[level2TabBar][level2Index] = contentWidget;

    // If this is the first level 2 tab for the current level 1 tab, set it as current
    if (level2Index == 0) {
        level2TabBar->setCurrentIndex(0);
        // Emit signal
        emit tabSelectionChanged(level1Index, 0);
    }
}

void MenuWidget::onLevel1TabChanged(int index)
{
    // Switch to the corresponding level 2 tab bar
    if (m_level2TabBars.contains(index)) {
        QTabBar *level2TabBar = m_level2TabBars[index];
        m_level2Container->show(level2TabBar);

        // Get current level 2 index and emit signal
        int currentLevel2Index = level2TabBar->currentIndex();
        if (currentLevel2Index >= 0) {
            emit tabSelectionChanged(index, currentLevel2Index);
        }
    }
}

void MenuWidget::onLevel2TabChanged(int index)
{
    // Get the current level 2 tab bar
    QTabBar *level2TabBar = qobject_cast<QTabBar*>(sender());
    if (!level2TabBar) {
        return;
    }

    // Find the level 1 index for this level 2 tab bar
    int level1Index = -1;
    for (auto it = m_level2TabBars.begin(); it != m_level2TabBars.end(); ++it) {
        if (it.value() == level2TabBar) {
            level1Index = it.key();
            break;
        }
    }

    if (level1Index >= 0) {
        emit tabSelectionChanged(level1Index, index);
    }
}

CustomWidget* MenuWidget::getContentWidget(int level1Index, int level2Index) const
{
    if (!m_level2TabBars.contains(level1Index)) {
        return nullptr;
    }

    QTabBar *level2TabBar = m_level2TabBars[level1Index];
    if (!m_contentWidgets.contains(level2TabBar)) {
        return nullptr;
    }

    if (!m_contentWidgets[level2TabBar].contains(level2Index)) {
        return nullptr;
    }

    return m_contentWidgets[level2TabBar][level2Index];
}

void MenuWidget::setCurrentTabs(int level1Index, int level2Index)
{
    // Validate level 1 index
    if (level1Index < 0 || level1Index >= m_level1TabBar->count()) {
        return;
    }

    // Block signals to avoid triggering tabSelectionChanged
    m_level1TabBar->blockSignals(true);
    m_level1TabBar->setCurrentIndex(level1Index);
    m_level1TabBar->blockSignals(false);

    // Show the corresponding level 2 tab bar
    if (m_level2TabBars.contains(level1Index)) {
        QTabBar *level2TabBar = m_level2TabBars[level1Index];
        m_level2Container->show(level2TabBar);

        // Validate and set level 2 index
        if (level2Index >= 0 && level2Index < level2TabBar->count()) {
            level2TabBar->blockSignals(true);
            level2TabBar->setCurrentIndex(level2Index);
            level2TabBar->blockSignals(false);
        }
    }
}
