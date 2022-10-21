//Ï³äêëş÷åííÿ á³áë³îòåê
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<assert.h>
#include<Windows.h>

//Äîäàºìî ôàéëè ç êîäîì äëÿ ìàéñòğà òà ï³äëåãëîãî
#include "Book.h"
#include "Character.h"
#include "IndexFileRecord.h"

//Äîäàºìî ğåñóğñí³ ôàéëè
#define FILE_MODE "rb+"
#define INDEX_FILE "Book.ind"
#define MASTER_FILE "Book.fl"
#define SLAVE_FILE "Character.fl"


//-----------------------------------------------------------------------------------------------//
//
// Ôóíêö³¿ ğîáîòè ç ôàéëàìè
//
//-----------------------------------------------------------------------------------------------//

// Çàïèñè, óïîğÿäêîâàí³ çà êëş÷åì çàïèñó
IndexFileRecord* ReadIndexFile(FILE* index_file, int& records_count)
{
	fseek(index_file, 0, SEEK_END);
	records_count = ftell(index_file) / sizeof(IndexFileRecord);
	fseek(index_file, 0, SEEK_SET);

	IndexFileRecord* irecords = new IndexFileRecord[records_count];
	fread((char*)irecords, sizeof(IndexFileRecord), records_count, index_file);

	assert(!ferror(index_file));

	return irecords;
}

void WriteIndexFile(FILE* index_file, IndexFileRecord* records, int records_count)
{
	index_file = freopen(INDEX_FILE, "wb", index_file);
	fwrite((char*)records, sizeof(IndexFileRecord), records_count, index_file);
	fflush(index_file);
	index_file = freopen(INDEX_FILE, FILE_MODE, index_file);
}

// Á³íàğíèé ïîøóê
int BinarySearchOfTheBook(int book_id, IndexFileRecord* irecords, int records_count)
{
	int low = 0;
	int high = records_count - 1;
	while (low <= high)
	{
		int mid = (low + high) / 2;
		if (irecords[mid].key > book_id)
			high = mid - 1;
		else
			if (irecords[mid].key < book_id)
				low = mid + 1;
			else
			{
				if (irecords[mid].isValid == true) {
					return mid;
				}
				else
				{
					low = mid + 1;
					continue;
				}
			}
	}
	return -1;
}

// Îòğèìóºìî àäğåñó â ³íäåêñíîìó ôàéë³
int GetAddressInIndexFile(int book_id, FILE* index_file)
{
	int records_number;
	IndexFileRecord* irecords = ReadIndexFile(index_file, records_number);

	int address = BinarySearchOfTheBook(book_id, irecords, records_number);
	delete[] irecords;
	return address;
}

// Ôàéë ìàéñòğà
bool CorrectMasterFile(int old_character_address, int new_character_address, FILE* master_file)
{
	fseek(master_file, 0, SEEK_SET);
	int qrecord_address = 0;
	BookRecord qrecord;
	while (!feof(master_file) && !ferror(master_file))
	{
		ReadFromFile(master_file, qrecord);

		if (qrecord.first_character_record_address == old_character_address)
		{
			qrecord.first_character_record_address = new_character_address;
			fseek(master_file, qrecord_address * size_of_book_record, SEEK_SET);
			WriteToFile(qrecord, master_file);
			return true;
		}
		qrecord_address++;
	}
	return false;
}

// Ôàéë ï³äëåãëîãî
bool CorrectSlaveFile(int old_character_address, int new_character_address, FILE* slave_file)
{
	fseek(slave_file, 0, SEEK_SET);
	int lrecord_address = 0;
	CharacterRecord lrecord;
	while (!feof(slave_file) && !ferror(slave_file))
	{
		ReadFromFile(slave_file, lrecord);

		if (lrecord.next_record_adress == old_character_address)
		{
			lrecord.next_record_adress = new_character_address;
			fseek(slave_file, lrecord_address * size_of_character_record, SEEK_SET);
			WriteToFile(lrecord, slave_file);
			return true;
		}
		lrecord_address++;
	}
	return false;
}

// Ôàéë ³íäåêñíèé
bool CorrectIndexFile(int old_qrecord_address, int new_qrecord_address,
	IndexFileRecord* irecords, int irecord_count)
{
	for (int i = 0; i < irecord_count; i++)
	{
		if (irecords[i].address == old_qrecord_address)
		{
			irecords[i].address = new_qrecord_address;
			return true;
		}
	}
	return false;
}

// Ó âèïàäêó ïîìèëêè
BookRecord GetBookRecord(int book_address, FILE* master_file)
{
	if (book_address == -1)
	{
		printf("\nERROR: Element does not exist\n");
		return BookRecord{};
	}

	BookRecord qrecord;
	fseek(master_file, book_address * size_of_book_record, SEEK_SET);
	ReadFromFile(master_file, qrecord);

	return qrecord;
}

// Ó âèïàäêó â³äñóòíîñò³ äàíèõ
CharacterRecord GetCharacterRecord(int character_address, FILE* slave_file)
{
	if (character_address == -1)
	{
		printf("\nERROR: Element does not exist\n");
		return CharacterRecord{};
		;
	}

	CharacterRecord lrecord;
	fseek(slave_file, character_address * size_of_character_record, SEEK_SET);
	ReadFromFile(slave_file, lrecord);

	return lrecord;
}



//-----------------------------------------------------------------------------------------------//
//
//Ôóíêö³¿
//
//-----------------------------------------------------------------------------------------------//
 


// get-m / get-s ------------------------------------------------------------------------------------
int Get_M(int book_id, FILE* index_file)
{
	int index_file_address = GetAddressInIndexFile(book_id, index_file);

	if (index_file_address == -1)
		return -1;

	IndexFileRecord irecord;
	fseek(index_file, index_file_address * sizeof(IndexFileRecord), SEEK_SET);
	fread((char*)&irecord, sizeof(IndexFileRecord), 1, index_file);

	assert(irecord.key == book_id);

	return irecord.address;
}

int Get_S(int character_id, int book_id,
	FILE* index_file, FILE* master_file, FILE* slave_file)
{
	int book_record_address = Get_M(book_id, index_file);
	BookRecord qrecord = GetBookRecord(book_record_address, master_file);

	
	CharacterRecord lrecord{};
	int cur_record_address = qrecord.first_character_record_address;
	int prev_record_address = 0;
	while ((lrecord.character.character_id != character_id || lrecord.isValid == false)
		&& cur_record_address != -1 && !feof(slave_file) && !ferror(slave_file))
	{
		prev_record_address = cur_record_address;
		fseek(slave_file, cur_record_address * size_of_character_record, SEEK_SET);
		ReadFromFile(slave_file, lrecord);
		cur_record_address = lrecord.next_record_adress;
	}

	if (lrecord.character.character_id == character_id)
		return prev_record_address;
	else
		return -1;
}

//del-m / del-s -------------------------------------------------------------------------------------

void Del_M(int book_id, FILE* index_file, FILE* master_file, FILE* slave_file)
{
	int book_address = Get_M(book_id, index_file);

	if (book_address == -1) return;

	BookRecord qrecord;
	fseek(master_file, book_address * size_of_book_record, SEEK_SET);
	ReadFromFile(master_file, qrecord);

	qrecord.isValid = false;
	fseek(master_file, book_address * size_of_book_record, SEEK_SET);
	WriteToFile(qrecord, master_file);

	int index_file_record_address = GetAddressInIndexFile(book_id, index_file);
	IndexFileRecord irecord;
	fseek(index_file, index_file_record_address * sizeof(IndexFileRecord), SEEK_SET);
	fread((char*)&irecord, sizeof(IndexFileRecord), 1, index_file);

	irecord.isValid = false;
	fseek(index_file, index_file_record_address * sizeof(IndexFileRecord), SEEK_SET);
	fwrite((char*)&irecord, sizeof(IndexFileRecord), 1, index_file);


	CharacterRecord lrecord;
	int cur_lrecord_address = qrecord.first_character_record_address;
	while (cur_lrecord_address != -1)
	{
		fseek(slave_file, cur_lrecord_address * size_of_character_record, SEEK_SET);
		ReadFromFile(slave_file, lrecord);

		lrecord.isValid = false;
		fseek(slave_file, cur_lrecord_address * size_of_character_record, SEEK_SET);
		WriteToFile(lrecord, slave_file);
		cur_lrecord_address = lrecord.next_record_adress;
	}
}

void Del_S(int character_id, int book_id,
	FILE* index_file, FILE* master_file, FILE* slave_file)
{
	int character_address = Get_S(character_id, book_id,
		index_file, master_file, slave_file);

	if (character_address == -1) return;

	CharacterRecord lrecord;
	fseek(slave_file, character_address * size_of_character_record, SEEK_SET);
	ReadFromFile(slave_file, lrecord);

	lrecord.isValid = false;
	fseek(slave_file, character_address * size_of_character_record, SEEK_SET);
	WriteToFile(lrecord, slave_file);
}

//update-m / update-s ---------------------------------------------------------------------------------

void Update_M(Book book,
	FILE* index_file, FILE* master_file)
{
	int book_address = Get_M(book.book_id, index_file);
	if (book_address == -1)
		return;
	BookRecord qrecord = GetBookRecord(book_address, master_file);

	qrecord.book = book;
	fseek(master_file, book_address * size_of_book_record, SEEK_SET);
	WriteToFile(qrecord, master_file);
}

void Update_S(Character character, int book_id,
	FILE* index_file, FILE* master_file, FILE* slave_file)
{
	int character_address = Get_S(character.character_id, book_id,
		index_file, master_file, slave_file);
	if (character_address == -1)
		return;
	CharacterRecord lrecord = GetCharacterRecord(character_address, slave_file);

	lrecord.character = character;
	fseek(slave_file, character_address * size_of_character_record, SEEK_SET);
	WriteToFile(lrecord, slave_file);
}

//garbage-collector -----------------------------------------------------------------------------------

void GarbageCollector(FILE* index_file, FILE* master_file, FILE* slave_file,
	const char* index_filename, const char* master_filename, const char* slave_filename)
{

	int non_valid_irecords_count;
	IndexFileRecord* irecords = ReadIndexFile(index_file, non_valid_irecords_count);
	IndexFileRecord* valid_irecords = new IndexFileRecord[non_valid_irecords_count];
	int cur_valid_irecord = 0;
	for (int i = 0; i < non_valid_irecords_count; i++)
	{
		if (irecords[i].isValid)
			valid_irecords[cur_valid_irecord++] = irecords[i];
	}
	int valid_irecords_count = cur_valid_irecord;
	delete irecords;



	FILE* temp = fopen("temp.fl", "wb");
	CharacterRecord lrecord;
	int old_character_address = 0;
	int	new_character_address = 0;
	fseek(slave_file, 0, SEEK_SET);
	int pos = getc(slave_file);
	while (!feof(slave_file) && !ferror(slave_file) && pos != EOF)
	{
		fseek(slave_file, -1, SEEK_CUR);

		ReadFromFile(slave_file, lrecord);

		if (lrecord.isValid)
		{
			WriteToFile(lrecord, temp);

			if (old_character_address != new_character_address)
			{
				if (!CorrectMasterFile(old_character_address, new_character_address,
					master_file))
				{
					CorrectSlaveFile(old_character_address, new_character_address,
						slave_file);
				}
			}

			new_character_address++;
		}

		old_character_address++;
		pos = getc(slave_file);
	}


	fflush(slave_file);
	fclose(slave_file);
	fflush(temp);
	fclose(temp);
	remove(slave_filename);
	rename("temp.fl", slave_filename);
	slave_file = fopen(slave_filename, FILE_MODE);



	BookRecord qrecord;
	temp = fopen("temp.fl", "wb");
	int old_qrecord_address = 0;
	int new_qrecord_address = 0;
	fseek(master_file, 0, SEEK_SET);
	pos = getc(master_file);
	while (!feof(master_file) && !ferror(master_file) && pos != EOF)
	{
		fseek(master_file, -1, SEEK_CUR);

		ReadFromFile(master_file, qrecord);

		if (qrecord.isValid)
		{
			WriteToFile(qrecord, temp);

			if (old_qrecord_address != new_qrecord_address)
				CorrectIndexFile(old_qrecord_address, new_qrecord_address,
					valid_irecords, valid_irecords_count);

			new_qrecord_address++;
		}

		old_qrecord_address++;
		pos = getc(master_file);
	}


	
	fclose(master_file);
	fflush(temp);
	fclose(temp);
	remove(master_filename);
	rename("temp.fl", master_filename);
	master_file = fopen(master_filename, FILE_MODE);


	// Îíîâèìî ³íäåêñíèé ôàéë
	WriteIndexFile(index_file, valid_irecords, valid_irecords_count);
	delete[] valid_irecords;
}

// insert-m / insert-s ---------------------------------------------------------------------------------

void Insert_M(Book book, FILE* index_file, FILE* master_file)
{

	int books_count;
	IndexFileRecord* irecords = ReadIndexFile(index_file, books_count);
	index_file = freopen(INDEX_FILE, "wb", index_file);

	if (books_count == 0)
	{
		IndexFileRecord new_irecord{ book.book_id, 0, true };
		fwrite((char*)&new_irecord, sizeof(IndexFileRecord), 1, index_file);
	}
	else
	{
		for (int i = 0; i < books_count; i++)
		{
			if (irecords[i].key > book.book_id)
			{
				IndexFileRecord new_irecord{ book.book_id, books_count, true };
				fwrite((char*)&new_irecord, sizeof(IndexFileRecord), 1, index_file);
				while (i < books_count)
				{
					fwrite((char*)&irecords[i++], sizeof(IndexFileRecord), 1, index_file);
				}
				break;
			}
			else
			{
				fwrite((char*)&irecords[i], sizeof(IndexFileRecord), 1, index_file);

				if (i + 1 == books_count)
				{
					IndexFileRecord new_irecord{ book.book_id, books_count, true };
					fwrite((char*)&new_irecord, sizeof(IndexFileRecord), 1, index_file);
				}
			}
		}
	}
	delete[] irecords;
	fflush(index_file);
	index_file = freopen(INDEX_FILE, FILE_MODE, index_file);


	master_file = freopen(MASTER_FILE, "ab", master_file);
	BookRecord qrecord{ book, -1, true };
	WriteToFile(qrecord, master_file);
	fflush(master_file);
	master_file = freopen(MASTER_FILE, FILE_MODE, master_file);
}

void Insert_S(Character character, int book_id,
	FILE* index_file, FILE* master_file, FILE* slave_file)
{
	int book_address = Get_M(book_id, index_file);
	if (book_address == -1)
		return;

	BookRecord qrecord;
	fseek(master_file, book_address * size_of_book_record, SEEK_SET);
	ReadFromFile(master_file, qrecord);

	CharacterRecord lrecord{ character, qrecord.first_character_record_address, true };
	slave_file = freopen(SLAVE_FILE, "ab", slave_file);
	WriteToFile(lrecord, slave_file);
	slave_file = freopen(SLAVE_FILE, FILE_MODE, slave_file);

	fseek(slave_file, 0, SEEK_END);
	int slave_records_count = ftell(slave_file) / size_of_character_record;

	qrecord.first_character_record_address = slave_records_count - 1;
	fseek(master_file, book_address * size_of_book_record, SEEK_SET);
	WriteToFile(qrecord, master_file);
}



//-----------------------------------------------------------------------------------------------//
//
// Ôóíêö³¿ ğîáîòè ç êîğèñòóâà÷åì
//
//-----------------------------------------------------------------------------------------------//



// Äîäàòè Êíèãó
void AddBook(FILE* index_file, FILE* master_file)
{
	system("cls");
	Book book;
	printf("Type ID:     ");
	scanf("%i", &book.book_id);
	printf("Type Book name:  ");
	scanf("%s", book.name);
	printf("Type Autor:    ");
	scanf("%s", &book.autor);
	printf("Type Date:    ");
	scanf("%i", &book.date);
	printf("Type Number of pages:    ");
	scanf("%i", &book.number_of_pages);
	printf("Type Notes:    ");
	scanf("%s", &book.notes);
	

	Insert_M(book, index_file, master_file);
}

// Îòğèìàòè Êíèãó
void GetBook(FILE* index_file, FILE* master_file, FILE* slave_file)
{
	system("cls");

	int book_id;
	printf("Type ID:	");
	scanf("%i", &book_id);

	int book_address = Get_M(book_id, index_file);
	BookRecord qrecord = GetBookRecord(book_address, master_file);
	PrintBook(qrecord.book);
}

// Âèäàëèòè Êíèãó
void RemoveBook(FILE* index_file, FILE* master_file, FILE* slave_file)
{
	system("cls");

	int book_id;
	printf("Type ID:	");
	scanf("%i", &book_id);

	Del_M(book_id, index_file, master_file, slave_file);
}

// Çì³íèòè Êíèãó
void UpdateBook(FILE* index_file, FILE* master_file)
{
	system("cls");

	int book_id;
	printf("Type ID:	");
	scanf("%i", &book_id);

	int book_address = Get_M(book_id, index_file);
	BookRecord qrecord = GetBookRecord(book_address, master_file);

	enum EditingMenu
	{
		kBookName = 1,
		kAutor,
		kDate,
		kNumberOfPages,
		kNotes,
		kUpdate = 0
	};

	while (true)
	{
		printf("\nWhat to edit:\n");
		printf("\n	1 - Book name");
		printf("\n	2 - Autor");
		printf("\n	3 - Date");
		printf("\n	4 - Number of pages");
		printf("\n	5 - Notes");
		printf("\n	0 - Update\n");

		EditingMenu ans;
		scanf("%i", &ans);
		switch (ans)
		{
		case kBookName:
			printf("\nEnter new Book name:	");
			scanf("%s", qrecord.book.name);
			break;
		case kAutor:
			printf("Enter new Autor:	");
			scanf("%s", &qrecord.book.autor);
			break;
		case kDate:
			printf("Enter new Date:	");
			scanf("%i", &qrecord.book.date);
			break;
		case kNumberOfPages:
			printf("Enter new Number of pages:	");
			scanf("%i", &qrecord.book.number_of_pages);
			break;
		case kNotes:
			printf("Enter new Note:	");
			scanf("%s", &qrecord.book.notes);
			break;
		
		case kUpdate:
			Update_M(qrecord.book, index_file, master_file);
			return;
		default:
			continue;
		}
	}
}

// Äîäàòè Ïåğñîíàæà
void AddCharacter(FILE* index_file, FILE* master_file, FILE* slave_file)
{
	system("cls");

	int book_id;
	printf("Type Book ID:   ");
	scanf("%i", &book_id);

	Character character;
	printf("Type Character ID:  ");
	scanf("%i", &character.character_id);
	printf("Type Name:      ");
	scanf("%s", character.name);
	printf("Type Role (main, second, episodic):        ");
	scanf("%s", &character.role);


	

	Insert_S(character, book_id, index_file, master_file, slave_file);
}

// Îòğèìàòè Ïåğñîíàæà
void GetCharacter(FILE* index_file, FILE* master_file, FILE* slave_file)
{
	system("cls");

	int book_id;
	printf("Type Book ID:	");
	scanf("%i", &book_id);

	int character_id;
	printf("Type character ID:	");
	scanf("%i", &character_id);

	int character_address = Get_S(character_id, book_id,
		index_file, master_file, slave_file);
	CharacterRecord lrecord = GetCharacterRecord(character_address, slave_file);
	PrintCharacter(lrecord.character);
}

// Âèäàëèòè Ïåğñîíàæà
void RemoveCharacter(FILE* index_file, FILE* master_file, FILE* slave_file)
{
	system("cls");

	int book_id;
	printf("Type Book ID:	");
	scanf("%i", &book_id);

	int character_id;
	printf("Type character ID:	");
	scanf("%i", &character_id);

	Del_S(character_id, book_id,
		index_file, master_file, slave_file);
}

// Çì³íèòè Ïåğñîíàæà
void UpdateCharacter(FILE* index_file, FILE* master_file, FILE* slave_file)
{
	system("cls");

	int book_id;
	printf("Type Book ID:	");
	scanf("%i", &book_id);

	int character_id;
	printf("Type Character ID:	");
	scanf("%i", &character_id);

	int character_address = Get_S(character_id, book_id,
		index_file, master_file, slave_file);
	CharacterRecord lrecord = GetCharacterRecord(character_address, slave_file);

	enum EditingMenu
	{
		kCharacterName = 1,
		kRole,
		kUpdate = 0
	};


	while (true)
	{
		printf("\nWhat to edit:\n");
		printf("\n	1 - Name");
		printf("\n	2 - Role");
	
		
		printf("\n	0 - Update\n");

		EditingMenu ans;
		scanf("%i", &ans);
		switch (ans)
		{
		case kCharacterName:
			printf("\nEnter new Character name:	");
			scanf("%s", lrecord.character.name);
			break;
		case kRole:
			printf("\nEnter new Role (main, second, episodic):	");
			scanf("%s", &lrecord.character.role);
			break;
		
		
		case kUpdate:
			Update_S(lrecord.character, book_id,
				index_file, master_file, slave_file);
			return;
		default:
			continue;
		}
	}
}

// Âèâåñòè ³íäåêñíèé ôàéë
void PrintIndexFile(FILE* index_file)
{
	fseek(index_file, 0, SEEK_SET);
	IndexFileRecord irecord;
	printf("\n_____________________________________________\n");
	printf("Key / Address in master file / Validity\n\n"); // ID ğåìîíòíèêà / Éîãî øëÿõ / Íàÿâí³ñòü
	printf("\n_____________________________________________\n");
	int pos = getc(index_file);
	while (!feof(index_file) && !ferror(index_file) && pos != EOF)
	{
		fseek(index_file, -1, SEEK_CUR);

		fread((char*)&irecord, sizeof(IndexFileRecord), 1, index_file);
		char* true_false = new char[7];
		irecord.isValid ? strcpy(true_false, "True") : strcpy(true_false, "False");
		printf("\n%i           %i               %s\n", irecord.key, irecord.address, true_false);

		pos = getc(index_file);
	}
	printf("\n_____________________________________________\n");
}

// Âèâåñòè ôàéë ìàéñòğà
void PrintMasterFile(FILE* master_file)
{
	fseek(master_file, 0, SEEK_SET);
	BookRecord qrecord;
	printf("\n_____________________________________________\n");
	printf("\n\n\n Book ID / Address of the first slave record / Validity\n\n"); // ID ğåìîíòíèêà / Éîãî øëÿõ / Íàÿâí³ñòü
	printf("\n_____________________________________________\n");
	int pos = getc(master_file);
	while (!feof(master_file) && !ferror(master_file) && pos != EOF)
	{
		fseek(master_file, -1, SEEK_CUR);

		ReadFromFile(master_file, qrecord);
		char* true_false = new char[7];
		qrecord.isValid ? strcpy(true_false, "True") : strcpy(true_false, "False");
		printf("\n %i                %i                      %s\n",
			qrecord.book.book_id, qrecord.first_character_record_address, true_false);

		pos = getc(master_file);
	}
	printf("\n_____________________________________________\n");
}

// Âèâåñòè ôàéë ï³äëåãëîãî
void PrintSlaveFile(FILE* slave_file)
{
	fseek(slave_file, 0, SEEK_SET);
	CharacterRecord lrecord;
	printf("\n_____________________________________________\n");
	printf("\n\n\n Character ID / Address of the next slave record / Validity\n\n"); // ID ğåìîíòíèêà / Éîãî øëÿõ / Íàÿâí³ñòü
	printf("\n_____________________________________________\n");
	int pos = getc(slave_file);
	while (!feof(slave_file) && !ferror(slave_file) && pos != EOF)
	{
		fseek(slave_file, -1, SEEK_CUR);

		ReadFromFile(slave_file, lrecord);
		char* true_false = new char[7];
		lrecord.isValid ? strcpy(true_false, "True") : strcpy(true_false, "False");
		printf("\n%i                %i                          %s\n",
			lrecord.character.character_id, lrecord.next_record_adress, true_false);

		pos = getc(slave_file);
	}
	printf("\n_____________________________________________\n");
}


//-----------------------------------------------------------------------------------------------//
// 
// Ìåíşøêà
//
//-----------------------------------------------------------------------------------------------//



void Menu(FILE* index_file, FILE* master_file, FILE* slave_file)
{
	enum MainMenu
	{
		kAddBook = 1,
		kGetBook,
		kRemoveBook,
		kUpdateBook,
		kAddCharacter,
		kGetCharacter,
		kRemoveCharacter,
		kUpdateCharacter,
		kCleanUpGarbage,
		kPrintIndexFile,
		kPrintMasterFile,
		kPrintSlaveFile,
		kExit = 0
	};
	while (true) {
		printf("\n	---------------Interaction With Book---------------\n");
		printf("\n	1 - Add Book\n");
		printf("	2 - Get Book\n");
		printf("	3 - Remove Book\n");
		printf("	4 - Update Book\n");
		printf("\n	---------------Interaction With Book---------------\n");
		printf("	5 - Add Character\n");
		printf("	6 - Get Character\n");
		printf("	7 - Remove Character\n");
		printf("	8 - Update Character\n");
		printf("\n	---------------Interaction With Garbage-----------------\n");
		printf("	9 - Clean up the garbage\n");
		printf("\n	---------------Interaction With Files-------------------\n");
		printf("	10 - Print index file\n");
		printf("	11 - Print master file\n");
		printf("	12 - Print slave file\n");
		printf("\n	---------------Exid-------------------------------------\n");
		printf("\n         0 - Exit\n");
		printf("\n	-------------------------------------------\n");

		MainMenu ans;
		scanf("%i", &ans);
		switch (ans)
		{
		case kAddBook:
			AddBook(index_file, master_file);
			break;
		case kGetBook:
			GetBook(index_file, master_file, slave_file);
			break;
		case kRemoveBook:
			RemoveBook(index_file, master_file, slave_file);
			break;
		case kUpdateBook:
			UpdateBook(index_file, master_file);
			break;
		case kAddCharacter:
			AddCharacter(index_file, master_file, slave_file);
			break;
		case kGetCharacter:
			GetCharacter(index_file, master_file, slave_file);
			break;
		case kRemoveCharacter:
			RemoveCharacter(index_file, master_file, slave_file);
			break;
		case kUpdateCharacter:
			UpdateCharacter(index_file, master_file, slave_file);
			break;
		case kCleanUpGarbage:
			GarbageCollector(index_file, master_file, slave_file,
				INDEX_FILE, MASTER_FILE, SLAVE_FILE);
			break;
		case kPrintIndexFile:
			PrintIndexFile(index_file);
			break;
		case kPrintMasterFile:
			PrintMasterFile(master_file);
			break;
		case kPrintSlaveFile:
			PrintSlaveFile(slave_file);
			break;
		case kExit:
			fclose(index_file);
			fclose(master_file);
			fclose(slave_file);
			exit(0);
			break;
		default:
			continue;
		}
	}
}


//Ğåñóğñí³ ôàéëè
int main()
{
	FILE* index_file = fopen("Book.ind", FILE_MODE);
	FILE* master_file = fopen("Book.fl", FILE_MODE);
	FILE* slave_file = fopen("Character.fl", FILE_MODE);

	Menu(index_file, master_file, slave_file);

}
