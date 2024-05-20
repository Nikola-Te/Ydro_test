#include <iostream>
#include <fstream>
#include <string> 
#include <vector>

using namespace std;

class Club
{
public:
	void ClubFileRead(string quer)
	{
		ifstream in(quer);
		if (in.is_open())
		{
			string str;
			in >> places; 
			in >> str;
			cout << str ;
			timeStartMinute = StringToTime(str, timeStartHour);
			in >> str;
			timeEnd = str;
			in >> price;
			for (int i = 0; i < places; i++)
			{
				placePrice.push_back(0);
				placeHour.push_back(0);
				placeMinute.push_back(0);
				placeBusyName.push_back("0");
				placeBusyHour.push_back(0);
				placeBusyMinute.push_back(0);
			}
		}
		clubWork(move(in));
		in.close();
	}

	int places;
	int timeStartHour;
	int timeStartMinute;
	string timeEnd;
	int price;
	vector <int> placePrice;
	vector <int> placeHour;
	vector <int> placeMinute;
	vector <string> clientWeit;
	vector <string> placeBusyName;
	vector <string> clienName;
	//int* placeBusyHour;
	//int* placeBusyMinute;
	vector <int> placeBusyHour;
	vector <int> placeBusyMinute;

private:
	void clubWork(ifstream in)
	{
			string str;
			while (in >> str) {
				int id;
				in >> id;            //считал id события
				cout << endl << str << " " << to_string(id) + " ";
				int hour;
				int minute = StringToTime(str, hour);
				in >> str;            //считывает имя клиента
				cout << str;
				switch (id)
				{
				case 1:
				{
					id1(hour, minute, str);
					break;
				}
				case 2:
				{
					int placeNumber;
					in >> placeNumber;
					id2(hour, minute, str, placeNumber);
					break;
				}
				case 3:
				{
					id3(hour, minute, str);
					break;
				}
				case 4:
				{
					id4(hour, minute, str);
					break;
				}
				}
			}
			vector <string> sort;
			if (clienName.size() <= 1)
				for (int i = 0; i < places; i++)
				{
					if (placeBusyName[i] != "0") {
						cout << endl;
						int hour;
						int minute = StringToTime(timeEnd, hour);
						id11(hour, minute, placeBusyName[i]);
						break;
					}
				}
			for (int i = 0; i < places; i++)
				sort.push_back(placeBusyName[i]);
			for (int i = 1; i < places; i++)
			{
				if (sort[i] < sort[i - 1])
				{
					string temp = sort[i];
					sort[i] = sort[i - 1];
					sort[i - 1] = temp;
				}
			}
			for (int i = 0; i < places; i++)
				if (sort[i] != "0") {
					cout << endl;
					int minute, hour;
					minute = StringToTime(timeEnd, hour);
					id11(hour, minute, sort[i]);
				}
			cout << endl << timeEnd;
			for (int i = 0; i < places; i++)
				cout << endl << i + 1 << " " << placePrice[i] << " " << TimeToString(placeHour[i], placeMinute[i]);
	}
	int StringToTime(string str, int &hour)
	{
		str.erase(2, 1);
		int minute = stoi(str) % 100;
		hour = (stoi(str) - minute) / 100;
		return minute;
	}
	string TimeToString(int hour, int minute)
	{
		string time;
		if (minute >= 60)
		{
			minute = minute - 60;
			hour++;
		}
		if (hour <= 9)
			time = time + "0" + to_string(hour) + ":";
		else
			time = time + to_string(hour) + ":";
		if (minute <= 9)
			time = time + "0" + to_string(minute);
		else
		time = time + to_string(minute);
		return time;
	}
	void id1(int hour, int minute, string name)
	{
		if (timeStartHour > hour) {
			id13("NotOpenYet", hour, minute);
			return;}
		else if (hour == timeStartHour && minute < timeStartMinute){
			id13("NotOpenYet", hour, minute);
		return;}
		for (int i = 0; i < clienName.size(); i++)
		{
			if (name == clienName[i])
			{
				id13("YouShallNotPass", hour, minute);
				return;
			}
		}
		clienName.push_back(name);
	}
	void id2(int hour, int minute, string name, int placeNumber)
	{
		cout << " " << to_string(placeNumber);
		placeNumber--;
		if (placeBusyName[placeNumber] != "0")
		{
			id13("PlaceIsBusy", hour, minute);
				return;
		}

		for (int i=0;i<clienName.size();i++)
			if (clienName[i] == name)
			{
				for (int j = 0; j < places; j++)
				{
					if (placeBusyName[j] == name)
					{
						placeBusyName[j] = "0";
						int timeInMinute = 0;
						timeInMinute = 60 - placeBusyMinute[j] + minute + (hour - 1 - placeBusyHour[j]) * 60;
						placeHour[j] = placeHour[j] + timeInMinute / 60;
						placeMinute[j] = placeMinute[j] + timeInMinute % 60;
						if (timeInMinute % 60 != 0)
							timeInMinute = timeInMinute / 60 + 1;
						else  timeInMinute = timeInMinute / 60;
						placePrice[j] = placePrice[j] + timeInMinute * price;
						placeBusyHour[j] = 0;
						placeBusyMinute[j] = 0;
					}
				}
				placeBusyName[placeNumber] = name;
				placeBusyMinute[placeNumber] = StringToTime(TimeToString(hour, minute), placeBusyHour[placeNumber]);
				return;
			}
		id13("ClientUnknown", hour, minute);
	}
	void id3(int hour, int minute, string name)
	{
		for (int i = 0; i < places; i++)
		{
			if (placeBusyName[i] == "0")
			{
				id13("ICanWaitNoLonger!", hour, minute);
				return;
			}
		}
		if (clientWeit.size() > places)
		{
			id11(hour,minute,name);
			return;
		}
		clientWeit.push_back(name);
	}
	void id4(int hour, int minute,string name)
	{
		for (int i = 0; i < clienName.size(); i++)
		{
			if (name == clienName[i])
			{
				clienName.erase(clienName.begin() + i);
				for (int j = 0; j < placeBusyName.size(); j++)
				{
					if (placeBusyName[j] == name)
					{
						placeBusyName[j] = "0";
						int timeInMinute = 0;
						timeInMinute = 60 - placeBusyMinute[j] + minute + (hour-1 - placeBusyHour[j]) * 60;
						placeHour[j] = placeHour[j]+ timeInMinute / 60;
						placeMinute[j] = placeMinute[j]+ timeInMinute % 60;
						if (timeInMinute % 60 != 0)
							timeInMinute = timeInMinute / 60 + 1;
						else  timeInMinute = timeInMinute / 60;
						placePrice[j] = placePrice[j]+ timeInMinute * price;
						placeBusyHour[j] = 0;
						placeBusyMinute[j] = 0;
						id12(hour, minute, name);
						return;
					}
				}
			}
		}
		id13("ClientUnknown", hour, minute);
	}
	void id11(int hour, int minute, string name)
	{
		cout << TimeToString(hour, minute) << " 11 " << name;
		for (int i = 0; i < clienName.size(); i++)
		{
			if (name == clienName[i])
			{
				clienName.erase(clienName.begin() + i);
			}
		}
		for (int j = 0; j < placeBusyName.size(); j++)
		{
			if (placeBusyName[j] == name)
			{
				placeBusyName[j] = "0";
				int timeInMinute = 0;
				timeInMinute = 60 - placeBusyMinute[j] + minute + (hour - 1 - placeBusyHour[j]) * 60;
				placeHour[j] = placeHour[j] + timeInMinute / 60;
				placeMinute[j] = placeMinute[j] + timeInMinute % 60;
				if (timeInMinute % 60 != 0)
					timeInMinute = timeInMinute / 60 + 1;
				else  timeInMinute = timeInMinute / 60;
				placePrice[j] = placePrice[j] + timeInMinute * price;
				placeBusyHour[j] = 0;
				placeBusyMinute[j] = 0;
			}
		}
		for (int j = 0; j < clientWeit.size(); j++)
		{
			if (name == clientWeit[j])
			{
				clientWeit.erase(clientWeit.begin() + j);
			}
		}
	}
	void id12(int hour, int minute, string name)
	{
		if (!clientWeit.empty())
			for (int i = 0; i < placeBusyName.size(); i++)
			{
				if (placeBusyName[i] == "0")
				{
					placeBusyName[i] = clientWeit[0];
					placeBusyHour[i] = hour;
					placeBusyMinute[i] = minute;
					cout << endl << TimeToString(hour, minute) << " 12 " << placeBusyName[i] << " " << i+1;
					clientWeit.erase(clientWeit.begin());
					return;
				}
			}
	}
	void id13(string mistake, int hour, int minute)
	{
		cout << endl << TimeToString(hour, minute) << " 13 " << mistake;
	}
};

int main()
{
	string quer;
	cout << "write puth  ";
	cin >> quer;
	setlocale(LC_ALL, "Russian");
	Club CompClub;
	CompClub.ClubFileRead(quer);
	return 0;
}