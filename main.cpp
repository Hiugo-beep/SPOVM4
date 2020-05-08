#include<windows.h>//for processes/events
#include <conio.h>//_getch()
#include <iostream>
#include <string>
#include <process.h>    /* _beginthread, _endthread */
using namespace std;

HANDLE events[10];
void myThread(void *);
int threadNumber = 0;
int stopThread = 0;

void main() {
	char choice = '+';
	bool flag = false;
	printf("+ : to create new process\n- : to delete last thread\nq : exit\n\n");
	while (true) {
		choice = _getch();//waiting for pressing, returns its code
		switch (choice) {
		case '+': {
			cout << "YOU PUT \"+\"\nCreate new thread!\n";
			char name[2];
			name[0]= threadNumber + 0x30;
			name[1] = '\0';
			events[threadNumber] = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)name);
			threadNumber++;
			if (_beginthread(myThread, 1024, NULL) == -1)  cout << "Error begin thread" << endl;
			break;
		}
		case '-': {
			cout << "YOU PUT \"-\"\nDelete thread!\n";
			if (threadNumber == 0) {
				cout << "Nothing to delete..." << endl;
				break;
			}
			else {
				stopThread = threadNumber;
				for (int j = 0; j < threadNumber; j++) {
					SetEvent(events[j]);
					Sleep(1000);
					WaitForSingleObject(events[j], INFINITE);
				}
				threadNumber--;
				CloseHandle(events[threadNumber]);
				stopThread = 0;
				break;
			}
		}
		case 'q': {
			if (threadNumber == 0) {
				cout << "Exit...\n";
				system("pause");
				return;
			}
			else {
				stopThread = threadNumber;
				int maxNumber = threadNumber;
				for (int j = 0; j < maxNumber; j++) {
					stopThread = threadNumber;
					SetEvent(events[j]);
					threadNumber--;
					CloseHandle(events[threadNumber]);
					stopThread = 0;
				}
				cout << "Exit...\n";
				system("pause");
				return;
			}
		}
		default: {
			printf("Wrong input! Try again...\n ");
			break;
		}
		}

		while (true) {
			for (int i = 0; i < threadNumber; i++) {
				SetEvent(events[i]);
				Sleep(1000);
				WaitForSingleObject(events[i], INFINITE);
			}
				if (_kbhit())  //нажата ли клавиша на клавиатуре
					break;
		}


	}
}

void myThread(void *) {
	int number = threadNumber;
	char temp = number + 0x30;
	DWORD dwWaitResult;
	while (true) {
		dwWaitResult = WaitForSingleObject(events[number - 1], INFINITE);
		if (number == stopThread) {
			SetEvent(events[number - 1]);
			_endthread();
		}
		for (int i = 0; i < 5; i++) {
			cout << temp << flush;
			Sleep(200);
		}
		rewind(stdin);
		cout << "\t";
		SetEvent(events[number - 1]);
	}
}



