#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h> // For exit()
#include <unistd.h> // For close()
#include <iostream>
#include <chrono>
#include "message.h"
#include <iomanip> 


int main(void) {                
    struct sockaddr_in server;
    int server_socket;
    int client_socket;
    int rc;
    char response = '1'; 


    server.sin_family = AF_INET; // address family 
    server.sin_port = htons(7500); // port 
    server.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY - conection from any interface acceptable 

    // create socket 
    server_socket = socket(AF_INET, SOCK_STREAM, 0);  //SOCK_STREAM (TCP), SOCK_DGRAM (UDP), SOCK_RAW (IP)
    if (server_socket < 0) {
        perror("Error socket creation");
        exit(1);
    }
    
    //  bind is used only  on the server side. It assigns a specific local IP address and port to a socket so that clients know where to connect 
    rc = bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    if (rc < 0) {
        perror("Error bind call");
        exit(1);
    }

    // change socket to listen mode 
    rc = listen(server_socket, 5); // second param (backlog) - max number of connections in line to connect
    if (rc < 0) { 
        perror("Error listen call");
        exit(1);
    }
    
    std::cout << std::endl << "Server is listening on port 7500..." << std::endl;

    while (true) { 

        //Waiting from client connection
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Error accept call");
            continue; // Go to next connction
        }
        std::cout  << std::endl << "Client connected!" << std::endl<< std::endl;

        auto begin = std::chrono::high_resolution_clock::now();
        int dataSize =0;
        while(true){

            
             // // Recieve client data
            Message message; 
            rc = recv(client_socket, (void*)&message, sizeof(message), 0);
            if (rc <= 0) {
                 std::cout  << std::endl << "Client breaks connection" << std::endl;
                close(client_socket);
                break;; // Go to next connction
            }        
            std::cout << "Received data: ";
            message.print();

            dataSize += rc;

            // Send answer to client
            rc = send(client_socket, (void*)&response, sizeof(response), 0);
            if (rc <= 0) {
                perror("Error send call");
            }
        
        }

        auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

		std::cout << std::endl << std::endl << "Data exchange time: " << std::fixed << std::setprecision(5) << duration.count() * 1e-9 << " seconds." << std::endl;
        std::cout << "Total received data: " << dataSize << " bytes" << std::endl;
       
    }

    return 0;
}
