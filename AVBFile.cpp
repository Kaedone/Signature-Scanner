#include "AVBFile.h"
#include "cstring"

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

        if(memcmp(Sign, "AVB", 3))
        {
            hFile.close(); //Чужой файл
            return false;
        }
        hFile.read((PCSTR)&this->RecordCount, sizeof(DWORD));
    }

    return true;

}

bool CAVBFileWriter::addRecord(PSAVRecord Record)
{
    if(Record == NULL || !hFile.is_open())
    {
        return false;
    }
    //Перемещаемся в конец файла
    hFile.seekp(0, ios::end);
    //Добавляем запись
    hFile.write((PSTR)&Record->Signature.Offset, sizeof(DWORD)); //Смещение сигнатуры
    hFile.write((PSTR)&Record->Signature.Lenght, sizeof(DWORD)); //Размер сигнатуры
    hFile.write((PSTR)&Record->Signature.Hash, 4*sizeof(DWORD)); //Контрольная сумма
    hFile.write((PSTR)&Record->NameLen, sizeof(BYTE)); //Размер имени
    hFile.write((PSTR)Record->Name, Record->NameLen); //Имя

    //Смещаемся к числу записей

    hFile.seekp(3, ios::beg);

    //Увеличиваем счетчик записей

    this->RecordCount++;

    return true;
}


//CAVBFileWReader

bool CAVBFileReader::open(PCSTR FileName)
{

    if(FileName == NULL) 
    {
        return false;
    }

    //Если файл не найден, то создаем его прототип

    if(isFileExists(FileName))
    {

        hFile.open(FileName, ios::in | ios::out | ios::binary);

        if(!hFile.is_open())
        {
            return false;
        }

        //Проверка сигнатуры

        CHAR Sign[3];
        hFile.read((PSTR)Sign, 3);

        if(memcmp(Sign, "AVB", 3))
        {
            hFile.close(); //Это чужой файл
            return false;
        }

        //Читаем число записей

        hFile.read((PSTR)&this->RecordCount, sizeof(DWORD));

    }else
    { 
        return false; 
    }

    return true;
    }

    bool CAVBFileReader::readNextRecord(PSAVRecord Record){

        if(Record == NULL || !hFile.is_open()) 
        {
            return false;
        }


        hFile.read((PSTR)&Record->Signature.Offset, sizeof(DWORD)); //Смещение сигнатуры
        hFile.read((PSTR)&Record->Signature.Lenght, sizeof(DWORD)); //Размер сигнатуры
        hFile.read((PSTR)&Record->Signature.Hash, 4 * sizeof(DWORD)); //Контрольная сумма
        hFile.read((PSTR)&Record->NameLen, sizeof(BYTE)); //Размер имени
        Record->allocName(Record->NameLen);
        hFile.read((PSTR)Record->Name, Record->NameLen); //Имя

        return true;

    }