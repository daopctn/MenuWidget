#ifndef CONTAINER_H
#define CONTAINER_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>

class Container : public QWidget
{
    Q_OBJECT

public:
    explicit Container(QWidget *parent = nullptr);
    ~Container();

    // Attach a widget to the container
    void attach(QWidget *widget);

    // Detach a widget from the container
    void detach(QWidget *widget);

    // Show a specific widget (hide all others)
    void show(QWidget *widget);

    // Hide all widgets
    void hideAll();

    // Get all attached widgets
    QList<QWidget*> getWidgets() const;

private:
    QVBoxLayout *m_layout;
    QList<QWidget*> m_widgets;
};

#endif // CONTAINER_H
