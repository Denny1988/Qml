#ifndef STABLE_H
#define STABLE_H

// Add C includes here
#include <math.h>

#if defined __cplusplus
// Add C++ includes here
#include <string.h>
using namespace std;

// Qt includes
#include <QApplication>
#include <QtNetwork>
#include <QObject>
#include <qglobal.h>
#include <QDir>
#include <QIntValidator>
#include <QToolTip>
#include <QDebug>
#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QtNetwork>
#include <QTextCodec>
#include <QThread>
#include <QDomDocument>
#include <QMessageBox>
#include <QMap>
#include <QSharedPointer>
#include <QUuid>
#include <QEvent>
#include <QList>
#include <QResizeEvent>
#include <QStackedWidget>
#if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#endif
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneWheelEvent>

#ifndef max
#define max(x, y) ((x) < (y) ? (y) : (x))
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

//解决UTF-8编码中文乱码的问题
#ifdef _MSC_VER
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#pragma warning(disable : 4819)
#endif // _MSC_VER >= 1600
#endif // _MSC_VER

//扩展qDebug以文件行列记录信息
#define QLOG_DEBUG(msg) qDebug() << QString("[%1][%2][%3][%4]%5")                                               \
                                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                        .arg("DEBUG")                                                           \
                                        .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)) \
                                        .arg(QThread::currentThread()->objectName())                            \
                                        .arg(msg);

#define FIRLOG //FIRLOG::日志开关,注释关闭打印日志
#ifdef FIRLOG
#define firecat_log qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") \
                             << "[DEBUG]" << __FILE__ << __LINE__ << __FUNCTION__ << "msg:"
#else
#define firecat_log qDebug()
#endif

#endif //__cplusplus

#endif // STABLE_H
