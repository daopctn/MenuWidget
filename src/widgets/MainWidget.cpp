#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "MenuWidget.h"
#include "Container.h"
#include "CustomWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , m_menuWidget(nullptr)
    , m_currentArea(0)
{
    ui->setupUi(this);

    // Initialize area indices - Area 1 starts with Category 1 Item 1, Area 2 with Category 1 Item 2
    m_areaLevel1Index[0] = 0;
    m_areaLevel1Index[1] = 0;
    m_areaLevel2Index[0] = 0;
    m_areaLevel2Index[1] = 1;

    // Create containers for each area
    for (int i = 0; i < 2; ++i) {
        m_areaContainers[i] = new Container(this);
    }

    // Setup containers in subWidget1 and subWidget2
    QVBoxLayout *layout1 = new QVBoxLayout(ui->subWidget1);
    layout1->setContentsMargins(0, 0, 0, 0);
    layout1->addWidget(m_areaContainers[0]);

    QVBoxLayout *layout2 = new QVBoxLayout(ui->subWidget2);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->addWidget(m_areaContainers[1]);

    // Setup area buttons and labels
    setupAreaButtons();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::setupAreaButtons()
{
    // Create buttons and labels
    m_area1Button = new QPushButton("Area 1", this);
    m_area2Button = new QPushButton("Area 2", this);
    m_area1Label = new QLabel("Area 1: [ACTIVE]", this);
    m_area2Label = new QLabel("Area 2:", this);

    // Set initial button states
    m_area1Button->setEnabled(false);
    m_area1Label->setStyleSheet("font-weight: bold; color: red;");

    // Get the main layout and insert buttons at the top
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        // Create button layout
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(m_area1Label);
        buttonLayout->addWidget(m_area1Button);
        buttonLayout->addWidget(m_area2Label);
        buttonLayout->addWidget(m_area2Button);
        buttonLayout->addStretch();

        // Insert at the top (index 0)
        mainLayout->insertLayout(0, buttonLayout);
    }

    // Connect button signals
    connect(m_area1Button, &QPushButton::clicked, this, &MainWidget::onArea1ButtonClicked);
    connect(m_area2Button, &QPushButton::clicked, this, &MainWidget::onArea2ButtonClicked);
}

void MainWidget::setMenuWidget(MenuWidget *menuWidget)
{
    if (m_menuWidget) {
        // Disconnect old signals
        disconnect(m_menuWidget, nullptr, this, nullptr);

        // Remove old menu widget if exists
        ui->menuWidgetContainer->layout()->removeWidget(m_menuWidget);
        m_menuWidget->setParent(nullptr);
    }

    m_menuWidget = menuWidget;

    if (m_menuWidget) {
        // Create layout if not exists
        if (!ui->menuWidgetContainer->layout()) {
            QVBoxLayout *layout = new QVBoxLayout(ui->menuWidgetContainer);
            layout->setContentsMargins(0, 0, 0, 0);
            ui->menuWidgetContainer->setLayout(layout);
        }

        // Add new menu widget
        m_menuWidget->setParent(ui->menuWidgetContainer);
        ui->menuWidgetContainer->layout()->addWidget(m_menuWidget);

        // Connect signal from MenuWidget
        connect(m_menuWidget, &MenuWidget::tabSelectionChanged,
                this, &MainWidget::onMenuTabSelectionChanged);
    }
}

void MainWidget::initializeAreas()
{
    // Initialize both areas with their default widgets
    updateAreaDisplay(0);  // Area 1
    updateAreaDisplay(1);  // Area 2
}

void MainWidget::onArea1ButtonClicked()
{
    switchToArea(0);
}

void MainWidget::onArea2ButtonClicked()
{
    switchToArea(1);
}

void MainWidget::switchToArea(int areaIndex)
{
    if (areaIndex < 0 || areaIndex > 1 || m_currentArea == areaIndex) {
        return;
    }

    // Update current area
    m_currentArea = areaIndex;

    // Update button states
    m_area1Button->setEnabled(areaIndex != 0);
    m_area2Button->setEnabled(areaIndex != 1);

    // Update labels
    if (areaIndex == 0) {
        m_area1Label->setText("Area 1: [ACTIVE]");
        m_area1Label->setStyleSheet("font-weight: bold; color: red;");
        m_area2Label->setText("Area 2:");
        m_area2Label->setStyleSheet("");
    } else {
        m_area1Label->setText("Area 1:");
        m_area1Label->setStyleSheet("");
        m_area2Label->setText("Area 2: [ACTIVE]");
        m_area2Label->setStyleSheet("font-weight: bold; color: blue;");
    }

    // Update menu tabs to match the new area's indices
    if (m_menuWidget) {
        int level1Index = m_areaLevel1Index[areaIndex];
        int level2Index = m_areaLevel2Index[areaIndex];
        m_menuWidget->setCurrentTabs(level1Index, level2Index);
    }
}

void MainWidget::onMenuTabSelectionChanged(int level1Index, int level2Index)
{
    // Save the indices for the current area
    m_areaLevel1Index[m_currentArea] = level1Index;
    m_areaLevel2Index[m_currentArea] = level2Index;

    // Update display for the current area
    updateAreaDisplay(m_currentArea);
}

void MainWidget::updateAreaDisplay(int areaIndex)
{
    if (!m_menuWidget) {
        return;
    }

    int level1Index = m_areaLevel1Index[areaIndex];
    int level2Index = m_areaLevel2Index[areaIndex];

    // Get the content widget for this area's indices
    CustomWidget *contentWidget = m_menuWidget->getContentWidget(level1Index, level2Index);
    if (!contentWidget) {
        m_areaContainers[areaIndex]->hideAll();
        return;
    }

    // Check if this widget is being displayed in the other area
    int otherArea = (areaIndex == 0) ? 1 : 0;
    bool isUsedByOtherArea = m_areaContainers[otherArea]->getWidgets().contains(contentWidget)
                             && contentWidget->isVisible();

    if (isUsedByOtherArea) {
        // Widget is being used by other area, don't take it - hide all widgets in this area
        m_areaContainers[areaIndex]->hideAll();
    } else {
        // Widget is not being used by other area, we can use it
        // Detach from other area if needed (in case it was attached but hidden)
        m_areaContainers[otherArea]->detach(contentWidget);

        // Set parent and attach to this area
        contentWidget->setParent(m_areaContainers[areaIndex]);
        if (!m_areaContainers[areaIndex]->getWidgets().contains(contentWidget)) {
            m_areaContainers[areaIndex]->attach(contentWidget);
        }
        m_areaContainers[areaIndex]->show(contentWidget);
    }
}
