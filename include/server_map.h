#include <unordered_map>
using namespace std;
class CLServerMap
{
private:
    std::unordered_map<int, bool> available_map;
    std::unordered_map<int, int> session_map;
    std::unordered_map<int, std::string> data_map;

public:
    bool isAvailable(int connfd);
    void addAvailablePair(int connfd, bool status);
    void modifyAvailablePair(int connfd, bool staus);

    void addSession(int key_connfd, int value_connfd);
    int searchSessionPair(int key_connfd);

    void addData(int value_connfd, std::string data);
    std::string getData(int value_connfd);
};