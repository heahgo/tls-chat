#pragma once

#include <QThread>
#include "tlsclient.h"

class RThread : public QThread, TlsClient
{
    Q_OBJECT
public:
    RThread();
    ~RThread();
    TlsClient* tc;

private:
    void run();
    void stop();
signals:
    void readMsg(char* msg);
};

