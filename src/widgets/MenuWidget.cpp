#include "MenuWidget.h"
#include "ui_MenuWidget.h"
#include <QResizeEvent>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWidget)
{
    // Setup UI from .ui file (contains area buttons frame)
    ui->setupUi(this);

    // Create CustomTabWidget (manages tab hierarchy)
    m_customTabWidget = new CustomTabWidget(this);

    // Connect CustomTabWidget signal to MenuWidget signal
    connect(m_customTabWidget, &CustomTabWidget::tabIndexChanged,
            this, &MenuWidget::tabSelectionChanged);

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
    int w = width();   // Widget width
    int h = height();  // Widget height

    // ========================================
    // PROPORTIONS AND POSITIONS
    // ========================================

    // Margins
    int marginLeft = w * 0.01;      // 1% width
    int marginRight = w * 0.01;     // 1% width
    int marginTop = h * 0.02;       // 2% height
    int spacing = h * 0.01;         // 1% height spacing

    // Area buttons frame (from .ui)
    int frameWidth = w * 0.35;      // 35% width
    int frameHeight = h * 0.06;     // 6% height
    ui->areaButtonsFrame->setGeometry(marginLeft, marginTop, frameWidth, frameHeight);

    // Level 1 tab bar position (from CustomTabWidget)
    int level1TabY = marginTop + frameHeight + spacing;
    int level1TabWidth = w - marginLeft - marginRight;
    int level1TabHeight = h * 0.05; // 5% height
    m_customTabWidget->getLevel1TabBar()->setGeometry(marginLeft, level1TabY, level1TabWidth, level1TabHeight);

    // Level 2 container position (from CustomTabWidget)
    int level2Y = level1TabY + level1TabHeight + spacing;
    int level2Width = w - marginLeft - marginRight;
    int level2Height = h * 0.05;    // 5% height
    m_customTabWidget->getLevel2Container()->setGeometry(marginLeft, level2Y, level2Width, level2Height);
}

void MenuWidget::addLevel1Tab(const QString &tabName)
{
    m_customTabWidget->addTab(tabName);
}

void MenuWidget::addLevel2Tab(int level1Index, const QString &tabName, CustomWidget *contentWidget)
{
    m_customTabWidget->addSubTab(level1Index, tabName, contentWidget);
}

CustomWidget* MenuWidget::getContentWidget(int level1Index, int level2Index) const
{
    // CustomTabWidget returns QWidget*, cast to CustomWidget*
    return qobject_cast<CustomWidget*>(m_customTabWidget->getContentWidget(level1Index, level2Index));
}

void MenuWidget::setCurrentTabs(int level1Index, int level2Index)
{
    m_customTabWidget->setCurrentIndices(level1Index, level2Index);
}

void MenuWidget::setLevel1TabText(int level1Index, const QString &newText)
{
    m_customTabWidget->setTabText(level1Index, newText);
}

void MenuWidget::setLevel2TabText(int level1Index, int level2Index, const QString &newText)
{
    m_customTabWidget->setSubTabText(level1Index, level2Index, newText);
}
