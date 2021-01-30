#include <QtWidgets>

#include "statuswidget.h"

StatusWidget::StatusWidget(QJsonObject const& a_config)
{
	setAlignment(Qt::AlignCenter);
	setEnabled(true);
}

void StatusWidget::onUpdate()
{

}

void StatusWidget::onUpdatePing(qint64 ping)
{
	m_ping = ping;
}

void StatusWidget::configure(QJsonObject const& a_config)
{

}