#include "tlsclient.h"
#include <cstdio>
#include <openssl/x509.h>

static void keylog_cb(const SSL*, const char* line) {
    const char* path = getenv("SSLKEYLOGFILE");
    if (!path) return;
    FILE* f = fopen(path, "a");
    if (f) { fprintf(f, "%s\n", line); fclose(f); }
}

static void info_cb(const SSL* ssl, int where, int) {
    if (where & (SSL_CB_CONNECT_LOOP | SSL_CB_CONNECT_EXIT))
        printf("[TLS Handshake] %s\n", SSL_state_string_long(ssl));
}

bool TlsClient::connect(Ip ip, int port) {
    if (!tcpClient_.connect(ip, port)) {
        error_ = tcpClient_.error_;
        return false;
    }
    const SSL_METHOD* method = TLS_client_method();
    ctx_ = SSL_CTX_new(method);
    assert(ctx_ != nullptr);

    SSL_CTX_set_keylog_callback(ctx_, keylog_cb);
    SSL_CTX_set_info_callback(ctx_, info_cb);

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

    printf("\n========== TLS Connection Established ==========\n");
    printf("Protocol    : %s\n", SSL_get_version(ssl_));
    printf("Cipher Suite: %s\n", SSL_CIPHER_get_name(SSL_get_current_cipher(ssl_)));
    X509* cert = SSL_get_peer_certificate(ssl_);
    if (cert) {
        char buf[256];
        X509_NAME_oneline(X509_get_subject_name(cert), buf, sizeof(buf));
        printf("Subject     : %s\n", buf);
        X509_NAME_oneline(X509_get_issuer_name(cert), buf, sizeof(buf));
        printf("Issuer      : %s\n", buf);
        X509_free(cert);
    }
    printf("=================================================\n\n");

    return true;
}
