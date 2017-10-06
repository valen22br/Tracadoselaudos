//============================================================================
// Name        : tracadoselaudos.cpp
// Author      : Luis Gustavo Grubert Valensuela
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <ostream>
#include <unistd.h>
#include <stdio.h>
#include <string>

#define ARRAYSIZE 20

//#include "/usr/local/include/cppconn/connection.h"

#include "/usr/local/include/mysql_connection.h"

#include "/usr/local/include/cppconn/driver.h"
#include "/usr/local/include/cppconn/exception.h"
#include "/usr/local/include/cppconn/resultset.h"
#include "/usr/local/include/cppconn/statement.h"

/*
#include "/usr/local/include/mysql.h"
#include "/usr/local/include/mysql_driver.h"
#include "/usr/local/include/mysql_error.h"
*/



using namespace std;

class clientInfo{
public:
    int clientId;
    string clientName;
    string clientInitial;
    string clientExamEmails;
    string clientChargeEmail;
    int clientActive;
};

class patientInfo{
public:
    patientInfo();
    string patientName;
    int exam_id;
    int patient_id;
    int file_name_id;
    int client_id;
    int exam_quantity = 1;
    double exam_value;
    double total_value;
};

patientInfo::patientInfo()
{
    exam_id = patient_id = file_name_id = client_id = exam_value = total_value = 0;
}

class examInfo{
public:
    int exam_id;
    string exam_name;
    string exam_extension;
};

class examValuesPerClient{
public:
    int exam_values_id;
    int client_list_client_id;
    int exam_list_exams_id;
    double exam_value;
};

class SystemCommand{
public:
    SystemCommand();
    string getCommandResult(string commandStr);
private:
    string commandStr = "";
    string resultStr = "";
};

SystemCommand::SystemCommand()
{
    commandStr = "ls";
}

string SystemCommand::getCommandResult(string commandStr){
    //system(("echo \"" + body + "\" | mail -s \"" + subject + "\" "+ toAddress).c_str());
    resultStr = system((commandStr).c_str());
    return resultStr;
}

class readFile{
public:
    readFile();
    readFile(const readFile & obj);
    ~readFile();
    void deleteDynamicArrayPatient();
    bool isFull();
    void doubleSize();
    patientInfo * getPatientInfo();
    int getSize();
    int getCount();
    string getListPacientsFormatedForEmail();
    void printReadFile(string text="Inside ReadFile");
    
private:
    patientInfo * patient;
    int patientSize = 50;
    int patientCount = 0;
    string listPatientsFormatedForEmail = "";
    
};

bool readFile::isFull(){
    return patientSize == patientCount;
}

void readFile::doubleSize(){
    patientSize *=2;
    
    patientInfo * temp;
    temp = new patientInfo[patientSize];
    for (int i = 0; i < patientCount; i ++)
    {
        temp[i].patientName = patient[i].patientName;
        temp[i].exam_id = patient[i].exam_id;
        temp[i].patient_id = patient[i].patient_id;
        temp[i].file_name_id = patient[i].file_name_id;
        temp[i].client_id = patient[i].client_id;
        temp[i].exam_quantity = patient[i].exam_quantity;
        temp[i].exam_value = patient[i].exam_value;
        temp[i].total_value = patient[i].total_value;
    }
    delete[] patient;
    patient = temp;
}

readFile::readFile()
{
    ifstream list;
    patient = new patientInfo[patientSize];
    //system("/Users/valen/Desktop/work/list.sh");
    
    
    FILE *in;
    char buff[512];
    
    if(!(in = popen("/Users/valen/Desktop/work/./list.sh ", "r"))){
        
        cout << "Error executing list.sh shell script command" << endl;
    }
    while(fgets(buff, sizeof(buff), in)!=NULL){
        
        cout << buff;
        listPatientsFormatedForEmail += buff;
        
    }
    
    pclose(in);
    
    list.open("/Users/valen/Desktop/work/list.txt");
    
    if(!list.fail())
    {
        while(!list.eof())
        {
            if(isFull())
            {
                doubleSize();
            }
            getline(list,patient[patientCount].patientName);
            //cin >> patient[count].patientName;
            //cout << patient[count].patientName<< endl;
            patientCount ++;
            
            
        }
    }
    else
    {
        cout << "Error while openingt the file list.txt"<< endl;
    }
    //Remove the last empty line after reading the file list.txt
    patientCount --;
    
    list.close();
    
}

readFile::readFile(const readFile & obj)
{
    cout << endl << "The Copy Constructor Inside readFile Class has been Called " << endl;
    patient = new patientInfo[obj.patientSize];
    for(int i = 0; i < obj.patientCount; i++)
    {
        patient[i].client_id = obj.patient[i].client_id;
        patient[i].exam_id = obj.patient[i].exam_id;
        patient[i].exam_quantity = obj.patient[i].exam_quantity;
        patient[i].exam_value = obj.patient[i].exam_value;
        patient[i].file_name_id = obj.patient[i].file_name_id;
        patient[i].patient_id = obj.patient[i].patient_id;
        patient[i].patientName = obj.patient[i].patientName;
        patient[i].total_value = obj.patient[i].total_value;
    }
}

patientInfo * readFile::getPatientInfo(){
    return patient;
}

int readFile::getSize()
{
    return patientSize;
}

int readFile::getCount()
{
    return patientCount;
}

void readFile::deleteDynamicArrayPatient()
{
    delete[] patient;
    patient = nullptr;
}

string readFile::getListPacientsFormatedForEmail()
{
    return listPatientsFormatedForEmail;
}

void readFile::printReadFile(string text)
{
    cout << text << endl;
    cout << "client_id|exam_id|exam_quantity|file_name_id|patient_id|patientName|total_value" << endl;
    for(int i = 0; i < patientCount; i++)
    {
        cout << patient[i].client_id << " ";
        cout << patient[i].exam_id << " ";
        cout << patient[i].exam_quantity << " ";
        cout << patient[i].file_name_id << " ";
        cout << patient[i].patient_id << " ";
        cout << patient[i].patientName << " ";
        cout << patient[i].total_value << " "<< endl;
        
    }
}

readFile::~readFile(){
    cout << "The ~readFile Descructor was called"<< endl;
}


//Continue from here
class Email
{
    public:
    Email(){};
    void EmailSend(string subject = "teste", string body = "date", string toAddress = "valensuela@gmail.com", string fromAddress = "valensuela@gmail.com");
    
    private:
};
void Email::EmailSend(string subject, string body, string toAddress, string fromAddress)
{
    cout << "system((echo\" " + body + "| mail -s \"" + subject + "\" "+ toAddress + ")";
    system(("echo \"" + body + "\" | mail -s \"" + subject + "\" "+ toAddress).c_str());
//    system((body + "| mail -s \"" + subject + " "+ toAddress).c_str());
}

class createTargz{
public:
    createTargz(string clinicInitial, int pastDays);
    ~createTargz();
    void getTime();
    void createFileName(clientInfo * clInformation, int count, int size, string fileComplementName);
    void createTargzFile();
    string getFileName();
    string getExamDate();
    string getActualDate();
    string getMysqlActualDateFormat();
    string getMysqlExamDateFormat();
    string getClinicInitials();
    int getClinicId();
    
    
private:
    time_t currentTime;
    struct tm *localTime;
    string examDate;
    string fileName;
    string clinicInitial;
    string actualDate;
    string mysqlActualDateFormat;
    string mysqlExamDateFormat;
    int day;
    int month;
    int year;
    int monthsDays[13];
    int clinic_id;
};

createTargz::~createTargz()
{
    cout << "The ~createTargz Descructor was called "<< endl;
}

createTargz::createTargz(string clinicInitial, int pastDays){
   
    this->clinicInitial = clinicInitial;
    monthsDays[0]=0;
    monthsDays[1]=31;
    monthsDays[2]=28;
    monthsDays[3]=31;
    monthsDays[4]=30;
    monthsDays[5]=31;
    monthsDays[6]=30;
    monthsDays[7]=31;
    monthsDays[8]=31;
    monthsDays[9]=30;
    monthsDays[10]=31;
    monthsDays[11]=30;
    monthsDays[12]=31;
    
    getTime();
    
    if(day - pastDays < 1){
        switch (month) {
            case 1:
                month = 12;
                day = monthsDays[12]+(day-pastDays);
                year--;
                break;
                
            case 2:
                month = 1;
                day = monthsDays[1]+(day-pastDays);
                break;
                
            case 3:
                month = 2;
                day = monthsDays[2]+(day-pastDays);
                break;
                
            case 4:
                month = 3;
                day = monthsDays[3]+(day-pastDays);
                break;
                
            case 5:
                month = 4;
                day = monthsDays[4]+(day-pastDays);
                break;
                
            case 6:
                month = 5;
                day = monthsDays[5]+(day-pastDays);
                break;
                
            case 7:
                month = 6;
                day = monthsDays[6]+(day-pastDays);
                break;
                
            case 8:
                month = 7;
                day = monthsDays[7]+(day-pastDays);
                break;
                
            case 9:
                month = 8;
                day = monthsDays[8]+(day-pastDays);
                break;
                
            case 10:
                month = 9;
                day = monthsDays[9]+(day-pastDays);
                break;
                
            case 11:
                month = 10;
                day = monthsDays[10]+(day-pastDays);
                break;
                
            case 12:
                month = 11;
                day = monthsDays[11]+(day-pastDays);
                break;
                
            default:
                break;
        }
        
    }
    else
    {
        day -=pastDays;
    }
    examDate = to_string(day)+"_"+to_string(month)+"_"+to_string(year);
    mysqlExamDateFormat =  to_string(year)+"-"+to_string(month)+"-"+to_string(day);
    cout << endl << "examDate => "<< examDate<< endl;
}

void createTargz::getTime(){
    time(&currentTime);
    localTime = localtime(&currentTime);
    day = localTime->tm_mday;
    month = localTime->tm_mon+1;
    year = localTime->tm_year+1900;
    actualDate = to_string(day) +  "_" + to_string(month) +  "_" + to_string(year);
    mysqlActualDateFormat = to_string(year) +  "-" + to_string(month) +  "-" + to_string(day);
}


void createTargz::createFileName(clientInfo * clInformation, int count, int size, string fileComplementName){
    int findFlag = 0;
    for(int i = 0; i < count; i++)
    {
        if(clInformation[i].clientInitial == clinicInitial)
        {
            findFlag = 1;
            clinic_id = clInformation[i].clientId;
        }
    }
    
    if(findFlag == 0)
    {
        cout<< "The clinic initials does not exist on the database. Extiting the program."<<endl;
        exit(1);
        
    }
    
    fileName = clinicInitial + "_" + examDate;
    if(fileComplementName != "")
        fileName += "_" + fileComplementName;
    fileName += ".zip";
    cout <<"fileName => " << fileName;
    
}

void createTargz::createTargzFile(){
    //system(("cp Archive.zip "+fileName).c_str());
    system(("cp /Users/valen/Desktop/work/Archive.zip /Users/valen/Desktop/work/"+fileName).c_str());
}

string createTargz::getFileName(){
    return fileName;
}

string createTargz::getExamDate(){
    return examDate;
}

string createTargz::getActualDate(){
    return actualDate;
}

string createTargz::getMysqlActualDateFormat()
{
    return mysqlActualDateFormat;
}

string createTargz::getMysqlExamDateFormat()
{
    return mysqlExamDateFormat;
}

string createTargz::getClinicInitials(){
    return clinicInitial;
}

int createTargz::getClinicId()
{
    return clinic_id;
}

class scpSendFile{
public:
    scpSendFile(string fineName, string clinicInitials);
    ~scpSendFile();
};

scpSendFile::scpSendFile(string fileName, string clinicInitials)
{
    system(("/Users/valen/Desktop/work/./sshcopy.sh "+ fileName + " " + clinicInitials).c_str());
}

scpSendFile::~scpSendFile()
{
    cout << "The ~scpSendFile Descructor was called"<< endl;
    system("rm -r /Users/valen/Desktop/work/Archive.zip");
    
}

class mysqlConnection{
public:
    mysqlConnection();
    ~mysqlConnection();
    void selectClients();
    void selectExams();
    void insertFileName(string fileName);
    void insertPatientNames(readFile & obj, int clinicId);
    void insertTransactionDetails(readFile & obj, string mysqlExamDateFormat, string mysqlActualDateFormat);
    void selectSumCurrentDateAndExamDate(createTargz & obj);
    bool isFull_clInfo();
    bool isFull(int diffSize, int diffCount);
    void doubleSize_clInfo();
    void doubleSizeExamInfo();
    clientInfo * getclInfo();
    int getCount();
    int getSize();
    clientInfo * getClientInfo();
    int getClientID(string clientInitials);
    string getClientExamEmail(int idClient);
    string getClientInitials(int idClient);
    examInfo * examInfoGetAddress();
    void checkConnection();
    double transaction_total_value = 0;
    void printClInfo(string text="Inside mysqlConnection - printing clInfo");
    
private:
    string hostName;
    string userName;
    string passWord;
    string bdScheme;
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int size = 5;
    int count = 0;
    int fileNameLastInsertId;
    clientInfo *clInfo;
    int examSize = 5;
    int examCount = 0;
    examInfo * exInfo;
    bool connected;
    examValuesPerClient examValues[ARRAYSIZE];
    int examValuesPerClientCout = 0;
};

mysqlConnection::mysqlConnection()
{
    hostName = "tracadoselaudos.tracadoselaudos.com.br";
    userName = "valen22br";
    passWord = "Gustavo2000";
    bdScheme = "tracadoselaudos";
    
    cout << endl;
    cout << "Creating Mysql Connection..." << endl;
    
    try {
        
        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(hostName,userName,passWord);
        /* Connect to the MySQL test database */
        con->setSchema(bdScheme);
        
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    cout << endl;
}

void mysqlConnection::checkConnection()
{
    connected = con != NULL && (con->isValid() || con->reconnect());
    
    if (!connected)
    {
        con = driver->connect(hostName,userName,passWord);
        //con->setSchema(bdScheme);
        connected = con->isValid();
        stmt = con->createStatement();
        stmt->executeQuery("USE tracadoselaudos");
        delete stmt;
    }
}

clientInfo * mysqlConnection::getClientInfo(){
    return clInfo;
}

examInfo * mysqlConnection::examInfoGetAddress(){
    return exInfo;
}

void mysqlConnection::selectClients(){
    clInfo = new clientInfo[size];
    try{
        
        checkConnection();
        
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * from client_list order by client_id asc");
        cout << "Client List Table:"<< endl;
        cout << "id Initial \t Name "<< endl;
        while (res->next()) {
            //cout << "\t... MySQL replies: ";
            /* Access column data by alias or column name */
            //cout << res->getString("client_name") << endl;
            //cout << "\t... MySQL says it again: ";
            /* Access column data by numeric offset, 1 is the first column */
            
            if(isFull_clInfo()) {
                doubleSize_clInfo();
            }
            
            clInfo[count].clientId = res->getInt(1);
            clInfo[count].clientName = res->getString(2);
            clInfo[count].clientInitial = res->getString(3);
            clInfo[count].clientExamEmails = res->getString(4);
            clInfo[count].clientChargeEmail = res->getString(5);
            clInfo[count].clientActive = res->getInt(6);
            
            count++;
            
            cout << res->getString(1) << " \t " << res->getString(3) <<  " \t " << res->getString(2) << " \t " << res->getString(4) << endl;
        }
        cout << endl;
        delete stmt;
        delete res;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
}

void mysqlConnection::selectExams(){
    exInfo = new examInfo[examSize];
    try{
        
        checkConnection();
        
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * from exams_list order by exams_id asc");
        cout << endl << "Exams List Table:"<< endl;
        cout << "id Exam Name"<< endl;
        while (res->next()) {
            //cout << "\t... MySQL replies: ";
            /* Access column data by alias or column name */
            //cout << res->getString("client_name") << endl;
            //cout << "\t... MySQL says it again: ";
            /* Access column data by numeric offset, 1 is the first column */
            
            if(isFull(examSize, examCount)) {
                doubleSizeExamInfo();
            }
            
            exInfo[examCount].exam_id = res->getInt("exams_id");
            exInfo[examCount].exam_name = res->getString("exam_name");
            exInfo[examCount].exam_extension = res->getString("exam_extension");
     
            //cout <<  exInfo[examCount].exam_id << " \t " << exInfo[examCount].exam_name << " \t " << exInfo[examCount].exam_extension << endl;
            
            examCount++;
            cout <<  res->getInt("exams_id") << " \t " << res->getString("exam_name") << " \t " << res->getString("exam_extension") << endl;
            
            
        }
        cout << endl;
        
        delete stmt;
        delete res;

    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void mysqlConnection::insertFileName(string fileName){
    try{
        
        checkConnection();
        
        stmt = con->createStatement();
        res = stmt->executeQuery(("SELECT * from file_name WHERE file_name = '"+fileName+"'").c_str());
        if(!res->next()){
            delete res;
            delete stmt;
            stmt = con->createStatement();
            stmt->execute(("INSERT INTO file_name(file_name) VALUES ('"+fileName+"')").c_str());
            delete stmt;
            stmt = con->createStatement();
            res = stmt->executeQuery("SELECT file_name_id from file_name ORDER BY file_name_id DESC limit 1");
            while (res->next()) {
                fileNameLastInsertId = res->getInt("file_name_id");
            }
            delete res;
            delete stmt;
        }
        else
        {
            cout << fileName << " was alread used with id " << res->getInt("file_name_id") << ". Exiting program."<<endl;
            delete stmt;
            exit(1);
        }
        
        
        cout << "  fileName lastInsertId = " << fileNameLastInsertId<<endl;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void mysqlConnection::insertPatientNames(readFile & obj, int clinicId)
{
    string extension="";
    string sqlValues = "";
    cout << endl;

    
    for(int i = 0; i < obj.getCount(); i++)
    {
        //cout << "inside insertPatientNames "+ patientInfo[i].patientName << endl;
        extension = obj.getPatientInfo()[i].patientName.substr(obj.getPatientInfo()[i].patientName.find(".")+1,obj.getPatientInfo()[i].patientName.length());;
        if(extension == "imp")
        {
            if(obj.getPatientInfo()[i].patientName.find("peri")!=string::npos)
            {
                extension = "peri";
                obj.getPatientInfo()[i].patientName = obj.getPatientInfo()[i].patientName.substr(0,obj.getPatientInfo()[i].patientName.size()-9);
            }
            else if (obj.getPatientInfo()[i].patientName.find("tomo")!=string::npos)
            {
                extension = "tomo";
                obj.getPatientInfo()[i].patientName = obj.getPatientInfo()[i].patientName.substr(0,obj.getPatientInfo()[i].patientName.size()-9);
            }
            else
            {
                obj.getPatientInfo()[i].patientName = obj.getPatientInfo()[i].patientName.substr(0,obj.getPatientInfo()[i].patientName.size()-4);
            }
        }
        if(extension == "rcf")
        {
            if(obj.getPatientInfo()[i].patientName.find("carpal")!=string::npos)
            {
                extension = "carpal";
                obj.getPatientInfo()[i].patientName = obj.getPatientInfo()[i].patientName.substr(0,obj.getPatientInfo()[i].patientName.size()-9);
            }
            else
            {
                obj.getPatientInfo()[i].patientName = obj.getPatientInfo()[i].patientName.substr(0,obj.getPatientInfo()[i].patientName.size()-4);
            }
        }
        //for(int j = 0; j < obj.getCount(); j++)
        for(int j = 0; j < examCount; j++)
        {
            if(exInfo[j].exam_extension == extension)
            {
                obj.getPatientInfo()[i].exam_id = exInfo[j].exam_id;
                obj.getPatientInfo()[i].client_id = clinicId;
                obj.getPatientInfo()[i].file_name_id = fileNameLastInsertId;
            }
           
        }
        if(obj.getPatientInfo()[i].patientName != "")
            sqlValues += "('" + obj.getPatientInfo()[i].patientName + "'), ";
    }
    sqlValues = sqlValues.substr(0,sqlValues.size()-2);
    
    try{
        
        checkConnection();
        
        stmt = con->createStatement();
        stmt->execute(("INSERT IGNORE INTO patient_list(patient_name) VALUES "+sqlValues).c_str());
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    for (int i = 0; i < obj.getCount(); i++)
    {
        try{
            
            checkConnection();
            
            stmt = con->createStatement();
            //cout << "SELECT patient_id from patient_list where patient_name = '"+ obj.getPatientInfo()[i].patientName << endl;
            res = stmt->executeQuery(("SELECT patient_id from patient_list where patient_name = '"+ obj.getPatientInfo()[i].patientName +"'").c_str());
            while (res->next()) {
                obj.getPatientInfo()[i].patient_id = res->getInt("patient_id");
                //cout << res->getInt("patient_id") << endl;
            }
            delete res;
            delete stmt;
        } catch (sql::SQLException &e) {
            cout << "# ERR: SQLException in " << __FILE__;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
    }
    
}

void mysqlConnection::insertTransactionDetails(readFile & obj, string mysqlExamDateFormat, string mysqlActualDateFormat){
    
    //obj.printReadFile("Inside InsertTransactionsDetails=: Guti");
    
    string sqlValues = "";
    
    try{
        
        checkConnection();
        
        stmt = con->createStatement();
        res = stmt->executeQuery(("SELECT * from exam_values_per_client where client_list_client_id = '"+ to_string(obj.getPatientInfo()[0].client_id) + "'").c_str());
        while (res->next()) {
            examValues[examValuesPerClientCout].exam_values_id = res->getInt("exam_values_id");
            examValues[examValuesPerClientCout].client_list_client_id = res->getInt("client_list_client_id");
            examValues[examValuesPerClientCout].exam_value = res->getDouble("exam_value");
            examValues[examValuesPerClientCout].exam_list_exams_id = res->getInt("exams_list_exams_id");
            examValuesPerClientCout++;
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    cout << endl;
    cout << "exam_values_id | client_list_client_id | exam_list_exams_id | exam_value "<< endl;
    for (int i = 0; i < examValuesPerClientCout; i++)
    {
        cout << examValues[i].exam_values_id << " ";
        cout << examValues[i].client_list_client_id << " ";
        cout << examValues[i].exam_list_exams_id << " ";
        cout << examValues[i].exam_value << endl;
    }
    
    try{
        checkConnection();
        stmt = con->createStatement();
        res = stmt->executeQuery(("SELECT transaction_total_value from transactions_table where client_list_client_id = '"+ to_string(obj.getPatientInfo()[0].client_id) + "' ORDER BY transaction_id DESC limit 1").c_str());
        while (res->next()) {
            transaction_total_value = res->getDouble("transaction_total_value");
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    cout << endl;
    cout << "Transaction Total Value: " << transaction_total_value << endl;
    
    
    for(int i = 0; i < obj.getCount(); i++)
    {
        for(int j = 0; j < examValuesPerClientCout; j++)
        {
            if(examValues[j].exam_list_exams_id == obj.getPatientInfo()[i].exam_id)
            {
                obj.getPatientInfo()[i].exam_value = examValues[j].exam_value;
                obj.getPatientInfo()[i].total_value = examValues[j].exam_value + transaction_total_value;
                transaction_total_value += examValues[j].exam_value;
            }
        }
    }
    
    cout << endl;
    cout << "Inside InsertTransactionDetails " << endl;
    cout << "client_id, patient_id, exam_id, file_name_id, patientName " << endl;
    for (int i = 0; i < obj.getCount(); i++)
    {
        cout << obj.getPatientInfo()[i].client_id << " " ;
        cout << obj.getPatientInfo()[i].patient_id << " " << obj.getPatientInfo()[i].exam_id << " ";
        cout << obj.getPatientInfo()[i].file_name_id << " " << obj.getPatientInfo()[i].patientName << " ";
        cout << obj.getPatientInfo()[i].exam_value << " " << obj.getPatientInfo()[i].total_value << " ";
        cout << endl;
        
        sqlValues +="("+to_string(obj.getPatientInfo()[i].client_id)+","
                   +to_string(obj.getPatientInfo()[i].patient_id)+","
                   +to_string(obj.getPatientInfo()[i].exam_id)+","
                   +"1,"
                   +to_string(obj.getPatientInfo()[i].file_name_id)+","
                   +"'" + mysqlActualDateFormat + "',"
                   "'" + mysqlExamDateFormat + "',"
                   +"1,"
                   +to_string(obj.getPatientInfo()[i].exam_value)+","
                   +to_string(obj.getPatientInfo()[i].total_value)+","
                   +"0),";
    }
    cout << endl;
    sqlValues = sqlValues.substr(0,sqlValues.length()-1); // removing the final ,
    
    try{
        checkConnection();
        stmt = con->createStatement();
        stmt->execute(("INSERT IGNORE INTO transactions_table(client_list_client_id, patient_list_patient_id, exams_list_exams_id, dollar_exchange_rate_dollar_id, file_name_file_name_id, transaction_current_date, transaction_exam_date, transaction_qtd, transaction_value, transaction_total_value, transaction_value_received) VALUES "+sqlValues).c_str());
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
   
    //Cleaning the array examValuesPerClient examValues[ARRAYSIZE]
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        examValues[i].exam_values_id = 0;
        examValues[i].client_list_client_id = 0;
        examValues[i].exam_list_exams_id = 0;
        examValues[i].exam_value = 0;
    }
    
    //Initialization of variables;
    examValuesPerClientCout = 0;
    transaction_total_value = 0;
    
    //delete [] obj.getPatientInfo();
    
    //obj.deleteDynamicArrayPatient();
}

void mysqlConnection::selectSumCurrentDateAndExamDate(createTargz & obj)
{
    double totalExamDate = 0;
    double totalActualDate = 0;
    
    //SELECT sum(transaction_value) from transactions_table where transaction_exam_date = '2017-01-12'
    try{
        checkConnection();
        stmt = con->createStatement();
        res = stmt->executeQuery(("SELECT sum(transaction_value) as total from transactions_table where transaction_exam_date = '"+ obj.getMysqlExamDateFormat()  + "'").c_str());
        while (res->next()) {
            totalExamDate = res->getDouble("total");
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    try{
        checkConnection();
        stmt = con->createStatement();
        res = stmt->executeQuery(("SELECT sum(transaction_value) as total from transactions_table where transaction_current_date = '"+ obj.getMysqlActualDateFormat() + "'").c_str());
        while (res->next()) {
            totalActualDate = res->getDouble("total");
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    cout << endl;
    cout << "============================================" << endl;
    cout << "Totais em Reais " << endl;
    cout << "Total Actual  Date: " << totalActualDate << endl;
    cout << "Total Exam Date: " << totalExamDate << endl;
    cout << "============================================" << endl << endl;
    
}

bool mysqlConnection::isFull_clInfo()
{
    return size == count;
}

bool mysqlConnection::isFull(int diffSize, int diffCount)
{
    return diffSize == diffCount;
}

void mysqlConnection::doubleSize_clInfo()
{
    size *= 2;
    clientInfo * temp;
    temp = new clientInfo[size];
    for(int i = 0; i< count; i++)
    {
        temp[i].clientActive = clInfo[i].clientActive;
        temp[i].clientChargeEmail = clInfo[i].clientChargeEmail;
        temp[i].clientExamEmails = clInfo[i].clientExamEmails;
        temp[i].clientInitial = clInfo[i].clientInitial;
        temp[i].clientName = clInfo[i].clientName;
        temp[i].clientId = clInfo[i].clientId;
    }
    
    delete[] clInfo;
    clInfo = temp;
}

void mysqlConnection::doubleSizeExamInfo()
{
    examSize *=2;
    examInfo * temp;
    temp = new examInfo[examSize];
    for (int i = 0; i < examCount; i++)
    {
        temp[i].exam_id = exInfo[i].exam_id;
        temp[i].exam_name = exInfo[i].exam_name;
        temp[i].exam_extension = exInfo[i].exam_extension;
    }
    delete[] exInfo;
    exInfo = temp;
}

clientInfo * mysqlConnection::getclInfo()
{
    return clInfo;
}

int mysqlConnection::getCount()
{
    return count;
}

int mysqlConnection::getSize(){
    return size;
}

int mysqlConnection::getClientID(string clientInitials)
{
    int clientId = 0;
    for(int i = 0; i < getCount(); i++)
    {
        if(clInfo[i].clientInitial == clientInitials)
            clientId = clInfo[i].clientId;
    }
    return clientId;
}

string mysqlConnection::getClientExamEmail(int idClient)
{
    string clientExamEmail = "NotFound";
    for(int i = 0; i < getCount(); i++)
    {
        if(clInfo[i].clientId == idClient)
            clientExamEmail = clInfo[i].clientExamEmails;
    }
    return clientExamEmail;
}

string mysqlConnection::getClientInitials(int idClient)
{
    string clientInitial = "NotFound";
    for(int i = 0; i < getCount(); i++)
    {
        if(clInfo[i].clientId == idClient)
            clientInitial = clInfo[i].clientInitial;
    }
    return clientInitial;
}

void mysqlConnection::printClInfo(string text)
{
    cout << text << endl;
    cout << "client_id|clientName|clientInitial|clientExamEmails|clientChargeEmail|clientActive" << endl;
    for(int i = 0; i < count; i++)
    {
        cout << clInfo[i].clientId << " ";
        cout << clInfo[i].clientName << " ";
        cout << clInfo[i].clientInitial << " ";
        cout << clInfo[i].clientExamEmails << " ";
        cout << clInfo[i].clientChargeEmail << " ";
        cout << clInfo[i].clientActive << " "<< endl;
        
//        string clientName;
//        string clientInitial;
//        string clientExamEmails;
//        string clientChargeEmail;
//        int clientActive;

        
    }
}

mysqlConnection::~mysqlConnection(){
    cout << "The ~mysqlConnection deconstructor called" << endl;
    //delete res;
    //delete stmt;
    delete con;
    delete[] clInfo;
    clInfo = nullptr;
    delete[] exInfo;
    exInfo = nullptr;
}


int main() {
    SystemCommand systemComm;
    systemComm.getCommandResult("echo \"677020\" | sudo -S postfix start");
    
    string wholeLine;
    string clinicInitial;
    int pastDays = 2;
    string fileComplementName = "";
    string sentinel = "";
    mysqlConnection mysqlConn;
    mysqlConn.selectClients();
    mysqlConn.selectExams();
    
    
    int size = mysqlConn.getSize();
    int count = mysqlConn.getCount();
    
    
    clientInfo *clientInformation;
    clientInformation = mysqlConn.getclInfo();
    
    
    do {
        cout << endl;
        cout << "Enter with clinic initial and past days separeted by a space, 'quit' to exit"<<endl;
        //cin >> clinicInitial >> pastDays >> fileComplementName;
        getline(cin, wholeLine);
        clinicInitial = wholeLine.substr(0,wholeLine.find(" "));
        wholeLine = wholeLine.substr(wholeLine.find(" ")+1, wholeLine.size());
        if(clinicInitial != wholeLine)
        {
            pastDays = atoi((wholeLine.substr(0,wholeLine.find(" "))).c_str());
            wholeLine = wholeLine.substr(wholeLine.find(" ")+1, wholeLine.size());
            fileComplementName = wholeLine;
            
            if (pastDays != 0 && pastDays == atoi((fileComplementName).c_str()))
                fileComplementName = "";
        }
        if (clinicInitial == "quit" || pastDays > 10)
        {
            cout << "Exiting the program with the sentinel or pastdays greater than 10."<<endl;
            sentinel = "quit";
            break;
        }
        
        createTargz newFile(clinicInitial, pastDays);
        
        newFile.createFileName(clientInformation, count, size, fileComplementName);
        mysqlConn.insertFileName(newFile.getFileName());
        newFile.createTargzFile();
        
        
        readFile newReadFile;
        //newReadFile.printReadFile("Inside Main ++++++++++++++++++++++++++++++++++++ ");
        
        cout << "Size of readFile => " << newReadFile.getCount() << endl;
        
        if(newReadFile.getCount() > 0)
        {
            mysqlConn.insertPatientNames(newReadFile, newFile.getClinicId());
        }
        else
        {
            cout << endl << "There are no files to compress to the file "<< newFile.getFileName()<< endl;
            cout << "Exiting the program."<< endl;
            exit(2);
        }
        
        scpSendFile newSCP(newFile.getFileName(), newFile.getClinicInitials());
        
        mysqlConn.insertTransactionDetails(newReadFile, newFile.getMysqlExamDateFormat(), newFile.getMysqlActualDateFormat());
        
        
        cout << newReadFile.getListPacientsFormatedForEmail() << endl;
        
        mysqlConn.selectSumCurrentDateAndExamDate(newFile);
        
        Email emailManipulation;
        cout << "Exam Date " << newFile.getExamDate() << endl;
        
        patientInfo * patientInformation = newReadFile.getPatientInfo();
        
        int clientId = mysqlConn.getClientID(clinicInitial);
        //mysqlConn.printClInfo("INSIDE MAIL");
        
        if(patientInformation != NULL)
        {
            
            string link = "http://www.tracadoselaudos.com.br/" + mysqlConn.getClientInitials(clientId) + "/"+ newFile.getFileName();
            
            string sendEmail = "no";
            
            cout << endl << link << endl;
            
            cout << "Send Email to " + mysqlConn.getClientExamEmail(clientId) + "? "<< endl;
            cin >> sendEmail;
            
            if(sendEmail == "s" || sendEmail == "S" || sendEmail == "y" || sendEmail == "Y")
            {
                
                
                emailManipulation.EmailSend("Análise(s) do dia " + newFile.getExamDate() + " | Arquivo: "+ newFile.getFileName(), link + "\n" + newReadFile.getListPacientsFormatedForEmail(), mysqlConn.getClientExamEmail(clientId));
                
                cout << endl<< "clientID=> " << clientId ;
                cout << endl<< "clientEmail=> " << mysqlConn.getClientExamEmail(clientId) << endl;
                
            }
            
        }
        //systemComm.getCommandResult("echo \"677020\" | sudo -S postfix stop");
        sentinel = "out";
        
    } while (sentinel != "out");
    return 0;
    
}


