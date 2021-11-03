#include <vector>
using namespace std;
class CLBuffer
{
private:
    vector<char> buffer;
    //使用整数下标而不是指针，以防止vector自动增长时，指针失效
    int writeIndex;
    int write_n;
    int read_n;
    //一个缓冲区对应一个客户端/服务端socket
    int socket_fd;
    int string_length;

public:
    CLBuffer(int socket_fd, int string_length);
    int writeByBuffer(char *data);
    int readByBuffer();
    void getDataFromBuffer(char *data);
};