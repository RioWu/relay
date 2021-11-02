#include "server_map.h"
using namespace std;
void CLServerMap::addBindPair(int client_id, int conn_fd)
{
    client_conn_map.insert(pair<int, int>(client_id, conn_fd));
    conn_client_map.insert(pair<int, int>(conn_fd, client_id));
}
int CLServerMap::getClientId(int conn_fd)
{
    if (conn_client_map.find(conn_fd) == conn_client_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return conn_client_map.at(conn_fd);
}
int CLServerMap::getConnFd(int client_fd)
{
    if (client_conn_map.find(client_fd) == client_conn_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return client_conn_map.at(client_fd);
}
void CLServerMap::addData(int client_id, std::string data)
{
    std::pair<unordered_map<int, std::string>::iterator, bool> ret;
    ret = data_map.insert(pair<int, std::string>(client_id, data));
    if (ret.second == false)
    {
        printf("add data to data_map has failed\n");
        exit(0);
    }
}
std::string CLServerMap::getData(int client_id)
{
    if (data_map.find(client_id) == data_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return data_map.at(client_id);
}
void CLServerMap::deleteData(int client_id)
{
    data_map.erase(client_id);
}