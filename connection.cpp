#include "connection.h"
#include "iostream"

void Connection::connect(unsigned short port, int qlen)
{
    std::unique_ptr <sockaddr_in> serv_addr(new sockaddr_in); // адресная структура сервера
    serv_addr->sin_family = AF_INET; // всегда так
    serv_addr->sin_port = htons(port); // конкретное значение
    serv_addr->sin_addr.s_addr = inet_addr("127.0.0.1");//конкретное значение

    int s = socket(AF_INET, SOCK_DGRAM, 0); // TCP
    if ( s == -1 )
        throw std::system_error(errno, std::generic_category());

    int rc = bind(s, reinterpret_cast<const sockaddr*>(serv_addr.get()), sizeof(sockaddr_in));
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    rc = listen(s, qlen);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category());
    //socklen_t len = sizeof (sockaddr_in);
    //sockaddr_in * client_addr = new sockaddr_in;
    socklen_t len = sizeof (sockaddr_in);
    std::string ip_addr;
    int work_sock;
    while(1) {
        std::unique_ptr <sockaddr_in> client_addr(new sockaddr_in); // пустая адресная структура клиента
        int work_sock = accept(s, reinterpret_cast<sockaddr*>(client_addr.get()), &len);
        std::string ip_addr(inet_ntoa(client_addr->sin_addr));
        if (work_sock == -1) {
            throw std::system_error(errno, std::generic_category());
        }

    }
    std::clog << "log: Соединение установлено с " << ip_addr <<std::endl;

    char buf[1024];
    rc = recv(s, buf, sizeof buf,0);

    char msg[] = "What a hell\n";
    rc = send(s, msg, sizeof msg, 0);
    close(work_sock);
}
