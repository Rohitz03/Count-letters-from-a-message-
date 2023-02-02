#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "example.pb.h"

using namespace std;

const int PORT = 8081;


int main() {
    int socket_desc;
    sockaddr_in server_address;

    // Create a socket
    // socket() function returns a socket descriptor that can be used to 
    // refer to that socket in later function calls 
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Connect to a server
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    //connects the socket referred to by the socket descriptor to a remote socket 
    //specified by the address and port 
    if (connect(socket_desc, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    while (true) {
        // Take message from user
        string message;
        string user_name;
        int age;
        cout<<"Enter name: "<<endl;
        getline(cin,user_name);
        cout << "Enter message: "<<endl;
        getline(cin, message);
        

        // Send data
        example::Message request;
        request.set_user_name(user_name);
        request.set_user_message(message);

        int size = request.ByteSizeLong();
        char* buffer = new char[size];
        request.SerializeToArray(buffer, size);

        //sends data over a connected socket
        send(socket_desc, &size, sizeof(size), 0);
        send(socket_desc, buffer, size, 0);
        delete[] buffer;

        // Receive data
        example::Message response;
        recv(socket_desc, &size, sizeof(size), 0);
        buffer = new char[size];
        recv(socket_desc, buffer, size, 0);
        response.ParseFromArray(buffer, size);
        delete[] buffer;

        // Print received data
        std::cout << "Received response from server : " <<endl;
        cout<<"Name: "<<response.user_name()<<endl;
        cout<<"Message was: "<<response.user_message() << std::endl;
        cout<<"Letter count: "<<response.letter_count()<<endl;
    }

    // Close socket
    close(socket_desc);

    return 0;
}
