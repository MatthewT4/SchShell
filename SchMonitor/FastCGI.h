#pragma once
#include <string>



#define THREAD_COUNT 8 
#define SOCKET_PATH ":9000" 


//хранит дескриптор открытого сокета.
static int socketId;
static std::string OldMethod = "";

/*Сохранения названия БД и запуск FastMain*/
/*Приём и обработка запросов FastCGI*/
static void* doit(void* a);
/*Создание потоков обрабобки*/
void FastMain(void);