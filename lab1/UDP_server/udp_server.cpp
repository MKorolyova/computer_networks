#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "message.h"
#include <iomanip> 

int main() {
    struct sockaddr_in server, client; // Structure to store local address information
    int server_socket; // Socket descriptor
    int rc;
    
    server.sin_family  = AF_INET; // IPv4 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(8500); 
    
    server_socket = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket
    if (server_socket < 0){// Check if the socket was created successfully
        perror("Socket creation error");
        exit(1);
    } 
        
    rc = bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    if (rc < 0) {
        perror("Bind error");
        exit(1);
    }
    
    std::cout << std::endl << "Server is listening on port 8500..." << std::endl;
    // Start the server logic
    while(true) {
        char request; 
        socklen_t client_len = sizeof(client);
        rc = recvfrom(server_socket, (void*)&request, sizeof(request), 0, (struct sockaddr *)&client, &client_len );
        if ( rc < 0 ){
             perror("Recvfrom call error");
             exit(1);
        }

        Message message;
        if(request == '1'){
            message.setFloat(0.25);
        }else if (request == '2'){
            message.setChar('a');
        }

        rc = sendto(server_socket, (void*)&message, sizeof(message), 0, (struct sockaddr *)&client, client_len);
        if ( rc < 0){
             perror("Sendto call error");
             exit(1);
        }

    }
    exit(0); // Exit successfully
}
