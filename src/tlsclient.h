#pragma once

#include <string>

#include "client.h"
#include "tcpclient.h"
#include "tlssession.h"

struct TlsClient : public Client, public TlsSession {
	TcpClient tcpClient_;
	SSL_CTX *ctx_{nullptr};
	std::string caFileName_;   // 설정 시 인증서 검증(SSL_VERIFY_PEER) 활성화

    bool connect(Ip ip, int port) override;
};
