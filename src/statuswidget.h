#ifndef STATUS_WIDGET_H
#define STATUS_WIDGET_H

#include "structures.h"

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
	void onUpdatePing(qint64 ping);
	void configure(QJsonObject const& a_config);

private:
	double m_ping;

};
#endif // STATUS_WIDGET_H
