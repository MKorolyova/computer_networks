#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h> // Для exit()
#include <unistd.h> // For close()
#include <iostream>
#include "message.h"

int main() {
    struct sockaddr_in peer;
    int s;
    int rc;
    char buf[1];
    
    // Инициализация структуры sockaddr_in
    peer.sin_family = AF_INET;
    peer.sin_port = htons(7500);  //port number
    peer.sin_addr.s_addr = inet_addr("172.21.0.4"); //ip address 127.0.0.1 / docker inspect 
    // Создание сокета
    s = socket(AF_INET, SOCK_STREAM, 0);  //SOCK_STREAM (TCP), SOCK_DGRAM (UDP), SOCK_RAW (IP)
    if (s < 0) {
        perror("Ошибка вызова socket");
        exit(1);
    }

    // Create connection to server
    rc = connect(s, (struct sockaddr *)&peer, sizeof(peer));
    if (rc < 0) { 
        perror("Connection refused");
        exit(1);
    }

    
    Message message;
    float numberToSend = 0.25;

    for(int i=0; i<9; i++){
        
        message.setFloat(numberToSend);

        // Send data
        rc = send(s, (void*)&message, sizeof(message), 0);
        if (rc <= 0) {
            perror("Ошибка вызова send");
            exit(1);
        }
        numberToSend ++; 
  
        char receivedChar;
        // Recieve client data
        rc = recv(s, (void*)&receivedChar, sizeof(receivedChar), 0);
        if (rc <= 0) {
            perror("Error recv call");
            exit(1);
        }


    }

    char charToSend = 'a';
    for(int i=0; i<5; i++){

        message.setChar(charToSend);

        // Send data
        rc = send(s, (void*)&message, sizeof(message), 0);
        if (rc <= 0) {
            perror("Ошибка вызова send");
            exit(1);
        }
        charToSend++; 
  
        char receivedChar;
        // Recieve client data
        rc = recv(s, (void*)&receivedChar, sizeof(receivedChar), 0);
        if (rc <= 0) {
            perror("Error recv call");
            exit(1);
        }

    }

    std::cout << "Messages was sent successfully!" << std::endl;
    close(s); 


    return 0; 
}
