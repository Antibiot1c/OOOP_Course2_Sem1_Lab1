// Підключення бібліотек
// Connection of libraries
#ifndef BOOK_H_
#define BOOK_H_
#include <stdio.h>

// Структура для збереження даних
// Structure for saving data
struct Book
{
	int book_id;
	char name[40];
	char autor[40];
	int date;
	int number_of_pages;
	char notes[40];

};

// Структура для запису даних
// Structure for recording data
struct BookRecord
{
	Book book;
	int first_character_record_address;
	bool isValid;
};

// Підрахунок розміру
// Calculate the size
const int size_of_book_record = (sizeof(int) + 40 * sizeof(char) + 40 * sizeof(char) + sizeof(int) + sizeof(int) + 40 * sizeof(char) +
	  sizeof(int) + sizeof(bool));

// Пишемо до файла
// Write to the file
void WriteToFile(BookRecord qrecord, FILE* file)
{
	fwrite((char*)&qrecord.book.book_id, sizeof(int), 1, file);
	fwrite(qrecord.book.name, 40 * sizeof(char), 1, file);
	fwrite(qrecord.book.autor, 40 * sizeof(char), 1, file);
	fwrite((char*)&qrecord.book.date, sizeof(int), 1, file);
	fwrite((char*)&qrecord.book.number_of_pages, sizeof(int), 1, file);
	fwrite(qrecord.book.notes, 40 * sizeof(char), 1, file);

	fwrite((char*)&qrecord.first_character_record_address, sizeof(int), 1, file);
	fwrite((char*)&qrecord.isValid, sizeof(bool), 1, file);
}

// Читаємо з файла
// Read from the file
void  ReadFromFile(FILE* file, BookRecord& qrecord)
{
	fread((char*)&qrecord.book.book_id, sizeof(int), 1, file);
	fread(qrecord.book.name, 40 * sizeof(char), 1, file);
	fread(qrecord.book.autor, 40 * sizeof(char), 1, file);
	fread((char*)&qrecord.book.date, sizeof(int), 1, file);
	fread((char*)&qrecord.book.number_of_pages, sizeof(int), 1, file);
	fread(qrecord.book.notes, 40 * sizeof(char), 1, file);
	
	fread((char*)&qrecord.first_character_record_address, sizeof(int), 1, file);
	fread((char*)&qrecord.isValid, sizeof(bool), 1, file);
}

// Вихідні данні у користувача в консолі
// Output data from the user in the console
void PrintBook(Book book)
{
	printf("---------------------------------------------------\n");
	printf("Book ID:	%i\n", book.book_id);
	printf("Book name:		%s\n", book.name);
	printf("Autor:		%s\n", book.autor);
	printf("Date publication:		%i\n", book.date);
	printf("Number of pages:		%i\n", book.number_of_pages);
	printf("Notes:		%s\n", book.notes);


	printf("---------------------------------------------------\n");
}
#endif 