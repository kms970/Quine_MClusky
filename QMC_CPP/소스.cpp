#include<iostream>
#include<vector>
#include<string>
#include<Windows.h>
#include<bitset>
#include<iomanip>
#include<algorithm>
#include<cstring>

using namespace std;

typedef struct QM {
	string bi;
	string decimal;
	int countOne = 0;
	bool checkPi = true;
}QM;

int printArray[100][100];
vector<vector<QM>> finalArray;
vector<QM> pi;
vector<string> stringArray;
int endOfMerge = 0;

bool operator <(const QM& a, const QM& b) {
	return a.countOne < b.countOne;
}

void gotoxy(int x, int y) {
	COORD pos = { x * 2,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printArr(int mergeCount) {
	int one = 0;
	for (int i = 0; i < finalArray[mergeCount].size(); i++) {
		if (finalArray[mergeCount][i].countOne == one) cout << left << setw(25) << finalArray[mergeCount][i].decimal << finalArray[mergeCount][i].bi << endl;
		else { //1의 갯수가 끝났을때 구분을 위해서 한줄 띄워주기 위해서 사용
			cout << endl;
			one++;
			i--;
		}
	}
}

void input(vector<QM> temp) {
	QM a;
	int input;
	for (int i = 0; i < 16; i++) {
		a.countOne = 0;
		cout << i << "\t" << bitset<4>(i) << "\t";
		cin >> input;
		if (input == 1) {
			a.bi = bitset<4>(i).to_string();
			a.countOne = bitset<4>(i).count();
			a.decimal = to_string(i);
			temp.push_back(a);
		}
	}
	finalArray.push_back(temp);
}

void merge(int mergeCount) {
	int compareBi, end = 0;
	vector<QM> temp;

	for (int i = 0; i < finalArray[mergeCount].size(); i++) //pi를 추출하기 위해 조합이 되기 전으로 checkPi 변수 초기화
		finalArray[mergeCount][i].checkPi = true;

	for (int i = 0; i < finalArray[mergeCount].size(); i++) {
		for (int j = i + 1; j < finalArray[mergeCount].size(); j++) {
			if ((finalArray[mergeCount][j].countOne - finalArray[mergeCount][i].countOne) == 1) {
				compareBi = 0;
				for (int k = 0; k < 4; k++)//2진수 하나씩 비교하는 부분 1일때만 하나만 비교하도록
					if (finalArray[mergeCount][j].bi[k] != finalArray[mergeCount][i].bi[k]) compareBi++;

				if (compareBi == 1) {
					end++;
					temp.push_back(finalArray[mergeCount][i]);
					for (int k = 0; k < 4; k++) //'-'표시하기
						if (finalArray[mergeCount][j].bi[k] != finalArray[mergeCount][i].bi[k]) temp[temp.size() - 1].bi[k] = '-';

					finalArray[mergeCount][j].checkPi = false;
					finalArray[mergeCount][i].checkPi = false;
					temp[temp.size() - 1].decimal = temp[temp.size() - 1].decimal + ' ' + finalArray[mergeCount][j].decimal; //10진수 넣는 부분
					temp[temp.size() - 1].countOne = finalArray[mergeCount][j].countOne - 1;
					for (int z = 0; z < temp.size() - 1; z++) //중복체크 중복비트는 지우기
						if (temp[z].bi == temp[temp.size() - 1].bi) temp.pop_back();
				}
			} //if
		} //for j
		if (finalArray[mergeCount][i].checkPi) pi.push_back(finalArray[mergeCount][i]); //조합이 안되면 pi로 빼기
	} //for i
	finalArray.push_back(temp);
	if (end == 0) endOfMerge = mergeCount + 1;
}

void delchart(int x) {//주항차트에서 epi를 따라서 지우는 함수
	for (int i = 0; i < finalArray[0].size(); i++) {
		if (printArray[x][i] == 100) {
			printArray[x][i] = 99;
			for (int j = 0; j < pi.size(); j++)
				if (printArray[j][i] == 100) printArray[j][i] = 99;
		}//if
	}//for
}

void printBack() { //주항차트 출력부분
	int biggerind = pi[pi.size() - 1].decimal.length();
	for (int i = 0; i < pi.size(); i++) {
		gotoxy(0, i + 3);
		cout << pi[i].decimal << endl;
	}

	for (int i = 0; i < finalArray[0].size(); i++) {
		gotoxy((biggerind + 1) + i * 2, 1);
		cout << finalArray[0][i].decimal;
	}

	for (int i = 0; i < pi.size(); i++) {
		gotoxy(biggerind + 1, i + 3);
		for (int j = 0; j < finalArray[0].size(); j++)
		{
			gotoxy((biggerind + 1) + j * 2, i + 3);
			if (printArray[i][j] == 100) cout << "X ";
			else if (printArray[i][j] == 101) cout << "ⓧ ";
			else if (printArray[i][j] == 99) cout << "─ ";
		}
		cout << endl;
	}
}

void splitStr(string a) { //string 공백에 맞춰 나누는 부분
	char* str_buff = new char[100];
	strcpy_s(str_buff, 100, a.c_str());

	char* tok = strtok(str_buff, " ");
	while (tok != nullptr) {
		stringArray.push_back(string(tok));
		tok = strtok(nullptr, " ");
	}
}

void makeChart() { //차트를 만드는 함수
	int f, cnt, tcount;
	for (int i = 0; i < pi.size(); i++) {//처음 차트에서 10진수에 맞춰서 X를 표시해주는 함수
		for (int j = 0; j < finalArray[0].size(); j++)
			printArray[i][j] = stoi(finalArray[0][j].decimal);

		f = 0;
		splitStr(pi[i].decimal);
		for (int k = 0; k < finalArray[0].size(); k++) {
			if (f == stringArray.size()) break;
			else if (stoi(stringArray[f]) == printArray[i][k]) {
				printArray[i][k] = 100;
				f++;
			} //if
		} //for k
		stringArray.clear();
	}//for i

	system("cls");
	printBack();
	system("pause");

	for (int i = 0; i < finalArray[0].size(); i++) {//EPI를 체크해주는 함수
		cnt = 0;
		for (int j = 0; j < pi.size(); j++)
			if (printArray[j][i] == 100) cnt++;

		for (int j = 0; j < pi.size(); j++) {
			if (printArray[j][i] == 100 && cnt == 1) {
				pi[j].checkPi = 0;
				printArray[j][i] = 101;
			} //if
		} //for j
	} //for i
	for (int i = 0; i < pi.size(); i++) //EPI 부분에서 지우기 위해 사용된 함수
		for (int j = 0; j < finalArray[0].size(); j++)
			if (printArray[i][j] == 101) delchart(i);

	for (int i = 0; i < pi.size(); i++) {//차트에서 지워졌는지 안지워졌는지 확인하는 함수 tcount++ 이면 지워짐 tcount가 0이면 지워진게 없다는 말이므로 epi가 됨
		tcount = 0;
		for (int j = 0; j < finalArray[0].size(); j++)
			if (printArray[i][j] == 99) tcount++;

		if (tcount == 0) pi[i].checkPi = 0;
	}

	tcount = 0;
	for (int i = 0; i < pi.size(); i++) //차트에서 논리식의 갯수를 구하는 함수
		if (pi[i].checkPi == 0) tcount++;
	system("cls");
	printBack();
	gotoxy(0, 4 + pi.size());
	cout << "F= ";
	for (int i = 0; i < pi.size(); i++) {//논리식 출력
		if (pi[i].checkPi == 0) {
			tcount--;
			if (pi[i].bi[0] == '0') cout << "A'";
			else if (pi[i].bi[0] == '1') cout << "A";
			if (pi[i].bi[1] == '0') cout << "B'";
			else if (pi[i].bi[1] == '1') cout << "B";
			if (pi[i].bi[2] == '0') cout << "C'";
			else if (pi[i].bi[2] == '1') cout << "C";
			if (pi[i].bi[3] == '0') cout << "D'";
			else if (pi[i].bi[3] == '1') cout << "D";
			if (tcount != 0) cout << " + ";
		}
	}
	gotoxy(0, 5 + pi.size());
}

void start() {
	int t = 1;
	vector<QM> temp;
	input(temp);
	finalArray.push_back(finalArray[0]);
	stable_sort(finalArray[1].begin(), finalArray[1].end());
	while (endOfMerge == 0) {
		merge(t);
		cout << endl;
		printArr(t);
		system("pause");
		system("cls");
		t++;
	}
	makeChart();
}

int main() {
	system("mode con: cols=150 lines=50");
	start();
}