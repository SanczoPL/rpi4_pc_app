#include "test.h"

void TestBroadcaster::BasicOperation()
{
    QJsonObject config{
        {"Port", 12345},
        {"Ip", "localhost"},
        {"InfoTopic", 1},
        {"CommandTopic", 2},
        {"ImageTopic", 3},
        {"Id", 2}
    };

    Broadcaster* _broadcaster = new Broadcaster(config);



}

void TestBroadcaster::t2()
{
    QVERIFY(true);
}
QTEST_MAIN(TestBroadcaster)
#include "test.moc"