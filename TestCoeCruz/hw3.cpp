//Victor Herrig
//Hostetler
//OOP
//hw3
#include <iostream>
#include<iomanip>
#include <sstream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

const int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const int START_YEAR = 2016;


//Class to keep track of the date
class Date {
private:
	int year, month, day;
	long daysFrom;

	//Checks to make sure the given date values are valid -- not compiling on school system
	bool checkDateValues(unsigned int d, unsigned int mo, unsigned int y) {
		if (d < 0 || mo < 0 || y < 0 || mo > 12) return false;
		else if (mo != 1) return d <= daysInMonth[mo - 1];
		else if (isLeapYear(y)) return d <= 29;
		else return d <= 28;
	}

	bool isLeapYear(unsigned int y) {
		return (((y % 4 == 0) && !(y % 100 == 0)) || y % 400 == 0);
	}
	//Finds the number of days from 1/1/2016 from the date
	void setDaysFrom() {
		daysFrom = 0;
		for (size_t i = START_YEAR; i < year; i++)
		{
			if (isLeapYear(i)) daysFrom += 366;
			else daysFrom += 365;
		}
		for (size_t i = 1; i < month; i++)
		{
			if(i != 2) daysFrom += daysInMonth[i - 1];
			else if (isLeapYear(year)) daysFrom += 29;
			else daysFrom += 28;
		}
		daysFrom += day;
	}

	//
	//
	//
	//
	//COME BACK TO CHECK THIS DUH////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//
	//

	//Sets the date knowing the days from 1/1/2016
	void resetDate() {
		long d = daysFrom;
		year = START_YEAR;
		month = day = 1;
		while ((isLeapYear(year) && daysFrom - d > 366) || (!isLeapYear(year) && daysFrom - d > 365)) {
			year++;
			if (isLeapYear(year)) d -= 366;
			else d -= 365;
		}
		while ((! month == 2 && daysFrom - d > daysInMonth[month + 1]) || 
			   (month == 2 && ((isLeapYear(year) && daysFrom - d > 29) || (isLeapYear(year) && daysFrom - d > 28)))) {
			if (month == 2 && isLeapYear(year)) d -= 29;
			else if (month == 2 && !isLeapYear(year)) d -= 28;
			else d -= daysInMonth[month - 1];
			month++;
		}
		day = d;
	}

public:

	/**************************************/
	/*************Constructors*************/
	/**************************************/

	//Default constructor
	Date() {
		year = 0;
		month = 0;
		day = 0;
	}

	//Constructor with all values given
	Date(unsigned int d, unsigned int mo, unsigned int y) {
		if (checkDateValues(d, mo, y)) {
			year = y;
			month = mo;
			day = d;
			setDaysFrom();
		}
	}

	//A constructor using the number of days from 1/1/2016
	Date(long ds) {
		daysFrom = ds;
		resetDate();
	}

	/************************************************/
	/***************Get/set functions****************/
	/************************************************/

	//Get/set date
	void getDate(unsigned int& d, unsigned int& mo, unsigned int& y) {
		d = day;
		mo = month;
		y = year;
	}
	void setDate(unsigned int d, unsigned int mo, unsigned int y) {
		//if(checkDateValues(y, mo, d)) { <--------Not working, always returning false :/
			day = d;
			month = mo;
			year = y;
			setDaysFrom();
		//}
		//else {
			//throw invalid_argument("Invalid Date");
		//}
	}

	/**************************************************/
	/***************Printing functions*****************/
	/**************************************************/

	//Returns the date as a string in D/M/Y format, clearly the more logical of the two
	string toStringDMY() {
		ostringstream out;
		out << day << "/" << month << "/" << (year > 2099 ? year % 2100 : year % 2000);
		return out.str();
	}

	//Returns the date as a string in M/D/Y
	string toStringMDY() {
		ostringstream out;
		out << month << "/" << day << "/" << (year > 2099 ? year % 2100 : year % 2000);
		return out.str();
	}

	/************************************************/
	/**********Overloading all the operators*********/
	/************************************************/

	Date operator=(Date rhs) {
		setDate(rhs.day, rhs.month, rhs.year);
		return *this;
	}

	Date operator+(long rhs) {
		Date newDate(rhs);
		newDate.resetDate();
		return newDate;
	}

	Date operator+=(long rhs) {
		return *this + rhs;
	}

	long operator-(Date rhs) {
		return abs(daysFrom - rhs.daysFrom);
	}

	bool operator==(Date rhs) {
		return daysFrom == rhs.daysFrom;
	}

	bool operator>(Date rhs) {
		return daysFrom > rhs.daysFrom;
	}

	bool operator>=(Date rhs) {
		return (*this > rhs) || (*this == rhs);
	}

	bool operator<(Date rhs) {
		return !(*this > rhs) && !(*this == rhs);
	}

	bool operator<=(Date rhs) {
		return !(*this > rhs);
	}
};

//Class to keep track of time
class Time {
private:
	//Values of 0 for all of the date values indicates that the date is being left blank
	//Will throw an exception if only one of the date values is 0, except for year
	int hour, minute, second, dayTick;

	//Checks to make sure the given time values are valid
	bool checkTimeValues(unsigned int h, unsigned int m, unsigned int s) {
		if (h < 0 || h > 23) {
			//throw invalid_argument("Hour out of Range");  <---------- All of these gave me errors, so the values simply won't be added if it's bad, no error
			return false;
		}
		else if (m < 0 || m > 59) {
			//throw invalid_argument("Minute out of Range");
			return false;
		}
		else if (s < 0 || s > 59) {
			//throw invalid_argument("Second out of Range");
			return false;
		}
		else
			return true;
	}
		
	void incrementDay() {
		dayTick++;
	}

	long secondsFrom() {
		return 3600 * hour + 60 * minute + second;
	}

public:
	/**************************************/
	/*************Constructors*************/
	/**************************************/

	//Default constructor
	Time() {
		hour = 0;
		minute = 0;
		second = 0;
		dayTick = 0;
	}

	//Only time constructor
	Time(unsigned int h, unsigned int m, unsigned int s) {
		if (checkTimeValues(h, m, s)) {
			hour = h;
			minute = m;
			second = s;
		}
		dayTick = 0;
	}

	/************************************************/
	/***************Get/set functions****************/
	/************************************************/

	//Get/set time
	void getTime(unsigned int& h, unsigned int& m, unsigned int& s) {
		h = hour;
		m = minute;
		s = second;
	}
	void setTime(unsigned int h, unsigned int m, unsigned int s) {
		checkTimeValues(h, m, s);
		hour = h;
		minute = m;
		second = s;
	}

	void decrementDayTick() {
		if (dayTick < 1) {
			throw invalid_argument("Can't decrement further!");
		}
		dayTick--;
	}


	int getDayTick() {
		return dayTick;
	}

	/**************************************************/
	/***************Printing functions*****************/
	/**************************************************/

	//Returns the time as a string in 12 hour format
	string toString12() {
		ostringstream out;
		out << ((hour == 0 || hour == 12) ? 12 : hour % 12) << ":" << setfill('0') << setw(2) << minute << ":" << setw(2) << second << (hour < 12 ? "AM" : "PM");
		return out.str();
	}

	//Returns the time as a string in 24 hour format
	string toString24() {
		ostringstream out;
		out << setfill('0') << setw(2) << hour << ":" << setw(2) << minute << ":" << setw(2) << second;
		return out.str();
	}

	long long toSeconds() {
		return 3600 * hour + 60 * minute + second;
	}
	/************************************************/
	/**********Overloading all the operators*********/
	/************************************************/
	Time operator=(Time rhs){
		setTime(rhs.hour, rhs.minute, rhs.second);
		return *this;
	}

	Time operator+(long long rhs) {
		Time newTime((hour + rhs / 3600) % 23, minute + (rhs % 3600) / 60, second + ((rhs % 3600) % 60));
		if (hour + rhs / 3600 > 23) incrementDay();
		return newTime;
	}

	Time operator+=(long long rhs) {
		return *this + rhs;
	}

	//Returns thge difference between times in number of seconds (and probably should be a long or long long)
	long long operator-(Time rhs) {
		//Time newTime(year - rhs.year, month - rhs.month, day - rhs.day, hour - rhs.hour, minute - rhs.minute, second - rhs.second);
		//return newTime.hour + (1 / 60) * newTime.minute + (1/ 3600 ) * newTime.second;
		return abs(3600 * (hour - rhs.hour) + 60 * (minute - rhs.minute) + (second - rhs.second));
	}

	/*Time operator-=(Time rhs) {
		return *this - rhs;
	}*/

	bool operator==(Time rhs) {
		//unsigned int rhsV[3];
		//unsigned int lhsV[3];
		//getTime(lhsV[0], lhsV[1], lhsV[2]);
		//rhs.getTime(rhsV[0], rhsV[1], rhsV[2]);
		//return rhsV == lhsV;
		return secondsFrom() == rhs.secondsFrom();
	}

	bool operator>(Time rhs) {
		//unsigned int rhsV[3];
		//unsigned int lhsV[3];
		//getTime(lhsV[0], lhsV[1], lhsV[2]);
		//rhs.getTime(rhsV[0], rhsV[1], rhsV[2]);
		//return rhsV > lhsV;
		return secondsFrom() > rhs.secondsFrom();
	}

	bool operator>=(Time rhs) {
		return (*this > rhs) || (*this == rhs);
	}

	bool operator<(Time rhs) {
		return !(*this > rhs) && !(*this == rhs);
	}

	bool operator<=(Time rhs) {
		return !(*this > rhs);
	}
};

//Struct to put together the date and time
struct Tempus {
	Time T;
	Date D;

	Tempus() {
		T.setTime(0, 0, 0);
		D.setDate(0, 0, 0);
	}

	Tempus(unsigned int d, unsigned int mo, unsigned int y, unsigned int h, unsigned int m, unsigned int s) {
		T.setTime(h, m, s);
		D.setDate(d, mo, y);
	}

	Tempus(Date d, Time t){
		D = d;
		T = t;
	}

	void setAll(unsigned int d, unsigned int mo, unsigned int y, unsigned int h, unsigned int m, unsigned int s) {
		T.setTime(h, m, s);
		D.setDate(d, mo, y);
	}

	void copyExisting(Tempus& Tem) {
		T = Tem.T;
		D = Tem.D;
	}

	//Returns the date and time in M/D/Y and 12-hour format with a connecting word/phrase
	string toStringMDY(string connector) {
		ostringstream out;
		out << D.toStringMDY() << connector << T.toString12();
		return out.str();
	}

	//Returns the date and time in D/M/Y and 24-hour format with a connecting word/phrase
	string toStringDMY(string connector) {
		ostringstream out;
		out << D.toStringDMY() << connector << T.toString24();
		return out.str();
	}

	bool operator==(Tempus rhs) {
		return (D == rhs.D && T == rhs.T);
	}

	bool operator<(Tempus rhs) {
		return (D < rhs.D && T < rhs.T);
	}

	bool operator>(Tempus rhs) {
		return !(*this == rhs || *this < rhs);
	}

	bool operator<=(Tempus rhs) {
		return (*this < rhs || *this == rhs);
	}

	bool operator>=(Tempus rhs) {
		return (*this > rhs || *this == rhs);
	}

	Tempus operator=(Tempus rhs) {
		Tempus newTemp;
		newTemp.T = rhs.T;
		newTemp.D = rhs.D;
		return newTemp;
	 }

	/*Tempus operator+(Time rhs) {
		Tempus newTemp(D, T);
		newTemp.T += rhs.toSeconds();
		for (size_t i = 0; i < T.getDayTick(); i++)
		{
			newTemp.D == 1;
			newTemp.T.decrementDayTick();
		}
		return newTemp;
	}*/

	Tempus operator+(long long rhs) {
		Tempus newTemp(D, T);
		newTemp.T += rhs;
		for (size_t i = 0; i < T.getDayTick(); i++)
		{
			newTemp.D == 1;
			newTemp.T.decrementDayTick();
		}
		return newTemp;
	}

	Tempus operator+=(long long rhs) {
		return *this + rhs;
	}

	long long operator-(Tempus rhs) {
		Time dayEnd(23, 59, 59);
		Time dayBeg(0, 0, 0);
		long dDif = D - rhs.D;
		if (dDif > 0) {
			return ((dDif - 1) * 86400 + (dayEnd - rhs.T) + (T - dayBeg));
		}
		else return T - rhs.T;
	}
};
//General class to hold the info from the files

const Tempus FAKE_TIME(12, 12, 2115, 00, 00, 00);

struct User {
	User() { name = " "; phone = " "; email = " "; }
	User(string n, string p, string e) {
		name = n; phone = p; email = e;
	}

	virtual void readLine(ifstream& inFile) = 0;
	virtual string toString() = 0;
	virtual bool isDriver() = 0;
	virtual bool isAlhp(User* user2) {
		for (size_t i = 0; i < name.size(); i++)
		{
			if (name[i] < user2->name[i]) {
				return true;
			}
			else if(name[i] > user2->name[i]) {
				return false;
			}
		}
		return false;
	}

	bool operator<(User* rhs) {
		return ((int)name[0] < (int)rhs->name[0]);
	}

	string name, phone, email;
};

struct Driver : public User {
	string toString() override {
		ostringstream out;
		out << name << "   " << phone << "   " << email << endl;
		return out.str();
	}
	virtual void readLine(ifstream& inFile) override {
		string word;
		inFile >> word;
		name = word;
		inFile >> word;
		phone = word;
		inFile >> word;
		email == word;
	}
	virtual bool isDriver() { return true; }
};

struct Rider : public User {
	string toString() override {
		ostringstream out;
		out << name << "   " << phone << "   " << email << endl;
		return out.str();
	}
	virtual void readLine(ifstream& inFile) override {
		string word;
		inFile >> word;
		name = word;
		inFile >> word;
		phone = word;
		inFile >> word;
		email == word;
	}
	virtual bool isDriver() { return false; }
};

Tempus findTempus(string word, string word2) {
	Tempus tempTime;
	int a = (word[3] - '0') * 10 + (word[4] - '0');
	int b = (word[0] - '0') * 10 + (word[1] - '0');
	int c = ((word[6] - '0') == 1 && (word[7] - '0') < 6) || (word[6] - '0') < 1 ?
		2100 + ((word[6] - '0') * 10 + (word[7] - '0')) : 2000 + ((word[6] - '0') * 10 + (word[7] - '0'));

	tempTime.D.setDate((word[3] - '0') * 10 + (word[4] - '0'), (word[0] - '0') * 10 + (word[1] - '0'),
		((word[6] - '0') == 1 && (word[7] - '0') < 6) || (word[6] - '0') < 1 ?
		2100 + ((word[6] - '0') * 10 + (word[7] - '0')) : 2000 + ((word[6] - '0') * 10 + (word[7] - '0')));
	//inFile >> word;
	tempTime.T.setTime(10 * (word2[0] - '0') + (word2[1] - '0'), 10 * (word2[3] - '0') + (word2[4] - '0'), 0);
	return tempTime;
}

struct Info abstract{
	//Vars for keeping track of entry info
	/*vector<Tempus>*/ Tempus time;
	/*vector<string>*/ string toLoc, fromLoc;
	string user;
	/*vector<int>*/ int numPassengers;
	//vector<double> price;
	//2-D vector of ints for keeping track of matches and time differences of those matches (only used in one instance of the struct)
	//vector<vector<int>> matches, timeDifference;
	

	/******************************************/
	/***************Constructors***************/
	/******************************************/

	//Default constructor
	Info() {
	}

	//Constructor for all info given
	Info(Tempus t, string u, string f, string to, int n) {
		time = t;
		user = u;
		fromLoc = f;
		toLoc = to;
		numPassengers = n;
	}

	virtual string toString() {
		ostringstream out;
		out << user << " " << fromLoc << " " << toLoc << " " << numPassengers;
		return out.str();
	}

	virtual Tempus checkMatch(Info* candidate) = 0;
	virtual void sortMatches() = 0;
	virtual void readLine(ifstream& inFile) {
		string word;
		inFile >> word;
		user = word;
		inFile >> word;
		fromLoc = word;
		inFile >> word;
		toLoc = word;
		inFile >> word;
		numPassengers = stoi(word);
	}
	virtual Tempus getTime() = 0;
	virtual string printAt() = 0;
	virtual size_t matchSize() = 0;
	virtual string printMatches() = 0;
};

struct Match {

	Match() { }
	Match(Info* a, double t) : arrangement(a), timeDiff(t) { }
	Match operator=(Match rhs) {
		Match newMatch;
		newMatch.arrangement = rhs.arrangement;
		newMatch.timeDiff = rhs.timeDiff;
		return newMatch;
	}
	string toString(size_t i) {
		ostringstream out;
		bool oc = (arrangement->getTime() == FAKE_TIME);
		out << "#" << i << " Ride with " << arrangement->user << arrangement->printAt();
		return out.str();
	}

	//Operator overloading for sorting 'n stuff
	bool operator==(Match rhs) {
		return timeDiff == rhs.timeDiff;
	}
	bool operator>(Match rhs) {
		return timeDiff > rhs.timeDiff;
	}
	bool operator<(Match rhs) {
		return !(*this > rhs || *this == rhs);
	}
	bool operator>=(Match rhs) {
		return (*this > rhs || *this == rhs);
	}
	bool operator<=(Match rhs) {
		return (*this < rhs || *this == rhs);
	}

	Info* arrangement;
	double timeDiff;
};

struct Request : public Info {
//Struct for holding the request info and matches
	Request() {};
	Request(Tempus t, string u, string f, string to, int n) : Info(t, u, f, to, n){ }

	virtual string toString() override {
		ostringstream out;
		out << time.toStringMDY(" ") << " " << Info::toString();
		return out.str();
	}

	virtual Tempus checkMatch(Info* candidate) override {
		static size_t depth = 0;
		
		if (++depth > 1) {
			depth--;
			return FAKE_TIME;
		}
		double diff = candidate->checkMatch(candidate) - time;
		if (((abs(diff) <= 3600) || (candidate->getTime() == FAKE_TIME)) && toLoc == candidate->toLoc && fromLoc == candidate->fromLoc && numPassengers <= candidate->numPassengers) {
			if (candidate->time == FAKE_TIME)
				diff = -1;
			matches.push_back(Match(candidate, diff));
		}
		depth = 0;
	}

	virtual void sortMatches() {
		sort(matches.begin(), matches.end());
		
		size_t begin = matches.size() - 1;
		for (size_t i = 0; i < matches.size(); i++) {
			if (matches.at(i).timeDiff == -1) {
				begin = i;
				break;
			}
		}
		for (size_t i = begin; i < matches.size(); i++)
		{
			for (size_t j = begin; j < matches.size(); j++) {
				if (matches[i].arrangement->user[0] < matches[j].arrangement->user[0]) {
					Match tempMatch;
					tempMatch = matches[j];
					matches[j] = matches[i];
					matches[i] = tempMatch;
				}
			}
		}

	}

	virtual void readLine(ifstream& inFile) {
		string word, word2;
		inFile >> word;
		inFile >> word2;
		if (inFile.good())
			time.copyExisting(findTempus(word, word2));// = findTempus(word, word2);
		Info::readLine(inFile);
	}

	virtual Tempus getTime() override { return time; }

	virtual string printAt() override {
		return " ";
	}
	virtual size_t matchSize() override {
		return matches.size();
	}
	virtual string printMatches() override {
		ostringstream out;
		for (size_t i = 0; i < matches.size(); i++)
		{
			out << matches[i].toString(i + 1);
		}
		return out.str();
	}

	vector<Match> matches;
	//vector<vector<int>> /*matches,*/ timeDifference;
};

struct Offer : public Info {
//Struct for holding the ride offer information
	Offer() {};
	Offer(Tempus t, string u, string f, string to, int n) : Info(t, u, f, to, n){ }

	virtual string toString() override {
		ostringstream out;
		out << time.toStringMDY(" ") << " " << Info::toString();
		return out.str();
	}
	virtual Tempus checkMatch(Info* candidate) override { return time; }
	virtual void sortMatches() override { return; }
	virtual void readLine(ifstream& inFile) {
		string word, word2;
		inFile >> word;
		inFile >> word2;
		if (inFile.good())
			time.copyExisting(findTempus(word, word2));// = findTempus(word, word2);
		Info::readLine(inFile);
	}
	virtual Tempus getTime() { return time + 31536000; }
	virtual string printAt() override {
		ostringstream out;
		out << user << " - leaving on " << time.toStringMDY(" at ") << " - " << numPassengers << " seats available" << endl;
		return out.str();
	}
	virtual size_t matchSize() override {
		return 0;
	}
	virtual string printMatches() override {
		return " ";
	}
};

struct OnCall : public Info {
//Struct for holding the on-call ride offer information
	OnCall() {};
	OnCall(Tempus t, string u, string f, string to, int n, double p) : Info(t, u, f, to, n){
		price = p;
	}

	virtual string toString() override {
		ostringstream out;
		out << Info::toString() << " $" << fixed << setfill('0') << setprecision(2) << price;
		return out.str();
	}
	virtual Tempus checkMatch(Info* candidate) override { return FAKE_TIME; }
	virtual void sortMatches() { return; }
	virtual void readLine(ifstream& inFile) {
		string word;
		Info::readLine(inFile);
		inFile >> word;
		price = stod(word);
	}

	virtual Tempus getTime() { return FAKE_TIME; }
	virtual string printAt() override {
		ostringstream out;
		out << user << " - " << numPassengers << " seats available anytime at $" << fixed << setfill('0') << setprecision(2) << price << endl;
		return out.str();
	}
	virtual size_t matchSize() override {
		return 0;
	}
	virtual string printMatches() override {
		return " ";
	}

	double price;
};

//Function for reading in all the requests, offers and on-call offers
void readIn(vector<Info*>& arrangements, vector<User*>& users) {
	//stringstream inString;
	//Open the ride requests file
	ifstream inFile("transactions.in");
	string word;
	Tempus tempTime;
	bool isUser = false;

	//Read in the first line
	inFile >> word;
	//Read in the file word by word and put that data into requests
	while (inFile.good()) {
			inFile >> word;
			if(inFile.good()){
			if (word == "RIDR") {
				users.push_back(new Rider());
				isUser = true;
			}
			else if (word == "DRVR") {
				users.push_back(new Driver());
				isUser = true;
			}
			else if (word == "REQU") {
				arrangements.push_back(new Request());
			}
			else if (word == "TOFR") {
				arrangements.push_back(new Offer());
			}
			else if (word == "ONCL") {
				arrangements.push_back(new OnCall());
			}

			if (isUser) {
				users.back()->readLine(inFile);
			}
			else {
				arrangements.back()->readLine(inFile);
			}

			isUser = false;
		}
	}
	inFile.close();
}

//Function for printing out all the matches to the matches.in file
void readOut(vector<Info*>& arrangements, vector<User*>& users, size_t brkpt, size_t numRequests, size_t numOff) {
	ofstream outFile("transactions.out");

	outFile << "Rider accounts: \n";
	for (size_t i = 0; i < brkpt; i++)
	{
		outFile << users[i]->toString();
	}
	outFile << "Driver accounts: \n";
	for (size_t i = brkpt; i < users.size(); i++)
	{
		outFile << users[i]->toString();
	}
	outFile << "Requests for rides: \n";
	for (size_t i = 0; i < arrangements.size(); i++)
	{
		if(i == numRequests) outFile << "\nOffers (time-specific) for rides: \n";
		if(i == numRequests + numOff) outFile << "On call offers for rides: \n";
		outFile << arrangements[i]->toString() << endl;
	}

	//for (size_t i = numRequests; i < numOff; i++)
	//{
	//	outFile << arrangements[i]->toString();
	//}
	//
	//for (size_t i = (numRequests + numOff); i < arrangements.size(); i++)
	//{
	//	outFile << arrangements[i]->toString();
	//}
	outFile << "Ride matches: \n\n";
	for (size_t i = 0; i < numRequests; i++)
	{
		outFile << arrangements[i]->user << " ride for " << arrangements[i]->numPassengers << " from " << arrangements[i]->fromLoc << " to " << arrangements[i]->toLoc
			<< " on " << arrangements[i]->time.toStringMDY(" at time ") << "\n"; //<< arrangements[i]->printMatches();
		if (arrangements[i]->matchSize() != 0) { outFile << arrangements[i]->matchSize() << " matches found!\n" << arrangements[i]->printMatches(); }
		else outFile << "Sorry, no matches found.\n";
			outFile << endl;
	}
	//
	//
	//
	//Need to do some output formatting for the matches
	//
	//
	outFile.close();
}

void findMatches() {
	vector<Info*> arrangements;
	vector<User*> users;
	size_t numRequests = 0;
	size_t numOc = 0;
	size_t numOff = 0;

	//Read the data into the objects
	readIn(arrangements, users);

	//Find the matches
	for (size_t i = 0; i < arrangements.size(); i++){
		for (size_t j = 0; j < arrangements.size(); j++){
			arrangements[i]->checkMatch(arrangements[j]);
		}
	}
	/*for each (Info* arr in arrangements)
	{
		for each (Info* arr2 in arrangements)
		{
			arr->checkMatch(arr2);
		}
	}*/
	//Sort the matches by time difference
	for (size_t i = 0; i < arrangements.size(); i++)
	{
		arrangements[i]->sortMatches();
	}
	//Sort the arrangements by time, leaving the oncall offers on the bottom
	for (size_t i = 0; i < arrangements.size(); i++)
	{
		for (size_t j = 0; j < arrangements.size(); j++)
		{
			if (arrangements[i]->getTime() > arrangements[j]->getTime()) {
				Info* tempInfo;														
				tempInfo = arrangements[j];
				arrangements[j] = arrangements[i];
				arrangements[i] = tempInfo;
			}
		}
	}
	//Find the nunmber of requests and offers, and filter the requests to the top of the vector
	for (size_t i = 0; i < arrangements.size(); i++)
	{
		Info* temp;
		if (arrangements[i]->printAt() == " ") {
			temp = arrangements[numRequests];
			arrangements[numRequests] = arrangements[i];
			arrangements[i] = temp;
			numRequests++;
		}
		else if (!(arrangements[i]->getTime() == FAKE_TIME)) {
			numOff++;
		}
	}
	//Find the number of on-call offers and filter them to the bottom of the vector
	for (int i = arrangements.size() - 1; i >= 0; i--)
	{
		Info* temp;
		if (arrangements[i]->getTime() == FAKE_TIME) {
			temp = arrangements[arrangements.size() - 1 -numOc];
			arrangements[arrangements.size() - 1 - numOc] = arrangements[i];
			arrangements[i] = temp;
			numOc++;
		}
	}

	//Sort the users by driver or rider
	size_t breakpoint = 0;
	for (size_t i = 0; i < users.size(); i++)
	{
		if (!users[i]->isDriver())
		{
			User* temp;
			temp = users[breakpoint];
			users[breakpoint] = users[i];
			users[i] = temp;
			breakpoint++;
		}
	}
	//Sort the riders by name
	for(size_t i = 0; i < breakpoint; i++)
	{
		for(size_t j = 0; j < breakpoint; j++)
		{
			if (users[i]->isAlhp(users[j])){
				User* tempUser = users[j];
				users[j] = users[i];
				users[i] = tempUser;
			}
		}
	}
	//Sort the drivers by name
	for (size_t i = breakpoint; i < users.size(); i++)
	{
		for (size_t j = breakpoint; j < users.size(); j++)
		{
			if (users[i]->isAlhp(users[j])) {
				User* tempUser = users[j];
				users[j] = users[i];
				users[i] = tempUser;
			}
		}
	}

	readOut(arrangements, users, breakpoint, numRequests, numOff);
}

int main() {
	//Find the matches
	findMatches();
	//My console doesn't stop at the end so I put in cin.get() at the end for convenience
	cin.get();
}
