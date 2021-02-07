#include <QtWidgets>

#include "statuswidget.h"

constexpr auto TIME{ "Time" };
constexpr auto ERROR_DATA{ "Error" };

StatusWidget::StatusWidget(QJsonObject const& a_config)
	:m_ping(1000)
	,m_error(0)
{
	setAlignment(Qt::AlignCenter);
	setEnabled(true);

}

void StatusWidget::onUpdate()
{
	this->setText("Ping:" + QString::number(m_ping) + "ms   " + "Error:" + QString::number(m_error));
	this->update();
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::red);
	this->setPalette(pa);

	QFont ft;
	ft.setPointSize(14);
	this->setFont(ft);

	setStyleSheet("color:red;");
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

	m_ping = diff;

}

void StatusWidget::onUpdateError(QJsonObject error)
{
	m_error = error[ERROR_DATA].toInt();


}

void StatusWidget::configure(QJsonObject const& a_config)
{

}