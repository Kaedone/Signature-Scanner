//Базовый класс файла "CAVBFile"
//Класс чтения файла "CAVBFileReader"
//Класс добавления записи "CAVBFileWriter"

#ifndef __AVBFILE_H__INCLUDED_
#define __AVBFILE_H__INCLUDED_
#include "windows.h"
#include <fstream>
#include "AVrecord.h"

using namespace std;

/*Формат файла антивирусной базы
[AVB] //Сигнатура
[RecordCount*4] //Количество записей
[Records...]

Record:
[Offset * 4 ] //Смещение
[Lenght * 4 ] //Размер
[Hash * 16 ] //Контрольная сумма
[NameLen * 1 ] //Размер имени
[Name ... ] //Имя зловреда



*/

//Клас файла антивирусной базы
typedef class CAVBFile
{
    protected:
        fstream hFile; //объект потока файла
        DWORD RecordCount; //Количество записей
    public:
        CAVBFile();

        //закрытие файла
    virtual void close();

    //проверка состояня файла
    virtual bool is_open();

    //получение числа записей
    virtual DWORD getRecordCount();

} *PCAVBFile; 

//Класс для записи файла
typedef class CAVBFileWriter : public CAVBFile
{
    public:
        CAVBFileWriter() :
        CAVBFile()
        {
        }

    //Открытие файла
    bool open(PCSTR FileName);
    //Добавление записи в файл
    bool addRecord(PSAVRecord Record);
    
} *PCAVBFileWriter;

//Класс для чтения файла
typedef class CAVBFileReader : public CAVBFile
{
    public:
        CAVBFileReader():CAVBFile()
        {
        }
    

    //Открытие файла
    bool open(PCSTR FileName);

    //Чтение записи
    bool readNextRecord(PSAVRecord Record);

} *PCAVBFileReader;

#endif
