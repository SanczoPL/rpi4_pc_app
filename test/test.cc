#include "test.h"

void TestBroadcaster::BasicOperation()
{
    Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");
    Logger->debug("Start main logger with LogLevel:{}", 0);
    Logger->set_level(static_cast<spdlog::level::level_enum>(0));
    QJsonObject config{
        {"Port", 12345},
        {"Ip", "127.0.0.1"},
        {"InfoTopic", 1},
        {"CommandTopic", 2},
        {"ImageTopic", 3},
        {"Id", 2}
    };

    Logger->trace("Create IOServer:");
    //IOServer server{ config };
    Logger->trace("Create Broadcaster:");

    QThread *m_serverThread = new QThread();
    IOServer *m_server = new IOServer(config);
    m_server->moveToThread(m_serverThread);
    connect(m_serverThread, &QThread::finished, m_server, &QObject::deleteLater);
    m_serverThread->start();

    QThread* m_broadcaster1 = new QThread();
    Broadcaster* _broadcaster1 = new Broadcaster(config);
    _broadcaster1->moveToThread(m_broadcaster1);
    connect(m_broadcaster1, &QThread::finished, _broadcaster1, &QObject::deleteLater);
    m_broadcaster1->start();

    QThread* m_broadcaster2 = new QThread();
    Broadcaster* _broadcaster2 = new Broadcaster(config);
    _broadcaster2->moveToThread(m_broadcaster2);
    connect(m_broadcaster2, &QThread::finished, _broadcaster2, &QObject::deleteLater);
    m_broadcaster2->start();


    
    QVector<qint32> topics1{1};
    QVector<qint32> topics2{2};
    _broadcaster1->subscribeRequest(topics1);
    

    QSignalSpy spy(_broadcaster1, SIGNAL(subscribeRequest(QVector<qint32>)));
    _broadcaster2->subscribeRequest(topics2);


    //QList<QVariant> arguments = spy.takeFirst(); // take the first signal

    //QVERIFY(arguments.at(0).toInt() == 1); // verify the first argument

    QCOMPARE(spy.count(), 1);

    _broadcaster2->onSubscribe(2);

    


    QVERIFY(true);

}

void TestBroadcaster::t2()
{
    QVERIFY(true);
}
QTEST_MAIN(TestBroadcaster)
#include "test.moc"