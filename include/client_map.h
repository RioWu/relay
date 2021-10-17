#include <unordered_map>
using namespace std;
class CLClientMap
{
private:
    std::unordered_map<int, std::string> data_map;
    std::unordered_map<int, int> sock_cli_map;
    std::unordered_map<int, int> cli_sock_map;

public:
    void addData(int session_id, std::string data);
    std::string getData(int session_id);

    void addBindPair(int socket_fd, int client_id);
    int getClientId(int socket_fd);
    int getSocketFd(int clinet_id);
};