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
        int res = tc->read(buf, 256);
        if (res <= 0) break;
        buf[res] = '\0';
        emit readMsg(buf);
    }
}
void RThread::stop() {
    this->quit();
    this->wait(1000);
}
