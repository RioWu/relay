#include "client_map.h"
using namespace std;
void CLClientMap::addData(int client_id, std::string data)
{
    data_map.insert(pair<int, std::string>(client_id, data));
}
std::string CLClientMap::getData(int client_id)
{
    if (data_map.find(client_id) == data_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return data_map.at(client_id);
}
void CLClientMap::addBindPair(int socket_fd, int client_id)
{
    sock_cli_map.insert(pair<int, int>(socket_fd, client_id));
    cli_sock_map.insert(pair<int, int>(client_id, socket_fd));
}
int CLClientMap::getClientId(int socket_fd)
{
    if (sock_cli_map.find(socket_fd) == sock_cli_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return sock_cli_map.at(socket_fd);
}
int CLClientMap::getSocketFd(int client_id)
{
    if (cli_sock_map.find(client_id) == cli_sock_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return cli_sock_map.at(client_id);
}