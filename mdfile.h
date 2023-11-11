#include <iostream>
#include <getopt.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
using namespace std;
using namespace CryptoPP;
#include <cryptopp/md5.h>
string MD(string sah);
void errors(string error, string name);
int er(string file_name, string file_error);
void msgsend(int work_sock, string mess);
int autorized(int work_sock, string file_name, string file_error);

class ErrWriter {
private:
	string err_file;
public:
	ErrWriter(const string& errors):err_file(errors){};
	ErrWriter(const ErrWriter& other):err_file(other.err_file){};
	void write_err(string& err);
};

class Communicator {
private:
    int sock;                                  // Сокет сервера
    int queue_length_;                         // Длина очереди клиентов
    unique_ptr< sockaddr_in > self_addr_;      // Данные сервера
    unique_ptr< sockaddr_in > remote_addr_;    // Данные клиента
    ErrWriter& writer;
    string client_file;
    
    int client_socket;

    int autorized();     // Обработка клиента
    int math();
    string MD(string sah);
    void msgsend(string mess);
public:
    Communicator() = delete;
    Communicator(int port, int queue_length, ErrWriter& errors, string clients);

    void run();     // Приём запросов от клиентов
};

