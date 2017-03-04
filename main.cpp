#include <iostream>
#include <fstream>
 #include <cstdlib>
#include<cstring>
#include<strstream>

using namespace std;
class Book
{
public:
	char ISBN[30];
    char Title[50];
    char Author[50];
    float Price;
    char Publisher[50];
    Book();
    Book(string,string,string,float,string);
    friend ostream &operator<<(ostream &std,Book &s);
};
Book::Book()
{
	
}
Book::Book(string sISBN,string sTitle,string sAuthor,float sPrice,string sPublisher)
{
	strncpy(ISBN,sISBN.c_str(),sizeof(ISBN));
	strncpy(Title,sTitle.c_str(),sizeof(Title));
	strncpy(Author,sAuthor.c_str(),sizeof(Author));
	strncpy(Publisher,sPublisher.c_str(),sizeof(Publisher));
	Price=sPrice;
}
ostream &operator<<(ostream &std,Book &s)
{
	std<<s.ISBN;
	std<<s.Title;
	std<<s.Author;
	std<<s.Publisher;
	std<<s.Price;
	return std;
}
int getlength(Book &s)
{
	int lengthISBN,lengthTitle,lengthAuthor,lengthPublisher,lengthPrice,RecordLength;
	lengthISBN=sizeof(s.ISBN);
	lengthTitle=sizeof(s.Title);
	lengthAuthor=sizeof(s.Author);
	lengthPublisher=sizeof(s.Publisher);
	lengthPrice=sizeof(s.Price);
	RecordLength=lengthISBN+lengthTitle+lengthAuthor+lengthPrice+lengthPublisher+15;
	return RecordLength;
}
int Check(char n[30],fstream &f)
{
	Book s;
	bool c=0;
	while(!f.eof())
	{
		if(strcmp(s.ISBN,n)==0)
		{
			c=0;
			break;
		}
	   else
		c=1;
	}
	return c;
}
void AddAvail(fstream &f,Book &s)
{
	int header,LDelete,PrevDelete;
	char deleteflag;
	f.seekg(0,ios::beg);
	f.read((char *)&header,sizeof(header));
	LDelete=header;
	f.seekg(4+(LDelete-1)*getlength(s),ios::beg);
	f.get(deleteflag);
	f.read((char*) &PrevDelete,sizeof(PrevDelete));
	f.seekp(0,ios::beg);
	f.write((char*) &PrevDelete, sizeof(PrevDelete));
	f.seekp(4 + (LDelete-1)*getlength(s),ios::beg);
}
void addBook(fstream &f,Book &s)
{
	int RecordLength=getlength(s);
	char delimeter='|';
	int header;
	int PrevDelete=-1;
	f.seekp(0,ios::beg);
	f.read((char*)&header,sizeof(header));
	if(header==-1)
		f.seekp(0,ios::end);
	else
	AddAvail(f,s);
	f.put(' ');
	f.write((char*)&PrevDelete,sizeof(PrevDelete));
	f.write((char*)&RecordLength,sizeof(RecordLength));
	f.write((char*)&s.ISBN,sizeof(s.ISBN));
	f.write((char*)&delimeter,sizeof(delimeter));
	f.write((char*)&s.Title,sizeof(s.Title));
	f.write((char*)&delimeter,sizeof(delimeter));
	f.write((char*)&s.Author,sizeof(s.Author));
	f.write((char*)&delimeter,sizeof(delimeter));
	f.write((char*)&s.Price,sizeof(s.Price));
	f.write((char*)&delimeter,sizeof(delimeter));
	f.write((char*)&s.Publisher,sizeof(s.Publisher));
	f.write((char*)&delimeter,sizeof(delimeter));
	f.seekp(0,ios::beg);
	f.close();
	}
int deleteBook(char n[30],fstream &f)
{
	Book s;
	bool c=0;
	bool k=0;
	char deleteflag;
	int PrevDelete;
	int RecordLength;
	int lengthISBN;
	f.seekp(4,ios::beg);
	while(!f.eof())
	{
		f.get(deleteflag);
		f.read((char*)&PrevDelete,sizeof(PrevDelete));
		f.read((char*)&RecordLength,sizeof(RecordLength));
		f.read((char*)&s.ISBN,sizeof(s.ISBN));
		if(strcmp(s.ISBN,n)==0)
		{
			c=0;
			lengthISBN=sizeof(s.ISBN);
			if(deleteflag=='*')
			{
				k=0;
			}
			else
			{
			k=1;
			}
			break;
		}
	   else
		{
		c=1;
		f.seekp(1,ios::cur);
		f.read((char*)&s.Title,sizeof(s.Title));
		f.seekp(1,ios::cur);
		f.read((char*)&s.Author,sizeof(s.Author));
		f.seekp(1,ios::cur);
		f.read((char*)&s.Price,sizeof(s.Price));
		f.seekp(1,ios::cur);
		f.read((char*)&s.Publisher,sizeof(s.Publisher));
		f.seekp(1,ios::cur);
		}
	}
	if(c==1)
	{
	cout<<"this ID not found ! "<<endl;
	}
	else if((c==0)&&(k==1))
	{
		cout<<"this Book already deleted";
	}
	else if((c==0)&&(k==0))
	{
		int m=lengthISBN+4+4;
		f.seekp(-m,ios::cur);
		int k=f.tellp();
		int header,PrevDelete;
		f.seekp(0,ios::beg);
		f.read((char*)&header,sizeof(header));
		PrevDelete=header;
		f.seekp(k,ios::beg);
		f.put('*');	
	}
}
void updateBook(fstream &f,Book &s,char n[30])
{
	bool l=0;
	bool k=0;
	char deleteflag;
	int PrevDelete;
	int RecordLength;
	int lengthISBN;
	f.seekp(4,ios::beg);
	while(!f.eof())
	{
		f.get(deleteflag);
		f.read((char*)&PrevDelete,sizeof(PrevDelete));
		f.read((char*)&RecordLength,sizeof(RecordLength));
		f.read((char*)&s.ISBN,sizeof(s.ISBN));
		if(strcmp(s.ISBN,n)==0)
		{
			l=0;
			lengthISBN=sizeof(s.ISBN);
			break;
		}
	   else
		l=1;
		f.seekp(1,ios::cur);
		f.read((char*)&s.Title,sizeof(s.Title));
		f.seekp(1,ios::cur);
		f.read((char*)&s.Author,sizeof(s.Author));
		f.seekp(1,ios::cur);
		f.read((char*)&s.Price,sizeof(s.Price));
		f.seekp(1,ios::cur);
		f.read((char*)&s.Publisher,sizeof(s.Publisher));
		f.seekp(1,ios::cur);
	}
	if(l==0)
	{	
		int m=lengthISBN;
		f.seekg(-m,ios::cur);
		int RecordLength=getlength(s);
		char delimeter='|';
		f.put(' ');
		f.write((char*)&PrevDelete,sizeof(PrevDelete));
		f.write((char*)&RecordLength,sizeof(RecordLength));
		f.write((char*)&s.ISBN,sizeof(s.ISBN));
		f.write((char*)&delimeter,sizeof(delimeter));
		f.write((char*)&s.Title,sizeof(s.Title));
		f.write((char*)&delimeter,sizeof(delimeter));
		f.write((char*)&s.Author,sizeof(s.Author));
		f.write((char*)&delimeter,sizeof(delimeter));
		f.write((char*)&s.Price,sizeof(s.Price));
		f.write((char*)&delimeter,sizeof(delimeter));
		f.write((char*)&s.Publisher,sizeof(s.Publisher));
		f.write((char*)&delimeter,sizeof(delimeter));
		f.seekp(0,ios::beg);
	}
}	
int searchbyISBN(char n[10],fstream &f)
{
	Book s;
	f.seekg(4,ios::beg);
	int RecordLength;
	bool l=0;
	while(!f.eof())
	{
		f.seekg(5,ios::cur);
		f.read((char*)&RecordLength,sizeof(RecordLength));
		f.read((char*)&s.ISBN,sizeof(s.ISBN));
		if(strcmp(s.ISBN,n)==0)
		{
			l=0;
			break;
		}
		else
		{
		l=1;
		f.seekg(1,ios::cur);
		f.read((char*)&s.Title,sizeof(s.Title));
		f.seekg(1,ios::cur);
		f.read((char*)&s.Author,sizeof(s.Author));
		f.seekg(1,ios::cur);
		f.read((char*)&s.Price,sizeof(s.Price));
		f.seekg(1,ios::cur);
		f.read((char*)&s.Publisher,sizeof(s.Publisher));
		f.seekg(1,ios::cur);
		}
	}
	if(l==0)
	{
		while(true)
		{
			cout<<s.ISBN<<"\t";
			f.seekg(1,ios::cur);
			f.read((char*)&s.Title,sizeof(s.Title));
			cout<<s.Title<<"\t";
			f.seekg(1,ios::cur);
			f.read((char*)&s.Author,sizeof(s.Author));
			cout<<s.Author<<"\t";
			f.seekg(1,ios::cur);
			f.read((char*)&s.Price,sizeof(s.Price));
			cout<<s.Price<<"\t";
			f.seekg(1,ios::cur);
			f.read((char*)&s.Publisher,sizeof(s.Publisher));
			cout<<s.Publisher<<"\t";
			cout<<endl;
			break;
		}	
	}
		else
	{
		cout<<"this Book not found"<<endl;
	}		
}
int searchbyAuthor(char m[50],fstream &f)
{
	Book s;
	f.seekg(4,ios::beg);
	int RecordLength;
	bool l=0;
	while(!f.eof())
	{
		f.seekg(5,ios::cur);
		f.read((char*)&RecordLength,sizeof(RecordLength));
		f.read((char*)&s.ISBN,sizeof(s.ISBN));
		f.seekg(1,ios::cur);
		f.read((char*)&s.Title,sizeof(s.Title));
		f.seekg(1,ios::cur);
		f.read((char*)&s.Author,sizeof(s.Author));
		if(strcmp(s.Author,m)==0)
		{
			l=0;
			break;
		}
		else
		{
		l=1;
		f.seekg(1,ios::cur);
		f.read((char*)&s.Price,sizeof(s.Price));
		f.seekg(1,ios::cur);
		f.read((char*)&s.Publisher,sizeof(s.Publisher));
		f.seekg(1,ios::cur);
		}
	}
	if(l==0)
	{
		while(true)
		{
			cout<<s.ISBN<<"\t";
			cout<<s.Title<<"\t";
			cout<<s.Author<<"\t";
			f.seekg(1,ios::cur);
			f.read((char*)&s.Price,sizeof(s.Price));
			cout<<s.Price<<"\t";
			f.seekg(1,ios::cur);
			f.read((char*)&s.Publisher,sizeof(s.Publisher));
			cout<<s.Publisher<<"\t";
			cout<<endl;
			break;
		}	
	}
	else
	{
		cout<<"this Book not found"<<endl;
	}	
}
int startscreen()
{
cout<<"\t\tBooks Management System\n\n";
        cout<<"1)Add New Book\n2)Update Book\n3)Delete Book\n4)Search for A Book By ISBN\n";
        cout<<"5)Search for A Book By Author\n6)Compact File\n\n7)Exit\n\nPlease Enter Your Choice :";
}
int main() 
{
	ofstream v;
	v.open("C:\\Users\\SeifMostafa\\Desktop\\Assignment1\\Books.txt",ios::out);
	v.close();
	fstream file;
	file.open("C:\\Users\\SeifMostafa\\Desktop\\Assignment1\\Books.txt",ios::out|ios::in|ios::binary);
	Book d;
	char ISBN[30];
    char Title[50];
    char Author[500];
    float Price;
    char Publisher[50];
    int x;
    char c;
	startscreen();
	while(true)
	{
		cin>>x;
		if(x==1)
		{
			system("cls");
			while(true)
			{
			cout<<"enter Book ISBN   : "<<endl;
			cin>>ISBN;
			cout<<"enter Book Title   : "<<endl;
			cin>>Title;
			cout<<"enter Book Author    : "<<endl;
			cin>>Author;
			cout<<"enter Book Price : "<<endl;
			cin>>Price;
			cout<<"enter Book Publisher : "<<endl;
			cin>>Publisher;
			Book d(ISBN,Title,Author,Price,Publisher);
			addBook(file,d);
			getlength(d);
			cout<<endl<<endl;
			cout<<"Do you want to add another student? [y] Yes [n] No"<<endl;
			cin>>c;
			if(c=='n')
			{
				system("cls");
				startscreen();
				break;
			}
			}
		}
		if(x==2)
		{
			system("cls");
			while(true)
			{
				char ISBN1[30];
				cout<<"write ISBN of the Book which you want to update :"<<endl;
				cin>>ISBN;
				int k=Check(ISBN,file);
				if(k==1)
				{
					cout<<"the Book not found"<<endl;
					break;
				}
				else if(k==0)
				{
				cout<<endl<<endl;
				cout<<"write the new ISBN : "<<endl;
				cin>>ISBN1;
				cout<<"write the new Title: "<<endl;
				cin>>Title;
				cout<<"write the new Author: "<<endl;
				cin>>Author;
				cout<<"write the new Price : "<<endl;
				cin>>Price;
				cout<<"write the new Publisher : "<<endl;
				cin>>Publisher;
				cout<<endl<<endl;
				Book d2(ISBN,Title,Author,Price,Publisher);
				updateBook(file,d,ISBN);
				cout<<"Do you want to update another Book? [y] Yes [n] No"<<endl;
				cin>>c;
				if(c=='n')
				{
					system("cls");
					startscreen();
					break;
				}
				}
			}
		}
		if (x==3)
		{
			system("cls");
			while(true)
			{
			cout<<"write ISBN of the Book who you want to delete : "<<endl;
			cin>>ISBN;
			deleteBook(ISBN,file);
			cout<<endl<<endl;
			cout<<"Do you want to delete another student ? [y] Yes [n] No"<<endl;
			cin>>c;
			if(c=='n')
			{
				system("cls");
				startscreen();
				break;
			}
			}
		}
		if(x==4)
		{
			system("cls");
			while(true)
			{
			cout<<"write ISBN of the Book which you want to search : "<<endl;
			cin>>ISBN;
			searchbyISBN(ISBN,file);
			cout<<endl<<endl;
			cout<<"Do you want to search another Book by ISBN ? [y] Yes [n] No";
			cin>>c;
			if(c=='n')
			{
				system("cls");
				startscreen();
				break;
			}
			}
		}
		if(x==5)
		{
			system("cls");
			while(true)
			{
			cout<<"write Author name of the Book which you want to search : "<<endl;
			cin>>ISBN;
			searchbyISBN(ISBN,file);
			cout<<endl<<endl;
			cout<<"Do you want to search another Book by Author name ? [y] Yes [n] No";
			cin>>c;
			if(c=='n')
			{
				system("cls");
				startscreen();
				break;
			}
			}
		}
		if(x==6)
		{
			file.close();
			exit(0);
		}	
		if(x==7)
		{
			file.close();
			exit(0);
		}
	system("cls");
	startscreen();
}
	return 0;
}
