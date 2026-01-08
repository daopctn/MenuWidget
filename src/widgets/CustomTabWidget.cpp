#include "CustomTabWidget.h"
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

int CustomTabWidget::addTab(const QString &label, QWidget *content)
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
    }

    return index;
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
    int index = getTabIndex(label);
    if (index < 0) {
        return nullptr;
    }
    return widget(index);  // QTabWidget::widget()
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
}

void CustomTabWidget::setTabText(const QString &oldLabel, const QString &newText)
{
    int index = getTabIndex(oldLabel);
    if (index >= 0) {
        setTabText(index, newText);
    }
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
}
