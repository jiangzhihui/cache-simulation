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
};

struct Packet{
    int data;
};

Config read_config(const char * file){
    ifstream in(file);
    Config cfg ; 
    if(in.good()){
        in >> cfg.destport;     
        in >> cfg.destip;
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
    int serversock,clientsock;    
    serversock = socket(AF_INET,SOCK_STREAM,0);
    if(serversock == -1)
        error("socket creation failed");
    Config cfg = read_config(argv[1]);
    struct sockaddr_in serveraddr,clientaddr;
    serveraddr.sin_family = AF_INET;
    //serveraddr.sin_addr.s_addr = inet_addr(cfg.destip.c_str());
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(cfg.destport);
    cout <<cfg.destip << " " << cfg.destport << endl;
    int serverlen = sizeof(serveraddr);
    int res = bind(serversock,(sockaddr*)&serveraddr,serverlen);
    if(res == -1)
        error("bind error");
    res = listen(serversock,MAX_LISTEN);
    if(res == -1)
        error("listen error");
    while(true){
        socklen_t clientlen = sizeof(clientaddr);
        clientsock = accept(serversock,(sockaddr*)&clientaddr,&clientlen);     
        Packet p; 
        read(clientsock,&p,sizeof(p));
        cout << "a apcket read from client with data : " << p.data << endl;
        close(clientsock);
    }
}

