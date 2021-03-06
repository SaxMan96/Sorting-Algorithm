#include "stdafx.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <sstream>

using namespace std;

//deklaracje
bool shift(int);

bool patternShift(int, int, int);
int movesCounter = 0;
int lastSortedElement(vector<int> vector);
bool MKII = true;
bool showMoves = false;
vector<int> warehouse;
vector<int> warehouseSave;
vector<int> orderFactor;
int partsNo;
int warehouseSize;
int passedTest = 0;


int wyborMenu() {
	while (1) {
		string ciag;
		int wybor;
		cin >> ciag;
		int i;
		int n = ciag.length();
		for (i = 0; i < n; ++i)
			if (!isdigit(ciag[i])) break;
		if (i == n) {
			istringstream iss(ciag);
			iss >> wybor;
			return wybor;
		}
		else cout << "Niepoprawny wybor. Wybierz cyfre z przedzialu <1,3>\n" << endl;
	}
}

void show() {
	if (!showMoves)
		return;
	cout << " ";
	for (int i : warehouse)
		cout << i << " ";
	cout << endl;
}

void sleep() {
	unsigned int miliseconds = 700;
	Sleep(miliseconds);
}

int fastSolve() {
	vector<int> result;
	vector<int> parts;
	for (int i = 0; i < partsNo; i++)
		parts.push_back(0);
	for (int i : warehouse)
		parts[i - 1]++;
	int minParts = parts[0];
	//szukam numeru części których liczba jest najmniejsza
	for (int i : parts)
		if (minParts > i)
			minParts = i;
	for (int i = 0; i < minParts; i++)
		for (int j = 0; j < partsNo; j++)
			result.push_back(j + 1);
	for (int i = 0; i < partsNo; i++)
		for (int j = 0; j < parts[i] - minParts; j++)
			result.push_back(i + 1);
	warehouse = result;
	return partsNo * minParts;
}

void move(int position) {
	movesCounter++;
	auto it = warehouse.begin() + position - 1;
	vector<int> replace(it, it + partsNo);
	warehouse.erase(it, it + partsNo);
	warehouse.insert(warehouse.end(), replace.begin(), replace.end());

	//setOrderFactorVector();
}

void moveShow2(int position) {
	if (!showMoves) {
		move(position);
		return;
	}
	if (position < 0) {
		cout << "Error: Wskazano przed wektor" << endl;
		return;
	}
	if (position > warehouse.size()) {
		cout << "Error: Wskazano za wektor" << endl;
		return;
	}
	auto it = warehouse.begin() + position - 1;

	auto iter = it;
	for (int i = 0; i < partsNo; i++, iter++)
		if (iter == warehouse.end()) {
			cout << "ERROR: funkcja move() it == warehouse.end()";
			return;
		}
	sleep();

	for (iter = warehouse.begin(); iter != warehouse.end(); iter++) {

		if (iter == it)
			cout << "(";
		else if (iter == it + partsNo)
			cout << ")";
		else
			cout << " ";
		cout << *iter;
	}
	cout << "\n";

	vector<int> replace(it, it + partsNo);
	warehouse.erase(it, it + partsNo);
	warehouse.insert(warehouse.end(), replace.begin(), replace.end());
	show();
	cout << "---------------------------------------\n";
	//setOrderFactorVector();
}

void moveShow(int position) {
	if (true) {
		moveShow2(position);
		return;
	}
	if (!showMoves) {
		move(position);
		return;
	}
	if (position < 0) {
		cout << "Error: Wskazano przed wektor" << endl;
		return;
	}
	if (position > warehouse.size()) {
		cout << "Error: Wskazano za wektor" << endl;
		return;
	}
	auto it = warehouse.begin() + position - 1;

	show();
	auto iter = it;
	for (int i = 0; i < partsNo; i++, iter++)
		if (iter == warehouse.end()) {
			cout << "ERROR: funkcja move() it == warehouse.end()";
			return;
		}
	sleep();


	for (iter = warehouse.begin(); iter != it; iter++)
		cout << *iter << " ";
	for (int i = 0; i < partsNo; i++)
		cout << "  ";
	for (iter = it + partsNo; iter != warehouse.end(); iter++)
		cout << *iter << " ";
	cout << endl;

	for (iter = warehouse.begin(); iter != it; iter++)
		cout << "  ";
	for (int i = 0; i < partsNo; i++, iter++)
		cout << *iter << " ";
	for (iter = it + partsNo; iter != warehouse.end(); iter++)
		cout << "  ";
	cout << endl;

	sleep();

	for (iter = warehouse.begin(); iter != it; iter++)
		cout << *iter << " ";
	for (int i = 0; i < partsNo; i++)
		cout << "  ";
	for (iter = it + partsNo; iter != warehouse.end(); iter++)
		cout << *iter << " ";
	cout << endl;

	for (iter = warehouse.begin(); iter != warehouse.end(); iter++)
		cout << "  ";
	iter = it;
	for (int i = 0; i < partsNo; i++, iter++)
		cout << *iter << " ";
	cout << endl;

	sleep();

	for (iter = warehouse.begin(); iter != it; iter++)
		cout << *iter << " ";
	for (int i = 0; i < partsNo; i++)
		cout << "  ";
	for (iter = it + partsNo; iter != warehouse.end(); iter++)
		cout << *iter << " ";
	iter = it;
	for (int i = 0; i < partsNo; i++, iter++)
		cout << *iter << " ";
	cout << endl;

	sleep();

	vector<int> replace(it, it + partsNo);
	warehouse.erase(it, it + partsNo);
	warehouse.insert(warehouse.end(), replace.begin(), replace.end());
	show();
}

int nextPart(int part) {
	return (part == partsNo) ? 1 : (part % partsNo) + 1;
}

int randPart() {
	return rand() % partsNo + 1;
}

vector<int> generate(unsigned int nextChance) {
	int part = rand() % partsNo + 1;
	int next;
	orderFactor.clear();
	warehouse.clear();
	warehouse.push_back(part);
	orderFactor.push_back(0);
	if (nextChance > 100) {
		cout << "współczynnik uporządkowania nie może być większy od 100%";
		nextChance = nextChance % 100;
	}
	for (int i = 0; i < warehouseSize - 1; i++) {
		if ((unsigned int)rand() % 100 <= nextChance)
			part = nextPart(part);
		else {
			do
				next = randPart();
			while (next == nextPart(part));
			part = next;
		}
		warehouse.push_back(part);
	}
	return warehouse;
}

void generationTest(int percentage) {
	warehouse = generate(percentage);
}

int valueAtPos(int position) {
	return *(warehouse.begin() + position - 1);
}

bool simpleShift(int position) {
	int part = (position - 1) % partsNo + 1;

	//to jest numer częśći, która powinna się znaleźć na wskazanej pozycji

	auto it = warehouse.begin() + position - 1;
	//ustawiam iterator na pozycję
	for (int i = position; i <= warehouse.size();) {
		if (*it == part) {
			for (int j = position; valueAtPos(position) != part; j += partsNo)
				moveShow(position);
			//przenoszenie pakietów tak, żeby taśmociąg dosunął pożądaną częśc na odpowiednie miejsce (warehouse.begin()+position-1)
			return true;
		}
		i += partsNo;
		if (i > warehouse.size())
			break;
		it += partsNo;
	}
	return false;
}

int findPartPositionMKII(int part, int startPosition) {
	auto it = prev(warehouse.end()); //zaczynam szukać od kolejnego elementu
	for (int i = warehouse.size(); it != warehouse.begin() + startPosition + 1; i--, it--)
		if (*it == part)
			return i;
	return 0;
}
int findPartPosition(int part, int startPosition) {
	if (MKII == true) {
		return	findPartPositionMKII(part, startPosition);
	}
	auto it = warehouse.begin() + startPosition; //zaczynam szukać od kolejnego elementu
	for (int i = startPosition + 1; it != warehouse.end(); i++, it++)
		if (*it == part)
			return i;
	return 0;
}

bool advancedPatternShift(int position, int partPosition) {
	position -= 2;
	if (partPosition == warehouse.size() - 1) {
		moveShow(position);
		moveShow(position + 1);
		moveShow(position);
		return true;
	}
	else if (partPosition == warehouse.size()) {
		moveShow(position + 1);
		moveShow(position + 1);
		moveShow(position);
		moveShow(position + 1);
		moveShow(position);
		moveShow(position + 1);
		moveShow(position + 1);
		moveShow(position + 1);
		return true;
	}
	return false;
}

bool shift(int position) {
	int part = (position - 1) % partsNo + 1;//position <1,2,3,4...,n>
											//	int additionalParts = warehouse.size() % partsNo;//nadmiarowe części (jeszcze nie wiemy, czy nie będzie ich więcej, ale co najmniej tyle)
											//	auto it = warehouse.begin() + position - 1;//najpierw przeszukuję taśmę, czy kilka przesunięć bezpośrednich przesunie wymaganą część na zadane miejsce
											//																 //nie tworzę licznika przesunięć, bo sprawdzam warunek zakończenia w funkcji simpleShift
	if (simpleShift(
		position))//Rozpoczynam szukanie przesuniecia, które pozwoli bezpośrednimi przesunięciami przysunąć wymaganą część na zadane miejsce
		return true;
	int partPosition = findPartPosition(part,
		position);//Jeżeli bezpośrednie przesuniecia nie zadziałają potrzeba znaleźć czy wogóle istnieje szukana część
	if (partPosition == 0)
		return false;
	int additionalParts = (warehouse.size() - position + 1) %
		partsNo;    // 1 2 3 1 _ _ 2 _ _  w tym przypadku jest 5 nieułożonych elementów co modulo partsNo daje 2 elementy których liczba jest potrzebna do dalszych obliczeń
	if (patternShift(position, additionalParts, partPosition))
		return true;
	if (advancedPatternShift(position, partPosition))
		return true;
	return false;
}


bool patternShift(int position, int additionalParts, int partPosition) {
	int przesuniecie = partPosition - (partsNo - additionalParts) % partsNo;
	if (przesuniecie < position) {
		if (partPosition + partsNo - 1 < warehouse.size()) {
			moveShow(partPosition);
			partPosition = warehouse.size() - partsNo + 1;
		}
		else if (warehouse.size() - position == partsNo) {
			do {
				moveShow(position);
				partPosition += 1;
			} while (!simpleShift(position));
			return true;
		}
		else return false;

	}
	else if (przesuniecie + partsNo - 1 >= warehouse.size()) {
		if (partPosition - partsNo >= position) {
			moveShow(partPosition - partsNo);
			partPosition -= partsNo;
		}
		else return false;
	}
	przesuniecie = partPosition - (partsNo - additionalParts) % partsNo;
	if (przesuniecie >= position && przesuniecie + partsNo - 1 < warehouse.size()) {
		moveShow(przesuniecie);
		if (simpleShift(position))
			return true;
	}
	return false;
}

void algorithmMKI() {
	auto it = warehouse.begin();
	int part = 1;
	show();
	//przeszukuje wszystkie pozycje od początku (pierwszy element to i = 1)
	for (unsigned int i = 1;
		i <= warehouse.size() - partsNo + 1 && it != warehouse.end() - partsNo + 1; part = nextPart(i), i++, it =
		warehouse.begin() + i - 1) {
		//jeżeli i-ty element jest na swoim miejscu to lecimy dalej
		if (*it == part)
			continue;
		//jeżeli nie to należy zrobić  wtym miejscu przesunięcie
		else {
			if (!shift(i))
				return;
		}
	}
}

void generateTestData(int howManyVectors, string dataFile, int nextChance) {
	ofstream myfile;
	myfile.open(dataFile);
	for (int i = 0; i < howManyVectors; i++) {
		generate(nextChance);
		for (int i : warehouse)
			myfile << i << " ";
		myfile << "\n";
		//        cout << "Wygenerowano: " << i + 1 << "/" << howManyVectors   << " danych testowych." << endl;
	}
	myfile.close();
}
void generateTestDataClean(int howManyVectors, string dataFile, int nextChance) {
	int part = rand() % partsNo + 1;
	int next;
	ofstream myfile;
	myfile.open(dataFile);
	for (int i = 0; i < howManyVectors; i++) {
		for (int i = 0; i < warehouseSize - 1; i++) {
			if ((unsigned int)rand() % 100 <= nextChance) {
				part = nextPart(part);
				myfile << part << " ";
			}
			else {
				do
					next = randPart();
				while (next == nextPart(part));
				myfile << next << " ";
			}
		}
		myfile << "\n";
		// cout << "Wygenerowano: " << i + 1 << "/" << howManyVectors  << " danych testowych." << endl;
	}
	myfile.close();
}

void loadWarehouseFromString(string line) {
	int pos = 0;
	int number;
	warehouse.clear();
	while (line.size() != 0) {
		number = std::stoi(line.substr(0, pos = line.find(' ')));
		line.erase(0, pos + 1);
		warehouse.push_back(number);
	}
	return;
}

void saveToOutputFile(bool writeWarehouse, string name, string resultFile) {
	std::ofstream outfile;
	outfile.open(resultFile, std::ios_base::app);
	if (writeWarehouse) {
		outfile << "  ";
		for (int i : warehouse)
			outfile << i;
		outfile << endl;
	}
	outfile << name << ": " << endl;
	outfile.close();
}

void saveToOutputFile(string name, bool writeWarehouse, string resultFile) {
	std::ofstream outfile;
	outfile.open(resultFile, std::ios_base::app);
	outfile << name << ": " << endl;
	if (writeWarehouse) {
		outfile << "  ";
		for (int i : warehouse)
			outfile << i;
		outfile << endl;
	}

	outfile.close();
}

void testAndSaveToFile(int testNo, string resultFile) {
	std::ostringstream oss;
	oss << testNo;
	string text = "----------- Test";
	text += oss.str();
	saveToOutputFile(text, false, resultFile);
	//Zamiast "Test"+testNo trzeba pisać takie cuda, ktoś się nie postarał
	warehouseSave = warehouse;
	saveToOutputFile("\ttask", true, resultFile);
	int lastSortedPos = fastSolve();
	saveToOutputFile("\tfast", true, resultFile);
	warehouse = warehouseSave;
	algorithmMKI();
	if (lastSortedPos <= lastSortedElement(warehouse)) {
		saveToOutputFile(true, "\tsolv OK", resultFile);
		passedTest++;
	}
	else {
		std::ostringstream oss2;
		oss2 << warehouse.size() - lastSortedElement(warehouse);
		string text = "\tsolv FALSE";
		text += oss2.str();
		saveToOutputFile(true, text, resultFile);
	}
}
void testAndSaveToFileClean() {
	warehouseSave = warehouse;
	int lastSortedPos = fastSolve();
	warehouse = warehouseSave;
	algorithmMKI();
	if (lastSortedPos <= lastSortedElement(warehouse))
		passedTest++;
}

int lastSortedElement(std::vector<int> vector) {
	for (int i = 0; i < vector.size(); i++)
		if ((i % partsNo) + 1 != vector.at(i))
			return i;
	return vector.size();
}

void clearOutput(string resultFile) {
	std::ofstream outfile;
	outfile.open(resultFile);
	outfile.close();
}

void Test(bool showM, const clock_t begin_time, string dataFile, string resultFile) {
	//clearOutput(resultFile);
	showMoves = showM;
	string line;
	ifstream myfile(dataFile);
	if (myfile.is_open()) {
		int i = 1;
		while (getline(myfile, line)) {
			cout << u8"Ładuję " << i << u8" ciąg ... ";
			loadWarehouseFromString(line);
			testAndSaveToFile(i, resultFile);
			cout << "OK" << endl;
			i++;
		}
		string text = "";
		std::ostringstream oss;
		oss << passedTest << "/" << i - 1 << " passed";
		text += oss.str();
		cout << text << endl;
		saveToOutputFile(text, false, resultFile);

		oss.clear();
		oss.str("");
		oss << float(clock() - begin_time) / CLOCKS_PER_SEC;
		text = "Czas wykonania: ";
		text += oss.str();
		text += "sec";
		cout << text;
		saveToOutputFile(text, false, resultFile);

		myfile.close();
		passedTest = 0;
	}
	else cout << "Nie mozna otworzyc pliku";
}
void TestClean(bool showM, const clock_t begin_time, string dataFile, string resultFile, int maxIter) {
	clearOutput(resultFile);
	showMoves = showM;
	string line;
	ifstream myfile(dataFile);
	if (myfile.is_open()) {
		int i = 1;
		while (getline(myfile, line) && i <= maxIter) {
			loadWarehouseFromString(line);
			testAndSaveToFileClean();
			i++;
			//cout<<"Przeszlo: "<<i<<endl;
		}
		string text = "";
		std::ostringstream oss;
		//oss << passedTest << "/" << i - 1 << " passed "<<"Czas wykonania: "<<float(clock() - begin_time) / CLOCKS_PER_SEC;
		oss << float(clock() - begin_time) / CLOCKS_PER_SEC;
		text += oss.str();
		//text += "sec";
		cout << text << endl;
		saveToOutputFile(text, false, resultFile);

		myfile.close();
		passedTest = 0;
	}
	else cout << "Nie mozna otworzyc pliku";
}
void CutDataFiles(string dataFile, string data1File, int howMany = 1000) {
	std::ofstream outfile;
	outfile.open(data1File, std::ios_base::app);
	string line;
	ifstream myfile(dataFile);
	if (myfile.is_open()) {
		int i = 0;
		while (getline(myfile, line) && i < howMany) {
			outfile << line << endl;
			cout << "Przeszlo: " << ++i << "/" << howMany << endl;
		}

		myfile.close();
		outfile.close();
		passedTest = 0;
	}
	else cout << "Nie mozna otworzyc pliku";
}

int main() {
	clock_t begin_time;
	srand((unsigned int)time(NULL));
	partsNo = 3;
	warehouseSize = 20;
	int howMany;
	int decyzja;
	int maxIter;
	while (1) {
		cout << "MENU:\n<0>Generowanie testow\n<1>Generowanie pojedyncze\n<2>Testowanie z pliku\n<3>Testowanie pojedyncze\n<4>Masowe Testowanie\n<5>Testowanie tercjowe\n<6>Wyjscie\n";
		decyzja = wyborMenu();
		switch (decyzja) {
		case 0:
			//-----------------------Generowanie testow------------------------
			cout << "Generowanie do pliku." << endl;
			cout << u8"Ile iteracji danych testowych wprowadzić" << endl;
			cin >> howMany;
			cout << u8"Wprowadź stopień uporządkowania 0-99" << endl;
			int nextChance;
			cin >> nextChance;
			cout << "Rozpoczynam tworzenie danych testowych" << endl;


			generateTestData(howMany, "../testDataMulti.txt", nextChance);
			cout << "Zakonczono zapisywanie danych testowych." << endl;

			break;
		case 1:
			//-----------------------Generowanie testow------------------------
			cout << "Generowanie Pojedyncze." << endl;
			cout << u8"Wprowadź stopień uporządkowania 0-99" << endl;
			cin >> nextChance;
			cout << "Rozpoczynam tworzenie danych testowych" << endl;


			generateTestData(1, "../testDataSingle.txt", nextChance);
			cout << "Zakonczono zapisywanie danych testowych." << endl;

			break;
		case 2:
			//----------------------Testowanie z pliku-----------------------
			cout << "Rozpoczynam testowanie z pliku." << endl;
			TestClean(false, clock(), "../testDataSingle.txt", "../testResultSingle.txt", 2);
			break;
		case 3:
			//----------------------------Testowanie pojedyncze---------------------------
			//showMoves = true;
			cout << "Testowanie pojedyncze." << endl;
			//                static const int arr[] = {
			//                        2, 1, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2
			//                };
			//                warehouse.assign(arr, arr + sizeof(arr) / sizeof(arr[0]));
			//Test(false, clock(), "../testDataSingle.txt", "../testResultSingle.txt");

			/*generateTestData(1, "../testDataSingle.txt", nextChance);
			MKII = false;
			TestClean(false, clock(), "../testDataSingle.txt", "../testResultSingle.txt", 2);
			cout << "liczba ruchów MKI: " << movesCounter << endl;
			movesCounter = 0;
			MKII = true;
			TestClean(false, clock(), "../testDataSingle.txt", "../testResultSingle.txt", 2);
			cout << "liczba ruchów MKII: " << movesCounter << endl;*/

			MKII = false;
			generateTestData(4000, "../testDataTerc.txt", 0);
			TestClean(false, clock(), "../testDataTerc.txt", "../testResultTerc.txt", 10000);
			cout << "liczba ruchów MKI: " << movesCounter << endl;
			movesCounter = 0;
			MKII = true;
			TestClean(false, clock(), "../testDataTerc.txt", "../testResultTerc.txt", 10000);
			cout << "liczba ruchów MKII: " << movesCounter << endl;
			break;

		case 4:
			//-----------------------Masowe Testowanie-----------------------
			cout << "Masowe Testowanie." << endl;
			maxIter = 100;
			begin_time = clock();

			TestClean(false, clock(), "../testData0.txt", "../testResult0.txt", maxIter);
			TestClean(false, clock(), "../testData20.txt", "../testResult20.txt", maxIter);
			TestClean(false, clock(), "../testData40.txt", "../testResult40.txt", maxIter);
			TestClean(false, clock(), "../testData60.txt", "../testResult60.txt", maxIter);
			TestClean(false, clock(), "../testData80.txt", "../testResult80.txt", maxIter);
			TestClean(false, clock(), "../testData100.txt", "../testResult100.txt", maxIter);
			break;
		case 5:
			//-----------------------Testowanie X-----------------------
			cout << "Testowanie tercjowe." << endl;
			nextChance = 50;
			cout << "podaj nextChance";
			cin >> nextChance;
			maxIter = 100000;
			for (int i : {10, 13, 16, 20, 25, 32, 40, 50, 64, 80, 100, 126, 159, 200}) {
				
				generateTestData(20 * i, "../testDataTerc.txt", nextChance);
				TestClean(false, clock(), "../testDataTerc.txt", "../testResultTerc.txt", maxIter);
			
			}
			break;
			TestClean(false, clock(), "../testData20.txt", "../testResult20.txt", maxIter);
			TestClean(false, clock(), "../testData40.txt", "../testResult40.txt", maxIter);
			TestClean(false, clock(), "../testData60.txt", "../testResult60.txt", maxIter);
			TestClean(false, clock(), "../testData80.txt", "../testResult80.txt", maxIter);
			TestClean(false, clock(), "../testData100.txt","../testResult100.txt",maxIter);
			break;

		case 6:
			//----------------------------Wyjscie---------------------------
			cout << "DZIEKUJE ZA SKORZYSTANIE Z PROGRAMU.";
			return 0;
		default:
			//-----------------------------Blad-----------------------------
			cout << "\nBLAD WYBORU.\nNacisnij Enter...";

			break;
		}
	}
	//cout << "Podaj ilosc elementow w samochodzie: ";
	//cin >> partsNo;
	//cout << "Podal ilosc czesci w magazynie: ";
	//cin >> warehouseSize;
}


