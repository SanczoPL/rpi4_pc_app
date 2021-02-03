#ifndef TEST_BROADCASTER
#define TEST_BROADCASTER

#include <QTest>
#include "../Broadcaster/include/broadcaster.h"
#include "../QtCommunicator/src/ioserver.h"

class TestBroadcaster : public QObject {
	Q_OBJECT

private:
	void BasicOperation();

private slots:
	void t1() { QVERIFY(true); }
	void t2();
};


#endif // TEST_BROADCASTER