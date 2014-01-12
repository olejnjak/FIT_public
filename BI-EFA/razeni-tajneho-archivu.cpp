void findMinMax (CSecretStorage& storage, CSecretItem ** min, CSecretItem ** max) {
	int size = storage.getSize();
	if (size) *min = *max = &storage.read(0);	
	
	for (int i = 1; i < size; i++) {
		CSecretItem& item = storage.read(i);
		if (item < **min) *min = &item;
		else if (item > **max) *max = &item;
	}
}

bool validateBucks (int * bsa, int bucks, int bucket_size) {
	for (int i = 0; i < bucks; i++) {
		if (bsa[i]>bucket_size) {
			return false;
		}
	}
		
	return true;
}

int * getBucketSize (CSecretStorage& storage, int bucks, CSecretItem& min, CSecretItem& max) {
	int * array = new int[bucks];
	for (int i = 0; i < bucks; i++) array[i] = 0;
	
	for (int i = 0; i < storage.getSize(); i++) {
		CSecretItem& item = storage.read(i);
		int index = item.getBucketIndex(min, max, bucks);
		array[index]++;
	}
	
	return array;
}

void SecretSort (CSecretStorage& storage) {
	//find minimal and maximal item
	CSecretItem *min,*max = NULL;
	findMinMax(storage, &min, &max);

//	const int BUCKET_SIZE = 20;
	int bucks = 0;
	int * bsa;
	
	//determine number of buckets
//	while (1) {
		//bucks += storage.getSize() / 5;
//		bucks += 25;
		bucks = storage.getSize();
		/*if (bucks >= storage.getSize()) {
			bucks = storage.getSize();
			bsa = getBucketSize(storage, bucks, *min, *max);
		}*/
		bsa = getBucketSize(storage, bucks, *min, *max);
		
//		if (validateBucks(bsa, bucks, BUCKET_SIZE)) break;
//		delete [] bsa;
//	}
	
	//create bucket storage
	CSecretStorage ** sa = new CSecretStorage*[bucks];
	for (int i = 0; i < bucks; i++) {
		if (bsa[i] > 20) continue;
		sa[i] = new CSecretStorage(bsa[i]);
	}
	
	//copy data to buckets
	//initialize substorage indexes
	int * sub_idx = new int[bucks];
	for (int i = 0; i < bucks; i++) sub_idx[i] = 0;
	
	
	for (int i = 0; i < storage.getSize(); i++) {
		CSecretItem& item = storage.read(i);
		int index = item.getBucketIndex(*min, *max, bucks);
		sa[index]->write(sub_idx[index]++, item);
	}
	
	//sort data calling stable sort
	for (int i = 0; i < bucks; i++) CallStableSort(*sa[i]);

	//copy data back to original storage
	int storage_idx = 0;
	for (int i = 0; i < bucks; i++) {
		for (int j = 0; j < sa[i]->getSize(); j++) 
			storage.write(storage_idx++, sa[i]->read(j));
	}
	
	//delete allocated memory
	for (int i = 0; i < bucks; i++) delete sa[i];
	delete [] sa;
	delete [] bsa;
	delete [] sub_idx;
}

void swap (CSecretItem& a, CSecretItem& b) {
	CSecretItem tmp (a);
	a = b;
	b = tmp;
}

void StableSort (CSecretStorage& storage) {
	if (storage.getSize() < 2) return;
	if (storage.getSize() > 20) {
		//insert sort z prednasky
		for (int i = 1; i < storage.getSize(); i++) {
			CSecretItem key = storage.read(i);
			int j = i -1;
			bool cycle = false;
			while (j>=0 && (storage.read(j) > key)) {
				cycle = true;
				storage.write(j+1, storage.read(j));
				j--;
			}
			if (cycle) storage.write(j+1, key);
		}
	} else {
		//insert sort z http://www.algolist.net/Algorithms/Sorting/Insertion_sort
    for (int i = 1; i < storage.getSize(); i++) {
			int j = i;
      while (j > 0 && storage.read(j-1) > storage.read(j)) {
      	swap (storage.read(j-1), storage.read(j));
        j--;
      }
    }
	}
}
