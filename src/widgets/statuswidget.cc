#include <QtWidgets>

#include "statuswidget.h"

constexpr auto TIME{ "Time" };
constexpr auto ERROR_DATA{ "Error" };

StatusWidget::StatusWidget(QJsonObject const& a_config)
	:m_ping(0)
	,m_error(0)
{
	setAlignment(Qt::AlignCenter);
	setEnabled(true);
	this->setText("status");
}

void StatusWidget::onUpdate()
{
	QString _name = "Ping:" + QString::number(m_ping) + "ms   " + "Error:" + QString::number(m_error);
	this->setText(_name);
	this->update();
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::red);
	this->setPalette(pa);

	QFont ft;
	ft.setPointSize(14);
	this->setFont(ft);

	setStyleSheet("color:red;");
	Logger->info("StatusWidget::onUpdate() status:{}", _name.toStdString());
}

void StatusWidget::onUpdatePing(QJsonObject ping)
{
	qint32  old = ping[TIME].toInt();
	qint32 now = int( QDateTime::currentMSecsSinceEpoch());

	QDateTime deltaOld = QDateTime::fromMSecsSinceEpoch(old);
	QDateTime deltaNew = QDateTime::fromMSecsSinceEpoch(now);

	QDateTime nowData = QDateTime::currentDateTime();
	qint64 delta = deltaOld.msecsTo(deltaNew);
	m_ping = delta;
}

void StatusWidget::onUpdateError(QJsonObject error)
{
	m_error = error[ERROR_DATA].toInt();
}

void StatusWidget::configure(QJsonObject const& a_config)
{}