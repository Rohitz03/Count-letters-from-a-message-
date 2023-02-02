#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main() {
    string inputString = "hello this is game";
    // create the command to run the bash script
    string command = "./count_letters.sh ";
    command += inputString;

    // run the command in the shell
    int result = system(command.c_str());

    int ans=WEXITSTATUS(result);
    cout<<ans;

    return result;
}
