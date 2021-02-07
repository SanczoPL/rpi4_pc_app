#include "mainwindow.h"

constexpr auto IMAGE_WIDGET{ "ImageWidget" };
constexpr auto IMAGE_PROCESSING_WIDGET{ "ImageProcessingWidget" };
constexpr auto STATUS_WIDGET{ "StatusWidget" };
constexpr auto GENERAL{ "General" };
constexpr auto THREADSMAX{ "ThreadsMax" };
constexpr auto SERVER{ "Server" };
constexpr auto IMAGE_ACQUISITION{ "ImageAcquisition" };
constexpr auto MESSAGE_TYPE{ "MessageType" };
constexpr auto PING{ "Ping" };
constexpr auto INFO_TOPIC{ "InfoTopic" };
constexpr auto COMMAND_TOPIC{ "CommandTopic" };
constexpr auto IMAGE_TOPIC{ "ImageTopic" };
constexpr auto TIME{ "Time" };


MainWindow::MainWindow(QJsonObject const& a_config)
	:m_config{ a_config },
	m_firstTime(true),
	m_threadsMax{ a_config[GENERAL].toObject()[THREADSMAX].toInt() }
{
	Logger->trace("MainWindow::MainWindow()");

	Logger->trace("MainWindow::createStartupThreads()");
	MainWindow::createStartupThreads();

	Logger->trace("MainWindow::MainWindow() configure");
	MainWindow::configure(a_config);

	Logger->trace("MainWindow::MainWindow() setupButtons");
	MainWindow::setupButtons(); // buttonContainer

	Logger->trace("MainWindow::MainWindow() setupLeftToolBar");
	MainWindow::setupMainWidget(); // leftToolBar
}

void MainWindow::setupMainWidget() {
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->setContentsMargins(0, 0, 0, 0);

	mainLayout->addWidget(m_imageWidget, 0, 0);
	mainLayout->addWidget(m_imageProcessingWidget, 1, 0);
	
	m_statusWidget = new StatusWidget(m_config[IMAGE_WIDGET].toObject());
	m_statusWidget->setMaximumHeight(30);

	m_statusBar = new QStatusBar(this);
	m_statusBar->addPermanentWidget(m_statusWidget);
	mainLayout->addWidget(m_statusBar, 2, 0);
	mainLayout->setRowStretch(1, 1);

	QWidget* mainCentralWidget = new QWidget(this);
	mainCentralWidget->setLayout(mainLayout);

	Logger->trace("MainWindow::MainWindow() MainLayout");
	QGridLayout* MainLayout = new QGridLayout;
	MainLayout->setContentsMargins(0, 0, 0, 0);
	MainLayout->addWidget(mainCentralWidget, 0, 0);

	setLayout(MainLayout);

	resize(1600, 600);
	this->show();
}

void MainWindow::setupButtons() {}

void MainWindow::onNewMessage(QJsonObject const& json) {
	QString cmd = json[MESSAGE_TYPE].toString();
	if (cmd == PING) {
		Logger->trace(" MainWindow::onNewMessage() ping");
	}
}

void MainWindow::configure(QJsonObject const& a_config) {
	m_config = a_config;
}

void MainWindow::onUpdate()
{
	if (m_firstTime)
	{
		QVector<int> topics{ 4, m_config[SERVER].toObject()[IMAGE_TOPIC].toInt(), m_config[SERVER].toObject()[COMMAND_TOPIC].toInt() };
		Logger->info("Sub on topics:{} and {}", m_config[SERVER].toObject()[IMAGE_TOPIC].toInt(), m_config[SERVER].toObject()[COMMAND_TOPIC].toInt());
		m_server->onSubscribe(topics);
		m_firstTime = false;
	}
	m_server->onSendPing( m_config[SERVER].toObject()[INFO_TOPIC].toInt());
}

void MainWindow::createStartupThreads()
{
	m_serverThread = new QThread();
	m_server = new Broadcaster(m_config[SERVER].toObject());
	m_server->moveToThread(m_serverThread);
	connect(m_serverThread, &QThread::finished, m_server, &QObject::deleteLater);
	connect(m_server, &Broadcaster::newMessage, this, &MainWindow::onNewMessage);
	connect(m_server, &Broadcaster::updateImage, this, &MainWindow::onUpdateImage);
	m_serverThread->start();
	
	m_imageWidget = new ImageWidget(m_config[IMAGE_WIDGET].toObject());
	m_imageProcessingWidget = new ImageWidget(m_config[IMAGE_PROCESSING_WIDGET].toObject());
	
	m_statusWidget = new StatusWidget(m_config[STATUS_WIDGET].toObject());
	connect(m_server, &Broadcaster::updatePing, m_statusWidget, &StatusWidget::onUpdatePing);
	connect(m_server, &Broadcaster::updateError, m_statusWidget, &StatusWidget::onUpdateError);
	
	m_timer = new QTimer(this);
	m_timer->start(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
	connect(m_timer, SIGNAL(timeout()), m_statusWidget, SLOT(onUpdate()));
}

void MainWindow::onUpdateImage(QByteArray image, qint32 topic)
{
	if (topic == 3)
	{
		m_imageWidget->onUpdateImage(image, topic);
	}
	if (topic == 4)
	{
		m_imageProcessingWidget->onUpdateImage(image, topic);
	}
}