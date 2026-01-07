#include <QApplication>
#include <QMainWindow>
#include "src/widgets/MainWidget.h"
#include "src/widgets/MenuWidget.h"
#include "src/widgets/CustomWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Test MainWidget API");
    mainWindow.resize(1000, 800);

    // ========================================
    // TEST: Create MainWidget (from library)
    // ========================================
    MainWidget *mainWidget = new MainWidget(&mainWindow);
    mainWindow.setCentralWidget(mainWidget);

    // ========================================
    // TEST: Create MenuWidget and add tabs
    // ========================================
    MenuWidget *menuWidget = new MenuWidget();

    // Add Level 1 tabs (Categories)
    menuWidget->addLevel1Tab("Electronics");
    menuWidget->addLevel1Tab("Clothing");
    menuWidget->addLevel1Tab("Books");
    menuWidget->addLevel1Tab("Sports");

    // Add Level 2 tabs for Electronics (index 0)
    menuWidget->addLevel2Tab(0, "Smartphones", new CustomWidget("📱 Smartphones Content\n\nLatest models available:\n- iPhone 15 Pro\n- Samsung Galaxy S24\n- Google Pixel 8"));
    menuWidget->addLevel2Tab(0, "Laptops", new CustomWidget("💻 Laptops Content\n\nTop picks:\n- MacBook Pro M3\n- Dell XPS 15\n- ThinkPad X1 Carbon"));
    menuWidget->addLevel2Tab(0, "Headphones", new CustomWidget("🎧 Headphones Content\n\nBest sellers:\n- Sony WH-1000XM5\n- AirPods Pro\n- Bose QuietComfort"));
    menuWidget->addLevel2Tab(0, "Cameras", new CustomWidget("📷 Cameras Content\n\nProfessional gear:\n- Sony A7 IV\n- Canon R6\n- Nikon Z8"));

    // Add Level 2 tabs for Clothing (index 1)
    menuWidget->addLevel2Tab(1, "Men", new CustomWidget("👔 Men's Clothing\n\nCategories:\n- Shirts\n- Pants\n- Shoes\n- Accessories"));
    menuWidget->addLevel2Tab(1, "Women", new CustomWidget("👗 Women's Clothing\n\nCategories:\n- Dresses\n- Tops\n- Skirts\n- Accessories"));
    menuWidget->addLevel2Tab(1, "Kids", new CustomWidget("👶 Kids Clothing\n\nCategories:\n- Boys\n- Girls\n- Toddlers\n- Babies"));

    // Add Level 2 tabs for Books (index 2)
    menuWidget->addLevel2Tab(2, "Fiction", new CustomWidget("📚 Fiction Books\n\nBestsellers:\n- Mystery\n- Romance\n- Sci-Fi\n- Fantasy"));
    menuWidget->addLevel2Tab(2, "Non-Fiction", new CustomWidget("📖 Non-Fiction Books\n\nCategories:\n- Biography\n- History\n- Science\n- Self-Help"));
    menuWidget->addLevel2Tab(2, "Technical", new CustomWidget("💾 Technical Books\n\nPopular topics:\n- Programming\n- AI/ML\n- Cloud Computing\n- Cybersecurity"));

    // Add Level 2 tabs for Sports (index 3)
    menuWidget->addLevel2Tab(3, "Football", new CustomWidget("⚽ Football Equipment\n\nEssentials:\n- Balls\n- Boots\n- Jerseys\n- Shin Guards"));
    menuWidget->addLevel2Tab(3, "Basketball", new CustomWidget("🏀 Basketball Equipment\n\nEssentials:\n- Balls\n- Shoes\n- Jerseys\n- Hoops"));
    menuWidget->addLevel2Tab(3, "Tennis", new CustomWidget("🎾 Tennis Equipment\n\nEssentials:\n- Rackets\n- Balls\n- Shoes\n- Bags"));
    menuWidget->addLevel2Tab(3, "Swimming", new CustomWidget("🏊 Swimming Equipment\n\nEssentials:\n- Swimsuits\n- Goggles\n- Caps\n- Fins"));

    // ========================================
    // TEST: Set MenuWidget to MainWidget
    // ========================================
    mainWidget->setMenuWidget(menuWidget);
    mainWidget->initializeAreas();

    // ========================================
    // TEST: Rename tabs using MenuWidget reference
    // ========================================

    // Use the menuWidget reference we already have (no need to get it back!)
    menuWidget->setLevel1TabText(0, "🔌 Electronics");
    menuWidget->setLevel1TabText(1, "👕 Clothing");
    menuWidget->setLevel1TabText(2, "📚 Books");
    menuWidget->setLevel1TabText(3, "⚽ Sports");

    // Rename some items (Level 2)
    menuWidget->setLevel2TabText(0, 0, "📱 Phones");
    menuWidget->setLevel2TabText(0, 1, "💻 Computers");
    menuWidget->setLevel2TabText(0, 2, "🎧 Audio");
    menuWidget->setLevel2TabText(0, 3, "📸 Photography");

    menuWidget->setLevel2TabText(1, 0, "👔 Men's Fashion");
    menuWidget->setLevel2TabText(1, 1, "👗 Women's Fashion");
    menuWidget->setLevel2TabText(1, 2, "👶 Children's Fashion");

    menuWidget->setLevel2TabText(2, 0, "📖 Fiction Novels");
    menuWidget->setLevel2TabText(2, 1, "📊 Non-Fiction");
    menuWidget->setLevel2TabText(2, 2, "💻 Tech Books");

    menuWidget->setLevel2TabText(3, 0, "⚽ Soccer");
    menuWidget->setLevel2TabText(3, 1, "🏀 B-Ball");
    menuWidget->setLevel2TabText(3, 2, "🎾 Tennis");
    menuWidget->setLevel2TabText(3, 3, "🏊 Swimming");

    // ========================================
    // Show window
    // ========================================
    mainWindow.show();

    return app.exec();
}
