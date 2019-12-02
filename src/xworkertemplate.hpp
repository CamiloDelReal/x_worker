#ifndef XWORKERTEMPLATE_HPP
#define XWORKERTEMPLATE_HPP

#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <QThread>


#define X_CHECK_PAUSE(CODE)                                                     \
    m_canContinue->lock();                                                      \
    if (m_pauseRequested)                                                       \
    {                                                                           \
        CODE                                                                    \
        m_pauseManager->wait(m_canContinue);                                    \
    }                                                                           \
    m_canContinue->unlock();

#define X_CHECK_CANCEL(CODE)                                                    \
    if(m_cancelRequested)                                                       \
    {                                                                           \
        CODE                                                                    \
    }


#define X_WORKER_DECLARATION_TEMPLATE(CLASS_NAME, PARENT_NAME, MACROS)          \
class CLASS_NAME : public PARENT_NAME                                           \
{                                                                               \
    MACROS                                                                      \
public:                                                                         \
    explicit CLASS_NAME(QObject *parent = nullptr);                             \
    ~CLASS_NAME();                                                              \
                                                                                \
    bool isRunning();                                                           \
    bool isPaused();                                                            \
                                                                                \
public slots:                                                                   \
    void start();                                                               \
    void pause();                                                               \
    void unPause();                                                             \
    void cancel();                                                              \
    void setProgress(const quint32 &progress);                                  \
                                                                                \
Q_SIGNALS:                                                                      \
    void runningChanged(const bool &isRunning);                                 \
    void progressChanged(const quint32 &progress);                              \
    void paused();                                                              \
    void unPaused();                                                            \
    void finished();                                                            \
                                                                                \
protected:                                                                      \
    virtual void execute() = 0;                                                 \
                                                                                \
private slots:                                                                  \
    void quitWrapper();                                                         \
    void executeWrapper();                                                      \
                                                                                \
protected:                                                                      \
    bool m_cancelRequested;                                                     \
    bool m_pauseRequested;                                                      \
    QWaitCondition *m_pauseManager;                                             \
    QMutex *m_canContinue;                                                      \
    bool m_running;                                                             \
                                                                                \
    QThread *getThread();                                                       \
                                                                                \
private:                                                                        \
    QThread *m_thread;                                                          \
    quint32 m_progress;                                                         \
                                                                                \
    void setIsRunning(const bool &running);                                     \
};

#define X_WORKER_DEFINITION_TEMPLATE(CLASS_NAME, PARENT_NAME)                   \
CLASS_NAME::CLASS_NAME(QObject *parent) : PARENT_NAME(parent)                   \
{																				\
    m_cancelRequested = false;													\
    m_pauseRequested = false;													\
                                                                                \
    m_pauseManager = new QWaitCondition();										\
    m_canContinue = new QMutex();												\
                                                                                \
    m_running = false;                                                          \
                                                                                \
    m_thread = new QThread();													\
    moveToThread(m_thread);														\
    connect(m_thread, &QThread::started, this, &CLASS_NAME::executeWrapper);    \
    connect(this, &CLASS_NAME::finished, this, &CLASS_NAME::quitWrapper);       \
}																				\
                                                                                \
CLASS_NAME::~CLASS_NAME()                                                       \
{																				\
    m_thread->disconnect();														\
    this->disconnect();															\
                                                                                \
    delete m_pauseManager;														\
    delete m_canContinue;														\
    delete m_thread;															\
}																				\
                                                                                \
void CLASS_NAME::quitWrapper()                                                  \
{																				\
    setIsRunning(false);                                                        \
    m_thread->quit();															\
}																				\
                                                                                \
void CLASS_NAME::executeWrapper()                                               \
{																				\
    execute();																	\
                                                                                \
    emit finished();															\
}																				\
                                                                                \
QThread *CLASS_NAME::getThread()                                                \
{																				\
    return m_thread;															\
}																				\
                                                                                \
void CLASS_NAME::start()                                                        \
{																				\
    m_cancelRequested = false;													\
    m_pauseRequested = false;													\
    setIsRunning(true);                                                         \
    if(!m_thread->isRunning())													\
        m_thread->start();														\
}																				\
                                                                                \
bool CLASS_NAME::isRunning()                                                    \
{																				\
    return m_running;                                                           \
}																				\
                                                                                \
void CLASS_NAME::pause()                                                        \
{																				\
    m_canContinue->lock();                                                      \
    m_pauseRequested = true;													\
    m_canContinue->unlock();                                                    \
    emit paused();                                                              \
}																				\
                                                                                \
void CLASS_NAME::unPause()                                                      \
{																				\
    m_canContinue->lock();                                                      \
    m_pauseRequested = false;													\
    m_canContinue->unlock();                                                    \
    m_pauseManager->wakeAll();													\
    emit unPaused();                                                            \
}																				\
                                                                                \
bool CLASS_NAME::isPaused()                                                     \
{																				\
    return m_pauseRequested;													\
}																				\
                                                                                \
void CLASS_NAME::cancel()                                                       \
{																				\
    unPause();																	\
    m_cancelRequested = true;													\
}                                                                               \
                                                                                \
void CLASS_NAME::setProgress(const quint32 &progress)                           \
{                                                                               \
    if(m_progress != progress)                                                  \
    {                                                                           \
        m_progress = progress;                                                  \
        emit progressChanged(m_progress);                                       \
    }                                                                           \
}                                                                               \
                                                                                \
void CLASS_NAME::setIsRunning(const bool &running)                              \
{                                                                               \
    if(m_running != running)                                                    \
    {                                                                           \
        m_running = running;                                                    \
        emit runningChanged(m_running);                                         \
    }                                                                           \
}
#endif // XWORKERTEMPLATE_HPP
