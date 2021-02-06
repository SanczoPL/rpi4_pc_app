#include "test.h"

constexpr auto PING{ "Ping" };
constexpr auto MESSAGE_TYPE{ "MessageType" };
constexpr auto ID{ "Id" };
constexpr auto IP{ "Ip" };
constexpr auto PORT{ "Port" };

void TestBroadcaster::BasicOperation()
{
	qRegisterMetaType<QVector<qint32>>("QVector<qint32>");
	qRegisterMetaType<qint32>("qint32");
	Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");
	Logger->debug("Start main logger with LogLevel:{}", 0);
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
	config2[ID] = 3;


	IOServer* m_server = new IOServer();
	QSignalSpy spy0(m_server, SIGNAL(listenForConnection()));

	m_server->configure(config3);
	while (spy0.count() == 0)
		QTest::qWait(50);
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
	while (spy1.count() == 2)
		QTest::qWait(50);

	QSignalSpy spy2(m_server, SIGNAL(subscribeAck()));

	QObject::connect(this, &TestBroadcaster::subscribe1, _broadcaster1, &Broadcaster::onSubscribeSingleTopic);
	emit(subscribe1(1));
	QObject::connect(this, &TestBroadcaster::subscribe2, _broadcaster2, &Broadcaster::onSubscribeSingleTopic);
	emit(subscribe2(2));

	Logger->info("Wait for _broadcaster2 to sub:");
	while (spy2.count() != 2)
		QTest::qWait(50);
	QCOMPARE(spy2.count(), 2);
	Logger->info("onSendPing");

	_broadcaster1->onSendPing(2);

	QSignalSpy spy3(_broadcaster2, SIGNAL(updatePing(QJsonObject)));
	while (spy3.count() == 0)
		QTest::qWait(50);
	QCOMPARE(spy3.count(), 1);
	if (spy3.count() == 1)
	{
		QList<QVariant> arguments = spy3.takeFirst();
		qDebug() << "arguments:" << arguments;
		QVERIFY(arguments.at(0).toJsonObject()[MESSAGE_TYPE].toString() == PING);
		QVERIFY(arguments.at(0).toJsonObject()[ID].toInt() == 1);
	}

	QVERIFY(true);
}

void TestBroadcaster::t2()
{
	QVERIFY(true);
}

QTEST_MAIN(TestBroadcaster)
//#include "test.moc"