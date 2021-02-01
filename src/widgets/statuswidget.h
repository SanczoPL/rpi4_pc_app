#ifndef STATUS_WIDGET_H
#define STATUS_WIDGET_H

#include "../structures.h"

#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFrame>

class Mat;

class StatusWidget : public QLabel
{
  Q_OBJECT

 public:
  explicit StatusWidget(QJsonObject const& a_config);
  
public slots:
	void onUpdate();
	void onUpdatePing(QJsonObject ping);
	void configure(QJsonObject const& a_config);

private:
	qint64 m_ping;

};
#endif // STATUS_WIDGET_H
