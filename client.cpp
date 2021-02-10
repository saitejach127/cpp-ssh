#include <sys/socket.h>	 // For socket
#include <unistd.h>		 // For socket reading
#include <netinet/in.h>	 // For IP PROTO TCP
#include <arpa/inet.h>	 // For inet addr ip mentioning
#include<bits/stdc++.h> // For bzero and cin cout
using namespace std;
#define PORT 4322

int main(){
	string text, output;
    char buffer[1024];
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in seraddr;
	//printf("%d\n",sfd);
	seraddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(PORT);
	bzero(&seraddr.sin_zero, 8);
	int k1 = connect(sfd, (struct sockaddr *)&seraddr, sizeof(struct sockaddr_in));
	while (1)
	{
        cout<<"shell>";
		getline(cin, text);
		send(sfd, text.c_str(), sizeof(text), 0);
        if(text == "exit"){
            break;
        }
        int statuscode = recv(sfd, buffer, 1000,0);
        output = buffer;
        cout<<output<<endl;
    }
	close(sfd);
	return 0;
}