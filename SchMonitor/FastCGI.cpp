#include <fastcgi/fcgi_config.h>
#include <fastcgi/fcgiapp.h>
#include "FastCGI.h"
#include <pthread.h>
#include <sys/types.h> 
#include <stdio.h> 
#include <Windows.h> 
#include <stdio.h>
#include <regex>
#include <iostream>
#include <nlohmann/json.hpp>
#include "DataBase.h"

using json = nlohmann::json;
//static std::string fileName = "logo.jpg";
static std::string fileName = "logo.jpg";
static void* doit(void* a)
{
    DataBase db("SchShell.db");
    int rc;
    FCGX_Request request;
    char* server_name;

    if (FCGX_InitRequest(&request, socketId, 0) != 0)
    {
        //ошибка при инициализации структуры запроса 
        printf("Can not init request\n");//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        return NULL;
    }
    printf("Request is inited\n");//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for (;;)
    {
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

        //попробовать получить новый запрос 
        printf("Try to accept new request\n");//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        pthread_mutex_lock(&accept_mutex);
        rc = FCGX_Accept_r(&request);
        pthread_mutex_unlock(&accept_mutex);

        if (rc < 0)
        {
            //ошибка при получении запроса 
            printf("Can not accept new request\n");
            break;
        }
        printf("request is accepted\n");//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        //получить значение переменной 
        server_name = FCGX_GetParam("SERVER_NAME", request.envp);
        std::cout << "Server name: " << server_name << std::endl;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::string method = FCGX_GetParam("REQUEST_METHOD", request.envp);
        std::cout << "Method name: " << method << std::endl;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::string DOCUMENT_URIq = FCGX_GetParam("CONTENT_TYPE", request.envp);
        std::cout << "CONTENT_TYPE name: " << DOCUMENT_URIq << std::endl;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::string CL = FCGX_GetParam("CONTENT_LENGTH", request.envp);
        std::cout << "CONTENT_LENGTH name: " << CL << std::endl; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::string InURL = FCGX_GetParam("REQUEST_URI", request.envp);
        std::cout << "REQUEST_URI name: " << InURL << std::endl; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //вывести тело ответа (например - html-код веб-страницы) 
        /*FCGX_PutS("<html>\r\n", request.out);
        FCGX_PutS("<head>\r\n", request.out);
        FCGX_PutS("<title>FastCGI Hello! (multi-threaded C, fcgiapp library)</title>\r\n", request.out);
        FCGX_PutS("</head>\r\n", request.out);
        FCGX_PutS("<body>\r\n", request.out);*/
        FCGX_PutS("Content-type: application/json\r\n", request.out);
        FCGX_PutS("\r\n", request.out);
        if (method == "GET") {
            ////std::string api = InURL.substr(0, 4);
            ////std::string add = InURL.substr(4, 7);
            if (InURL.substr(0, 5) == "/data") {
                std::string token = InURL.substr(6, InURL.size() - 1);
                auto r = db.SelectData("SELECT * FROM TOKEN WHERE Token = \'" + token + "\'");
                if (r != "{\"status\": 200, \"body\": []}" and r != "{\"status\": 404, \"body\": []") {
                    FCGX_PutS(db.SelectData("SELECT * FROM FileName").c_str(), request.out);
                    std::cout << "data in token ok!" << std::endl;
                }
                else { FCGX_PutS("Error Token!", request.out); }
            }
            if (InURL.substr(0, 5) == "/user") {
                std::cout << ("{\"body\": \"" + fileName + "\"}") << std::endl;
                FCGX_PutS(("{\"body\": \"" + fileName + "\"}").c_str(), request.out);
            //    if (InURL.substr(4, 4) == "/add") {
            //        string zapr = "";
            //        for (auto iter = InURL.begin() + 7; iter != InURL.end(); iter++) {
            //            zapr += *iter;
            //        }
            //        cout << zapr;
            //    }
            //}
            //if (InURL == "/data") {
            //    std::string dataa = WH.GetDB().SelectData("Select Po2.UUID, Po2.Name, Po2.Height, Po2.Width, Po2.Depth, Po2.Weight, Po2.Comments, WH1.PositionCell FROM WareHouse WH1 INNER JOIN Positions Po2 ON WH1.PositionCell = Po2.Position WHERE NOT Po2.Name = 'RemoteWarehouse'"); // Where TypeCell = 'Midlle' AND Empty = 0 ORDER BY HeightCell");
            //    FCGX_PutS(dataa.c_str(), request.out);
            }
        }
        else if (method == "POST") { 
            if (InURL.substr(0, 6) == "/admin") {
                int Max = atoi(FCGX_GetParam("CONTENT_LENGTH", request.envp));
                //    Sleep(300);
                char* buffer = new char[Max];
                FCGX_GetStr(buffer, Max, request.in);
                std::string buf = buffer;
                buf = buf.substr(0, Max);
                std::cout << buf << std::endl;
                delete[] buffer;
                if (InURL.substr(6, 4) == "/add") {
                    auto i = db.InsertDBData("INSERT INTO FileName (Name) VALUES (\'" + buf + "\')");
                    if (!i) {
                        db.GeSsqlError();
                    }
                    FCGX_PutS("OK", request.out);
                }
                if (InURL.substr(6, 4) == "/del") {
                    auto i = db.InsertDBData("DELETE FROM FileName WHERE Name = \'" + buf + "\'");
                    if (!i) {
                        db.GeSsqlError();
                    }
                    FCGX_PutS("OK", request.out);
                }
                if (InURL.substr(6, 4) == "/manualSQL") {
                    auto i = db.SelectData(buf);
                }
                if (InURL.substr(6, 6) == "/login") {
                    json responseJson = json::parse(buf);
                    json login = responseJson["login"];
                    json password = responseJson["password"];
                    json r = (db.SelectData(("SELECT * FROM User WHERE login = " + to_string(login) + " AND password = " + to_string(password))));
                    if (r != "{\"status\": 200, \"body\": []}" and r != "{\"status\": 404, \"body\": []") {
                        FCGX_PutS(to_string(login).c_str(), request.out);
                        //std::cout << "I get data token!";
                        db.InsertDBData("DELETE FROM TOKEN  WHERE TokLogin = " + to_string(login) + "");
                        auto RT = db.InsertDBData("INSERT INTO TOKEN (TokLogin, Token) VALUES (" + to_string(login) + "," + to_string(login) + ")");
                        std::cout << "Result Add Token:" << RT << std::endl;
                        if (!RT) {
                            std::cout << db.GeSsqlError() << std::endl;
                        }
                    }
                }
                if (InURL.substr(6, 13) == "/offInErrorOK") {
                    exit(0);
                }
                if (InURL.substr(6, 4) == "/set") {
                    fileName = buf;
                }
            //    if (InURL.substr(4, 4) == "/add") {
            //        SetConsoleCP(1251);
            //        SetConsoleOutputCP(1251);
            //        //std::cerr << "stdbuffer: " << buf << std::endl;
            //    }
            //    else if (InURL.substr(4, 6) == "/issue") {
            //        //cout << buf << endl;
            //    }
            }
        }

        //"заснуть" - имитация многопоточной среды 
        Sleep(2);

        //закрыть текущее соединение 
        FCGX_Finish_r(&request);

        // OldMethod = method;
         //завершающие действия - запись статистики, логгирование ошибок и т.п. 
    }
    return NULL;
}
void FastMain(void)
{
    int i;
    pthread_t id[THREAD_COUNT];

    //инициализация библилиотеки 
    FCGX_Init();
    printf("Lib is inited\n"); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //открываем новый сокет 
    socketId = FCGX_OpenSocket(SOCKET_PATH, 20);
    if (socketId < 0)
    {
        //ошибка при открытии сокета 
        exit(1);      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    printf("Socket is opened\n"); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //создаём рабочие потоки 
    for (i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&id[i], NULL, doit, NULL);
    }

    //ждем завершения рабочих потоков 
    for (i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(id[i], NULL);
    }

} 