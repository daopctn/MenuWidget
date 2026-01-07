#include "Container.h"

Container::Container(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
}

Container::~Container()
{
}

void Container::attach(QWidget *widget)
{
    if (!widget || m_widgets.contains(widget)) {
        return;
    }

    // Add widget to layout and list
    m_layout->addWidget(widget);
    m_widgets.append(widget);

    // Hide the widget by default
    widget->hide();
}

void Container::detach(QWidget *widget)
{
    if (!widget || !m_widgets.contains(widget)) {
        return;
    }

    // Remove from layout and list
    m_layout->removeWidget(widget);
    m_widgets.removeOne(widget);

    // The widget is not deleted, just removed from container
    widget->setParent(nullptr);
}

void Container::show(QWidget *widget)
{
    if (!widget || !m_widgets.contains(widget)) {
        return;
    }

    // Hide all widgets first
    hideAll();

    // Show the specified widget
    widget->show();
}

void Container::hideAll()
{
    for (QWidget *widget : m_widgets) {
        if (widget) {
            widget->hide();
        }
    }
}

QList<QWidget*> Container::getWidgets() const
{
    return m_widgets;
}
