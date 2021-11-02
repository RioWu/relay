#include <unordered_map>
class CLServerMap
{
private:
    std::unordered_map<int, int> client_conn_map;
    std::unordered_map<int, int> conn_client_map;
    std::unordered_map<int, std::string> data_map;

public:
    void addBindPair(int client_id, int conn_fd);
    int getClientId(int conn_fd);
    int getConnFd(int client_id);
    void addData(int client_id, std::string data);
    void deleteData(int client_id);
    std::string getData(int client_id);
};