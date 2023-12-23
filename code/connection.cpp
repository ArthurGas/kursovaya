#include "connection.h"

Connection::Connection(unsigned short port):
    sock(socket(AF_INET, SOCK_STREAM, 0)), // TCP сокет
    serv_addr(new sockaddr_in), // пустая адресная структура сервера
    client_addr(new sockaddr_in) // пустая адресная структура клиента
{
    //проверка создания сокета
    if (sock == -1)
        throw std::system_error(errno, std::generic_category(), "Socket error");
    //установка параметров для сокета: возможность исп. сокета сразу после закрытия сервера
    int on = 1;
    int rc = setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    if (rc == -1 )
        throw std::system_error(errno, std::generic_category(), "Socket setopt error");
    //установка параметров для сокета: настройка таймаута для снятия блокировки

    struct timeval timeout {
        60, 0
    };
    rc = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (rc == -1 )
        throw std::system_error(errno, std::generic_category(), "Socket setopt error");
    //заполнение адресной структуры сервера
    serv_addr->sin_family = AF_INET; // всегда так
    serv_addr->sin_port = htons(port); // конкретное значение
    serv_addr->sin_addr.s_addr = inet_addr("127.0.0.1"); //конкретное значение
    //Привязка сокета к адресу сервера
    rc = bind(sock, reinterpret_cast<const sockaddr*>(serv_addr.get()), sizeof(sockaddr_in));
    if (rc == -1 )
        throw std::system_error(errno, std::generic_category(), "Bind error");

}
void Connection::connect(Calculation& clc, Auth& ath, Error& er)
{

    //режим ожидания соединения для сокета
    if (listen(sock, queue_len)==-1){
        er.critic_set();
        throw std::system_error(errno, std::generic_category(), "Listen error");
    }
    socklen_t len = sizeof (sockaddr_in);
    //бесконесный цикл обработки входящих соединений
    while(1) {
        int work_sock = accept(sock, reinterpret_cast<sockaddr*>(client_addr.get()), &len);
        if (work_sock == -1) {
            throw std::system_error(errno, std::generic_category(), "Accept error");
        }
        std::string ip_addr(inet_ntoa(client_addr->sin_addr));
        std::clog << "log: Соединение установлено с " << ip_addr <<std::endl;
        ath(work_sock, er); //передача сокета модулю аутентификации и выполнении идент. и аутент.
        clc(work_sock, er);	//передача сокета модулю вычислений и расчет
        std::clog << "log: Соединение закрыто c " << ip_addr <<std::endl;
        close(work_sock);
    }
}
