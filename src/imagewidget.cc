#include <QtWidgets>

#include "imagewidget.h"

constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto WIDTH_RESIZE{ "WidthResize" };
constexpr auto HEIGHT_RESIZE{ "HeightResize" };

ImageWidget::ImageWidget(QJsonObject const& a_config)
: m_width{ a_config[WIDTH].toInt() }, m_height{ a_config[HEIGHT].toInt() }
, m_widthResize{ a_config[WIDTH_RESIZE].toInt() }, m_heightResize{ a_config[HEIGHT_RESIZE].toInt() }
{
	Logger->trace("ImageWidget::ImageWidget() m_width:{}, m_height:{}", m_width, m_height);
	cv::Mat decodedMat = cv::Mat(m_height, m_width, CV_8UC1, 255);
	cv::resize(decodedMat, decodedMat, cv::Size(m_widthResize, m_heightResize), 0, 0, cv::INTER_NEAREST);
	QImage image1 = QImage((uchar*)decodedMat.data, decodedMat.cols, decodedMat.rows, decodedMat.step, QImage::Format_Grayscale8);
	setAlignment(Qt::AlignCenter);
	setEnabled(true);
}

void ImageWidget::onUpdateImage(QByteArray image)
{
	cv::Mat decodedMat;
	Logger->info("ImageWidget::updateImage()");
	if (image.size() != 768)
	{
		Logger->error("data.size() != 768");
	}
	try
	{
		decodedMat = cv::Mat(24, 32, CV_8UC1, image.data());
		cv::resize(decodedMat, decodedMat, cv::Size(320, 240), 0, 0, cv::INTER_NEAREST);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		qDebug() << "exception caught: " << err_msg;
		Logger->error("exception caught:");
	}
	QImage image1 = QImage((uchar*)decodedMat.data, decodedMat.cols, decodedMat.rows, decodedMat.step, QImage::Format_Grayscale8);
	this->setPixmap(QPixmap::fromImage(image1));
}

void ImageWidget::onUpdate()
{

}

void ImageWidget::configure(QJsonObject const& a_config)
{

}