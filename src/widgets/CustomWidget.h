#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class CustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomWidget(const QString &text, QWidget *parent = nullptr);
    ~CustomWidget();

    void setText(const QString &text);
    QString getText() const;

private:
    QLabel *m_label;
    QVBoxLayout *m_layout;
};

#endif // CUSTOMWIDGET_H
