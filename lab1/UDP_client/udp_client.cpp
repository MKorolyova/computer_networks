#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "message.h"
#include <chrono>
#include <iomanip> 

int main() {
    struct sockaddr_in server, client; // Structure to store local address information
    int server_socket; // Socket descriptor
    int rc;
    
    server.sin_family  = AF_INET; // IPv4 
    server.sin_addr.s_addr = inet_addr("172.21.0.2"); 
    server.sin_port = htons(8500); 
    
    server_socket = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket
    if (server_socket < 0){// Check if the socket was created successfully
        perror("Socket creation error");
        exit(1);
    } 
            
    // Start the server logic
    auto begin = std::chrono::high_resolution_clock::now();
    int dataSize = 0;
    std::cout<< std::endl << std::endl << "Client start " << std::endl <<std::endl;

    for(int i = 0; i < 5; i++) {
        
        char request = '1'; 
        rc = sendto(server_socket, (void*)&request, sizeof(request), 0, (struct sockaddr *)&server, sizeof(server));
        if ( rc < 0){
             perror("Sendto call error");
             exit(1);
        }

        Message message;
        rc = recvfrom(server_socket, (void*)&message, sizeof(message), 0, (struct sockaddr *)&client, (socklen_t*)&client);
        if ( rc < 0 ){
             perror("Recvfrom call error");
             exit(1);

        }
        dataSize += rc;
        std::cout <<  "Response: ";
        message.print();

    }

    
    for(int i = 0; i < 9; i++) {
        auto begin = std::chrono::high_resolution_clock::now();

        char request = '2'; 
        rc = sendto(server_socket, (void*)&request, sizeof(request), 0, (struct sockaddr *)&server, sizeof(server));
        if ( rc < 0){
             perror("Sendto call error");
             exit(1);
        }


        Message message;
        rc = recvfrom(server_socket, (void*)&message, sizeof(message), 0, (struct sockaddr *)&client, (socklen_t*)&client);
        if ( rc < 0 ){
             perror("Recvfrom call error");
             exit(1);

        }
        dataSize += rc;
        std::cout <<  "Request: ";
        message.print();

    }

    std::cout << std::endl << "Total received data: " << dataSize << " bytes" << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    std::cout <<  "Data exchange time: " << std::fixed << std::setprecision(5) << duration.count() * 1e-9 << " seconds." << std::endl << std::endl ;


    
    exit(0); // Exit successfully
}
