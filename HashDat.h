/*
 * HashDat.h
 *
 *  Created on: 28.11.2014
 *      Author: jakob
 */

#ifndef HASHDAT_H_
#define HASHDAT_H_

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <stdexcept>

using namespace std;

#define BUCKET_COUNT 3 // Total number of buckets in the hash file
#define STEP_SIZE 2 // STEP_SIZE und BUCKET_COUNT teilerfremd!
#define DEFAULT_OPEN_MODE ios::in | ios::out | ios::binary

template <unsigned bucketSize>
class HashDat;

#include "Bucket.h"

template <unsigned bucketSize>
class HashDat {
public:
	HashDat(const string& filename);

	void insert(const Kunde& customer);
	Kunde* retrieve(const int kundennummer);
	void show(ostream& outstream);
	void test();

	class BadArticle : public exception { };
	class NotFound : public exception {
		int kundennummer;
	public:
		NotFound(const int kundennummer) : kundennummer(kundennummer) { };
		int getKundennummer() const { return kundennummer; };
	};

private:
	string filename;
	fstream filestream;
	Bucket<bucketSize> bucket;
	unsigned generateHashValue(int kundennummer) const;
	void exitWithIOError() const;
};


template <unsigned bucketSize>
HashDat<bucketSize>::HashDat(const string& filename) : // C++11 erlaubt string als Arg. für filestream, oder?
	filename(filename), filestream(filename.c_str(), DEFAULT_OPEN_MODE) {
	if (!filestream) {
		// probably the file does not exist, try to create:
		filestream.open(filename.c_str(), DEFAULT_OPEN_MODE | ios::trunc);
		if (!filestream) {
			exitWithIOError();
		}
	}
}

template <unsigned bucketSize>
void HashDat<bucketSize>::insert(const Kunde& customer) {
	const unsigned hash = generateHashValue(customer.getKundennummer());
	filestream.clear();
	for (unsigned k = 0; k < BUCKET_COUNT; k++) {
		unsigned position = ((hash + k*STEP_SIZE) % BUCKET_COUNT) * BUCKET_SIZE_IN_BYTES;
		bucket.read(filestream, position);
		if (!filestream) {
			// An dieser Position steht noch nichts!
			bucket.clear();
			filestream.clear(); // sonst geht das Schreiben nicht!
		}
		for (int i = 0; i < bucketSize; i++) {
			Kunde& c = bucket.slots[i];
			if (c.getKundennummer() == 0) {
				c = customer;
				bucket.write(filestream, position);
				return;
			}
			if (c.getKundennummer() == customer.getKundennummer()) {
				throw BadArticle();
			}
		}
	}
	throw runtime_error("All slots in all buckets are already occupied.");
}

template <unsigned bucketSize>
Kunde* HashDat<bucketSize>::retrieve(const int kundennummer) {
	const unsigned hash = generateHashValue(kundennummer);
	filestream.clear();
	for (unsigned k = 0; k < BUCKET_COUNT; k++) {
		unsigned position = ((hash + k*STEP_SIZE) % BUCKET_COUNT) * BUCKET_SIZE_IN_BYTES;
		bucket.read(filestream, position);
		if (!filestream) {
			// An dieser Stelle steht noch nichts!
			throw NotFound(kundennummer);
		}
		for (int i = 0; i < bucketSize; i++) {
			const Kunde& c = bucket.slots[i];
			if (c.getKundennummer() == kundennummer) {
				return new Kunde(c);
			}
			if (c.getKundennummer() == 0) {
				// Optimierung: Wenn Kunde existiert, dann ist er im nächsten freien Slot, und nirgendwo sonst.
				throw NotFound(kundennummer);
			}
		}
	}
	throw NotFound(kundennummer);
}

template <unsigned bucketSize>
void HashDat<bucketSize>::show(ostream& outstream) {
	filestream.clear();
	unsigned position = 0;
	while (bucket.read(filestream, position)) {
		for (int i = 0; i < bucketSize; i++) {
			const Kunde& c = bucket.slots[i];
			if (c.getKundennummer() != 0) {
				c.output(outstream);
				outstream << endl;
			} else {
				// Optimierung, weil ja immer erster freier Slot gefuellt wird.
				// Wenn Slot leer ist, wird im aktuellen Bucket nichts mehr kommen.
				break;
			}
		}
		position += BUCKET_SIZE_IN_BYTES;
	}
}

template <unsigned bucketSize>
void HashDat<bucketSize>::exitWithIOError() const {
	cerr << "Error while reading or writing file: " << filename << endl
	     << "Exit program." << endl;
	exit(EXIT_FAILURE);
}

template <unsigned bucketSize>
unsigned HashDat<bucketSize>::generateHashValue(int kundennummer) const {
	return kundennummer % BUCKET_COUNT;
}
template <unsigned bucketSize>
void HashDat<bucketSize>::test() {
	class A { };
	cout << "size of one bucket: " << BUCKET_SIZE_IN_BYTES << endl;
	cout << "size of int: " << sizeof (int) << endl;
	cout << "size of empty class: " << sizeof (A) << endl;
	A a;
	cout << "empty class as character: " << (int) *((char*) &a) << endl;
//	bucket.write(filestream, 0);
//	bucket.write(filestream, BUCKET_SIZE_IN_BYTES);
//	bucket.write(filestream, 2 * BUCKET_SIZE_IN_BYTES);
//	if (!filestream) cout << "einfaches write mit leerem bucket geht nicht" << endl;
	bucket.read(filestream, 0); if (!filestream) cout << "read 0 geht nicht" << endl; bucket.slots[0].output(cout);
	bucket.read(filestream, BUCKET_SIZE_IN_BYTES); if (!filestream) cout << "read 1 geht nicht" << endl; bucket.slots[0].output(cout);
	bucket.read(filestream, 2 * BUCKET_SIZE_IN_BYTES); if (!filestream) cout << "read 2 geht nicht" << endl; bucket.slots[0].output(cout);
}


#endif /* HASHDAT_H_ */
