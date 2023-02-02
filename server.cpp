#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "example.pb.h"

using namespace std;

const int PORT = 8081;

int main() {
  int socket_desc, client_socket;
  sockaddr_in server_address, client_address;

  // Create a socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  // Bind to an address and port
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;
  if (bind(socket_desc, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
    std::cerr << "Error binding socket" << std::endl;
    return 1;
  }

  // Listen for incoming connections
  if(listen(socket_desc, 10)<0){
    std::cout<<"Error listening on socket"<<std::endl;
  }

  std::cout << "Server is listening on port " << PORT << std::endl;

  // Accept a connection
  socklen_t client_address_length = sizeof(client_address);
  client_socket = accept(socket_desc, (sockaddr*)&client_address, &client_address_length);
  if (client_socket < 0) {
    std::cerr << "Error accepting connection" << std::endl;
    return 1;
  }

  while (true) {
    // Receive data
    example::Message request;
    int size;
    recv(client_socket, &size, sizeof(size), 0);
    char* buffer = new char[size];
    recv(client_socket, buffer, size, 0);
    request.ParseFromArray(buffer, size);
    delete[] buffer;

    // Print received data
    std::cout << "Received: "<<endl;
    cout<<"Name: "<<request.user_name()<<endl;
    cout<<"Message: "<<request.user_message()<<endl;

    //call script
    std::string s=request.user_message();
    string name=request.user_name();
    std::string command = "./count_letters.sh ";
    command += name;
    command += " ";
    command += s;
    int letter_cnt=system(command.c_str());

    if(letter_cnt==-1){
      cout<<"Failed to run the script"<<endl;
      return 1;
    }

    int exit_status=WEXITSTATUS(letter_cnt);


    // Send data
    example::Message response;
    response.set_user_name(request.user_name());
    response.set_user_message(request.user_message());
    response.set_letter_count(exit_status);
    size = response.ByteSizeLong();
    buffer = new char[size];
    response.SerializeToArray(buffer, size);
    send(client_socket, &size, sizeof(size), 0);
    send(client_socket, buffer, size, 0);

    delete[] buffer;

  }

  // Close sockets
  close(client_socket);
  close(socket_desc);
  

  return 0;
}
