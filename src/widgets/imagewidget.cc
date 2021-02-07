#include <QtWidgets>

#include "imagewidget.h"

constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto WIDTH_RESIZE{ "WidthResize" };
constexpr auto HEIGHT_RESIZE{ "HeightResize" };


ImageWidget::ImageWidget(QJsonObject const& a_config)
: m_width{ a_config[WIDTH].toInt() }, m_height{ a_config[HEIGHT].toInt() }
, m_widthResize{ a_config[WIDTH_RESIZE].toInt() }, m_heightResize{ a_config[HEIGHT_RESIZE].toInt() }
, m_dataSize{ a_config[WIDTH].toInt() *a_config[HEIGHT].toInt() }
{
	Logger->trace("ImageWidget::ImageWidget() m_width:{}, m_height:{}", m_width, m_height);
	cv::Mat decodedMat = cv::Mat(m_height, m_width, CV_8UC1, 255);
	cv::resize(decodedMat, decodedMat, cv::Size(m_widthResize, m_heightResize), 0, 0, cv::INTER_NEAREST);
	QImage image1 = QImage((uchar*)decodedMat.data, decodedMat.cols, decodedMat.rows, decodedMat.step, QImage::Format_Grayscale8);
	setAlignment(Qt::AlignCenter);
	this->setPixmap(QPixmap::fromImage(image1));
	setEnabled(true);
}

void ImageWidget::onUpdateImage(QByteArray image, qint32 topic)
{
	cv::Mat decodedMat;
	Logger->trace("ImageWidget::updateImage(image)");
	if (image.size() != m_dataSize)
	{
		Logger->error("data.size() != {}", m_dataSize);
	}
	try
	{
		decodedMat = cv::Mat(m_height, m_width, CV_8UC1, image.data());
		cv::resize(decodedMat, decodedMat, cv::Size(m_widthResize, m_heightResize), 0, 0, cv::INTER_NEAREST);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		qDebug() << "exception caught: " << err_msg;
		Logger->error("exception caught:");
		return;
	}
	QImage image1 = QImage((uchar*)decodedMat.data, decodedMat.cols, decodedMat.rows, decodedMat.step, QImage::Format_Grayscale8);
	this->setPixmap(QPixmap::fromImage(image1));
}

void ImageWidget::onUpdate()
{}

void ImageWidget::configure(QJsonObject const& a_config)
{}