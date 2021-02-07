#include "server.h"

constexpr auto GENERAL{"General"};
constexpr auto THREADSMAX{"ThreadsMax"};
constexpr auto PORT{ "Port" };

Server::Server(QJsonObject a_config)
	:m_config{ a_config }
{}

void Server::onUpdate()
{}

void Server::configure(QJsonObject const& a_config)
{
	Logger->info("MainLoop::configure()");
}

