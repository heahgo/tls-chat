# TLS Chat

TCP/TLS 기반 채팅 라이브러리와 Qt GUI 클라이언트 프로젝트.

## 구조

```
tls-chat/
├── src/                    # 네트워크 라이브러리
│   ├── crt/                # TLS 인증서 파일
│   ├── tcpclient.h/.cpp    # TCP 클라이언트
│   ├── tcpserver.h/.cpp    # TCP 서버
│   ├── tcpsession.h/.cpp   # TCP 세션
│   ├── tlsclient.h/.cpp    # TLS 클라이언트
│   ├── tlsserver.h/.cpp    # TLS 서버
│   └── tlssession.h/.cpp   # TLS 세션
└── app/
    ├── chat/               # Qt GUI 채팅 클라이언트 (TLS)
    ├── tcpclient-test/     # TCP 클라이언트 CLI 테스트
    ├── tcpserver-test/     # TCP 서버 CLI 테스트
    ├── tlsclient-test/     # TLS 클라이언트 CLI 테스트
    └── tlsserver-test/     # TLS 서버 CLI 테스트
```

## 의존성

- **OpenSSL** — TLS 암호화
- **Qt 5 또는 Qt 6** — GUI 클라이언트(`chat`)에만 필요

```bash
# Ubuntu/Debian
sudo apt install libssl-dev qtbase5-dev

# Fedora/RHEL
sudo dnf install openssl-devel qt5-qtbase-devel
```

## 빌드

```bash
cmake -B build
cmake --build build
```

Qt가 기본 경로에 없는 경우:

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
cmake --build build
```

빌드 결과물은 `bin/` 디렉토리에 생성됩니다.

## 실행 방법

### TCP 테스트 (암호화 없음)

터미널 1 — 서버 실행:
```bash
./bin/tcpserver-test <port>
# 예시
./bin/tcpserver-test 8080
```

터미널 2, 3, ... — 클라이언트 실행:
```bash
./bin/tcpclient-test <host> <port>
# 예시
./bin/tcpclient-test 127.0.0.1 8080
```

- 입력한 메시지가 접속한 모든 클라이언트에게 브로드캐스트됩니다.
- `q` 입력 시 클라이언트 종료.

---

### TLS 테스트 (암호화)

터미널 1 — 서버 실행 (PEM 파일 경로를 인수로 전달):
```bash
./bin/tlsserver-test <port> <pem>
# 예시
./bin/tlsserver-test 443 ../src/crt/rootCA.pem
```

터미널 2, 3, ... — 클라이언트 실행:
```bash
./bin/tlsclient-test <host> <port>
# 예시
./bin/tlsclient-test 127.0.0.1 443
```

- 동작은 TCP 테스트와 동일하며 OpenSSL TLS로 암호화됩니다.
- `q` 입력 시 클라이언트 종료.

---

### GUI 채팅 클라이언트

TLS 서버가 먼저 실행 중이어야 합니다.

```bash
./bin/chat
```

1. IP 필드에 서버 주소 입력 (예: `127.0.0.1`)
2. Port 필드에 서버 포트 입력 (예: `8443`)
3. **Connect** 버튼 클릭
4. 메시지 입력 후 **Send** 버튼 클릭
5. 종료 시 **Close** 버튼 클릭

## 인증서

`src/crt/`에 테스트용 자체 서명 인증서가 포함되어 있습니다.

| 파일 | 설명 |
|------|------|
| `rootCA.pem` | 인증서 + 개인키 (서버에서 사용) |
| `rootCA.crt` | 인증서 |
| `rootCA.key` | 개인키 |
| `rootCA.csr` | 인증서 서명 요청 |

새 인증서 생성이 필요한 경우:

```bash
# 개인키 생성
openssl genrsa -out rootCA.key 2048

# 자체 서명 인증서 생성
openssl req -x509 -new -nodes -key rootCA.key -sha256 -days 3650 -out rootCA.crt

# PEM 합치기 (서버용)
cat rootCA.crt rootCA.key > rootCA.pem
```
