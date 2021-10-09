ip:104.225.151.183

密码 crw15525881104

# Install new OS

  Operating system ubuntu-18.04-x86_64 has been successfully installed on server localhost.localdomain [104.225.151.183].

Your temporary root password: IyFinIi1FEL1

New SSH Port: 28882

Server IP:104.225.151.183

Server Port:20171

Password: Ruveymus9

Encryption: chacha20-ietf-poly1305



服务端：ss-server-libev [安装教程](https://my.oschina.net/u/2663124/blog/1551299)

ss-server:config.json

```
{
    "server":"104.225.151.183",
    "server_port":20171,
    "local_port":1080,
    "password":"Ruveymus9",
    "timeout":60,
    "method":"chacha20-ietf-poly1305"
}
```



启动服务端：

nohup ss-server -c /etc/shadowsocks-libev/config.json &
