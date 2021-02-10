#include "test_broadcaster.h"

constexpr auto PING{ "Ping" };
constexpr auto PING_PONG{ "PingPong" };
constexpr auto MESSAGE_TYPE{ "MessageType" };
constexpr auto ID{ "Id" };
constexpr auto IP{ "Ip" };
constexpr auto PORT{ "Port" };

void TestBroadcaster::BasicOperation()
{
	qRegisterMetaType<QVector<qint32>>("QVector<qint32>");
	qRegisterMetaType<qint32>("qint32");
	Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");
	Logger->warn("Start main logger with LogLevel:{}", 0);
	Logger->set_level(static_cast<spdlog::level::level_enum>(0));
	QJsonObject config1{
		{PORT, 12345},
		{IP, "127.0.0.1"},
		{"InfoTopic", 1},
		{"CommandTopic", 2},
		{"ImageTopic", 3},
		{ID, 1}
	};
	QJsonObject config2{ config1 };
	config2[ID] = 2;

	QJsonObject config3{ config1 };
	config3[ID] = 3;

	IOServer* m_server = new IOServer();
	Logger->debug("m_server");
	QSignalSpy spy0(m_server, SIGNAL(listenForConnection()));

	m_server->configure(config3);

	waitForSignal(1000, spy0, 50, 1);

	QCOMPARE(spy0.count(), 1);

	QThread* m_broadcaster1 = new QThread();
	Broadcaster* _broadcaster1 = new Broadcaster(config1);
	_broadcaster1->moveToThread(m_broadcaster1);
	connect(m_broadcaster1, &QThread::finished, _broadcaster1, &QObject::deleteLater);
	m_broadcaster1->start();

	QThread* m_broadcaster2 = new QThread();
	Broadcaster* _broadcaster2 = new Broadcaster(config2);
	_broadcaster2->moveToThread(m_broadcaster2);
	connect(m_broadcaster2, &QThread::finished, _broadcaster2, &QObject::deleteLater);
	m_broadcaster2->start();

	QSignalSpy spy1(m_server, SIGNAL(connected()));
	waitForSignal(1000, spy1, 50, 2);

	QSignalSpy spy2(m_server, SIGNAL(subscribeAck()));

	QObject::connect(this, &TestBroadcaster::subscribe1, _broadcaster1, &Broadcaster::onSubscribeSingleTopic);
	emit(subscribe1(1));
	QObject::connect(this, &TestBroadcaster::subscribe2, _broadcaster2, &Broadcaster::onSubscribeSingleTopic);
	emit(subscribe2(2));

	Logger->trace("Wait for _broadcaster2 to sub:");
	waitForSignal(1000, spy2, 50, 2);

	QCOMPARE(spy2.count(), 2);

	_broadcaster1->onSendPing(2);

	QSignalSpy spy3(_broadcaster1, SIGNAL(updatePing(QJsonObject)));
	waitForSignal(2000, spy3, 50, 1);

	QCOMPARE(spy3.count(), 1);
	if (spy3.count() == 1)
	{
		QList<QVariant> arguments = spy3.takeFirst();
		qDebug() << "arguments:" << arguments;
		QVERIFY(arguments.at(0).toJsonObject()[MESSAGE_TYPE].toString() == PING_PONG);
		QVERIFY(arguments.at(0).toJsonObject()[ID].toInt() == 1);
	}

	QVERIFY(true);
}

void TestBroadcaster::waitForSignal(int timeout, QSignalSpy &spy, int deltaTime, int signalToCatch)
{
	int time{ 0 };
	while (spy.count() != signalToCatch)
	{
		time += deltaTime;
		QTest::qWait(deltaTime);
		if (time >= timeout)
		{
			break;
		}
	}
}

QTEST_MAIN(TestBroadcaster)
//#include "test.moc"
