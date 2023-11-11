#include "mdfile.h"
#include <limits>
using namespace std;

void ErrWriter::write_err(string &err){
    ofstream file;
    file.open(err_file, ios::app);
    if(file.is_open()){
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file << err << ':' << asctime(timeinfo) << endl;
        cout << "Detected error: " << err << endl;
    }
}

Communicator::Communicator(int port, int queue_length, ErrWriter& err_wr, string clients):
    sock(socket(AF_INET, SOCK_STREAM, 0)),
    queue_length_(queue_length),
    self_addr_(new sockaddr_in), 
    remote_addr_(new sockaddr_in),
    writer(err_wr),
    client_file(clients) {
    
    if( sock == -1 ) {
    	cout << "Ошибка сокета!" << endl;
        exit(1);
    }

    const int enable = 1;

    if( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof enable) == -1 ) {
        cout << "Установка повторного использования не удалась!" << endl;
    }

    self_addr_->sin_family = AF_INET;
    self_addr_->sin_port = htons(port);
    self_addr_->sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(sock, reinterpret_cast< const sockaddr* >(self_addr_.get()), sizeof(sockaddr_in)) == -1 ) {
        cout << "Ошибка сокета!" << endl;
        exit(1);
    }
}

void Communicator::run(){
    if( listen(sock, queue_length_) == -1 ) {
        cout << "Ошибка сокета!" << endl;
        exit(1);
    }
    cout << "Server is running\nClient connection is expected...\n" << endl;

    socklen_t addr_len = sizeof(sockaddr_in);

    while( true ) {
    	client_socket = accept(sock, reinterpret_cast< sockaddr* >(remote_addr_.get()), &addr_len);
        if( client_socket == -1 ) {
            cout << "Ошибка подключения клиента!" << endl;
            continue;
        }

        struct timeval timeout{30, 0};
        if( setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) == -1 ) {
            continue;
        }
        
        autorized();
    	math();
    }
}

int Communicator::autorized(){

					cout << "Start client connection..." << endl;

                    string ok = "OK";
                    string salt = "0011CF334455DA66";
                    string err = "ERR";
                    string error;
                    char msg[255];
                    
                    int msg_size;
                    msg_size = recv(client_socket, &msg, sizeof(msg), 0);
                    string message(msg, msg_size);
                        string login, hashq;
                        fstream file;
                        file.open(client_file);
                        getline (file, login, ':');
                        getline (file, hashq);
                        file.close();

                    if (message != login){
                        msgsend(err);
                        error = "Login error";
                        writer.write_err(error);
                        close(client_socket);
                        return 1;
                    } else{
                    	msgsend(salt);
                    msg_size = recv(client_socket, msg, sizeof(msg), 0);
                    string HASH(msg, msg_size);
                    string sah = salt + hashq;
                    string digest;
                    digest = MD(sah);

                    if (digest != HASH){
                        cout << digest << endl;
                        cout << HASH << endl;
                        msgsend(err);
                        error = "Password error";
                        writer.write_err(error);
                        close(client_socket);
                        return 1;
                    } else{
                        msgsend(ok);
                    }
}
return 1;
}
     
int Communicator::math(){
	uint32_t quantity;
	uint32_t number;
	uint64_t numeric_vector;
	recv(client_socket, &quantity, sizeof(quantity), 0);

	for(uint32_t j=0; j<quantity; j++){
		recv(client_socket, &number, sizeof(number), 0);
        uint64_t sum = 0;
        bool flag = false;

        for(uint32_t i=0; i<number; i++){
        	recv(client_socket, &numeric_vector, sizeof(numeric_vector), 0);
            if (sum > (numeric_limits < uint64_t > ::max() - numeric_vector)) {
            	sum = numeric_limits < uint64_t > ::max();
            	flag = true;
                break;
            }
            sum = sum+numeric_vector;
        }
        
        uint64_t average;
        if( flag )
        	average = sum;
        else
        	average = sum/number;
        send(client_socket, &average, sizeof(average), 0);
    }
                    
    cout << "Server operation is completed\n" <<endl;
    close(client_socket);
    return 1;
}

void Communicator::msgsend(string mess){
    char *buffer = new char[4096];
    strcpy(buffer, mess.c_str());
    send(client_socket, buffer, mess.length(), 0);
}

string Communicator::MD(string sah){
Weak::MD5 hash;
    string digest;
    StringSource(sah, true,  new HashFilter(hash, new HexEncoder(new StringSink(digest))));
      return digest;
 }
