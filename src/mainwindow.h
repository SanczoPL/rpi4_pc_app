#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "structures.h"
#include "widgets/imagewidget.h"
#include "widgets/statuswidget.h"
#include "server.h"
#include "mainwindow.h"

#include "../Broadcaster/include/broadcaster.h"

#include <QGraphicsItem>
#include <QJsonObject>
#include <QList>
#include <QMainWindow>
#include <QtWidgets>





class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QJsonObject const& a_config);
	

private:
	void setupButtons();
	void setupMainWidget();
	void createStartupThreads();
	void onPing(QJsonObject const& json);

private slots:
	void onUpdate();
	void configure(QJsonObject const& a_config);
	void onNewMessage(QJsonObject const& json);


signals:
	void updatePing(qint64 ping);

public slots:
	

private:


	QStatusBar* m_statusBar;
	ImageWidget* m_imageWidget;
	StatusWidget* m_statusWidget;

private:
	// QWidgets:

private:
	// Threads:
	const qint32 m_threadsMax{};

	QThread* m_serverThread;
	Broadcaster* m_server;

	QThread* m_mainWindowThread;
	MainWindow* m_mainWindow;

private:
	// Configs:
	QJsonObject m_config;


private:
	// Timers:
	QTimer* m_timer;

private:
	// Bools:
	bool m_register{};
	bool m_recvTask{};
	bool m_validTask{};
	bool m_firstTime{};
};

#endif
