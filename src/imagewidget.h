#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H

#include "structures.h"

#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFrame>

class Mat;

class ImageWidget : public QLabel
{
  Q_OBJECT

 public:
  explicit ImageWidget(QJsonObject const& a_config);
  

public slots:
	void onUpdateImage(QByteArray image);
	void onUpdate();
	void configure(QJsonObject const& a_config);

private:
	int m_width{};
	int m_height{};
	int m_widthResize{};
	int m_heightResize{};

	QGraphicsScene* m_scene;

};

#endif // IMAGE_WIDGET_H
