/*                                                                              
 ============================================================================
 Name        : 
 Author      : Zhihui,Jiang             
 Email       : coboy123456@gmail.com    
 Version     : 0.0 
 Copyright   : Your copyright notice    
 Description :  
 ============================================================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map> 
#include <set>
#include <deque>
#include <list>
#include <algorithm>
#include <cmath>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std; 

const int MAX_LISTEN = 10;
void error(string s){
    cerr  << s  << endl;
    exit(1);
}

struct Config{
    string destip;
    short destport; 
    short listenport;
};

struct Packet{
    int data;
};

Config read_config(const char * file){
    ifstream in(file);
    Config cfg ; 
    if(in.good()){
        in >> cfg.listenport;
        in >> cfg.destip;
        in >> cfg.destport;     
    }else 
        throw exception();

    return cfg;
}

int main(int argc, char ** argv)
{
    if(argc <= 1){
        cout << "No configuration file speficied" << endl;
        exit(1);
    }

    int sock,clientsock;
    //serversock is the socket of the step switch's communication socket
    int serversock = socket(AF_INET,SOCK_STREAM,0);    
    if(serversock == -1)
        error("socket creation error");
    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error("socket creation failed");
  
    Config cfg = read_config(argv[1]);
    struct sockaddr_in addr,clientaddr,serveraddr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(cfg.listenport);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(cfg.destip.c_str());
    serveraddr.sin_port = htons(cfg.destport); 
    cout <<cfg.destip << " " << cfg.listenport << endl;

    int serverlen = sizeof(addr);
    int res = bind(sock,(sockaddr*)&addr,serverlen);
    if(res == -1)
        error("bind error");

    res = listen(sock,MAX_LISTEN);
    if(res == -1)
        error("listen error");
    
    res = connect(serversock,(sockaddr*)&serveraddr,sizeof(serveraddr));
    if(res == -1)
        error("can not connect to server");

    while(true){
        socklen_t clientlen = sizeof(clientaddr);
        clientsock = accept(sock,(sockaddr*)&clientaddr,&clientlen);     
        Packet p; 
        read(clientsock,&p,sizeof(p));
        write(serversock,&p,sizeof(p));
        cout << "a apcket read from client with data : " << p.data << endl;
        close(clientsock);
    }
}

