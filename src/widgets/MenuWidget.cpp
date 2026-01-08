#include "MenuWidget.h"
#include "ui_MenuWidget.h"
#include <QResizeEvent>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWidget)
{
    // Setup UI from .ui file (contains area buttons frame)
    ui->setupUi(this);

    // Create level 1 CustomTabWidget
    m_level1TabWidget = new CustomTabWidget(this);

    // Connect level 1 signals
    connect(m_level1TabWidget, QOverload<int>::of(&CustomTabWidget::currentChanged),
            this, QOverload<int>::of(&MenuWidget::onLevel1TabChanged));
    connect(m_level1TabWidget, &CustomTabWidget::currentTabChanged,
            this, QOverload<const QString&>::of(&MenuWidget::onLevel1TabChanged));

    // Note: resizeEvent() will position widgets when shown
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
    int w = width();
    int h = height();

    // Margins and spacing
    int marginLeft = w * 0.01;
    int marginRight = w * 0.01;
    int marginTop = h * 0.02;
    int spacing = h * 0.01;

    // Area buttons frame (from .ui)
    int frameWidth = w * 0.35;
    int frameHeight = h * 0.06;
    ui->areaButtonsFrame->setGeometry(marginLeft, marginTop, frameWidth, frameHeight);

    // Level 1 tab bar
    int level1TabY = marginTop + frameHeight + spacing;
    int level1TabWidth = w - marginLeft - marginRight;
    int level1TabHeight = h * 0.05;
    m_level1TabWidget->tabBar()->setGeometry(marginLeft, level1TabY, level1TabWidth, level1TabHeight);

    // Level 2 position (below level 1)
    int level2Y = level1TabY + level1TabHeight + spacing;
    int level2Width = w - marginLeft - marginRight;
    int level2Height = h * 0.05;

    // Position all level 2 CustomTabWidgets' tab bars
    for (CustomTabWidget *level2Widget : m_level2TabWidgets) {
        if (level2Widget) {
            level2Widget->tabBar()->setGeometry(marginLeft, level2Y, level2Width, level2Height);
        }
    }

    // Content area (below level 2 tabs)
    int contentY = level2Y + level2Height + spacing;
    int contentHeight = h - contentY - (h * 0.02);

    // Position level 1 widget's content area
    m_level1TabWidget->setGeometry(marginLeft, contentY, level2Width, contentHeight);

    // Position all level 2 widgets' content areas
    for (CustomTabWidget *level2Widget : m_level2TabWidgets) {
        if (level2Widget) {
            // Level 2 widgets are inside level 1's content, position relative
            level2Widget->setGeometry(0, 0, level2Width, contentHeight);
        }
    }
}

int MenuWidget::addLevel1Tab(const QString &tabName)
{
    // Create level 2 CustomTabWidget for this category
    CustomTabWidget *level2TabWidget = new CustomTabWidget(this);

    // Add tab to level 1 with level 2 widget as content
    int index = m_level1TabWidget->addTab(tabName, level2TabWidget);

    if (index >= 0) {
        // Store level 2 widget
        m_level2TabWidgets[index] = level2TabWidget;

        // Connect level 2 signals (capture index)
        connect(level2TabWidget, QOverload<int>::of(&CustomTabWidget::currentChanged),
                this, [this, index](int level2Index) {
                    onLevel2TabChanged(index, level2Index);
                });
    }

    return index;
}

int MenuWidget::getLevel1Index(const QString &tabName) const
{
    return m_level1TabWidget->getTabIndex(tabName);
}

int MenuWidget::addLevel2Tab(int level1Index, const QString &tabName, CustomWidget *contentWidget)
{
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Index);
    if (!level2Widget) {
        return -1;
    }

    return level2Widget->addTab(tabName, contentWidget);
}

int MenuWidget::addLevel2Tab(const QString &level1Name, const QString &tabName, CustomWidget *contentWidget)
{
    int level1Index = getLevel1Index(level1Name);
    return addLevel2Tab(level1Index, tabName, contentWidget);
}

int MenuWidget::getLevel2Index(const QString &level1Name, const QString &level2Name) const
{
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Name);
    if (!level2Widget) {
        return -1;
    }

    return level2Widget->getTabIndex(level2Name);
}

CustomWidget* MenuWidget::getContentWidget(int level1Index, int level2Index) const
{
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Index);
    if (!level2Widget) {
        return nullptr;
    }

    return qobject_cast<CustomWidget*>(level2Widget->widget(level2Index));
}

CustomWidget* MenuWidget::getContentWidget(const QString &level1Name, const QString &level2Name) const
{
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Name);
    if (!level2Widget) {
        return nullptr;
    }

    return qobject_cast<CustomWidget*>(level2Widget->getContent(level2Name));
}

void MenuWidget::setCurrentTabs(int level1Index, int level2Index)
{
    // Set level 1
    m_level1TabWidget->setCurrentIndex(level1Index);

    // Set level 2
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Index);
    if (level2Widget) {
        level2Widget->setCurrentIndex(level2Index);
    }
}

void MenuWidget::setCurrentTabs(const QString &level1Name, const QString &level2Name)
{
    int level1Index = getLevel1Index(level1Name);
    int level2Index = getLevel2Index(level1Name, level2Name);
    setCurrentTabs(level1Index, level2Index);
}

void MenuWidget::setLevel1TabText(int level1Index, const QString &newText)
{
    m_level1TabWidget->setTabText(level1Index, newText);
}

void MenuWidget::setLevel1TabText(const QString &oldText, const QString &newText)
{
    m_level1TabWidget->setTabText(oldText, newText);
}

void MenuWidget::setLevel2TabText(int level1Index, int level2Index, const QString &newText)
{
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Index);
    if (level2Widget) {
        level2Widget->setTabText(level2Index, newText);
    }
}

void MenuWidget::setLevel2TabText(const QString &level1Name, const QString &oldLevel2Name, const QString &newText)
{
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Name);
    if (level2Widget) {
        level2Widget->setTabText(oldLevel2Name, newText);
    }
}

CustomTabWidget* MenuWidget::getLevel2TabWidget(int level1Index) const
{
    return m_level2TabWidgets.value(level1Index, nullptr);
}

CustomTabWidget* MenuWidget::getLevel2TabWidget(const QString &level1Name) const
{
    int level1Index = getLevel1Index(level1Name);
    return getLevel2TabWidget(level1Index);
}

void MenuWidget::onLevel1TabChanged(int index)
{
    // Get current level 2 index
    CustomTabWidget *level2Widget = getLevel2TabWidget(index);
    if (level2Widget) {
        int level2Index = level2Widget->currentIndex();
        if (level2Index >= 0) {
            emit tabSelectionChanged(index, level2Index);
        }
    }
}

void MenuWidget::onLevel1TabChanged(const QString &label)
{
    // Get current level 2 label
    CustomTabWidget *level2Widget = getLevel2TabWidget(label);
    if (level2Widget) {
        QString level2Label = level2Widget->currentLabel();
        if (!level2Label.isEmpty()) {
            emit tabSelectionChanged(label, level2Label);
        }
    }
}

void MenuWidget::onLevel2TabChanged(int level1Index, int level2Index)
{
    // Emit index-based signal
    emit tabSelectionChanged(level1Index, level2Index);

    // Emit name-based signal
    QString level1Name = m_level1TabWidget->getTabLabel(level1Index);
    CustomTabWidget *level2Widget = getLevel2TabWidget(level1Index);
    if (level2Widget && !level1Name.isEmpty()) {
        QString level2Name = level2Widget->getTabLabel(level2Index);
        if (!level2Name.isEmpty()) {
            emit tabSelectionChanged(level1Name, level2Name);
        }
    }
}

CustomTabWidget* MenuWidget::ensureLevel2TabWidget(int level1Index)
{
    if (m_level2TabWidgets.contains(level1Index)) {
        return m_level2TabWidgets[level1Index];
    }

    // Create new level 2 widget
    CustomTabWidget *level2Widget = new CustomTabWidget(this);
    m_level2TabWidgets[level1Index] = level2Widget;

    // Note: This should ideally be done during addLevel1Tab, not lazily

    return level2Widget;
}
