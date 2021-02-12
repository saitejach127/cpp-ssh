#include <sys/socket.h>  // For socket
#include <netinet/in.h>  // For IP PROTO TCP
#include <arpa/inet.h>   // For inet addr ip mentioning
#include <bits/stdc++.h> // For memset
#include <unistd.h> // For close socket
#include <stdexcept> // For running commandss
using namespace std;
#define PORT 4322

class Dirstack{
    string arr[100];
    int index = -1;
    public :
        void push(string dir){
            arr[++index] = dir;
        }

        string pop(){
            if(index == -1){
                return "None";
            }
            return arr[index--];
        }

        void splitinsert(string path){
            string temp = "";
            for(int i=0;i<path.length();i++){
                if(path[i] == '/'){
                    if(temp != "" && temp != "\n"){
                       push(temp);
                       temp = "";
                    }
                } else {
                    temp += path[i];
                }
            }
            if(temp != ""){
                push(temp);
                temp = "";
            }
        }

        string cwd(){
            string tempdir = "/";
            for(int i=0;i<=index;i++){
                tempdir += arr[i];
                tempdir += "/";
            }
            return tempdir;
        }
};

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
    Dirstack st;
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
    cout<<"Connected"<<endl;
    string cwd = exec("pwd");
    cwd.erase(cwd.length()-1);
    st.splitinsert(cwd);
    cout<<"Sent path"<<endl;
    while (1)
    {
        string currpath = st.cwd();
        send(nsfd, currpath.c_str(), 1000 , 0);
        char buffer[1024];
        bzero(buffer, sizeof(buffer));
        // memset(buffer, sizeof(buffer));
        int temp = recv(nsfd, buffer, 1000, 0);
        string tstr = buffer;
        cout<<tstr<<endl;
        if(tstr == "exit"){
            break;
        }
        currpath = st.cwd();
        string result = exec("cd " + currpath + " && " + tstr);
        if(tstr[0] == 'c' && tstr[1] == 'd'){
            string dir = "";
            for(int i=3;i<tstr.length();i++){
                dir+= tstr[i];
            }
            cout<<"DIRECTORY "<<dir<<endl;
            if(dir == ".."){
                st.pop();
            } else {
                string grepcmd = "ls | grep " + dir;
                string grepres = exec("cd " + currpath + " && " + grepcmd);
                cout<<"GREP RESULT"<<grepres<<endl;
                if(grepres.length() != 0){
                    st.push(dir);
                } else {
                    result = "NO_EXISTING_DIR";
                }
            }
        }
        send(nsfd, result.c_str(), 1000,0);
    }
    close(nsfd);
    close(sfd);
    return 0;
}