#include <QtWidgets>

#include "statuswidget.h"

StatusWidget::StatusWidget(QJsonObject const& a_config)
	:m_ping(1000)
{
	setAlignment(Qt::AlignCenter);
	setEnabled(true);
	this->setText(QString::number(m_ping));

	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::red);
	this->setPalette(pa);

	QFont ft;
	ft.setPointSize(14);
	this->setFont(ft);

	setStyleSheet("color:red;");
}

void StatusWidget::onUpdate()
{

}

void StatusWidget::onUpdatePing(qint64 ping)
{
	Logger->info("StatusWidget::onUpdatePing()");
	m_ping = ping;
	this->setText(QString::number(m_ping));
}

void StatusWidget::configure(QJsonObject const& a_config)
{

}