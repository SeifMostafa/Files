#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include<vector>
#include <algorithm>
using namespace std;
char LOCACTION[]="/home/seifmostafa/Documents/fci/summer2016/files/assignments/f.txt";
short int HEADER = -1,COUNTER=0 ,OLD_COUNTER=0;
vector<pair<short int,short int > >avail_list;
struct Book
{
    char ISBN[30];
    char Title[50];
    char Author[30];
    float Price;
    char publisher[50];
};
istream& operator>>(istream & in, Book &b)
{
    cout<<"Enter Book's ISBN: ";
    cin.ignore();
    in.getline(b.ISBN, 30);
    cout<<"Enter Book's Title: ";
    in.getline(b.Title, 50);
    cout<<"Enter Book's Author: ";
    in.getline(b.Author, 50);
    cout<<"Enter Book's Price: ";
    in>>b.Price;
    cin.ignore();
    cout<<"Enter Book's publisher: ";
    in.getline(b.publisher, 50);
    return in;
}
ostream& operator<<(ostream & os, Book &b)
{
    os << b.ISBN << endl;
    os << b.Title << endl;
    os << b.Author << endl;
    os << b.Price << endl;
    os << b.publisher << endl;
    return os;

}
bool check(int x,int y)
{
    if(x==y)
        return 1;
    else
        return 0;
}
void addBook(ofstream&F)
{
    Book b;
    cin>>b;
    short length = strlen(b.ISBN)+strlen(b.Title)+strlen(b.Author)+strlen(b.publisher)+sizeof(float)+4;
    F.write((char*)&length,sizeof(short));
    F.write((char*)&b.ISBN,strlen(b.ISBN));
    F.write((char*)"|",sizeof(char));
    F.write((char*)&b.Title,strlen(b.Title));
    F.write((char*)"|",sizeof(char));
    F.write((char*)&b.Author,strlen(b.Author));
    F.write((char*)"|",sizeof(char));
    F.write((char*)&b.Price,sizeof(float));
    F.write((char*)"|",sizeof(char));
    F.write((char*)&b.publisher,strlen(b.publisher));
    F.close();
}
void updateBook()
{
    string isbn;
    getline(std::cin,isbn);
}
int SearchBookByISBN(ifstream&F)
{
    int off,len;
    string isbn,tested;
    cin>>isbn;
    F.seekg(0,ios::beg);
    F.read((char*)len,sizeof(short));
    while(F.get()!='|')
    tested+=F.get();
}
void SearchBookByAuthor()
{
    char author[50];
}
void DeleteBook()
{
    char isbn[30];
}
void CompactFile()
{

}
void VisulaiseFile()
{

}
void DisplayBook()
{

}
int main()
{
   int cmd;
    ofstream OF;
    ifstream IF;
    do
    {
        cout<<"\t\tBooks Management System\n\n";
        cout<<"1)Add New Book\n2)Update Book\n3)Delete Book\n4)Search for A Book By ISBN\n";
        cout<<"5)Search for A Book By Author\n6)Compact File\n7)VisualizeFile\n8)Exit\n\nPlease Enter Your Choice :";
        cin>>cmd;
        switch(cmd)
        {
        case 1:
        {
            OF.open(LOCACTION,ios::out|ios::binary|ios::app);
            addBook(OF);
            IF.open(LOCACTION);
            string tested="";
             while(IF.get()!='|')
            tested+=IF.get();
            cout<<tested;
            break;
        }
        case 2:
        {
            updateBook();
            break;
        }
        case 3:
        {
            DeleteBook();
            break;
        }
        case 4:
        {


        }
        case 5:
        {
            SearchBookByAuthor();
            break;
        }
        case 6:
        {
            CompactFile();
            break;
        }
        case 7:
        {
            VisulaiseFile();
            break;
        }
        default:
        {
            break;
        }

        }
    }
    while(cmd!=8);
    return 0;
}


