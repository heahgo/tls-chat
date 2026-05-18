#include "rthread.h"

RThread::RThread()
{

}

RThread::~RThread() {
    tc->close();
}
void RThread::run() {
    while (true) {
        char buf[256];
        int res = tc->read(buf, 255);
        if (res <= 0) break;
        emit readMsg(QString::fromUtf8(buf, res));
    }
}
void RThread::stop() {
    this->quit();
    this->wait(1000);
}
