struct TPARAM {
	void (* officer) ( const TMESSAGE *, TRESULTS *, int );
	const unsigned char (*keys)[KEY_LENGTH];
};

class Results { //trida pro reprezentaci hotovy jobu pro kazdou zpravu
	TRESULTS * m_Array; //pole vysledku
	int m_ArrayItems; //pocet prvku v poli vysledku
	int m_ArrayItemsMax; //maximalni velikost pole
	int m_JobsDone; //pocet dokoncenych jobu pro zpravu
	int m_Jobs;
	pthread_mutex_t m_Mutex; //mutex pro synchronizaci vlaken pracujicich na stejne zprave
	
	void enlargeArray() { //metoda pro zvetseni pole
		pthread_mutex_lock(&m_Mutex); //zamknu mutex, aby nikdo jiny na pole nesahal
			m_ArrayItemsMax *= 2; //zvetsim maximum
			m_Array = (TRESULTS*) realloc(m_Array, m_ArrayItemsMax*sizeof(TRESULTS));
		pthread_mutex_unlock(&m_Mutex); //odemknu mutex
	}
	
	public: 
		Results (int agents, int length) { //konstruktor implicitne alokuje pole pro 10 vysledku
			m_Array = (TRESULTS*) malloc (10*sizeof(TRESULTS));
			m_ArrayItems = 0;
			m_ArrayItemsMax = 10;
			m_JobsDone = 0;
			m_Jobs = agents*length;
			pthread_mutex_init(&m_Mutex, NULL);
		}
		~Results() { //destruktor uvolni alokovane prostredky
			free(m_Array);
			pthread_mutex_destroy(&m_Mutex);
		}
		
		void JobDone () { //metoda zaregistruje neuspesne dokonceny job (desifrovany text neobsahuje podpis)
			m_JobsDone++;
		}
		
		void AddResult(TRESULTS result) { //metoda vlozi uspesne desifrovany text do pole vysledku a zvedne citac dokoncenych jobu
			if (m_ArrayItems == m_ArrayItemsMax) enlargeArray(); //zkontroluji, zda pole neni plne
			m_Array[m_ArrayItems++] = result; //vlozim novy prvek a inkrementuji citac prvku v pole
			m_JobsDone++; //inkrementuji citac dokoncenych jobu
		}
		
		TRESULTS * ReturnArray(int & count) { //metoda vrati pocet prvku v poli vysledku a pole samotne jako vystupni parametr
			count = m_ArrayItems;
			return m_Array;
		}
		
		bool Finished() { //metoda rozhodne, zda je desifrovani zpravy hotove
			return (m_JobsDone == m_Jobs);
		}
		
		void Lock() { //umozni zamknout mutex
			pthread_mutex_lock(&m_Mutex);
		}
		void Unlock() { //umozni odemknout mutex
			pthread_mutex_unlock(&m_Mutex);
		}
};

struct QueueItem { //struktura reprezentuje polozky fronty pro zpracovani
	const TMESSAGE * m_Message;
	int m_Key, m_ShiftFrom, m_ShiftTo;
	Results* m_Results;
	
	public: 	
		QueueItem(const TMESSAGE * message = NULL, int key = 0, int shiftFrom = 0, int shiftTo = 0, Results* results = NULL) {
			m_Message = message;
			m_Key = key;
			m_ShiftFrom = shiftFrom;
			m_ShiftTo = shiftTo;
			m_Results = results;
		}
};

class Queue { //trida realizujici frontu jobu
	QueueItem ** m_Array;
	int m_Begin, m_Size, m_MaxSize;
	pthread_mutex_t m_Mutex; //mutex pro synchronizaci
	sem_t m_SemFull, m_SemFree; //semafory pro synchronizaci
	
	public:
		Queue(int jobs) { //konstruktor inicializuje clenske promenne
			pthread_mutex_init(&m_Mutex, NULL);
			sem_init(&m_SemFull, 0, 0);
			sem_init(&m_SemFree, 0, jobs-1);
			m_Begin = 0;
			m_Size = 0;
			m_MaxSize = jobs;
			m_Array = new QueueItem*[jobs];
		}
		~Queue() { //destruktor uvolni prostredky
			pthread_mutex_destroy(&m_Mutex);
			sem_destroy(&m_SemFull);
			sem_destroy(&m_SemFree);
			delete [] m_Array;
		}
		
		void Insert(QueueItem * ins) {
			sem_wait(&m_SemFree); //pokud neni volne misto uspim se
			pthread_mutex_lock(&m_Mutex);
				if (m_Size == 0) {
					m_Begin = 0;
					m_Array[m_Begin] = ins;
					m_Size++;
				} else {
					m_Array[(m_Begin + m_Size++) % m_MaxSize] = ins;
				}
			pthread_mutex_unlock(&m_Mutex);
			sem_post(&m_SemFull);
		}
		
		QueueItem * Get () {
			QueueItem * ret;
			sem_wait(&m_SemFull);
			pthread_mutex_lock(&m_Mutex);
				ret = m_Array[m_Begin];
				m_Begin++;
				m_Size--;
				m_Begin %= m_MaxSize;
			pthread_mutex_unlock(&m_Mutex);
			sem_post(&m_SemFree);
			return ret;
		}
};

Queue * g_Queue;

bool decrypt (const TMESSAGE * mess, int shift, const unsigned char * key) {
	int last_xor, remember=0;
	char * pole = new char[strlen(SIGNATURE)+1];
	int length = 0;
	length = mess->m_Length;
	remember = length - strlen(SIGNATURE);

	for (int j = 0, k =0, klic = shift; j < length; j++,k++,klic++) { //projdu a desifruju zpravu
		if (klic == length) klic = 0;
		if (k == KEY_LENGTH) k = 0;
		int x;
		if (j > 0) x = key[k] ^ j ^ last_xor;
		else x = key[k] ^ j;
		char c = x ^ mess->m_Message[klic];
		if (j >= remember) pole[j - remember] = c;
		last_xor = x;
	}
	pole[strlen(SIGNATURE)] = 0;
	if (!strcmp(pole, SIGNATURE)) {
		delete [] pole;
		return true;
	}
	delete [] pole;
	return false;
}

void * thrFunc(void * param) {
	TPARAM * params = (TPARAM*) param;
	const unsigned char (* keys)[KEY_LENGTH] = params->keys;
	void (* officer) ( const TMESSAGE *, TRESULTS *, int ) = params->officer;
	
	while (1) {
		QueueItem * get = g_Queue->Get();
		if (get == NULL) return NULL;
		
		while(get->m_ShiftFrom != get->m_ShiftTo) {
			bool valid = decrypt(get->m_Message, get->m_ShiftFrom, keys[get->m_Key]);
			TRESULTS result;
			if (valid) {
				result.m_Agent = get->m_Key;
				result.m_Shift = get->m_ShiftFrom;
			}
				
			get->m_Results->Lock();
				if (valid) get->m_Results->AddResult(result);
				else get->m_Results->JobDone();
			bool done = get->m_Results->Finished();
			get->m_Results->Unlock();
		
			if (done) {
				int count = 0;
				TRESULTS * array = get->m_Results->ReturnArray(count);
				officer(get->m_Message, array, count);
				delete get->m_Results;
				break;
			}
			get->m_ShiftFrom++;
		}
		
		delete get;
	}
	
	return NULL;
}

void SecretService ( int agents, const unsigned char (* keys)[KEY_LENGTH], int threads, const TMESSAGE *(* receiver) ( void ), void (* officer) ( const TMESSAGE *, TRESULTS *, int )) {
	pthread_t * f_threads = new pthread_t[threads];
	pthread_attr_t attr;
	
  pthread_attr_init ( &attr );
  pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );
  
	g_Queue = new Queue (100); //vytvoreni fronty pro 100 jobu
	
	TPARAM * params = new TPARAM;
	params->officer = officer;
	params->keys = keys;
	
	for(int i = 0; i < threads; i++) pthread_create(&f_threads[i], &attr, thrFunc, (void*)params); //vytvoreni vlaken
	pthread_attr_destroy(&attr);

	const TMESSAGE * received; 
	
	while (1) {
		received = receiver();//nacteni prvni zpravy	
		if (received == NULL) break;

		int i,j, length = received->m_Length;
		Results * resultStruct = new Results(agents, length);
		
		int shiftForThread = received->m_Length / threads;
		int remain = received->m_Length % threads;
		
		for(i = 0; i < agents; i++) { //vlozi joby do fronty
			int from, to;
			for (j = 0; j < threads-1; j++) {
				from = j*shiftForThread;
				to = from + shiftForThread;
				QueueItem * item = new QueueItem (received, i, from, to, resultStruct);
				g_Queue->Insert(item);
			}
			from = j*shiftForThread;
			to = from + shiftForThread + remain;
			QueueItem * item = new QueueItem (received, i, from, to, resultStruct);
			g_Queue->Insert(item);
		}
	}
  for (int i = 0; i < threads; i++) g_Queue->Insert(NULL);
	for(int i = 0; i < threads; i++) pthread_join(f_threads[i], NULL);	
	
	delete params;
	delete [] f_threads;
	delete g_Queue;
}
