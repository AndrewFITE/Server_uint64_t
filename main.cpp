#include "mdfile.h"
using namespace std;

int main(int argc, char *argv[]) {
    if(argc == 1){
        cout << "\n   Arithmetic mean server"  << endl;
        cout << "*Сервер среднего арифметического*\n"  << endl;
        cout << "-h — руководство по запуску сервера\n" << endl;  
    }

    int opt;
    int port = 33333;
    string file_name = "/ect/vcalc.conf";
    string file_error = "/var/log/vcalc.log";
    string error;
    while ((opt = getopt(argc, argv, "hl:p:j:" ))!=-1 ){
    switch(opt){
        case 'h':
        cout << "\n      Arithmetic mean server"  << endl;
        cout << "\nВы открыли руководство по запуску сервера\nОсновные параметры и их предназначение указаны ниже:\n"  << endl;
        cout << "-l — параметр для последующего ввода имени файла с БД пользователей" << endl;
        cout << "-p — параметр для последующего ввода порта" << endl;
        cout << "-j — параметр для последующего ввода имени файла-журнала ошибок" << endl;
        cout << "\nДля запуска сервера вам необходимо ввести следующее:\n./main -l *БД пользователей* (logpas.txt) -p *порт* (33333) -j *файл-журнал ошибок* (error.txt)" << endl;
        cout << "\nПосле запуска команды сервер начнёт свою работу" << endl;
        cout << "Далее вам необходимо открыть новое окно терминала и запустить в нём обработку клиента с помощью следующей команды: ./client1 или ./client2" << endl;
        cout << "\nДля остановки сервера закройте окно с обработкой клиентов и воспользуйтесь сочетанием клавиш 'Ctrl+C'\n" << endl;
        return 1;
        break;
        case 'l':{
        file_name = string(optarg);
        };
        break;
        case 'p':{
        port = stoi(string(optarg));
        }
        case 'j':{
        file_error = string(optarg);
        };
        break;
        }
        }

        fstream file;
        file.exceptions(ifstream::badbit | ifstream::failbit);
        try{
        	file.open(file_name);
        } catch(const exception & ex){
        	string error = "Error is entered in the Error file!";
        	return 1;
        }
        
        ErrWriter writer(file_error);
        
        Communicator com(port, 5, writer, file_name);
        
        com.run();
        
     return 0;
}
