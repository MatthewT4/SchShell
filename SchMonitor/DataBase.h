#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string> 
#include <stdexcept> 
#include <iostream> 


static int callback(void* NotUsed, int argc, char** argv, char** azColName);
class DataBase
{
public:
    DataBase();
    /*��������� ������ � ��������� ����� ���������� � ������ �� � ��������� �� ������� ������.*/
    void ConnectToDB(std::string DBName);
    /*����������� ������, ��������� ���������� � ������ �� � ��������� �� ������� ������.*/
    DataBase(std::string DBName);
    /*���������� ������, ��������� ���������� � ������ ��.*/
    ~DataBase();
    /*��������� ������ �� ��������� �������.*/
    std::string SelectData(std::string GetData);
    /*�������� ������ �� ���� ������ �� ��������� �������.*/
    bool InsertDBData(std::string GetData);

    bool CheckingForValuesDB();
    /*�������� ������ �� ������ ����������� �������.*/
    std::string GeSsqlError();
    void ClearDataBaseConf();

private:
    sqlite3* db;
    char* zErrMsg = 0;
    int rc = 0;
    char* sql;
    std::string sqlError;
};
void TestDB();
/*
int M(int argc, char* argv[]) {
    /*sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    /* Open database 
    rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    /*sql = "CREATE TABLE COMPANY(" \
        "ID INT PRIMARY KEY NOT NULL," \
        "NAME TEXT NOT NULL," \
        "AGE INT NOT NULL," \
        "ADDRESS CHAR(50)," \
        "SALARY REAL );";

    /* Execute SQL statement 
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
    return 0;
}*/
