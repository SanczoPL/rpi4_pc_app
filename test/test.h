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
private slots:
	void t3()
	{
		QTest::addColumn<QString>("string");
		QTest::addColumn<QString>("result");

		QTest::newRow("all lower") << "hello" << "HELLO";
		QTest::newRow("mixed") << "Hello" << "HELLO";
		QTest::newRow("all upper") << "HELLO" << "HELLO";
	}
};


#endif // TEST_BROADCASTER