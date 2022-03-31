#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct Team {
	char team_name[20];
	char project_name[20];
	char manager[20];
	char members[3][20];
};

struct Meeting {
	char team_name[20];
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

struct Meeting Meetings[255];
int nMeetings = 0;

struct Team Teams[255];
int nTeams = 0;

//

void cls() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#endif
}

void flush() {
	// a function to make sure there isn't any unread data
	// in stdin. (unread \n causes fgets to skip)
	int c;
	do c = getchar();
	while (c != EOF && c != '\n');
}

void parseMeetingRequest(char *buffer) {
	// parse input and add data into struct Meeting

	char date[11];
	char time[6];

	char delim[] = " -:";
	char dateDelim[] = "-";
	char timeDelim[] = ":";

	// add team name
	char *ptr = strtok(buffer, delim);
	strcpy(Meetings[nMeetings].team_name, ptr);

	// add year
	ptr = strtok(NULL, delim);
	Meetings[nMeetings].year = atoi(ptr);
	strcpy(date, ptr);

	// add month
	ptr = strtok(NULL, delim);
	Meetings[nMeetings].month = atoi(ptr);
	strcat(date, dateDelim);
	strcat(date, ptr);

	// add day
	ptr = strtok(NULL, delim);
	Meetings[nMeetings].day = atoi(ptr);
	strcat(date, dateDelim);
	strcat(date, ptr);

	// add hours
	ptr = strtok(NULL, delim);
	Meetings[nMeetings].hours = atoi(ptr);
	strcpy(time, ptr);

	// add minutes
	ptr = strtok(NULL, delim);
	Meetings[nMeetings].minutes = atoi(ptr);
	strcat(time, timeDelim);
	strcat(time, ptr);

	// add meeting duration hours
	ptr = strtok(NULL, delim);
	strcpy(Meetings[nMeetings].str_dur_hours, ptr);
	Meetings[nMeetings].dur_hours = atoi(ptr);

	// add strings time and date
	strcpy(Meetings[nMeetings].time, time);
	strcpy(Meetings[nMeetings].date, date);

	// increment index
	nMeetings++;
}

void batchMeetingRequest() {
	printf("Enter file name | ");

	char filename[20];

	flush();
	fgets(filename, 20, stdin);

	// remove trailing newline char (replace it with null terminator)
	filename[strcspn(filename, "\n")] = 0;

	// return if file doesnt exist
	if(access(filename, F_OK) != 0) {
		printf("\nCannot find file, make sure its in current dir.");
		getchar();
		cls();
		return;
	}

	FILE *file;
	int bufferLen = 255;
	char buffer[bufferLen];

	file = fopen(filename, "r");

	int i = 0;

	while(fgets(buffer, bufferLen, file)) {
		buffer[strcspn(buffer, "\n")] = 0;
		parseMeetingRequest(buffer);
		i++;
	}

	fclose(file);

	printf("\n%d meeting requests received.\n", i);

	getchar();
	cls();
}

void inputMeetingRequest() {
	printf("Team_Name yyyy-mm-dd hh:mm hours");
	printf("\n\nEnter data in above format | ");

	char str[255];

	flush();
	fgets(str, 255, stdin);

	parseMeetingRequest(str);

	printf("\nMeeting request received.\n");

	getchar();
	cls();
	// Team_A 2022-04-24 09:40 2
}

void createProjectTeam() {
	printf("Team_Name Project_Name Manager Member_1 Member_2 Member_3");
	printf("\n\nEnter data in above format | ");

	char str[255];
	char delim[] = " ";

	flush();
	fgets(str, 255, stdin);

	// add team name
	char *ptr = strtok(str, delim);
	strcpy(Teams[nTeams].team_name, ptr);

	// add project name
	ptr = strtok(NULL, delim);
	strcpy(Teams[nTeams].project_name, ptr);

	// add manager
	ptr = strtok(NULL, delim);
	strcpy(Teams[nTeams].manager, ptr);

	// add members
	for (size_t i = 0; i < 3; i++) {
		ptr = strtok(NULL, delim);
		strcpy(Teams[nTeams].members[i], ptr);
	}

	printf("\n%s %s is created.\n", Teams[nTeams].team_name, Teams[nTeams].project_name);

	nTeams++;
	getchar();
	cls();
	// Team_A Project_A Alan Cathy Fanny Helen
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
			batchMeetingRequest();
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
	while (1) mainMenu();
}