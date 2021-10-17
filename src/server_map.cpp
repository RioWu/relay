#include "server_map.h"

void CLServerMap::addBindPair(int client_id, int conn_fd)
{
    client_conn_map.insert(pair<int, int>(client_id, conn_fd));
    conn_client_map.insert(pair<int, int>(conn_fd, client_id));
}
int CLServerMap::getClientId(int conn_fd)
{
    return conn_client_map.at(conn_fd);
}
int CLServerMap::getConnFd(int client_fd)
{
    return client_conn_map.at(client_fd);
}
void CLServerMap::addData(int client_id, std::string data)
{
    data_map.insert(pair<int, std::string>(client_id, data));
}
std::string CLServerMap::getData(int client_id)
{
    return data_map.at(client_id);
}
void CLServerMap::deleteData(int client_id)
{
    data_map.erase(client_id);
}