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
#include <exception>
#include <cmath>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std; 

struct DestConfig{
    short listenport;
    void read_config(string file);
};

void DestConfig::read_config(string file){
    ifstream in(file.c_str());
    if(in.good()){
        in >> listenport;
    }else 
        throw exception();
}

struct Packet{
    int data;
};

void error(string msg){
    cerr << msg << endl;
    exit(1);
}

int main(int argc, char ** argv)
{
    if(argc == 1){
        cout << "Configuration file not specified" << endl;
        exit(1);
    }
    DestConfig cfg;    
    cfg.read_config(argv[1]);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error("Socket creatio failed");
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(cfg.listenport);
    int res = bind(sock,(sockaddr*)&serveraddr,sizeof(serveraddr));
    if(res == -1)
        error("Bind Error");
    res = listen(sock,20);
    if(res == -1)
        error("Listen Error");
    cout << "Listening on port " << cfg.listenport << endl;
    
    struct sockaddr_in clientaddr;
    while(true){
        socklen_t len = sizeof(clientaddr);
        int clientsock = accept(sock,(sockaddr*)&clientaddr,&len);
        Packet p; 
        read(clientsock,&p,sizeof(p));
        cout << "Packet get from source : " << p.data << endl;
        close(clientsock);
    }
    
    return 0; 
}

