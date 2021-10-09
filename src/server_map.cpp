#include "server_map.h"
bool CLServerMap::isAvailable(int connfd)
{
    return available_map.at(connfd);
}
void CLServerMap::addAvailablePair(int connfd, bool status)
{
    available_map.insert(pair<int, bool>(connfd, status));
}
void CLServerMap::modifyAvailablePair(int connfd, bool status)
{
    std::unordered_map<int, bool>::iterator it;
    it = available_map.find(connfd);
    it->second = status;
}

void CLServerMap::addSession(int key_connfd, int value_connfd)
{
    session_map.insert(pair<int, int>(key_connfd, value_connfd));
}
int CLServerMap::searchSessionPair(int key_connfd)
{
    return session_map.at(key_connfd);
}

void CLServerMap::addData(int value_connfd, std::string data)
{
    data_map.insert(pair<int, std::string>(value_connfd, data));
}
std::string CLServerMap::getData(int value_connfd)
{
    return data_map.at(value_connfd);
}