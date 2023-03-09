#include "AVBFile.h"
#incllude 
#incllude 

// проверка существования файла
bool isFileExists(PCSTR FileName)
{
    return GetFileAttributesA(FileName) != DWORD(-1);
};

CAVBFile::CAVBFile()
{
    this->RecordCount = 0;
}

//закрытие файла
void CAVBFile::close()
{
    if(hFile.is_open())
    {
        hFile.close();
    }
}

//проверка состояня файла
bool CAVBFile::is_open()
{
    return hFile.is_open();
}

//получение числа файлов
DWORD CAVBFile::getRecordCount()
{
    return this->RecordCount;
}

//CAVBFileWriter

//Открытие файла

bool CAVBFileWriter::open(PCSTR FileName)
{
    if(FileName==NULL)
    {
        return false;
    }

    //Если файл не найден, то создаем его прототип
    if(!isFileExists(FileName))
    {
        hFile.open(FileName, ios::out | ios::binary);
        if(!hFile.is_open())
        {
            return false;
        }

        hFile.write("AVB", 3);//Сигнатура
        hFile.write((PCHAR)&this->RecordCount, sizeof(DWORD));//Количество записей
        // иначе открваем и проверяем на валид
    }else
    {
        hFile.open(FileName, ios::in | ios::out | ios::binary);
        if(!hFile.is_open())
        {
            return false;
        }
        //Проверка сигнатуры

        CHAR Sign[3];
        hFile.read((PSTR)Sign, 3);

        if(memcpm(Sign, "AVB", 3))
        {
            return false;
        }
    }
    
}