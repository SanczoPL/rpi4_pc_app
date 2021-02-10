#ifndef TEST_BROADCASTER
#define TEST_BROADCASTER

#include <QTest>
#include <QSignalSpy>
#include "broadcaster.h"
#include "ioserver.h"
#include "spdlog.h"

class TestBroadcaster : public QObject {
	Q_OBJECT

private slots:
	void BasicOperation();

signals:
	void subscribe1(const qint32 topic);
	void subscribe2(const qint32 topic);

private:
	void waitForSignal(int timeout, QSignalSpy& spy, int deltaTime, int signalToCatch);
};


#endif // TEST_BROADCASTER