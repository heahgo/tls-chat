#include "tlsclient.h"

bool TlsClient::connect(Ip ip, int port) {
    if (!tcpClient_.connect(ip, port)) {
		error_ = tcpClient_.error_;
		return false;
	}
    // openssl 1.1.0 부터는 자동으로 처리
    //OpenSSL_add_all_algorithms(); /* Load cryptos, et.al. */
    //OPENSSL_init_crypto();
    //SSL_load_error_strings(); /* Bring in and register error messages */
    //OPENSSL_init_ssl();
	const SSL_METHOD *method = TLS_client_method();
	ctx_ = SSL_CTX_new(method);
	assert(ctx_ != nullptr);

	sock_ = tcpClient_.sock_;
	ssl_ = SSL_new(ctx_);
	assert(ssl_ != nullptr);

	SSL_set_fd(ssl_, sock_);

	int res = SSL_connect(ssl_);
	if (res <= 0) {
		char buf[256];
		int error_res = SSL_get_error(ssl_, res);
		sprintf(buf, "SSL_connect return %d SSL_get_error=%d", res, error_res);
		error_ = buf;
		return false;
	}

	return true;
}
