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
        in >> cfg.destip;
        in >> cfg.destport;     
    }else 
        throw exception();

    return cfg;
}


int main(int argc, char** argv)
{
    if(argc  <= 1){
        cout  << "No configuration file specified"  << endl;
        exit(1);
    }
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error("socket created failed");
    struct sockaddr_in addr;
    Config cfg = read_config(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(cfg.destip.c_str());
    addr.sin_port = htons(cfg.destport);
    int len = sizeof(addr);
    int res = connect(sock,(sockaddr*) & addr,len);
    if(res == -1)
        error("Connect error");
    Packet p; 
    p.data = 1234;
    write(sock,&p,sizeof(p));
    close(sock);        
}

