#include "../Common.h"

#define TESTNAME "google.com"

bool GetIpaddr(const char* name, struct in_addr *addr){
	struct hostent *ptr = gethostbyname(name);
	if(ptr==NULL){
		err_display("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET) return false;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return true;
	
}

bool GetDomainName(struct in_addr addr, char* name, int namelen){
	struct hostent *ptr = gethostbyaddr((const char*) &addr, sizeof(addr), AF_INET);
	if(ptr==NULL){
		err_display("gethostbyadd()");
		return false;
	}
	if(ptr->h_addrtype != AF_INET) return false;
	strncpy(name,ptr->h_name,namelen);
	return true;
}

int main(int argc, char *argv[]){
	printf("도메인 이름 변환전 = %s\n",TESTNAME);

	struct in_addr addr; // 32바이트 주소
	if(GetIpaddr(TESTNAME,&addr)){
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr, str, sizeof(str));
		printf("ip 주소 변환 후 = %s\n",str);
		
		char name[256];
		if(GetDomainName(addr,name,sizeof(name))) printf("도메인 네임 다시 변환 = %s",name);
	}
}
