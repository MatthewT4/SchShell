#include "DataBase.h"
#include <sstream>
#include <exception>
std::string Result = "";
static bool Che = false;
/*Проверяет, являются ли все символы строки цифрами
    return:
    true - все символы строки цифры
    false - в строке есть другие символы, кроме цифр*/
static bool IsInt(std::string Val) {
    std::string Valid = "1234567890";
    bool C = false;
    for (const auto& i : Val) {
        C = false;
        for (const auto& t : Valid) {
            if (i == t) {
                C = true;
            }
        }
        if (!C) {
            return false;
        }
    }
    return true;
}

/*Принимает значение из базы данных и вовращает в виде JSON отрезка*/
static int SelectCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    std::string Value;
    if (Che) {
        Result += ", ";
    }
    else { Che = true; }
    Result += "{";
    bool ch = false;
    for (i = 0; i < argc; i++) {
        if (ch) {
            Result += ", ";
        }
        else {
            ch = true;
        }
        Result += '"';
        Result += azColName[i];
        Result += '"';
        Result += ": ";
        Value = (argv[i] ? argv[i] : "NULL");
        if (IsInt(Value)) {
            Result += Value;
        }
        else {
            Result += '"';
            Result += Value;
            Result += '"';

        }
    }
    Result += "}";
    return 0;
}
/*конструктор класса без объявляния переменных
и входных данных.*/
DataBase::DataBase() {}
/*Фу===Реконнект к базе данных*/
void DataBase::ConnectToDB(std::string DBName) {
    sqlite3_close(db);
    rc = sqlite3_open(DBName.c_str(), &db);
    if (rc) {
        throw std::domain_error("Can't open database:" + *sqlite3_errmsg(db));
    }
}
/*Конструктор класса с открытием базы данных.*/
DataBase::DataBase(std::string DBName) {
    rc = sqlite3_open(DBName.c_str(), &db);
    if (rc) {
        throw std::domain_error("Can't open database:" + *sqlite3_errmsg(db));
    }
};
/*Деструктор класса с закрытием БД*/
DataBase::~DataBase() {
    sqlite3_free(zErrMsg);
    sqlite3_close(db);
}
/*Получение данных из запроса и генерация JSON из них*/
std::string DataBase::SelectData(std::string GetData) {
    Result = "{";
    Result += "\"status\": 200, \"body\": [";

    const char* sql = GetData.c_str();
    Che = false;
    rc = sqlite3_exec(db, sql, SelectCallback, 0, &zErrMsg);
    Result += "]}";
    if (rc != SQLITE_OK) {
       // fprintf(stderr, "[SQL error]: ", zErrMsg);
        std::cerr << "[SQL error](SelectData): " << zErrMsg << std::endl;
        return "{\"status\": 404, \"body\": []";
    }
    return Result;
}/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
bool DataBase::CheckingForValuesDB() { //Если записи ячеек склада есть в БД return true, если нет - false;
    const char* sql = "SELECT * FROM WareHouse WHERE NOT (PositionCell = 'RemoteWarehouse') LIMIT 3";
    Result = "";
    rc = sqlite3_exec(db, sql, SelectCallback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
       // fprintf(stderr, "[SQL error]: ", zErrMsg);
        std::cerr << "[SQL error](CheckingForValuesDB): " << zErrMsg << std::endl;
        return false;
    }
    if (Result.size() == 0) {
        return false;
    }
    return true;
}
/*Добавление данных а БД,
    return:
    true - успешно
    false - ошибка
    Ошибка записывается в переменную sqlError класса DataBase*/
bool DataBase::InsertDBData(std::string GetData) {
    const char* sql = GetData.c_str();
    rc = sqlite3_exec(db, sql, SelectCallback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        // fprintf(stderr, "[SQL error]: ", zErrMsg);
        std::cerr << "[SQL error](InsertDBData): " << zErrMsg << std::endl;
        sqlError = zErrMsg;
        return false;
    }
    return true;
}
std::string DataBase::GeSsqlError() {
    return sqlError;
}
//void TestDB() {
//    DataBase db("test.db");
//    std::cout << db.SelectData("SELECT * FROM WareHouse Where TypeCell = 'Midlle' AND Empty = 0 ORDER BY HeightCell") << std::endl << std::endl;
//}
//void DataBase::ClearDataBaseConf() {
//    rc = sqlite3_exec(db, "DELETE FROM Positions", SelectCallback, 0, &zErrMsg);
//    rc = sqlite3_exec(db, "DELETE FROM WareHouse", SelectCallback, 0, &zErrMsg);
//    bool c = InsertDBData("INSERT INTO WareHouse(PositionCell, TypeCell, HeightCell) VALUES ('RemoteWarehouse', 'RemoteWarehouse', 0)");
//    c = InsertDBData("INSERT INTO Positions(Position, UUID, Height) VALUES ('RemoteWarehouse', 'SERVICE', 0)");
//}