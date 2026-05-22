#pragma once

#include "compat.h"
#include "session.h"

struct TcpSession : public Session {
	int sock_;

	TcpSession(int sock = 0);
	virtual ~TcpSession();

	int read(char* buf, int size) override;
	int write(char* buf, int size) override;
	bool close() override;
};
typedef TcpSession *PTcpSession;
