#include <iostream>
#include <thread>

#include "tlsclient.h"

struct Param {
    Ip ip_;
	int port_;
	std::string ca_;   // 선택: 지정 시 서버 인증서 검증

	bool parse(int argc, char** argv) {
		if (argc != 3 && argc != 4) return false;
        ip_ = Ip(inet_addr(argv[1]));
		port_ = std::stoi(argv[2]);
		if (argc == 4) ca_ = argv[3];
		return true;
	}

	static void usage() {
		printf("syntax : tlsclient-test <host> <port> [ca]\n");
		printf("sample : tlsclient-test 127.0.0.1 443 ../src/crt/rootCA.pem\n");
	}
};

void readAndPrint(Session* session) {
	std::puts("connected");
	char buf[256];
	while (true) {
		int res = session->read(buf, 256);
		if (res <= 0) break;
		buf[res] = '\0';
		std::puts(buf);
	}
	std::puts("disconnected");
    exit(0);
}

int main(int argc, char* argv[]) {
	TlsClient tc;

	Param param;
	if (!param.parse(argc, argv)) {
		Param::usage();
		return -1;
	}
	tc.caFileName_ = param.ca_;

    if (!tc.connect(param.ip_, param.port_)) {
		std::cerr << tc.error_ << std::endl;
		return -1;
	}

	std::thread thread(&readAndPrint, &tc);

	while (true) {
		std::string msg;
		std::getline(std::cin, msg);
        if (msg == "q") break;
        int writeLen = tc.write(msg.data(), msg.size());
        if (writeLen == -1 ) break;
	}
	tc.close();
}
