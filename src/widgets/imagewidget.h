#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H

#include "../structures.h"

#include <QWidget>
#include <QLabel>


class Mat;

class ImageWidget : public QLabel
{
	Q_OBJECT

public:
	explicit ImageWidget(QJsonObject const& a_config);

public slots:
	void onUpdateImage(QByteArray image, qint32 topic);
	void onUpdate();
	void configure(QJsonObject const& a_config);

private:
	int m_width{};
	int m_height{};
	int m_widthResize{};
	int m_heightResize{};
	int m_dataSize{};
};

#endif // IMAGE_WIDGET_H