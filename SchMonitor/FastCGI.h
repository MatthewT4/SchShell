#pragma once
#include <string>



#define THREAD_COUNT 8 
#define SOCKET_PATH ":9000" 


//������ ���������� ��������� ������.
static int socketId;
static std::string OldMethod = "";

/*���������� �������� �� � ������ FastMain*/
/*���� � ��������� �������� FastCGI*/
static void* doit(void* a);
/*�������� ������� ���������*/
void FastMain(void);