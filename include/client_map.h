#include <unordered_map>
#include "buffer.h"
class CLClientMap
{
private:
    std::unordered_map<int, std::string> data_map;
    std::unordered_map<int, int> sock_cli_map;
    std::unordered_map<int, int> cli_sock_map;
    std::unordered_map<int, CLBuffer> sock_buff_map;

public:
    void addData(int client_id, std::string data);
    std::string getData(int client_id);

    void addBindPair(int socket_fd, int client_id);
    int getClientId(int socket_fd);
    int getSocketFd(int client_id);
};