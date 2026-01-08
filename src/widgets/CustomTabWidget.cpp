#include "CustomTabWidget.h"
#include "Container.h"
#include <QLabel>
#include <QVBoxLayout>

CustomTabWidget::CustomTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    // Connect to QTabWidget's currentChanged signal
    connect(this, QOverload<int>::of(&QTabWidget::currentChanged),
            this, &CustomTabWidget::onCurrentChanged);
}

CustomTabWidget::~CustomTabWidget()
{
    // Clean up fake content widgets
    qDeleteAll(m_fakeContents);
    m_fakeContents.clear();
}

int CustomTabWidget::addTab(const QString &label, QWidget *content, Container *container)
{
    // Check for duplicate label
    if (m_nameToIndex.contains(label)) {
        qWarning("CustomTabWidget::addTab - Duplicate label: %s", qPrintable(label));
        return -1;
    }

    // If no content provided, create fake content
    if (!content) {
        content = createFakeContent();
    }

    // Add tab using QTabWidget's method
    int index = QTabWidget::addTab(content, label);

    if (index >= 0) {
        // Update dual mapping
        m_nameToIndex[label] = index;
        m_indexToName[index] = label;
        m_tabIndexMapping[index] = label;  // For compatibility

        // Store content widget
        m_mapWgContents[label] = content;

        // Store container if provided
        if (container) {
            m_mapWgContainer[label] = container;
            // Attach widget to container with label as key
            container->attach(content);
        }
    }

    return index;
}

int CustomTabWidget::addSubTabWidget(const QString &label, CustomTabWidget *subTabWidget, Container *container)
{
    // Check for duplicate label
    if (m_nameToIndex.contains(label)) {
        qWarning("CustomTabWidget::addSubTabWidget - Duplicate label: %s", qPrintable(label));
        return -1;
    }

    if (!subTabWidget) {
        qWarning("CustomTabWidget::addSubTabWidget - subTabWidget is nullptr");
        return -1;
    }

    // Add tab using QTabWidget's method
    int index = QTabWidget::addTab(subTabWidget, label);

    if (index >= 0) {
        // Update dual mapping
        m_nameToIndex[label] = index;
        m_indexToName[index] = label;
        m_subTabIndexMapping[index] = label;  // For sub tabs

        // Store nested tab widget
        m_mapSubTabContents[label] = subTabWidget;

        // Store container if provided
        if (container) {
            m_mapSubTabContainer[label] = container;
            // Attach nested widget to container with label as key
            container->attach(subTabWidget);
        }
    }

    return index;
}

void CustomTabWidget::setTabWidth(int singleTabWidth)
{
    // Set fixed width for all tabs
    QString styleSheet = QString("QTabBar::tab { width: %1px; }").arg(singleTabWidth);
    tabBar()->setStyleSheet(styleSheet);
}

int CustomTabWidget::getTabIndex(const QString &label) const
{
    return m_nameToIndex.value(label, -1);
}

QString CustomTabWidget::getTabLabel(int index) const
{
    return m_indexToName.value(index, QString());
}

QWidget* CustomTabWidget::getContent(const QString &label) const
{
    return m_mapWgContents.value(label, nullptr);
}

Container* CustomTabWidget::getContainer(const QString &label) const
{
    // Check both regular container and sub tab container maps
    if (m_mapWgContainer.contains(label)) {
        return m_mapWgContainer[label];
    }
    return m_mapSubTabContainer.value(label, nullptr);
}

CustomTabWidget* CustomTabWidget::getSubTabWidget(const QString &label) const
{
    return m_mapSubTabContents.value(label, nullptr);
}

QString CustomTabWidget::currentLabel() const
{
    int index = currentIndex();  // QTabWidget::currentIndex()
    return getTabLabel(index);
}

void CustomTabWidget::setCurrentTab(const QString &label)
{
    int index = getTabIndex(label);
    if (index >= 0) {
        setCurrentIndex(index);  // QTabWidget::setCurrentIndex()
    }
}

void CustomTabWidget::setTabText(int index, const QString &newText)
{
    if (index < 0 || index >= count()) {
        return;
    }

    // Get old label
    QString oldLabel = m_indexToName.value(index, QString());

    // Check if new text conflicts with existing tabs
    if (m_nameToIndex.contains(newText) && m_nameToIndex[newText] != index) {
        qWarning("CustomTabWidget::setTabText - New text conflicts with existing tab: %s", qPrintable(newText));
        return;
    }

    // Update tab bar using QTabWidget's method
    QTabWidget::setTabText(index, newText);

    // Update dual mapping
    updateDualMapping(index, oldLabel, newText);

    // Update content mappings
    if (m_mapWgContents.contains(oldLabel)) {
        QWidget *content = m_mapWgContents[oldLabel];
        m_mapWgContents.remove(oldLabel);
        m_mapWgContents[newText] = content;
    }

    if (m_mapWgContainer.contains(oldLabel)) {
        Container *container = m_mapWgContainer[oldLabel];
        m_mapWgContainer.remove(oldLabel);
        m_mapWgContainer[newText] = container;
    }

    if (m_mapSubTabContents.contains(oldLabel)) {
        CustomTabWidget *subTab = m_mapSubTabContents[oldLabel];
        m_mapSubTabContents.remove(oldLabel);
        m_mapSubTabContents[newText] = subTab;
    }

    if (m_mapSubTabContainer.contains(oldLabel)) {
        Container *container = m_mapSubTabContainer[oldLabel];
        m_mapSubTabContainer.remove(oldLabel);
        m_mapSubTabContainer[newText] = container;
    }
}

void CustomTabWidget::setTabText(const QString &oldLabel, const QString &newText)
{
    int index = getTabIndex(oldLabel);
    if (index >= 0) {
        setTabText(index, newText);
    }
}

void CustomTabWidget::onIndexViewChanged(int index)
{
    // Compatibility slot for STabWidget API
    setCurrentIndex(index);
}

void CustomTabWidget::onCurrentChanged(int index)
{
    if (index < 0) {
        return;
    }

    // Emit name-based signal
    QString label = getTabLabel(index);
    if (!label.isEmpty()) {
        emit currentTabChanged(label);
    }

    // Emit index-based signal for compatibility
    emit tabIndexChanged(index);
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

void CustomTabWidget::updateDualMapping(int index, const QString &oldLabel, const QString &newLabel)
{
    // Remove old mapping
    if (!oldLabel.isEmpty()) {
        m_nameToIndex.remove(oldLabel);
    }

    // Add new mapping
    m_nameToIndex[newLabel] = index;
    m_indexToName[index] = newLabel;
    m_tabIndexMapping[index] = newLabel;
}
