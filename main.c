#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char Teams[100][100];
char Projects[100][100];
char Employees[100][100];
int nTeams = 0;
int nProjects = 0;
int nEmployees = 0;

void cls() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#endif
}

void flush() {
	int c;
	do c = getchar();
	while (c != EOF && c != '\n');
}

void initialize() {
	printf("Initializing...");
	for (size_t i = 0; i < 100; i++)
		for (size_t j = 0; j < 100; j++) {
			Teams[i][j] = '\0';
			Projects[i][j] = '\0';
			Employees[i][j] = '\0';
		}

	cls();
}

void createProjectTeam() {
	printf("Team_Name Project_Name Manager Member_1 Member_2 Member_3");
	printf("\n\nEnter data in above format | ");

	char str[255];
	char delim[] = " ";

	flush();
	fgets(str, 255, stdin);

	// add team
	char *ptr = strtok(str, delim);
	strcpy(Teams[nTeams++], ptr);

	// add projects
	ptr = strtok(NULL, delim);
	strcpy(Projects[nProjects++], ptr);

	// add employees
	for (size_t i = 0; i < 4; i++) {
		ptr = strtok(NULL, delim);
		strcpy(Employees[nEmployees++], ptr);
	}

	printf("\n\n%s %s is created", Projects[nProjects - 1], Teams[nTeams - 1]);

	getchar();
	cls();
}

void meetingRequestMenu() {
	printf("1. Single input");
	printf("\n2. Batch input");
	printf("\n3. Meeting Attendance");

	int choice;

	do {
		printf("\n\nEnter an option | ");
		scanf("%d", &choice);
	} while (choice < 0 || choice > 3);

	cls();

	switch (choice) {
		case 1:
			//
			break;

		case 2:
			//
			break;

		case 3:
			//
			break;
	}
}

void printMeetingSchedule() {
	printf("1. FCFS");
	printf("\n2. Priority");

	int choice;

	do {
		printf("\n\nEnter an option | ");
		scanf("%d", &choice);
	} while (choice < 0 || choice > 2);

	cls();

	switch (choice) {
		case 1:
			//
			break;

		case 2:
			//
			break;

		case 3:
			//
			break;
	}
}

void mainMenu() {
	printf(" ~~ WELCOME TO PolyStar ~~");
	printf("\n\n1. Create Project Team");
	printf("\n2. Project Meeting Request");
	printf("\n3. Print Meeting Schedule");
	printf("\n4. Exit");

	int choice;

	do {
		printf("\n\nEnter an option | ");
		scanf("%d", &choice);
	} while (choice < 0 || choice > 4);

	cls();

	switch (choice) {
		case 1:
			createProjectTeam();
			break;

		case 2:
			meetingRequestMenu();
			break;

		case 3:
			printMeetingSchedule();
			break;

		case 4:
			exit(0);
	}
}

int main() {
	initialize();
	while (1) mainMenu();
}