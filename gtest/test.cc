#include "test.h"
#include "../Broadcaster/include/broadcaster.h"


//using ::testing::Return;

TestBroadcaster::TestBroadcaster()
{

}


TestBroadcaster::~TestBroadcaster() {};

void TestBroadcaster::SetUp() {
    QJsonObject config{
           {"Port", 12345},
           {"Ip", "127.0.0.1"},
           {"InfoTopic", 1},
           {"CommandTopic", 2},
           {"ImageTopic", 3},
           {"Id", 2}
    };
    m_config = config;
};

void TestBroadcaster::TearDown() {};

TEST_F(TestBroadcaster, Constructor) {
    Broadcaster * _broadcaster = new Broadcaster(m_config);

    QThread* m_serverThread = new QThread();
    IOServer* m_server = new IOServer(m_config);
    m_server->moveToThread(m_serverThread);
    QObject::connect(m_serverThread, &QThread::finished, m_server, &QObject::deleteLater);
    m_serverThread->start();

    QThread* m_broadcaster1 = new QThread();
    Broadcaster* _broadcaster1 = new Broadcaster(m_config);
    _broadcaster1->moveToThread(m_broadcaster1);
    QObject::connect(m_broadcaster1, &QThread::finished, _broadcaster1, &QObject::deleteLater);
    m_broadcaster1->start();

    QThread* m_broadcaster2 = new QThread();
    Broadcaster* _broadcaster2 = new Broadcaster(m_config);
    _broadcaster2->moveToThread(m_broadcaster2);
    QObject::connect(m_broadcaster2, &QThread::finished, _broadcaster2, &QObject::deleteLater);
    m_broadcaster2->start();



    QVector<qint32> topics1{ 1 };
    QVector<qint32> topics2{ 2 };
    _broadcaster1->subscribeRequest(topics1);

}

TEST_F(TestBroadcaster, ByDefaultBazFalseIsFalse) {

}

TEST_F(TestBroadcaster, SometimesBazFalseIsTrue) {

}