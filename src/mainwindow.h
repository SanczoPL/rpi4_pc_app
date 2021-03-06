#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "structures.h"
#include "widgets/imagewidget.h"
#include "widgets/statuswidget.h"
#include "server.h"
#include "mainwindow.h"

#include "broadcaster.h"

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

private slots:
	void onUpdate();
	void configure(QJsonObject const& a_config);
	void onNewMessage(QJsonObject const& json);

public slots:
	void onUpdateImage(QByteArray image, qint32 topic);

private:
	QStatusBar* m_statusBar;
	ImageWidget* m_imageWidget;
	StatusWidget* m_statusWidget;
	ImageWidget* m_imageProcessingWidget;

private:
	const qint32 m_threadsMax{};

	QThread* m_serverThread;
	Broadcaster* m_server;

	QThread* m_mainWindowThread;
	MainWindow* m_mainWindow;

private:
	QJsonObject m_config;

private:
	QTimer* m_timer;

private:
	// Bools:
	bool m_register{};
	bool m_recvTask{};
	bool m_validTask{};
	bool m_firstTime{};
};

#endif