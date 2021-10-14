#include "client_map.h"
void CLClientMap::addData(int client_id, std::string data)
{
    data_map.insert(pair<int, std::string>(client_id, data));
}
std::string CLClientMap::getData(int client_id)
{
    return data_map.at(client_id);
}
/**
 * 0 means add pair to sock_cli_map
 * 1 means add pair to cli_sock_map
**/
void CLClientMap::addBindPair(int socket_fd, int client_id, int option)
{
    if (option == 0)
        sock_cli_map.insert(pair<int, int>(socket_fd, client_id));
    if (option == 1)
        cli_sock_map.insert(pair<int, int>(client_id, socket_fd));
}
int CLClientMap::getClientId(int socket_fd)
{
    return sock_cli_map.at(socket_fd);
}
int CLClientMap::getSocketFd(int client_id)
{
    return cli_sock_map.at(client_id);
}