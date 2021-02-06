#ifndef TEST_BROADCASTER
#define TEST_BROADCASTER

#include <QTest>
#include <QSignalSpy>
#include "../Broadcaster/include/broadcaster.h"
#include "../QtCommunicator/include/ioserver.h"
#include "../IncludeSpdlog/include/spdlog.h"

class TestBroadcaster : public QObject {
	Q_OBJECT

private slots:
	void BasicOperation();

private slots:
	void t1() { QVERIFY(true); }
	void t2();

signals:
	void subscribe1(const qint32 topic);
	void subscribe2(const qint32 topic);
};


#endif // TEST_BROADCASTER