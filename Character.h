// ϳ��������� �������
// Connection of libraries
#ifndef CHARACTER_H_
#define CHARACTER_H_
#include <stdio.h>

// ��������� ��� ���������� �����
// Structure for saving data
struct Character
{
	int character_id;
	char name[40];
	char role[40];
};

// ��������� ��� ������ �����
// Structure for recording data
struct CharacterRecord
{
	Character character;
	int next_record_adress;
	bool isValid;
};

// ϳ�������� ������
// Calculate the size
const int size_of_character_record = sizeof(int) + 40 * sizeof(char) + 40 * sizeof(char)
+ sizeof(int) + sizeof(bool);

// ������ �� �����
// Write to the file
void WriteToFile(CharacterRecord lrecord, FILE* file)
{
	fwrite((char*)&lrecord.character.character_id, sizeof(int), 1, file);
	fwrite(lrecord.character.name, 40 * sizeof(char), 1, file);
	fwrite(lrecord.character.role, 40 * sizeof(char), 1, file);

	fwrite((char*)&lrecord.next_record_adress, sizeof(int), 1, file);
	fwrite((char*)&lrecord.isValid, sizeof(bool), 1, file);
}

// ������ � �����
// Read from the file
void ReadFromFile(FILE* file, CharacterRecord& lrecord)
{
	fread((char*)&lrecord.character.character_id, sizeof(int), 1, file);
	fread(lrecord.character.name, 40 * sizeof(char), 1, file);
	fread(lrecord.character.role, 40 * sizeof(char), 1, file);

	fread((char*)&lrecord.next_record_adress, sizeof(int), 1, file);
	fread((char*)&lrecord.isValid, sizeof(bool), 1, file);
}

// ������ ������ � ����������� � ������
// Output data from the user in the console
void PrintCharacter(Character character)
{
	printf("---------------------------------------------------\n");
	printf("Character ID:	%i\n", character.character_id);
	printf("Character name:		%s\n", character.name);
	printf("Role (main, second, episodic):		%s\n", character.role);

	printf("---------------------------------------------------\n");
}
#endif 