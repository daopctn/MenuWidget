#include "CustomWidget.h"

CustomWidget::CustomWidget(const QString &text, QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    m_label = new QLabel(text, this);
    m_label->setAlignment(Qt::AlignCenter);

    m_layout->addWidget(m_label);
    setLayout(m_layout);
}

CustomWidget::~CustomWidget()
{
}

void CustomWidget::setText(const QString &text)
{
    m_label->setText(text);
}

QString CustomWidget::getText() const
{
    return m_label->text();
}
