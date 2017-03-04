#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
using namespace std;


short record;
struct Student
{
	char Name[50];
	char GPA[5];
	char Address[51];
	char Mobile[12];
	char Department[3];
};

short Hash1(char *a)
{
	return (a[0] * a[1]) % 100;
}
short Hash2(char *a)
{

	return (a[2] * a[3]) % 20;
}
void Display_Record(short offset)
{
	char temp[50] = { 0	 };
	if (offset != -1)
	{
		short name_size, id_size, gpa_size, address_size, mobile_size;
		fstream file("Main file.bin", ios::out | ios::binary | ios::in);
		file.clear();
		file.seekg(offset + 3, ios::beg);
		cout << "=================================\n";

		file.read((char*)&name_size, 2), file.read((char*)&temp, name_size);
		temp[name_size] = '\0';
		cout << "Student Name: ";
		cout << temp << endl;

		file.read((char*)&id_size, 2), file.read((char*)&temp, id_size);
		temp[id_size] = '\0';
		cout <<"Student ID:";
		cout << temp << endl;

		file.read((char*)&gpa_size, 2), file.read((char*)&temp, gpa_size);
		temp[gpa_size] = '\0';
		cout << "Student GPA:";
		cout << temp << endl;

		file.read((char*)&address_size, 2), file.read((char*)&temp,
			address_size);
		temp[address_size] = '\0';
		cout << "Student Address:";
		cout << temp << endl;

		file.read((char*)&mobile_size, 2), file.read((char*)&temp,
			mobile_size);
		temp[mobile_size] = '\0';
		cout <<"Student Mobile:";
		cout << temp << endl;

		file.clear(), file.close();
	}
	else
		puts("No Such ID Found !!!");
}

//[hash file]=[name]=[offset main]=[offset hash file]
void Way_1(fstream &file, char * name, short off, short pos)
{
	file.seekg(pos * 156, ios::beg);
	while (1)
	{
		char a;
		file.read(&a, 1);
		if (file.eof())
			break;

		if (a == '$')
		{
			file.seekg(-1, ios::cur);
			char name1[50] = {0};
			strcpy(name1, name);
			file.write(name1, sizeof name1);

			file.write((char*)&off, sizeof off);
			break;
		}
		else
			file.seekg(51, ios::cur);
	}
	file.clear(), file.close();
}
void Way_2(fstream &file, char * name, short off, short pos, short pos1)
{
	file.seekg(0, ios::end);
	file.clear();
	short end_of_file=file.tellg(),From_begin,From_current,tmp;
	file.seekg(pos * 104, ios::beg);
	bool ok = 0;
	while (1)
	{
		for (int i = 0; i < 2; i++)
		{
			char a;
			file.read(&a, 1);
			if (file.eof())
				break;

			if (a == '$')
			{
				file.seekg(-1, ios::cur);
				char name1[50] = {0};
				strcpy(name1, name);
				file.write(name1, sizeof name1);

				file.write((char*)&off, sizeof off);
				ok = 1;
				break;
			}
			else
				file.seekg(51, ios::cur);
		}
		if (ok)
			break;

		From_current = pos1-1, From_begin = 0;
		while (int(file.tellg()) + (104 * From_current) > end_of_file)
		{
			From_current--;
			From_begin++;
		}
		if (From_begin > 0)
		{
			file.seekg((104 * From_current), ios::beg);
			tmp=From_begin;
		}
		else
		{
			file.seekg((104 * From_current), ios::cur);
			tmp=From_current;
		}

		if (tmp == pos)
			return;
	}
	file.clear(), file.close();

}
int check_hash_file(fstream &file, char * name ,short pos)
{
	file.seekg(pos * 52, ios::beg);
	char a;
	file.read(&a, sizeof a);
	if (a == '$')
	{
		file.seekg(-1, ios::cur);
		char tmp[50] = {0};
		strcpy(tmp, name);
		file.write(tmp, sizeof tmp);
		file.write((char*)&record-1, sizeof record);
		record++;
		return -1;
	}
	else{

		short s;
		file.seekg(49, ios::cur);
		file.read((char*)&s, sizeof s);

		record++;
		return s;
	}
}
void linked(fstream &file, char name[], short off,short pos,bool exist)
{
	cout << pos << endl;
	file.seekg(54 * pos, ios::beg);
	char tmp[50] = {0};
	strcpy(tmp, name);

	if (!exist)
	{
		short off1 = -1;
		file.write(tmp, sizeof tmp);
		file.write((char*)&off, sizeof off);
		file.write((char*)&off1, sizeof off1);

		return;
	}
	short off1;

	while (1)
	{
		file.clear();
		char tmp1[50] = {0};
		file.read(tmp1, sizeof tmp1);
		file.read((char*)&off1, sizeof off1);
		file.read((char*)&off1, sizeof off1);
		if (off1 == -1)
			break;
		file.seekg(54 * off1, ios::beg);
	}
	file.seekp(-2, ios::cur);
	file.write((char*)&record-1, sizeof record);
	file.seekp(0, ios::end);
	file.write(tmp, sizeof tmp);
	file.write((char*)&off, sizeof off);
	file.write((char*)&off1, sizeof off1);
}

void Way_3(fstream &file,char *name,short offset, short hash_value)
{

	int check = check_hash_file(file, name, hash_value);
	file.clear(), file.close();
	file.open("Linked list.bin", ios::in | ios::out | ios::binary);

	if (check == -1)
		linked(file, name,offset,record-1 ,0);

	else
		linked(file, name,offset,check ,1);
	file.clear(), file.close();
}
void fill_hash_file(short tech)
{
	ofstream file("Hash file.bin", ios::out | ios::binary);
	ofstream file2("Main file.bin", ios::out | ios::binary);
	ofstream file3("Linked list.bin", ios::out | ios::binary);
	file.seekp(0, ios::beg);
	int limit = (tech == 1 ? 156: 104);
	if (tech == 3)
	{
		limit = 54;
	}
	for (int i = 0; i < 100; i++)
	{
		for (int i = 0; i < limit; i++)
			file.put('$');
	}
}
Student Read_input_from_user()
{
	cin.ignore();
	Student x;

	cout <<"Enter Student Name:";  cin.getline(x.Name, 50);

	cout <<"Enter Student GPA :"; cin.getline(x.GPA, 5);

	cout <<"Enter Student Address:"; cin.getline(x.Address, 51);

	cout << "Enter Student Mobile:"; cin.getline(x.Mobile, 12);

	cout << "Enter Student Department:"; cin.getline(x.Department, 3);
	return x;
}

void Write_report_on_file(fstream &out, Student &x, short record_size,
	short Name_size, short gpa_size, short address_size, short mobile_size,
	short department_size)
{
	char check_delete = ' ';

	out.write((char*)&record_size, 2);
	out.write(&check_delete, 1);

	out.write((char*)&Name_size, 2);
	out.write(x.Name, Name_size);

	out.write((char*)&gpa_size, 2);
	out.write((char*)&x.GPA, gpa_size);

	out.write((char*)&address_size, 2);
	out.write(x.Address, address_size);

	out.write((char*)&mobile_size, 2);
	out.write(x.Mobile, mobile_size);

	out.write((char*)&department_size, 2);
	out.write((char*)&x.Department, department_size);
}
void Add(short tech)
{
	Student x = Read_input_from_user();
	short Name_size, address_size, mobile_size;
	short department_size, gpa_size, record_size = 0;


	fstream file("Main file.bin", ios::out | ios::binary | ios::in | ios::ate);

	Name_size = strlen(x.Name);

	gpa_size = strlen(x.GPA);

	address_size = strlen(x.Address);

	mobile_size = strlen(x.Mobile);

	department_size = strlen(x.Department);

	record_size = 11 + Name_size + department_size + gpa_size + mobile_size
		+ address_size;

	short offset = file.tellg();
	Write_report_on_file(file, x, record_size, Name_size, gpa_size,
		address_size, mobile_size, department_size);

	file.clear(), file.close();


	fstream file2("Hash file.bin", ios::in | ios::out | ios::binary);
	if (tech == 1) Way_1(file2, x.Name, offset, Hash1(x.Name));
	else if (tech == 2) Way_2(file2, x.Name, offset, Hash1(x.Name), Hash2(x.Name));
	else Way_3(file2, x.Name, offset, Hash1(x.Name));



	file2.clear(), file.close();
}
short search_hash_1(fstream &file, char *name, short pos, bool ok)
{
	file.seekg(pos * 156, ios::beg);
	short tmppos = pos;
	while (1)
	{
		for (int i = 0; i < 3; i++)
		{
			char a;
			file.read(&a, sizeof a);
			if (a == '$')
			{
				return -1;
			}
			else
			{
				char nametmp[50] = {0};
				short offset;
				file.seekg(-1, ios::cur);
				file.read(nametmp, sizeof nametmp);
				file.read((char*)&offset, sizeof offset);

				if (strcmp(name, nametmp) == 0)
				{
					if (ok)
					{
						file.seekg(-52, ios::cur);
						file.put('*');
					}

					return offset;
				}
			}

		}
		pos++;
		if (pos == tmppos)
		{
			return -1;
		}
		if (pos > 100)
		{
			pos = 0;
			file.seekg(0, ios::beg);
		}
	}
	return -1;
}
short search_hash_2(fstream & file, char *name,short pos,short pos1,bool ok)
{

	file.seekg(0,ios::end);
	short end_of_file = file.tellg(), From_begin, From_current,tmp;
	file.seekg(pos * 104, ios::beg);
	while (true)
	{
		for (int i = 0; i < 2; i++)
		{
			char a;
			file.read(&a, sizeof a);
			if (a == '$')
			{
				return -1;
			}
			else
			{
				char nametmp[50] = {0};
				short offset;
				file.seekg(-1, ios::cur);
				file.read(nametmp, sizeof nametmp);
				file.read((char*)&offset, sizeof offset);

				if (strcmp(name, nametmp) == 0)
				{
					if (ok)
					{
						file.seekg(-52, ios::cur);
						file.put('*');
					}

					return offset;
				}
				cout << nametmp << endl;
			}

		}
		From_current = pos1 - 1;
		From_begin = 0;
		while (int(file.tellg()) + (104 * From_current) > end_of_file)
			From_current--, From_begin++;
		if (From_begin > 0)
		{
			file.seekg((104 * From_current), ios::beg);
			tmp = From_begin;
		}
		else
		{
			file.seekg((104 * From_current), ios::cur);
			tmp = From_current;
		}
		if (tmp == pos)
			return -1;
	}
}
void search(short tech)
{
	cin.ignore();
	cout << "Enter name you want to search for : ";
	char name[51] = {0};
	short offset;
	cin.getline(name, 51);
	cout << name << endl;
	if (tech == 1) {
		fstream file("Hash file.bin", ios::binary | ios::in);
		offset = search_hash_1(file, name, Hash1(name), 0);

		if (offset != -1) Display_Record(offset);
		else cout << "No such name :(";
		file.close();
	}
	else if (tech == 2) {
		fstream file("Hash file.bin", ios::binary | ios::in);
		offset = search_hash_2(file, name, Hash1(name),Hash2(name), 0);

		if (offset != -1) Display_Record(offset);
		else cout << "No such name :(";
		file.close();
	}
	return;

}

void delete_from_main_file(short offset)
{
	fstream file("Main file.bin", ios::in | ios::out | ios::binary);
	file.seekg(offset + 2, ios::beg);
	file.put('*');
	file.close();
	return;
}

bool Delete(short tech)
{
	char name[51] ={0};
	cin.ignore();
	cout << "Enter name to delete : ";
	cin.getline(name, 50);

	if (tech == 1)
	{
		fstream file2("Hash file.bin", ios::in | ios::out | ios::binary);
		short offset = search_hash_1(file2, name, Hash1(name), 1);
		if (offset == -1)
		{
			puts("No such name");
			return 0;
		}
		file2.close();
		delete_from_main_file(offset);
		return 1;
	}
	else if (tech == 2)
	{
		fstream file2("Hash file.bin", ios::in | ios::out | ios::binary);
		short offset = search_hash_2(file2, name, Hash1(name),Hash2(name), 1);
		if (offset == -1)
		{
			cout<<"No such name";
			return 0;
		}
		file2.close();
		delete_from_main_file(offset);
		return 1;
	}

	return 0;
}
int main()
{
	cout <<"=====================================\n"
		<<"Enter which technique you want to use"
		<<"\nEnter your choice :\n"
		<<"1)Progressive overflow\n"
		<<"2)Double hashing\n"
		<<"3)Chained with separate overflow area\n";

	short tech, choice;
	cout << "\nChoice : ";
	cin >> tech;
	fill_hash_file(tech);
	while (true)
	{
		cout <<"=====================================\n"<<"\n1) Add"<<"\n2) Search"<<"\n3) Delete"<<"\n4) Exit"<< "\nChoice : ";
		cin >> choice;
		cout <<"=====================================\n";
		if (choice == 1) Add(tech);
		else if (choice == 2) search(tech);
		else if (choice == 3) Delete(tech);
		else if (choice == 4) break;

	}
	return 0;

}
