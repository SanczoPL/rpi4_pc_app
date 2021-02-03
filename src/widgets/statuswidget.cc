#include <QtWidgets>

#include "statuswidget.h"

constexpr auto TIME{ "Time" };

StatusWidget::StatusWidget(QJsonObject const& a_config)
	:m_ping(1000)
{
	setAlignment(Qt::AlignCenter);
	setEnabled(true);

}

void StatusWidget::onUpdate()
{

}

void StatusWidget::onUpdatePing(QJsonObject ping)
{
	QString pastTime = ping[TIME].toString();


	Logger->info("StatusWidget::onUpdatePing() ping:{}", ping[TIME].toString().toStdString());
	QDateTime nowData = QDateTime::currentDateTime();
	QDateTime pastData = QDateTime::fromString(pastTime, "hh:mm:ss AP dd/MM/yyyy");
	quint64 diff = pastData.msecsTo(nowData);
	
	Logger->info("StatusWidget::onUpdatePing() nowData:{}", nowData.toMSecsSinceEpoch());
	Logger->info("StatusWidget::onUpdatePing() pastData:{}", pastData.toMSecsSinceEpoch());
	Logger->info("StatusWidget::onUpdatePing() diff:{}", diff);
	this->setText(QString::number(diff));
	this->update();
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::red);
	this->setPalette(pa);

	QFont ft;
	ft.setPointSize(14);
	this->setFont(ft);

	setStyleSheet("color:red;");

}

void StatusWidget::configure(QJsonObject const& a_config)
{

}