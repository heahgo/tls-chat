#include "tlsserver.h"

#include <mutex>

bool TlsServer::start(int port) {

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *method = TLS_server_method();
    ctx_ = SSL_CTX_new(method);
    if ( ctx_ == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    SSL_CTX_set_keylog_callback(ctx_, [](const SSL*, const char* line) {
        const char* path = getenv("SSLKEYLOGFILE");
        if (!path) return;
        FILE* f = fopen(path, "a");
        if (f) { fprintf(f, "%s\n", line); fclose(f); }
    });
    SSL_CTX_set_info_callback(ctx_, [](const SSL* ssl, int where, int) {
        if (where & (SSL_CB_ACCEPT_LOOP | SSL_CB_ACCEPT_EXIT))
            printf("[TLS Handshake] %s\n", SSL_state_string_long(ssl));
    });

    if ( SSL_CTX_use_certificate_file(ctx_, pemFileName_.data(), SSL_FILETYPE_PEM) <= 0 )
        {
            ERR_print_errors_fp(stderr);
            abort();
        }
    if ( SSL_CTX_use_PrivateKey_file(ctx_, pemFileName_.data(), SSL_FILETYPE_PEM) <= 0 )
        {
            ERR_print_errors_fp(stderr);
            abort();
        }
    if ( !SSL_CTX_check_private_key(ctx_) )
        {
            fprintf(stderr, "Private key does not match the public certificate\n");
            abort();
        }
    acceptSock_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (acceptSock_ == -1) {
        error_ = sock_error();
        return false;
    }

    int res;
#ifndef _WIN32
    int optval = 1;
    res = ::setsockopt(acceptSock_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (res == -1) {
        error_ = sock_error();
        return false;
    }
#endif

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int res2 = ::bind(acceptSock_, (struct sockaddr *)&addr, sizeof(addr));
    if (res2 == -1) {
        error_ = sock_error();
        return false;
    }

    res = listen(acceptSock_, 5);
    if (res == -1) {
        error_ = sock_error();
        return false;
    }

    acceptThread_ = new std::thread(&TlsServer::acceptRun, this);
	return true;
}

bool TlsServer::stop() {
	::shutdown(acceptSock_, SHUT_RDWR);
	::close(acceptSock_);
    SSL_CTX_free(ctx_);
	if (acceptThread_ != nullptr) {
		delete acceptThread_;
		acceptThread_ = nullptr;
	}

	sessions_.lock();
    for (TlsSession* session: sessions_)
		session->close();
	sessions_.unlock();

	while (true) {
		sessions_.lock();
		bool exit = sessions_.size() == 0;
		sessions_.unlock();
        if (exit) break;
        platform_usleep(1000);
	}

	return true;
}

void TlsServer::acceptRun() {
	while (true) {
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
        int newSock = ::accept(acceptSock_, (struct sockaddr *)&addr, &len);
        if (newSock == -1) {
            error_ = sock_error();
            break;
        }
        SSL* ssl = SSL_new(ctx_);
        SSL_set_fd(ssl, newSock);
        if ( SSL_accept(ssl) == -1 )  {
             ERR_print_errors_fp(stderr);
             break;
        }

        printf("\n========== TLS Client Connected ==========\n");
        printf("Protocol    : %s\n", SSL_get_version(ssl));
        printf("Cipher Suite: %s\n", SSL_CIPHER_get_name(SSL_get_current_cipher(ssl)));
        printf("==========================================\n\n");

        TlsSession* session = new TlsSession(newSock, ssl);
        std::thread* thread = new std::thread(&TlsServer::_run, this, session);
		thread->detach();
	}
}

void TlsServer::_run(TlsSession* session) {
	sessions_.lock();
	sessions_.push_back(session);
	sessions_.unlock();

	run(session);

	sessions_.lock();
	sessions_.remove(session);
	sessions_.unlock();
}
