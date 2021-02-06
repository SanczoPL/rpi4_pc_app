#ifndef TEST_BROADCASTER_GTEST
#define TEST_BROADCASTER_GTEST

#include <QJsonObject>
#include <QObject>


#include "gtest/gtest.h"

#include "../Broadcaster/include/broadcaster.h"
#include "../QtCommunicator/include/ioserver.h"
#include "../IncludeSpdlog/include/spdlog.h"


class TestBroadcaster : public::testing::Test {

protected:

    // You can do set-up work for each test here.
    TestBroadcaster();

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestBroadcaster();

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();

    QJsonObject m_config;
};


#endif // TEST_BROADCASTER_GTEST