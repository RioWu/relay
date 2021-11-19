#include "../include/buffer.h"
#include <errno.h>
#include <cstring>
#include <unistd.h>
CLBuffer::CLBuffer(int socket_fd, int string_length)
{
    CLBuffer::buffer = vector<char>(0);
    CLBuffer::socket_fd = socket_fd;
    CLBuffer::string_length = string_length;
    CLBuffer::write_n = 0;
    CLBuffer::read_n = 0;
    CLBuffer::writeIndex = 0;
}
/**
 *  return 1 means all data has been sent
 *  put data after writeIndex
 **/
int CLBuffer::writeByBuffer(char *data)
{
    int n = 0;
    // if there is no data writable,should put data first.
    if (writeIndex == (int)buffer.size())
    {
        for (int i = 0; i < string_length; i++)
        {
            buffer.push_back(*(data + i));
        }
    }
    while (errno != EAGAIN && write_n != string_length)
    {
        //&* 表示取迭代器->取迭代器的值->取地址
        n = write(socket_fd, &*(buffer.begin() + writeIndex), buffer.size() - writeIndex);
        buffer.erase(buffer.begin() + writeIndex, buffer.begin() + writeIndex + n - 1);
        writeIndex = writeIndex + n;
        write_n = write_n + n;
    }
    if (write_n == string_length)
    {
        return 1;
    }
    else
        return 0;
}
/**
 *  return 1 means all data has been received
 *  put data after begin()
 **/
int CLBuffer::readByBuffer()
{
    int n = 0;
    int tem_buf_index = 0;
    char tem_buf[string_length] = {'\0'};
    while (errno != EAGAIN && read_n != string_length)
    {
        n = read(socket_fd, tem_buf + tem_buf_index, string_length);
        for (int i = 0; i < n; i++)
        {
            // vector.insert(iterator,data) 在iterator表示的这个元素之前插入新数据
            buffer.insert(buffer.begin() + writeIndex + i, tem_buf[i]);
        }
        writeIndex = writeIndex + n;
        read_n = read_n + n;
        tem_buf_index = tem_buf_index + n;
    }
    if (read_n == string_length)
        return 1;
    else
        return 0;
}
// when finish read,use this function to get data
void CLBuffer::getDataFromBuffer(char *data)
{
    for (int i = 0; i < string_length; i++)
    {
        data[i] = buffer[i];
    }
    buffer.erase(buffer.begin(), buffer.begin() + writeIndex - 1);
    writeIndex = 0;
}
