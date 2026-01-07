#include "MainWindow.h"
#include "widgets/MainWidget.h"
#include "widgets/MenuWidget.h"
#include "widgets/CustomWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_mainWidget(nullptr)
    , m_menuWidget(nullptr)
{
    // Create MainWidget
    m_mainWidget = new MainWidget(this);
    setCentralWidget(m_mainWidget);

    // Setup MenuWidget
    setupMenuWidget();

    // Set window properties
    setWindowTitle("MenuWidget Demo - Main Window");
    resize(900, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupMenuWidget()
{
    // Create MenuWidget
    m_menuWidget = new MenuWidget(this);

    // Add level 1 tabs
    m_menuWidget->addLevel1Tab("Category 1");
    m_menuWidget->addLevel1Tab("Category 2");
    m_menuWidget->addLevel1Tab("Category 3");

    // Add level 2 tabs for Category 1 (index 0)
    m_menuWidget->addLevel2Tab(0, "Item 1-1", new CustomWidget("Content for Category 1 - Item 1"));
    m_menuWidget->addLevel2Tab(0, "Item 1-2", new CustomWidget("Content for Category 1 - Item 2"));
    m_menuWidget->addLevel2Tab(0, "Item 1-3", new CustomWidget("Content for Category 1 - Item 3"));

    // Add level 2 tabs for Category 2 (index 1)
    m_menuWidget->addLevel2Tab(1, "Item 2-1", new CustomWidget("Content for Category 2 - Item 1"));
    m_menuWidget->addLevel2Tab(1, "Item 2-2", new CustomWidget("Content for Category 2 - Item 2"));

    // Add level 2 tabs for Category 3 (index 2)
    m_menuWidget->addLevel2Tab(2, "Item 3-1", new CustomWidget("Content for Category 3 - Item 1"));
    m_menuWidget->addLevel2Tab(2, "Item 3-2", new CustomWidget("Content for Category 3 - Item 2"));
    m_menuWidget->addLevel2Tab(2, "Item 3-3", new CustomWidget("Content for Category 3 - Item 3"));
    m_menuWidget->addLevel2Tab(2, "Item 3-4", new CustomWidget("Content for Category 3 - Item 4"));

    // Set MenuWidget to MainWidget
    m_mainWidget->setMenuWidget(m_menuWidget);

    // Initialize both areas with their default widgets
    m_mainWidget->initializeAreas();
}
