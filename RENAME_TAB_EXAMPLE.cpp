// ========================================
// EXAMPLE: How to rename tabs dynamically
// ========================================
// IMPORTANT: MenuWidget is kept PRIVATE for good encapsulation!
// Use MainWindow's public wrapper methods instead.
// ========================================

// Assuming you have a MainWindow instance:
MainWindow *mainWindow = ...;

// ========================================
// 1. Rename Category (Level 1 Tab)
// ========================================

// Rename "Category 1" to "Products"
mainWindow->renameCategory(0, "Products");

// Rename "Category 2" to "Services"
mainWindow->renameCategory(1, "Services");

// Rename "Category 3" to "About"
mainWindow->renameCategory(2, "About");

// ========================================
// 2. Rename Item (Level 2 Tab)
// ========================================

// Rename "Item 1-1" to "iPhone"
// Parameters: (categoryIndex, itemIndex, newText)
mainWindow->renameItem(0, 0, "iPhone");

// Rename "Item 1-2" to "iPad"
mainWindow->renameItem(0, 1, "iPad");

// Rename "Item 1-3" to "MacBook"
mainWindow->renameItem(0, 2, "MacBook");

// Rename items in Category 2
mainWindow->renameItem(1, 0, "Consulting");
mainWindow->renameItem(1, 1, "Development");

// ========================================
// 3. Dynamic renaming example
// ========================================

void MainWindow::onUserClickedRenameButton() {
    // Get new name from user (e.g., QInputDialog)
    QString newName = QInputDialog::getText(this, "Rename Tab", "Enter new name:");

    if (!newName.isEmpty()) {
        // Rename current category
        int currentCategory = 0; // Get from your current state
        renameCategory(currentCategory, newName);
    }
}

// ========================================
// 4. Batch rename from database/config
// ========================================

void MainWindow::loadTabNamesFromDatabase() {
    // Example: Load from database
    QMap<int, QString> categoryNames = database->getCategoryNames();

    for (auto it = categoryNames.begin(); it != categoryNames.end(); ++it) {
        int index = it.key();
        QString name = it.value();
        renameCategory(index, name);
    }

    // Load item names
    for (int categoryIndex = 0; categoryIndex < 3; ++categoryIndex) {
        QMap<int, QString> itemNames = database->getItemNames(categoryIndex);

        for (auto it = itemNames.begin(); it != itemNames.end(); ++it) {
            int itemIndex = it.key();
            QString itemName = it.value();
            renameItem(categoryIndex, itemIndex, itemName);
        }
    }
}

// ========================================
// 5. Validation notes
// ========================================

// Invalid indices are silently ignored:
mainWindow->renameCategory(-1, "Invalid");          // Does nothing
mainWindow->renameCategory(999, "Invalid");         // Does nothing
mainWindow->renameItem(0, -1, "Invalid");           // Does nothing
mainWindow->renameItem(0, 999, "Invalid");          // Does nothing

// ========================================
// COMPLETE EXAMPLE IN MainWindow.cpp
// ========================================
/*
void MainWindow::setupMenuWidget() {
    // Create MenuWidget (private member)
    m_menuWidget = new MenuWidget(this);

    // Add tabs with initial names
    m_menuWidget->addLevel1Tab("Category 1");
    m_menuWidget->addLevel1Tab("Category 2");
    m_menuWidget->addLevel1Tab("Category 3");

    m_menuWidget->addLevel2Tab(0, "Item 1-1", new CustomWidget("Content 1-1"));
    m_menuWidget->addLevel2Tab(0, "Item 1-2", new CustomWidget("Content 1-2"));
    m_menuWidget->addLevel2Tab(0, "Item 1-3", new CustomWidget("Content 1-3"));

    // ... more tabs ...

    // Set MenuWidget to MainWidget
    m_mainWidget->setMenuWidget(m_menuWidget);
    m_mainWidget->initializeAreas();

    // ========================================
    // NOW RENAME TABS DYNAMICALLY (using public API)
    // ========================================

    // Rename categories
    renameCategory(0, "Products");
    renameCategory(1, "Services");
    renameCategory(2, "Company");

    // Rename items under "Products"
    renameItem(0, 0, "iPhone 15 Pro");
    renameItem(0, 1, "iPad Air");
    renameItem(0, 2, "MacBook Pro M3");

    // The UI will update immediately!
}
*/

// ========================================
// DESIGN PATTERN: ENCAPSULATION
// ========================================
/*
 * WHY this design is good:
 *
 * 1. MenuWidget is PRIVATE in MainWindow
 *    - External code cannot directly access it
 *    - Prevents tight coupling
 *    - Easy to refactor internal implementation
 *
 * 2. Public wrapper methods in MainWindow:
 *    - renameCategory(index, name)
 *    - renameItem(categoryIndex, itemIndex, name)
 *
 * 3. Benefits:
 *    - Clear, simple API for external use
 *    - MainWindow controls all access to MenuWidget
 *    - Can add validation, logging, signals in wrappers
 *    - Can switch internal implementation without breaking external code
 *
 * 4. Usage from main.cpp or other classes:
 *    MainWindow mainWindow;
 *    mainWindow.renameCategory(0, "New Name");  // Clean API!
 *    // Cannot do: mainWindow.m_menuWidget->...  // ERROR: private!
 */
