#include <cstdio>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace {

const int kINet6AddrStrLen = 100;  // Long enough to hold the string.

}

void show_ip_test() {
  addrinfo hints;
  addrinfo *res, *p;
  int status;

  char ipstr[kINet6AddrStrLen];

  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_UNSPEC;  // AF_INET or AF_INET6 to force version.
  hints.ai_socktype = SOCK_STREAM;

  status = getaddrinfo("www.google.com", NULL /* 80 */,
                       &hints, &res);

  if (status) {
    printf("getaddrinfo: %s\n", gai_strerror(status));
    return;
  }

  printf("IP addresses below.\n");

  printf("Address of res: %lld.\n", (long long)(res));

  char ipver[10];
  for (p = res; p; p = p->ai_next) {
    void *addr;

    // Get the pointer to the address itself,
    // different fields in IPv4 and IPv6.
    if (p->ai_family == AF_INET) {  // IPv4
      sockaddr_in *ipv4 = (sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      strcpy(ipver, "IPv4");
    } else {
      sockaddr_in6 *ipv6 = (sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      strcpy(ipver, "IPv6");
    }

    printf("ipver: %s\n", ipver);

    // Convert the IP to a string and print it.
    inet_ntop(p->ai_family, addr, ipstr, sizeof(char) * kINet6AddrStrLen);
    printf(" %s: %s\n", ipver, ipstr);

    int s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    printf("s = %d\n", s);
  }

  freeaddrinfo(res);  // Free the linked list.
}

int main() {
  show_ip_test();

  return 0;
}

