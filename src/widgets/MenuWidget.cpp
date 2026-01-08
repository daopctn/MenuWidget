#include "MenuWidget.h"
#include "ui_MenuWidget.h"
#include <QResizeEvent>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWidget)
{
    // Setup UI from .ui file (contains area buttons frame)
    ui->setupUi(this);

    // Create level 1 tab bar (manual positioning)
    m_level1TabBar = new QTabBar(this);

    // Create container for level 2 tab bars (manual positioning)
    m_level2Container = new Container(this);

    // Set initial positions
    updateWidgetPositions();

    // Connect level 1 tab change signal
    connect(m_level1TabBar, &QTabBar::currentChanged,
            this, &MenuWidget::onLevel1TabChanged);
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateWidgetPositions();
}

void MenuWidget::updateWidgetPositions()
{
    int w = width();   // Chiều rộng widget
    int h = height();  // Chiều cao widget

    // ========================================
    // TỈ LỆ VÀ VỊ TRÍ (có thể tùy chỉnh)
    // ========================================

    // Margins
    int marginLeft = w * 0.01;      // 1% width
    int marginRight = w * 0.01;     // 1% width
    int marginTop = h * 0.02;       // 2% height
    int spacing = h * 0.01;         // 1% height spacing giữa các phần tử

    // Area buttons frame (đã có trong .ui, chỉ cần resize)
    int frameWidth = w * 0.35;      // 35% width
    int frameHeight = h * 0.06;     // 6% height
    ui->areaButtonsFrame->setGeometry(marginLeft, marginTop, frameWidth, frameHeight);

    // Level 1 tab bar position
    int level1TabY = marginTop + frameHeight + spacing;
    int level1TabWidth = w - marginLeft - marginRight;
    int level1TabHeight = h * 0.05; // 5% height
    m_level1TabBar->setGeometry(marginLeft, level1TabY, level1TabWidth, level1TabHeight);

    // Level 2 container position
    int level2Y = level1TabY + level1TabHeight + spacing;
    int level2Width = w - marginLeft - marginRight;
    int level2Height = h * 0.05;    // 5% height
    m_level2Container->setGeometry(marginLeft, level2Y, level2Width, level2Height);

    // ========================================
    // TÓM TẮT TỈ LỆ:
    // ========================================
    // - Area buttons frame: 1% từ trái, 2% từ trên, rộng 35%, cao 6%
    // - Level 1 TabBar:     1% margin ngang, dưới frame + 1%, rộng 98%, cao 5%
    // - Level 2 Container:  1% margin ngang, dưới level1 + 1%, rộng 98%, cao 5%
    // ========================================
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

void MenuWidget::setLevel1TabText(int level1Index, const QString &newText)
{
    // Validate level 1 index
    if (level1Index < 0 || level1Index >= m_level1TabBar->count()) {
        return;
    }

    // Set the new text for the tab
    m_level1TabBar->setTabText(level1Index, newText);
}

void MenuWidget::setLevel2TabText(int level1Index, int level2Index, const QString &newText)
{
    // Validate level 1 index and check if level 2 tab bar exists
    if (!m_level2TabBars.contains(level1Index)) {
        return;
    }

    QTabBar *level2TabBar = m_level2TabBars[level1Index];

    // Validate level 2 index
    if (level2Index < 0 || level2Index >= level2TabBar->count()) {
        return;
    }

    // Set the new text for the tab
    level2TabBar->setTabText(level2Index, newText);
}
