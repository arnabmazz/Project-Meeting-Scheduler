#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char Teams[100][100];
char Projects[100][100];
char Employees[100][100];
int nTeams = 0;
int nProjects = 0;
int nEmployees = 0;

struct Team {
	char name[50];
	char date[11];
	char time[6];
	char str_dur_hours[3];

	int year;
	int month;
	int day;

	int hours;
	int minutes;

	int dur_hours;
};

struct Team stTeams[255];
int nStTeams = 0;

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
	for (size_t i = 0; i < 100; i++)
		for (size_t j = 0; j < 100; j++) {
			Teams[i][j] = '\0';
			Projects[i][j] = '\0';
			Employees[i][j] = '\0';
		}
}

void inputMeetingRequest() {
	printf("Team_Name yyyy-mm-dd hh:mm hours");
	printf("\n\nEnter data in above format | ");

	char str[255];
	char delim[] = " -:";

	flush();
	fgets(str, 255, stdin);

	// going to parse input and add data into struct Team

	char date[11];
	char time[6];
	char dateDelim[] = "-";
	char timeDelim[] = ":";

	// add name
	char *ptr = strtok(str, delim);
	strcpy(stTeams[nStTeams].name, ptr);

	// add year
	ptr = strtok(NULL, delim);
	stTeams[nStTeams].year = atoi(ptr);
	strcpy(date, ptr);

	// add month
	ptr = strtok(NULL, delim);
	stTeams[nStTeams].month = atoi(ptr);
	strcat(date, dateDelim);
	strcat(date, ptr);

	// add day
	ptr = strtok(NULL, delim);
	stTeams[nStTeams].day = atoi(ptr);
	strcat(date, dateDelim);
	strcat(date, ptr);

	// add hours
	ptr = strtok(NULL, delim);
	stTeams[nStTeams].hours = atoi(ptr);
	strcpy(time, ptr);

	// add minutes
	ptr = strtok(NULL, delim);
	stTeams[nStTeams].minutes = atoi(ptr);
	strcat(time, timeDelim);
	strcat(time, ptr);

	// add meeting duration hours
	ptr = strtok(NULL, delim);
	strcpy(stTeams[nStTeams].str_dur_hours, ptr);
	stTeams[nStTeams].dur_hours = atoi(ptr);

	// add strings time and date
	strcpy(stTeams[nStTeams].time, time);
	strcpy(stTeams[nStTeams].date, date);

	// increment index
	nStTeams++;

	printf("\nMeeting request received.\n");

	getchar();
	// Team_A 2022-04-24 09:40 2
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
			inputMeetingRequest();
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
	cls();
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