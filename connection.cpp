#include "connection.h"
#include "iostream"

Connection::Connection(unsigned short port, int qlen):
    sock(socket(AF_INET, SOCK_STREAM, 0)), // TCP сокет
    serv_addr(new sockaddr_in), // пустая адресная структура сервера
    client_addr(new sockaddr_in), // пустая адресная структура клиента
    queue_len(qlen)
{
    //проверка создания сокета
    if (sock == -1 )
        throw std::system_error(errno, std::generic_category(), "socket error");
    //заполнение адресной структуры сервера
    serv_addr->sin_family = AF_INET; // всегда так
    serv_addr->sin_port = htons(port); // конкретное значение
    serv_addr->sin_addr.s_addr = inet_addr("127.0.0.1"); //конкретное значение
    //Привязка сокета к адресу сервера
    int rc = bind(sock, reinterpret_cast<const sockaddr*>(serv_addr.get()), sizeof(sockaddr_in));
    if (rc == -1 )
        throw std::system_error(errno, std::generic_category(), "bind error");

}
void Connection::connect(Interface & str)
{
    //режим ожидания соединения для сокета
    if (listen(sock, queue_len)==-1)
        throw std::system_error(errno, std::generic_category(), "listen error");
    socklen_t len = sizeof (sockaddr_in);
    //бесконесный цикл обработки входящих соединений
    while(1) {
        int work_sock = accept(sock, reinterpret_cast<sockaddr*>(client_addr.get()), &len);
        if (work_sock == -1) {
            throw std::system_error(errno, std::generic_category(), "accept error");
        }
        std::string ip_addr(inet_ntoa(client_addr->sin_addr));
        std::clog << "log: Соединение установлено с " << ip_addr <<std::endl;
        try {
            str(work_sock);	// serve the client
        } catch (std::system_error &e) {
            std::cerr << e.what() << "\nConnection with " << ip_addr << " aborted\n";
        }
        close(work_sock);
    }
    //char buf[1024];
    //rc = recv(s, buf, sizeof buf,0);

    //char msg[] = "What a hell\n";
    //rc = send(s, msg, sizeof msg, 0);
}
