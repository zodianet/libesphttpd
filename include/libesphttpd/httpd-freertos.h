#pragma once

#include "httpd.h"

#ifdef FREERTOS
#ifdef ESP32
#include "lwip/sockets.h"
#else
#include "lwip/lwip/sockets.h"
#endif
#endif // #ifdef FREERTOS

#ifdef CONFIG_ESPHTTPD_SSL_SUPPORT
#include <openssl/ssl.h>
#ifdef linux
#include <openssl/err.h>
#endif
#endif

#ifdef linux
#include <netinet/in.h>
#endif

struct RtosConnType{
	int fd;
	int needWriteDoneNotif;
	int needsClose;
	int port;
	char ip[4];
#ifdef CONFIG_ESPHTTPD_SSL_SUPPORT
	SSL *ssl;
#endif
};

#define RECV_BUF_SIZE 2048

typedef struct
{
    RtosConnType rconn[HTTPD_MAX_CONNECTIONS];

    int httpPort;
    struct sockaddr_in httpListenAddress;
    HttpdFlags httpdFlags;

#ifdef CONFIG_ESPHTTPD_SHUTDOWN_SUPPORT
	int udpShutdownPort;
#endif

	bool isShutdown;

	// storage for data read in the main loop
	char precvbuf[RECV_BUF_SIZE];

#ifdef linux
    pthread_mutex_t httpdMux;
#else
    xQueueHandle httpdMux;
#endif

#ifdef CONFIG_ESPHTTPD_SSL_SUPPORT
    SSL_CTX *ctx;
#endif

    HttpdInstance httpdInstance;
} HttpdFreertosInstance;

HttpdInitStatus httpdFreertosInit(HttpdFreertosInstance *pInstance,
                                  const HttpdBuiltInUrl *fixedUrls,
                                  int port, HttpdFlags flags);

/* NOTE: listenAddress is in network byte order */
HttpdInitStatus httpdFreertosInitEx(HttpdFreertosInstance *pInstance,
									const HttpdBuiltInUrl *fixedUrls, int port,
									uint32_t listenAddress, int maxConnections,
									HttpdFlags flags);