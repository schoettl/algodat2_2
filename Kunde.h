/*
 * Kunde.h
 *
 *  Created on: 28.11.2014
 *      Author: jakob
 */

#ifndef KUNDE_H_
#define KUNDE_H_

#include <string>
#include <fstream>

using namespace std;

#define MAX_NAME_LENGTH 9

class Kunde {
public:
	Kunde() : Kunde(0, "", 0.0) { }; // C++11 (constructor delegation)
	Kunde(int kundennummer, const string& name, double skonto);
	int getKundennummer() const { return kundennummer; }
	string getName() const { return name; }
	double getSkonto() const { return skonto; }
	ostream& output(ostream& outstream) const;
	fstream& read(fstream& istream);
	fstream& write(fstream& ostream) const;
private:
	int kundennummer;
	char name[MAX_NAME_LENGTH + 1];
	double skonto;
};

#endif /* KUNDE_H_ */
