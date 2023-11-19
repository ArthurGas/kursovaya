#include "connection.h"

//#include <boost/asio.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

//namespace as = boost::asio;

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
    //установка параметров для сокета: настройка времени приема данных
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 5000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (rc == -1 )
        throw std::system_error(errno, std::generic_category(), "Socket setopt error");
    //заполнение адресной структуры сервера
    serv_addr->sin_family = AF_INET; // всегда так
    serv_addr->sin_port = htons(port); // конкретное значение
    serv_addr->sin_addr.s_addr = inet_addr("127.0.0.1"); //конкретное значение
    //Привязка сокета к адресу сервера
    rc = bind(sock, reinterpret_cast<const sockaddr*>(serv_addr.get()), sizeof(sockaddr_in));
    if (rc == -1 )
        throw std::system_error(errno, std::generic_category(), "bind error");

}
void Connection::connect(Calculation& clc, Auth& ath)
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
            ath(work_sock); //передача сокета модулю аутентификации и выполнении идент. и аутент.
            //intr(work_sock); //передача сокета модулю интерфейса
            clc(work_sock);	//передача сокета модулю вычислений и расчет
        } catch (std::system_error &e) {
            std::cerr << e.what() << "\nConnection with " << ip_addr << " aborted\n";
        }
        std::clog << "log: Соединение закрыто c " << ip_addr <<std::endl;
        close(work_sock);
    }
}
