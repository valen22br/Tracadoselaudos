/************************************************************************************************************************
File Name: csv_to_sql.cpp
Name: Luis Gustavo Grubert Valensuela Z#:23351882 lvalensuela2015@fau.edu
Course: Foundations of Computer Science (COP3014)
Professor: Dr. Lofton Bullard
Due Date:11/20/2016             Due Time: 11:30PM
Total Points: 100
Assignment Number 10: call_stats7
Last Changed: 11/14/16

Description:
This programm reads a csv file and generates specific sql queries to insert values on mysql tables from 
tracadoselaudos project
/************************************************************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;



class table_structure
{
public:
	int client_id;
	int patient_id;
	string exam_id_initial;
	int exam_id;
	int dollar_id;
	int filename_id;
	string complete_date;
	int current_date_day;
	int current_date_month;
	int current_date_year;
	int exam_qtd;
	double total_transaction;
	double payment_valuel;
};


class csv_to_sql_class
{
public:
	csv_to_sql_class();
	~csv_to_sql_class(); //de-allocates all memory allocate to table_data by operator new.
	bool is_empty(); //inline implementation
	bool is_full();//inline implementation
	//int search(const string key);//returns location if item in listl; otherwise return -1
	//void add(); //adds a call record to table_data
	//void remove(const string key); //removes an item from the list
	void double_size();
	
	//void process();
	void process(int client_id, int patient_id, int dollar_id, int filename_id);
	void print(); //prints all the elements in the list to the screen
	//csv_to_sql_class & operator-(const string key); //removes an item from the list
	//friend ostream & operator<<(ostream & out_to_file, csv_to_sql_class & Org); //prints all the elements in the 
																		  //list to the screen.
private:
	string scape_str;
	int count;
	int size;
	int dynamic_array_size = 7;
	table_structure *table_data;
	//enum exam_id { traçados=1, tomografia=2, periapical=3, carpal=4, laudo=5, recebimento=6 };
	typedef string* StrPtr;
	StrPtr exams_type_array = new string[dynamic_array_size];
};




/************************************************************************************************************************************/
//Name: Default Constructor
//Precondition: An object of the type call_class have been created.
//Postcondition: It reads the content of the file callstats_data.txt, is processed and populates the dynamic array
//Decription: Reads the data file of call information (cell number, relays and call length) into the dynamic array of call record, 
//table_data. If the count because equal to the size the function double_size is called and the memory allocated to table_data is doubled.
/************************************************************************************************************************************/
csv_to_sql_class::csv_to_sql_class()
{
	//int size = 5; //total amount of memory (cells) allocated for the dynamic array of call records
	//int count = 0;
	count = 0;
	size = 5;
	table_data = new table_structure[size];
	
	exams_type_array[1] = "traçados";
	exams_type_array[2] = "tomo";
	exams_type_array[3] = "periapicais";
	exams_type_array[4] = "carpal";
	exams_type_array[5] = "laudos";
	exams_type_array[6] = "recebimento";
	
	ifstream in;
	
	in.open("csv.txt");
	int guti = 1;
	while (!in.eof())
	{

		cout << guti++ << endl;
		if (is_full())
		{
			double_size();
		}

	
		in >> table_data[count].complete_date
			>> table_data[count].exam_id_initial
			>> table_data[count].exam_qtd
			>> scape_str
			>> table_data[count].total_transaction
			>> scape_str
			>> table_data[count].payment_valuel;
		
		count++;

	}

	
	in.close();
	

}

/***********************************************************************************************************************************/
//Name: is_empty
//Precondition: An object of the type call_class have been created.
//Postcondition: Returns true if table_data is empty
//Decription: returns true if table_data is empty
/**********************************************************************************************************************************/
bool csv_to_sql_class::is_empty()
{
	return count == 0;
}

/**********************************************************************************************************************************/
//Name: is_full 
//Precondition: An object of the type call_class have been created.
//Postcondition: returns true if table_data is full.
//Decription: returns true if table_data is full
/*********************************************************************************************************************************/
bool csv_to_sql_class::is_full()
{
	return count == size;
}

/**********************************************************************************************************************************/
//Name: search
//Precondition: An object of the type call_class have been created.
//Postcondition: Returns the index from the dynamic array where the first occurrence was found, otherwise return -1.
//Decription: locates key in table_data if it is there; otherwise -1 is returned
/*********************************************************************************************************************************/
//int csv_to_sql_class::search(const string key)
//{
//	int sresult = -1;
//	for (int i = 0; i < count; i++)
//	{
//		if (table_data[i].cell_number == key)
//			sresult = i;
//	}
//	return sresult;
//
//
//	//return -1;
//}

/*********************************************************************************************************************************/
//Name: add
//Precondition: An object of the type call_class have been created.
//Postcondition: add register to table_structure dynamic array.
//Decription: adds a table_structure to table_data; if table_data is full, double_size is called to increase the size of table_data. The user 
//            is prompted to enter the firstname, lastname, cell number, relays and call length. 
/********************************************************************************************************************************/
//void csv_to_sql_class::add()
//{
//	if (is_full())
//	{
//		double_size();
//	}
//
//	cout << endl << endl;
//	cout << "Enter firstname, lastname, cell_number, relays and call_length:\n";
//
//	cin >> table_data[count].firstname
//		>> table_data[count].lastname
//		>> table_data[count].cell_number
//		>> table_data[count].relays
//		>> table_data[count].call_length;
//
//	if (cin.fail())
//	{
//		cout << "The values you entered are not compatible for number of relays and call length." << endl;
//		cout << "Exiting the program..." << endl;
//		exit(1);
//
//	}
//
//	count++;
//
//	process();
//
//
//}

/********************************************************************************************************************************/
//Name: remove
//Precondition: An object of the type call_class have been created.
//Postcondition: remove the register where key was found. If ther is more than one occurence, all of then will be deleted.
//Decription: remove key from table_data if it is there.
/*******************************************************************************************************************************/
//void csv_to_sql_class::remove(const string key)
//{
//	int loc = -1;
//	if (is_empty())
//	{
//		cout << endl << endl << "** Array is empty **" << endl << endl;
//	}
//	else
//	{
//		loc = search(key);
//
//		if (loc == -1)
//		{
//			cout << endl << endl << "** Cell phone number not found. No Register were deleted. **" << endl << endl;
//		}
//		else
//		{
//			do
//			{
//				cout << endl << "** register " << table_data[loc].lastname << " "
//					<< table_data[loc].firstname << " " << table_data[loc].cell_number
//					<< " successfuly deleted **" << endl;
//
//				for (int j = loc; j < count - 1; j++)
//				{
//					table_data[j] = table_data[j + 1];
//				}
//				count--;
//				loc = search(key);
//			} while (loc != -1);
//		}
//	}
//
//}

/******************************************************************************************************************************/
//Name: double_size
//Precondition: An object of the type call_class have been created.
//Postcondition: Double the size of the dynimic array.
//Decription: doubles the size (capacity) of table_data
/******************************************************************************************************************************/
void csv_to_sql_class::double_size()
{
	size *= 2;
	table_structure *temp = new table_structure[size];

	for (int i = 0; i<count; i++)
	{
		temp[i] = table_data[i];
	}

	delete[] table_data;
	table_data = temp;
}


/******************************************************************************************************************************/
//Name: process
//Precondition: An object of the type call_class have been created.
//Postcondition: Sets table_data member variables: tax_rate, call_tax and net_cost
//Description:  Calculares tax_rate, call_tax and net_cost for each table_structure members
/*****************************************************************************************************************************/
void csv_to_sql_class::process(int client_id, int patient_id, int dollar_id, int filename_id)
{
	int i;
	string delimiter = "/";
	int first_position, second_position;

	for (i = 0; i < count; i++)
	{

		/*in >> table_data[count].complete_date
			>> table_data[count].exam_id_initial
			>> table_data[count].exam_qtd
			>> scape_str
			>> table_data[count].total_transaction
			>> scape_str
			>> table_data[count].payment_valuel;*/

		//cout << table_data[i].complete_date << " " << table_data[i].exam_id_initial << " " << table_data[i].exam_qtd << " " << table_data[i].total_transaction << " " << table_data[i].payment_valuel << endl;

		for (int j = 1; j < dynamic_array_size; j++)
		{
			if (table_data[i].exam_id_initial == exams_type_array[j])
			{
				table_data[i].exam_id =  j ;
			}
		}

		first_position = table_data[i].complete_date.find_first_of(delimiter);
		second_position = table_data[i].complete_date.find_last_of(delimiter);

		table_data[i].current_date_day = stoi(table_data[i].complete_date.substr(0, first_position));
		table_data[i].current_date_month = stoi(table_data[i].complete_date.substr(first_position + 1, second_position));
		table_data[i].current_date_year = stoi(table_data[i].complete_date.substr(second_position + 1, 2));

		table_data[i].client_id = client_id;
		table_data[i].patient_id = patient_id;
		table_data[i].dollar_id = dollar_id;
		table_data[i].filename_id = filename_id;
	}



}


/****************************************************************************************************************************/
//Name: print
//Precondition: An object of the type call_class have been created.
//Postcondition: Prints the content of the dynamic array on the screen and also on the file call_stats_5.txt
//Decription: prints every field of every table_structure in table_data formatted to the screen and write them to the file call_stats_7.txt
/***************************************************************************************************************************/
void csv_to_sql_class::print()
{
	ofstream fout_file;
	fout_file.open("sql.txt");

	if (fout_file.fail())
	{
		cout << "Error trying to accesss output file \"call_stats_5.txt\"" << endl;
	}
	else
	{

		fout_file.setf(ios::showpoint);
		fout_file.precision(2);
		fout_file.setf(ios::fixed);

		cout.setf(ios::showpoint);
		cout.precision(2);
		cout.setf(ios::fixed);

		cout << count << " Register(s) " << endl;
		cout << "INSERT INTO `transactions_table` (`client_list_client_id`, ";
		cout << "`patient_list_patient_id`, `exams_list_exams_id`, `dollar_exchange_rate_dollar_id`,";
		cout << "`file_name_file_name_id`, `transaction_current_date`, `transaction_exam_date`,";
		cout << "`transaction_qtd`, `transaction_value`, `transaction_total_value`, `transaction_value_received`) VALUES";

		fout_file << count << " Register(s) " << endl;
		fout_file << "INSERT INTO `transactions_table` (`client_list_client_id`, ";
		fout_file << "`patient_list_patient_id`, `exams_list_exams_id`, `dollar_exchange_rate_dollar_id`,";
		fout_file << "`file_name_file_name_id`, `transaction_current_date`, `transaction_exam_date`,";
		fout_file << "`transaction_qtd`, `transaction_value`, `transaction_total_value`, `transaction_value_received`) VALUES";

		for (int i = 0; i < count; i++)
		{
			cout << "('";
			cout << table_data[i].client_id << "','";
			cout << table_data[i].patient_id << "','";
			cout << table_data[i].exam_id << "','";
			cout << table_data[i].dollar_id << "','";
			cout << table_data[i].filename_id << "','";

			cout << "20";
			cout << table_data[i].current_date_year << "-";
			cout << table_data[i].current_date_month << "-";
			cout << table_data[i].current_date_day << "','";

			cout << "20";
			cout << table_data[i].current_date_year << "-";
			cout << table_data[i].current_date_month << "-";
			cout << table_data[i].current_date_day << "','";

			cout << table_data[i].exam_qtd << "','";

			if (table_data[i].exam_id != 6 && i > 0)
			{
				if (table_data[i - 1].total_transaction != 0)
				{
					cout << abs(table_data[i].total_transaction - table_data[i - 1].total_transaction) << "','";
				}
				else
				{
					cout << table_data[i].total_transaction << "','";
				}
			}
			else if (i == 0)
			{
				cout << table_data[i].total_transaction << "','";
			}
			else
			{
				cout << "0" << "','";
			}


			cout << table_data[i].total_transaction << "','";
			cout << table_data[i].payment_valuel << "'";
			cout << "),";
			
			fout_file << "('";
			fout_file << table_data[i].client_id << "','";
			fout_file << table_data[i].patient_id << "','";
			fout_file << table_data[i].exam_id << "','";
			fout_file << table_data[i].dollar_id << "','";
			fout_file << table_data[i].filename_id << "','";

			fout_file << "20";
			fout_file << table_data[i].current_date_year << "-";
			fout_file << table_data[i].current_date_month << "-";
			fout_file << table_data[i].current_date_day << "','";

			fout_file << "20";
			fout_file << table_data[i].current_date_year << "-";
			fout_file << table_data[i].current_date_month << "-";
			fout_file << table_data[i].current_date_day << "','";

			fout_file << table_data[i].exam_qtd << "','";

			if (table_data[i].exam_id != 6 && i > 0)
			{
				if (table_data[i - 1].total_transaction != 0)
				{
					fout_file << abs(table_data[i].total_transaction - table_data[i - 1].total_transaction) << "','";
				}
				else
				{
					fout_file << table_data[i].total_transaction << "','";
				}
			}
			else if (i == 0)
			{
				fout_file << table_data[i].total_transaction << "','";
			}
			else
			{
				fout_file << "0" << "','";
			}

			fout_file << table_data[i].total_transaction << "','";
			fout_file << table_data[i].payment_valuel << "'";
			fout_file << "),";

		}

	}


}

/****************************************************************************************************************************/
//Name: operator - overloading
//Precondition: An object of the type call_class have been created.
//Postcondition: remove the register where key was found. If ther is more than one occurence, all of then will be deleted.
//Decription: remove key from table_data if it is there.
/***************************************************************************************************************************/

//csv_to_sql_class & csv_to_sql_class::operator-(const string key)
//{
//	int loc = -1;
//	if (is_empty())
//	{
//		cout << endl << endl << "** Array is empty **" << endl << endl;
//	}
//	else
//	{
//		loc = search(key);
//
//		if (loc == -1)
//		{
//			cout << endl << endl << "** Cell phone number not found. No Register were deleted. **" << endl << endl;
//		}
//		else
//		{
//			do
//			{
//				cout << endl << "** register " << table_data[loc].lastname << " "
//					<< table_data[loc].firstname << " " << table_data[loc].cell_number
//					<< " successfuly deleted **" << endl;
//
//				for (int j = loc; j < count - 1; j++)
//				{
//					table_data[j] = table_data[j + 1];
//				}
//				count--;
//				loc = search(key);
//			} while (loc != -1);
//		}
//	}
//	return *this;
//
//}

/****************************************************************************************************************************/
//Name: operator << overloading
//Precondition: a object of type call_class have been created
//Postcondition: Prints the content of the dynamic array on the screen and also on the file call_stats_7.txt
//Decription: prints every field of every table_structure in table_data formatted to the screen and write them to the file call_stats_7.txt
/***************************************************************************************************************************/
//ostream & operator<<(ostream & out_to_file, csv_to_sql_class & Org)
//{
//	ofstream fout_file;
//	fout_file.open("call_stats_7.txt");
//
//	if (fout_file.fail())
//	{
//		cout << "Error trying to accesss output file \"call_stats_7.txt\"" << endl;
//	}
//	else
//	{
//
//		fout_file.setf(ios::showpoint);
//		fout_file.precision(2);
//		fout_file.setf(ios::fixed);
//
//		cout.setf(ios::showpoint);
//		cout.precision(2);
//		cout.setf(ios::fixed);
//
//		//cout << count << " Register(s) " << endl;
//		for (int i = 0; i < Org.count; i++)
//		{
//			cout << Org.table_data[i].firstname << "\t";
//			cout << Org.table_data[i].lastname << "\t";
//			cout << Org.table_data[i].cell_number << "\t";
//			cout << Org.table_data[i].relays << "\t";
//			cout << Org.table_data[i].call_length << "\t";
//
//			fout_file << Org.table_data[i].firstname << "\t";
//			fout_file << Org.table_data[i].lastname << "\t";
//			fout_file << Org.table_data[i].cell_number << "\t";
//			fout_file << Org.table_data[i].relays << "\t";
//			fout_file << Org.table_data[i].call_length << "\t";
//
//			if (Org.table_data[i].net_cost > 0)
//			{
//				cout << Org.table_data[i].net_cost << "\t";
//				cout << Org.table_data[i].tax_rate << "\t";
//				cout << Org.table_data[i].call_tax << "\t";
//				cout << Org.table_data[i].total_cost << endl;
//
//				fout_file << Org.table_data[i].net_cost << "\t";
//				fout_file << Org.table_data[i].tax_rate << "\t";
//				fout_file << Org.table_data[i].call_tax << "\t";
//				fout_file << Org.table_data[i].total_cost << endl;
//			}
//			else
//			{
//				cout << endl;
//				fout_file << endl;
//			}
//		}
//
//	}
//	return out_to_file;
//}
//
///****************************************************************************************************************************/
////Name: destructor
////Precondition: An object of the type call_class have been created.
////Postcondition:  de-allocates all memory allocated to table_data.  This should be the last function to be called before the program
////            is exited.
////Decription: de-allocates all memory allocated to table_data.  This should be the last function to be called before the program
////            is exited.
///***************************************************************************************************************************/
csv_to_sql_class::~csv_to_sql_class()
{
	cout << endl << "Deconstructor was called" << endl;
	delete[] table_data;
	delete[] exams_type_array;
}


//driver to test the functionality of your class.
int main()
{
	cout << "TEST1: Testing the Default Constructor, is_full and double_size\n";
	csv_to_sql_class MyClass;
	cout << "Finish TEST1: testing the default constructor\n\n\n\n";
	MyClass.process(57, 1, 1, 1);
	MyClass.print();


	return 0;
}



