#include "../Common.h"

int main(int argc, char *argv[]){
	const char *ipv4test = "127.0.0.1";
	printf("IPv4 주소 변환전 =%s\n",ipv4test);
	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("IPv4 주소(변환 ) = %#x\n", ipv4num.s_addr);
	
	char ipv4str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("IPv4 주소(다시 변환 후) = %s\n", ipv4str);
}
