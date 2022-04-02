#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

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

struct Range {
	int s_year;
	int e_year;
	int s_month;
	int e_month;
	int s_day;
	int e_day;
};

struct Range range;

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

void Pause() {
	getchar();
	cls();
}

bool getRange() {
	printf("\nyyyy-mm-dd yyyy-mm-dd\nEnter starting and ending date in above format | ");
	char str[255];
	char delim[] = " -";

	flush();
	fgets(str, 255, stdin);

	if(str[0] == '0') {
		cls();
		return false;
	}

	char *ptr = strtok(str, delim);
	range.s_year = atoi(ptr);
	ptr = strtok(NULL, delim);
	range.s_month = atoi(ptr);
	ptr = strtok(NULL, delim);
	range.s_day = atoi(ptr);
	ptr = strtok(NULL, delim);
	range.e_year = atoi(ptr);
	ptr = strtok(NULL, delim);
	range.e_month = atoi(ptr);
	ptr = strtok(NULL, delim);
	range.e_day = atoi(ptr);
	return true;
}

int getTeamIndex(char *name) {
	for (size_t i = 0; i < nTeams; i++)
		if(strcasecmp(name, Teams[i].team_name) == 0)
			return i;
	return -1;
}

bool checkTimeOverlap(int m, int n) {
	struct Meeting A = Meetings[m];
	struct Meeting B = Meetings[n];

	if(A.year != B.year) return false;
	if(A.month != B.month) return false;
	if(A.day != B.day) return false;
	if(A.hours == B.hours) return true;

	if(A.hours > B.hours && A.hours < B.hours + B.dur_hours) return true;
	if(B.hours > A.hours && B.hours < A.hours + A.dur_hours) return true;

	return false;
}

bool checkOverlapMeetings(int m, int n) {
	struct Team A = Teams[getTeamIndex(Meetings[m].team_name)];
	struct Team B = Teams[getTeamIndex(Meetings[n].team_name)];

	bool commonMembers = false;

	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			if(strcasecmp(A.members[i], B.members[j]) == 0)
				commonMembers = true;

	// if there are no common members then there cant be an overlap
	if(!commonMembers) return false;

	return checkTimeOverlap(m, n);
}

void outputModule(char *approved, char *algo) {
	int fd_1[2];
	int fd_2[2];

	pipe(fd_1);
	pipe(fd_2);

	int c_write = fd_2[1];
	int p_read = fd_2[0];
	int p_write = fd_1[1];
	int c_read = fd_1[0];

	int pid = fork();

	if(pid == 0) {
		close(p_read);
		close(p_write);

		char buffer[200] = {0};
		read(c_read, buffer, 200);


		char filename[] = "Schedule_FCFS.txt";

		FILE *file = fopen(filename, "w");

		fprintf(file, "*** Project Meeting ***");
		fprintf(file, "\n\nAlgorithm used: FCFS");
		fprintf(file, "\nPeriod: %d-%d-%d to %d-%d-%d", range.s_year, range.s_month, range.s_day, range.e_year, range.e_month, range.e_day);
		fprintf(file, "\n\nDate\t\t\t\tStart\t\tEnd\t\t\tTeam\t\t\t\tProject");
		fprintf(file, "\n=============================================================");
		
		char members[50][20];
		int nMembers = 0;
		int approved[50];
		int nApproved = 0;
		char delims[] = "|";
		char *ptr = strtok(buffer, delims);

		do
		{
			approved[nApproved++] = atoi(ptr);
			struct Meeting meeting = Meetings[atoi(ptr)];
			struct Team team = Teams[getTeamIndex(meeting.team_name)];
			char end_time[6];
			snprintf(end_time, 6, "%02d:%02d", meeting.hours + meeting.dur_hours, meeting.minutes);
			fprintf(file, "\n%s\t%s\t\t%s\t\t%s\t\t\t%s", meeting.date, meeting.time, end_time, team.team_name, team.project_name);

			// add each member to members array if not already there
			for (int i = 0; i < 3; i++)
			{
				bool included = false;
				for (int j = 0; j < nMembers; j++)
					if(strcasecmp(team.members[i], members[j]) == 0) {
						included = true;
						break;
					}
				if(!included)
					strcpy(members[nMembers++], team.members[i]);
			}

		} while (ptr = strtok(NULL, delims));

		fprintf(file, "\n=============================================================");

		for (int i = 0; i < nMembers; i++)
		{
			fprintf(file, "\nStaff: %s", members[i]);
			fprintf(file, "\n\nDate\t\t\t\tStart\t\tEnd\t\t\tTeam\t\t\t\tProject");
			fprintf(file, "\n=============================================================");

			for (int j = 0; j < nApproved; j++)
			{
				struct Meeting meeting = Meetings[approved[nApproved]];
				struct Team team = Teams[getTeamIndex(meeting.team_name)];

				for (int k = 0; k < 3; k++)
					if(strcasecmp(team.members[k], members[i]) == 0) {
						char end_time[6];
						snprintf(end_time, 6, "%02d:%02d", meeting.hours + meeting.dur_hours, meeting.minutes);
						fprintf(file, "\n%s\t%s\t\t%s\t\t%s\t\t\t%s", meeting.date, meeting.time, end_time, team.team_name, team.project_name);
						break;
					}
			}
			fprintf(file, "\n=============================================================");
		}
		
		fprintf(file, "\n\n\t\t\t\t\t\t\t\t\t\t- End -");
		fclose(file);

		char msg[100];
		snprintf(msg, 100, "Schedule exported to %s", filename);
		write(c_write, msg, 100);

		exit(0);
	}

	close(c_read);
	close(c_write);

	write(p_write, approved, 200);

	char buffer[100] = {0};
	read(p_read, buffer, 100);
	waitpid(pid, NULL, 0);
	printf("\n%s\n", buffer);

	flush();
	Pause();
}

void scheduleFCFS() {
	if(!getRange()) return;

	int fd_c2p[2];

	pipe(fd_c2p);

	int c_write = fd_c2p[1];
	int p_read = fd_c2p[0];

	int pid = fork();

	if(pid == 0) {
		close(p_read);

		int approved[50];	// storing approved meeting indexes in here
		int rejected[50];	// storing approved meeting indexes in here
		int a = 0;	// approved last index
		int r = 0;	// approved last index

		for (int i = 0; i < nMeetings; i++) {
			bool overlap = false;
			for (int j = 0; j < a; j++)
			{
				overlap = checkOverlapMeetings(i, approved[j]);
				if(overlap) break;
			}

			if(overlap)
				rejected[r++] = i;
			else
				approved[a++] = i;
		}

		char buffer[100] = {0};
		char buffer_[200] = {0};

		for (int i = 0; i < a; i++) {
			snprintf(buffer_, 200, "%d|", approved[i]);
			strcat(buffer, buffer_);
		}

		write(c_write, buffer, 100);

		exit(0);
	}

	close(c_write);

	char buffer[200] = {0};
	read(p_read, buffer, 200);

	waitpid(pid, NULL, 0);

	char algo[] = "FCFS";
	outputModule(buffer, algo);
}

bool validTeam(char *team) {
	// checks if given person is already managing a team or not
	int matches = 0;
	for (size_t i = 0; i <= nTeams; i++)
		if(strcasecmp(team, Teams[i].team_name) == 0) matches++;
	return matches <= 1;
}

bool validProject(char *project) {
	// checks if given person is already managing a team or not
	int matches = 0;
	for (size_t i = 0; i <= nTeams; i++)
		if(strcasecmp(project, Teams[i].project_name) == 0) matches++;
	return matches <= 1;
}

bool validManager(char *manager) {
	// checks if given person is already managing a team or not
	int matches = 0;
	for (size_t i = 0; i <= nTeams; i++)
		if(strcasecmp(manager, Teams[i].manager) == 0) matches++;
	return matches <= 1;
}

bool validMember(char *member) {
	// checks if given person is in less 3 teams or not
	int matches = 0;
	for (size_t i = 0; i <= nTeams; i++)
		for (size_t j = 0; j < 3; j++)		
			if(strcasecmp(member, Teams[i].members[j]) == 0) matches++;
	return matches <= 3;
}

void parseMeetingRequest(char *buffer) {
	// parse input and add data into struct Meeting

	char date[11];
	char time[6];

	char delim[] = " -:";
	char dateDelim[] = "-";
	char timeDelim[] = ":";

	// clear the current struct just in case
	bzero(&Meetings[nMeetings], sizeof(Meetings[nMeetings]));

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

	if(Meetings[nMeetings].dur_hours > 9) {
		printf("\n\nMeeting hours cannot exceed 9 hours, request ignored.\n\n");
		return;
	}

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

	if(filename[0] == '0') {
		cls();
		return;
	}

	// remove trailing newline char (replace it with null terminator)
	filename[strcspn(filename, "\n")] = 0;

	// return if file doesnt exist
	if(access(filename, F_OK) != 0) {
		printf("\nCannot find file, make sure its in current dir.");
		Pause();
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
	Pause();
}

void inputMeetingRequest() {
	printf("Team_Name yyyy-mm-dd hh:mm hours");
	printf("\n\nEnter data in above format | ");

	char str[255];

	flush();
	fgets(str, 255, stdin);

	if(str[0] == '0') {
		cls();
		return;
	}

	parseMeetingRequest(str);

	printf("\nMeeting request received.\n");

	Pause();
	// Team_A 2022-04-24 09:40 2
}

void createProjectTeam() {
	printf("Team_Name Project_Name Manager Member_1 Member_2 Member_3");
	printf("\n\nEnter data in above format | ");

	char str[255];
	char delim[] = " ";

	flush();
	fgets(str, 255, stdin);

	if(str[0] == '0') {
		cls();
		return;
	}

	// clear the current struct just in case
	bzero(&Teams[nTeams], sizeof(Teams[nTeams]));

	// add team name
	char *ptr = strtok(str, delim);
	strcpy(Teams[nTeams].team_name, ptr);
	if(!validTeam(ptr)) {
		printf("\n%s already exists!\n", ptr);
		Pause();
		return;
	}

	// add project name
	ptr = strtok(NULL, delim);
	strcpy(Teams[nTeams].project_name, ptr);
	if(!validProject(ptr)) {
		printf("\n%s already exists!\n", ptr);
		Pause();
		return;
	}

	// add manager
	ptr = strtok(NULL, delim);
	strcpy(Teams[nTeams].manager, ptr);

	if(!validManager(ptr)) {
		printf("\n%s is already manager of some project!\n", ptr);
		Pause();
		return;
	}

	// add members
	for (size_t i = 0; i < 3; i++) {
		ptr = strtok(NULL, delim);
		strcpy(Teams[nTeams].members[i], ptr);
		if(!validMember(ptr)) {
			printf("\n%s is already in 3 projects!\n", ptr);
			Pause();
			return;
		}
	}

	printf("\n%s %s is created.\n", Teams[nTeams].team_name, Teams[nTeams].project_name);
	nTeams++;

	Pause();
	// Team_A Project_A Alan Cathy Fanny Helen
	// Team_B Project_B Cathy Allan Fanny Helen
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
			scheduleFCFS();
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