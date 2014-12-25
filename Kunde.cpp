/*
 * Kunde.cpp
 *
 *  Created on: 28.11.2014
 *      Author: jakob
 */

#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#include "Kunde.h"

Kunde::Kunde(int kundennummer, const string& name, double skonto) :
	kundennummer(kundennummer), skonto(skonto) {
	strncpy(this->name, name.c_str(), MAX_NAME_LENGTH);
	this->name[MAX_NAME_LENGTH] = '\0';
}

ostream& Kunde::output(ostream& outstream) const {
	outstream << "Kundennummer: " << kundennummer << endl
	          << "Name: " << name << endl
	          << "Skonto: " << skonto << endl;
	return outstream;
}

fstream& Kunde::read(fstream& istream) {
	istream.read((char*) &kundennummer, sizeof (int));
	istream.read((char*) name, MAX_NAME_LENGTH);
	istream.read((char*) &skonto, sizeof (double));
	return istream;
}

fstream& Kunde::write(fstream& ostream) const {
	ostream.write((char*) &kundennummer, sizeof (int));
	ostream.write((char*) name, MAX_NAME_LENGTH);
	ostream.write((char*) &skonto, sizeof (double));
	return ostream;
}
