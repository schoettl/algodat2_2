/*
 * Algorithmen und Datenstrukturen II
 * Praktikum -- Uebung 2
 *
 * Jakob Schoettl <jschoett@hm.edu>
 *
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <climits>
#include <sstream>

using namespace std;

#define BUCKET_SIZE 2
#define HASH_FILE_NAME "HASHFILE"

#include "Kunde.h"
#include "HashDat.h"

void showMenu();
int promptForCustomerId();
void promptForCustomer(Kunde& customer);
void operationComplete(const string& message);

typedef HashDat<BUCKET_SIZE> MyHashDat;

int main(int argc, char** argv) {
	cout << "Algorithmen und Datenstrukturen II" << endl
	     << "Praktikum -- Uebung 2" << endl
	     << "Jakob Schoettl <jschoett@hm.edu>" << endl << endl
	     << "Usage: hashdb" << endl << endl;

	MyHashDat file(HASH_FILE_NAME);

	showMenu();

	while (1) {
		cout << "Choose an option: ";
		char option;
		cin >> option;
		switch (option) {
		case 'm':
			showMenu();
			break;
		case 'i':
			try {
				Kunde customer;
				promptForCustomer(customer);
				file.insert(customer);
				operationComplete("Inserted new customer.");
			} catch (MyHashDat::BadArticle& e) {
				operationComplete("Customer exists already.");
			}
			break;
		case 'r':
			try {
				int customerId = promptForCustomerId();
				Kunde* customer = file.retrieve(customerId);
				customer->output(cout);
				delete customer;
				operationComplete("Retrieved customer.");
			} catch (MyHashDat::NotFound& e) {
				stringstream str;
				str << "Customer not found: " << e.getKundennummer();
				operationComplete(str.str());
			}
			break;
		case 's':
			file.show(cout);
			break;
		case 'q':
			return 0;
		default:
			operationComplete("Invalid option.");
			break;
		case 't':
			file.test();
			break;
		}
	}
}


void showMenu() {
	cout << "Interactive menu:" << endl
	     << "m Show this menu" << endl
	     << "i Insert record" << endl
	     << "r Retrieve record" << endl
	     << "s Show all records" << endl
	     << "q Quit" << endl << endl;
}

int promptForCustomerId() {
	cout << "Kundennummer: ";
	int customerId;
	while (!(cin >> customerId) || customerId <= 0) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Please enter a positive number. Try again: ";
	}
	return customerId;
}

void promptForCustomer(Kunde& customer) {
	int id = promptForCustomerId();

	cout << "Name: ";
	string name;
	cin.ignore(INT_MAX, '\n');
	getline(cin, name);
	if (name.length() > MAX_NAME_LENGTH) {
		name = name.substr(0, MAX_NAME_LENGTH);
		cout << "Name is too long. Using '" << name << "'." << endl;
	}

	cout << "Skonto: ";
	double skonto;
	while (!(cin >> skonto)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Please enter a number. Try again: ";
	}

	customer = Kunde(id, name, skonto);
}

void operationComplete(const string& message) {
	cout << message << endl << endl;
}
