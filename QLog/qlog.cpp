#include "qlog.h"
#include <QTime>
#include <QEvent>
#include <QThread>
#include <QCoreApplication>
#include <iostream>
#include <QDir>
#include <QDate>
#include <memory>
#include <QDebug>

namespace QLOG {

static const char * LevelName[6] = {"Test","Debug","Info","Warning","Critical","Fatal"};

void customMessageHandler(QtMsgType type, const QMessageLogContext &context,const QString & msg)
{
    switch (type) {
    //调试信息提示
    case QtDebugMsg:
        if (QLog::checkLevel(DebugMsg)) {
            LogMessage(context.file,context.line,context.function,DebugMsg).stream() << "[Qt]: " << msg ;
        }
        break;
        //一般的warning提示
    case QtWarningMsg:
        if (QLog::checkLevel(WarningMsg)) {
            LogMessage(context.file,context.line,context.function,WarningMsg).stream() << "[Qt]: " << msg ;
        }
        break;
        //严重错误提示
    case QtCriticalMsg:
        if (QLog::checkLevel(CriticalMsg)) {
            LogMessage(context.file,context.line,context.function,CriticalMsg).stream() << "[Qt]: " << msg ;
        }
        break;
        //致命错误提示
    case QtFatalMsg:
        if (QLog::checkLevel(FatalMsg)) {
            LogMessage(context.file,context.line,context.function,FatalMsg).stream() << "[Qt]: " << msg ;
        }
        break;
    case QtInfoMsg :
        if (QLog::checkLevel(InfoMsg)) {
            LogMessage(context.file,context.line,context.function,InfoMsg).stream() << "[Qt]:" << msg ;
        }
        break;
    }
}

class LogEvent : public QEvent
{
public:
    LogEvent(QString && str): QEvent(eventType),str_(str) {}
    ~LogEvent() {}

    const QString & getString() const {return str_;}

    static QEvent::Type eventType;
private:
    QString str_;
};

QEvent::Type LogEvent::eventType = static_cast<QEvent::Type>(QEvent::registerEventType());

LogMessage::LogMessage(const char * file, int line,const char * funtion, LogLevel level) : ts(&str_),level_(level)
{
    ts << "[" << QTime::currentTime().toString("hh:mm:ss.zzz") << "]:[In File:" << file << ", On Line:" << line << ",At Funtion:" << funtion <<
          "]:[" << LevelName[level] << "]:";
}

LogMessage::~LogMessage()
{
    ts << "\r\n";
    ts.setDevice(nullptr);
    if (QLog::log != nullptr) {
        QCoreApplication::postEvent(QLog::log,new LogEvent(std::move(str_)));
        if (level_ == FatalMsg) {
            QCoreApplication::exit(-1);
        }
    }
}


QLog * QLog::log = nullptr;

QLog::QLog(OutState state,LogLevel level,QObject *parent)
        : QObject(parent),state_(state),level_(level)
{
    QThread * th = new QThread(this);
    this->moveToThread(th);
    th->start();
}

QLog::~QLog()
{
    releaseLog();
}
void QLog::enbleQtMessage(bool isEnble)
{
    if (isEnble) {
        qInstallMessageHandler(customMessageHandler);
    } else {
        qInstallMessageHandler(0);
    }
}

void QLog::releaseLog()
{
    QThread * th = this->thread();
    if (th->isRunning()) {
        th->quit();
        th->wait(10000);
    }
    if (file_.isOpen()) {
        ts_ <<  "[" << QTime::currentTime().toString("hh:mm:ss.zzz") << "]: [INFO]: QCoreApplication::exitd\r\n\r\n";
        ts_.flush();
        file_.close();
    }
    log = nullptr;
}

void QLog::customEvent(QEvent * event)
{
    if (event->type() == LogEvent::eventType) {
        LogEvent * ev = static_cast<LogEvent *>(event);
        switch (state_) {
        case Console :
            std::cout << ev->getString().toStdString() << std::endl;
            break;
        case File :
            if (!file_.isOpen()) {
                if (path_.isEmpty()) {
                    path_ = QCoreApplication::applicationDirPath();
                } else {
                    QDir dir(path_);
                    if (!dir.exists()) {
                        if (!dir.mkpath(path_))
                            path_ = QCoreApplication::applicationDirPath();
                    }
                }
                QString path = path_;
                if (!path.endsWith("/")) path += "/";
                path += QDate::currentDate().toString("yyyy-MM-dd");
                path += ".";
                path += QTime::currentTime().toString("hh-mm-ss.zzz");
                path += ".log";
                file_.setFileName(path);
                if (file_.open(QFile::Append))
                    ts_.setDevice(&file_);
            }
            if (ts_.device() != nullptr) {
                ts_ << ev->getString();
                ts_.flush();
            }
            break;
        default:
            break;
        }
    } else {
        QObject::customEvent(event);
    }
}

}//namespace QLOG
