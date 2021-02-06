#include "gtest/gtest.h"

#include <QJsonObject>
#include <QMetaEnum>
#include <QApplication>
#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "../src/structures.h"
#include "../src/mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

constexpr auto GENERAL{ "General" };
constexpr auto LOG_LEVEL{ "LogLevel" };
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}