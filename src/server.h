#ifndef SERVER_H
#define SERVER_H

#include <QDebug>
#include <QJsonObject>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <deque>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "structures.h"


class Server : public QObject {
	Q_OBJECT
public:
	explicit Server(QJsonObject m_config);

signals:

public slots:
	void onUpdate();
	void configure(QJsonObject const& a_config);
	
private:
	// Configs:
	QJsonObject m_config;
	QTcpServer * m_server;
	QTcpSocket *m_socket;
	QByteArray m_frameBuffer;

private:
	cv::Mat m_frame;
	int m_frameSize;
	int m_bytesReceived;
};

#endif // SERVER_H