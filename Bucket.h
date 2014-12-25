/*
 * Bucket.h
 *
 *  Created on: 28.11.2014
 *      Author: jakob
 */

#ifndef BUCKET_H_
#define BUCKET_H_

#include <fstream>

using namespace std;

#include "Kunde.h"

#define BUCKET_SIZE_IN_BYTES sizeof (Bucket<bucketSize>)

template <unsigned bucketSize>
class Bucket {
public:
	Bucket();
	// iterator zum zugriff auf slots oder so?
	fstream& read(fstream& istream, unsigned position);
	fstream& write(fstream& ostream, unsigned position) const;
	void clear();
	friend class HashDat<bucketSize>; // C++11 class keyword is optional
private:
	Kunde slots[bucketSize]; // array<Kunde, bucketSize> wäre besser, ist aber C++11
};


template <unsigned bucketSize>
Bucket<bucketSize>::Bucket() { }

template <unsigned bucketSize>
fstream& Bucket<bucketSize>::read(fstream& istream, unsigned position) {
	istream.seekg(position);
//	cout << "reading at position " << position << endl;
	if (!istream) cout << "stream is not ok" << endl;
	for (int i = 0; i < bucketSize; i++) {
		slots[i].read(istream);
	}
//	istream.read((char*) this, BUCKET_SIZE_IN_BYTES); // geht auch, ist aber evtl. nicht portable.
	// Wenn ich gewusst hätte, dass bei C++ jedes Objekt auch Verwalungsinfos enthält,
	// haette ich statt `Kunde slots[]` `vector<Kunde> slots` verwendet.
	return istream;
}

template <unsigned bucketSize>
fstream& Bucket<bucketSize>::write(fstream& ostream, unsigned position) const {
	ostream.seekp(position);
//	cout << "writing at position " << position << endl;
	for (int i = 0; i < bucketSize; i++) {
		slots[i].write(ostream);
	}
//	ostream.write((char*) this, BUCKET_SIZE_IN_BYTES); // geht auch, ist aber evtl. nicht portable
	return ostream;
}

template <unsigned bucketSize>
void Bucket<bucketSize>::clear() {
	for (int i = 0; i < bucketSize; i++) {
		slots[i] = Kunde();
	}
}


#endif /* BUCKET_H_ */
