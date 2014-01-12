//Author: Jakub Olejnik
//jakub.olejnik@email.cz

//Inspired by FAT FS implementation. 
//0 value in table means free, the same value as cluster number means end, other value is the number of the follower.
//In sector 0 is stored number of stored files. 
//In sectors 1-10 is stored array of stored files
//In following sectors (from 11) is stored FAT table to manage free space.
//Allocation unit is set to 8 disk sectors (effective 4kB)

int   FsCreate        (struct TBlkDev * dev);                      //done
int   FsMount         (struct TBlkDev * dev);                      //done
int   FsUmount        (void);                                      //almost done
int   FileOpen        (const char * fileName, int writeMode);      //done
int   FileRead        (int fd, void * buffer, int len);
int   FileWrite       (int fd, const void * buffer, int len);
int   FileClose       (int fd);                                    //done
int   FileDelete      (const char * fileName);                     //done
int   FileFindFirst   (struct TFile * info);                       //done
int   FileFindNext    (struct TFile * info);		                   //done
int   FileSize        (const char * fileName);                     //done

struct FS;
FS * g_FS;
//=================================================================================================================================================================
struct FS {
	struct FD;
	
	friend struct FS::FD;
	static const int SECTORS_IN_CLUSTER = 8;
  static const int CLUSTER_SIZE = SECTOR_SIZE * SECTORS_IN_CLUSTER;

  struct TFileInfo;

  struct FAT {
    int             * m_Table;
    int               m_Size;
    int               m_CurrentPos, m_Start;

                      FAT           (const TBlkDev & dev);
                      ~FAT          (void);
    int               NextFree      (void);                         //return next free cluster
    static int        Make          (const TBlkDev * dev);                //write empty FAT to disk
    int               Store         (const TBlkDev & dev) const;          //save FAT to disk
    int               operator[]    (int idx);
    const int         operator[]    (int idx) const;
    private:
    	void            load          (const TBlkDev & dev);
  };
  
  struct FD {
  	TFileInfo       * m_File;
  	int               m_Bytes;
  	int               m_BytesToEnd;
  	int               m_Write;
  	char            * m_Buffer;
  	int               m_BufferSize, m_BufferPos;
  	int               m_LastSec, m_CurrentSec;
  	
  	                  FD            (TFileInfo * file, int write);
  	                  ~FD           (void);
  	int               Write         (void);
  	void              Read          (void * buffer, int len);
  	void              loadToBuff    (int sec);
  };
  
  struct TFileInfo {
    TFile             m_Info;
    int               m_Sector;
    
                      TFileInfo     (void);
                      TFileInfo     (const char * fileName, int fileSize, int sector);
  };
  
  FD               ** m_Open;
  TFileInfo         * m_Files;
  int                 m_FilesNr;
  mutable int         m_FilesCurr;
  TBlkDev             m_Dev;
  FAT               * m_FAT;
  
  static int          countSectors  (int sizeBytes);
  
  TFileInfo         * findFile      (const char * fileName) const;
  void                physDel       (int sector);
  public:
                      FS            (const TBlkDev * dev);
                      ~FS           (void);
                      
    int               FileSize      (const char * fileName) const;
    int               FindFirst     (struct TFile * info) const;
    int               FindNext      (struct TFile * info) const;
    int               Open          (const char * fileName, int writeMode);
    int               Delete        (const char * fileName);
    int               Close         (int fd);
    int               Write         (int fd, const void * buffer, int len);
    int               Read          (int fd, void * buffer, int len);
                      
	  static int        MyCeil        (double number);
	  static int        MakeFS        (TBlkDev * dev);
};

//=================================================================================================================================================================
int FS::MakeFS (TBlkDev * dev) {
  int ret = FAT::Make	(dev);
  
  int * buffer = new int[128];
  memset (buffer, 0, 128*sizeof(*buffer));
 	buffer[0] = 0;
 	
 	ret = ret && dev->m_Write(0, buffer, 1);
 	delete [] buffer;
 	return ret;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::countSectors (int sizeBytes) {
	return sizeBytes / SECTOR_SIZE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::MyCeil (double number) {
	int n = number;
	return (number == n ? n : n+1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
FS::FS (const TBlkDev * dev) {
	m_Dev = *dev;
	m_FAT = new FAT(m_Dev);
	m_Files = new TFileInfo[DIR_ENTRIES_MAX];
//  memset(m_Files, 0, sizeof(*m_Files)*DIR_ENTRIES_MAX);
	m_FilesCurr = -1;
	m_Open = new FD*[OPEN_FILES_MAX];
	for (int i = 0; i < OPEN_FILES_MAX; i++) m_Open[i] = NULL;
	
	int * buffer = new int[128];
	dev->m_Read(0, buffer, 1);
	m_FilesNr = buffer[0];
	delete [] buffer;
	dev->m_Read(1, m_Files, 10);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
FS::~FS (void) {
	//SAVE OPENED FILES!!!!
	for (int i = 0; i < OPEN_FILES_MAX; i++) {
		FileClose(i);
	}


	int * buffer = new int[128];
	memset(buffer, 0, 512); //only to silence valgrind, not neccessary
	buffer[0] = m_FilesNr;
	m_Dev.m_Write(0, buffer, 1);
	delete [] buffer;
	m_Dev.m_Write(1, m_Files, 10);
	
	for (int i = 0; i < OPEN_FILES_MAX; i++) delete m_Open[i];
	delete [] m_Open;
  delete [] m_Files;
	m_FAT->Store(m_Dev);
	delete m_FAT;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::FileSize (const char * fileName) const {
	for (int i = 0; i < m_FilesNr; i++) 
	{
		if (!strcmp(m_Files[i].m_Info.m_FileName, fileName)) return m_Files[i].m_Info.m_FileSize;
	}
	return -1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::FindFirst (struct TFile * info) const {
	m_FilesCurr = 0;
	return FindNext(info);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::FindNext (struct TFile * info) const {
	if (m_FilesCurr >= m_FilesNr) return 0;
	*info = m_Files[m_FilesCurr].m_Info;
	m_FilesCurr++; 
	return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::Open (const char * fileName, int writeMode) {
	int fd = -1;
	for (int i = 0; i < OPEN_FILES_MAX; i++)
		if (m_Open[i]==NULL) { fd = i; break; }
	if (fd == -1) return -1;
	if (writeMode) {
		Delete(fileName);
		int sec = m_FAT->NextFree();
		if (sec == -1) { return -1;}
		if (m_FilesNr >= DIR_ENTRIES_MAX) return -1;
		
//		TFileInfo * a = new TFileInfo(fileName, 0, -1);
//		m_Files[m_FilesNr++] = *a;
		strncpy(m_Files[m_FilesNr].m_Info.m_FileName,fileName,29);
		m_Files[m_FilesNr].m_Info.m_FileSize = 0;
		m_Files[m_FilesNr].m_Sector = -1;
		m_Open[fd] = new FD(&m_Files[m_FilesNr],1);
		m_FilesNr++;
	}
	else {
		for (int i = 0; i < m_FilesNr; i++) {
			if (!strcmp(m_Files[i].m_Info.m_FileName, fileName)) {
				m_Open[fd] = new FD(&m_Files[i], 0);
				break;
			}
		}
		if (!m_Open[fd]) return -1;
	}	

	return fd;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::Close (int fd) {
  if (fd < 0 || fd >= OPEN_FILES_MAX) return 0;
	delete m_Open[fd];
	m_Open[fd] = NULL;
	return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::Delete (const char * fileName) {
	for (int i = 0; i < m_FilesNr; i++) {
		if(!strcmp(fileName, m_Files[i].m_Info.m_FileName)) {
			physDel(m_Files[i].m_Sector);
			m_Files[i] = m_Files[--m_FilesNr];
			return 1;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FS::physDel (int sector) {
  int index = sector;
  while(index != g_FS->m_FAT->m_Table[index]) {
    int pom = g_FS->m_FAT->m_Table[index];
    g_FS->m_FAT->m_Table[index] = 0;
    index = pom;
  }
  g_FS->m_FAT->m_Table[index] = 0;
  
/*	if (sector != m_FAT->m_Table[sector] && sector != 0) physDel (m_FAT->m_Table[sector]);
	m_FAT->m_Table[sector] = 0;*/
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::Write (int fd, const void * buffer, int len) {
	int length = len;
	if (fd < 0 || fd >= OPEN_FILES_MAX) return 0;
	if (!m_Open[fd]) return 0;
	
	if (m_Open[fd]->m_Bytes + len <= CLUSTER_SIZE) {
		memcpy(m_Open[fd]->m_Buffer+m_Open[fd]->m_Bytes, buffer, len);
		m_Open[fd]->m_Bytes += len;
	}
	else {
		int left = len+m_Open[fd]->m_Bytes;
		int l = 0;
		while (left >= CLUSTER_SIZE) {
			memcpy (m_Open[fd]->m_Buffer+m_Open[fd]->m_Bytes, (char*)buffer+l, CLUSTER_SIZE-m_Open[fd]->m_Bytes);
  		l += CLUSTER_SIZE-m_Open[fd]->m_Bytes;
			m_Open[fd]->m_Bytes += CLUSTER_SIZE-m_Open[fd]->m_Bytes;
			if (m_Open[fd]->Write() == -1) return l;
			left -= CLUSTER_SIZE-m_Open[fd]->m_Bytes;
			//left -= l;
		}
		memcpy (m_Open[fd]->m_Buffer+m_Open[fd]->m_Bytes, (char*)buffer+l, left);
		m_Open[fd]->m_Bytes += left;
	}
	if (m_Open[fd]->m_Bytes == CLUSTER_SIZE) m_Open[fd]->Write();
	return length;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::Read (int fd, void * buffer, int len) {
  if (!m_Open[fd]) return 0;
  
  int toRead = (len > m_Open[fd]->m_BytesToEnd ? m_Open[fd]->m_BytesToEnd : len);
  /*  	TFileInfo       * m_File;
  	int               m_Bytes;
  	int               m_BytesToEnd;
  	int               m_Write;
  	char            * m_Buffer;
  	int               m_BufferSize, m_BufferPos;
F  	int               m_LastSec, m_CurrentSec;
  	
  	                  FD            (TFileInfo * file, int write);
  	                  ~FD           (void);
  	int               Write         (void);*/
//  	printf("======================\n");
//  	printf("start FILESIZE: %d, toRead=%d\n", m_Open[fd]->m_File->m_Info.m_FileSize, toRead);
  m_Open[fd]->Read(buffer, toRead);
//  printf("end FILESIZE: %d\n", m_Open[fd]->m_File->m_Info.m_FileSize);
  return toRead;  
}
//=================================================================================================================================================================
int FS::FAT::Make (const TBlkDev * dev) {
	int   clusters = dev->m_Sectors / SECTORS_IN_CLUSTER;
	int   sectors  = clusters*sizeof(clusters) / SECTOR_SIZE;
	int * array    = new int[clusters];
	memset(array, 0, clusters*sizeof(clusters));
	
	for (int i = 0; i < 11; i++) array[i] = i;
	int i;
	for (i = 0; i < (dev->m_Sectors / SECTORS_IN_CLUSTER) * (int)sizeof(*array) / SECTOR_SIZE; i++) {
 		array[11+i] = 11+i;
 	}
// 	printf("Service clusters = %d => sectors %d from %d sectors.\n", i, i*SECTORS_IN_CLUSTER, dev->m_Sectors);
	
	int ret = dev->m_Write(11, array, sectors);

  delete [] array;
	return ret;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
FS::FAT::FAT (const TBlkDev& dev) {
	m_Size  = dev.m_Sectors / SECTORS_IN_CLUSTER;
	m_Table = new int[m_Size];
//	m_CurrentPos = 1;
	m_Start = m_CurrentPos = (dev.m_Sectors / SECTORS_IN_CLUSTER) * (int)sizeof(*m_Table) / SECTOR_SIZE + 11;
  load(dev);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::FAT::operator[] (int idx) {
	return m_Table[idx];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
const int FS::FAT::operator[] (int idx) const {
	return m_Table[idx];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
FS::FAT::~FAT () {
  Store(g_FS->m_Dev);
	delete [] m_Table;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FS::FAT::load (const TBlkDev& dev) {
  dev.m_Read(11, m_Table, m_Size*sizeof(*m_Table)/SECTOR_SIZE);	 
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::FAT::NextFree (void) {
/*	for (int i = m_CurrentPos; i <  m_Size; i++) {
		if (m_Table[i] == 0 && i!=0) {
		m_CurrentPos = i;
		return i;
		}
	}
	for (int i = m_Start; i <  m_CurrentPos; i++) {
		if (m_Table[i] == 0 && i!=0) {
		m_CurrentPos = i;
		return i;
		}
	}*/
/*	for (int i = m_CurrentPos, j = 0; j < g_FS->m_Dev.m_Sectors/8; i++,j++) {
	  if (i>=m_Size) i = 0;
	  if (m_Table[i] == 0 && i != 0) {
	    m_CurrentPos = i;
	    return i;
	  }
	}*/
	for (int i = m_CurrentPos, j = 0; j < m_Size; i++,j++) {
	  if (i >= m_Size) i = 1;
	  if (m_Table[i] == 0) {
	    m_CurrentPos = i;
	    return i;
	  }
	}
/*	for (int i = 1; i < m_Size; i++) {
  	if (m_Table[i] == 0) {
	    m_CurrentPos = i;
	    return i;
	  }
	}*/
	m_CurrentPos = 1;
//	for (int i = 1; i < m_Size; i++) fprintf(stderr, "[%d]=>%d==%d: %s\n", i, m_Table[i], 0, (m_Table[i]==0 ? "true" : "false"));
//	WriteFat();
//	fprintf(stderr, "Returning -1\n");
//	exit(1);
	return -1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::FAT::Store (const TBlkDev & dev) const {
	return dev.m_Write(11, m_Table, m_Size*sizeof(*m_Table)/SECTOR_SIZE);
}
//=================================================================================================================================================================
FS::FD::FD (TFileInfo * file, int write) {
  m_Buffer       = new char[CLUSTER_SIZE];
  memset(m_Buffer, 0, CLUSTER_SIZE);
  m_File         = file;
  m_Write        = write;	
  m_Bytes        = 0;
  m_LastSec      = m_CurrentSec = 0;
  m_BytesToEnd   = m_File->m_Info.m_FileSize;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
FS::FD::~FD (void) {
	Write();
	delete [] m_Buffer;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
int FS::FD::Write (void) {
  if (!m_Write) return -1;
  if (!m_Bytes) return -1;
  int sec = g_FS->m_FAT->NextFree();
  if (sec == -1) return -1;
//    g_FS->m_FAT->m_CurrentPos = sec;
  if (!m_LastSec) m_File->m_Sector = sec;
  else g_FS->m_FAT->m_Table[m_LastSec] = sec;
 	g_FS->m_FAT->m_Table[sec] = sec;
  m_File->m_Info.m_FileSize += m_Bytes;
  m_Bytes = 0;
//	char * buf = new char [CLUSTER_SIZE+1];
//	strncpy(buf, m_Buffer, CLUSTER_SIZE);
//	buf[CLUSTER_SIZE] = 0;
  g_FS->m_Dev.m_Write(sec*SECTORS_IN_CLUSTER, m_Buffer, SECTORS_IN_CLUSTER);
  memset(m_Buffer, 0, CLUSTER_SIZE);
  m_LastSec = sec;
  
  return 1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FS::FD::Read(void * buffer, int len) {
/*  	TFileInfo       * m_File;
  	int               m_Bytes;
  	int               m_BytesToEnd;
  	int               m_Write;
  	char            * m_Buffer;
  	int               m_BufferSize, m_BufferPos;
  	int               m_LastSec, m_CurrentSec;
  	
  	                  FD            (TFileInfo * file, int write);
  	                  ~FD           (void);
  	int               Write         (void);*/
  if (!m_CurrentSec) {
  	loadToBuff(m_File->m_Sector);
  }
  if (len + m_Bytes < CLUSTER_SIZE) {
    memcpy(buffer, m_Buffer+m_Bytes, len);
    m_Bytes += len;
    m_BytesToEnd -= len;
  }
  else if (len + m_Bytes == CLUSTER_SIZE) {
  	memcpy(buffer, m_Buffer+m_Bytes, len);
    m_BytesToEnd -= len;
    if (m_CurrentSec != g_FS->m_FAT->m_Table[m_CurrentSec]) loadToBuff(g_FS->m_FAT->m_Table[m_CurrentSec]);
  }
  else if (len + m_Bytes > CLUSTER_SIZE) {
    int remain = CLUSTER_SIZE - m_Bytes;
    memcpy(buffer, m_Buffer+m_Bytes, remain);
    m_BytesToEnd -= remain;
    len -= remain;
    
    int buffShift = remain;
    int count = len / CLUSTER_SIZE;
    int i;
    
    for (i = 0; i < count; i++) {
    	if (m_CurrentSec != g_FS->m_FAT->m_Table[m_CurrentSec]) loadToBuff(g_FS->m_FAT->m_Table[m_CurrentSec]);
    	memcpy((char*)buffer+buffShift, m_Buffer+m_Bytes, CLUSTER_SIZE);
    	len -= CLUSTER_SIZE;
    	m_BytesToEnd -= CLUSTER_SIZE;
    	buffShift += CLUSTER_SIZE;
    }
    if (m_CurrentSec != g_FS->m_FAT->m_Table[m_CurrentSec]) loadToBuff(g_FS->m_FAT->m_Table[m_CurrentSec]);
   	memcpy((char*)buffer+buffShift, m_Buffer+m_Bytes, len);
   	m_Bytes = len;
   	m_BytesToEnd -= len;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FS::FD::loadToBuff (int sec) {
  m_Bytes = 0;
  m_CurrentSec = sec;
  g_FS->m_Dev.m_Read(sec*SECTORS_IN_CLUSTER, m_Buffer, SECTORS_IN_CLUSTER);
}
//=================================================================================================================================================================
FS::TFileInfo::TFileInfo (void) {}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
FS::TFileInfo::TFileInfo (const char * fileName, int fileSize, int sector) {
	strncpy(m_Info.m_FileName, fileName, 29);
	m_Info.m_FileSize = fileSize;
	m_Sector = sector;
}
//*****************************************************************************************************************************************************************
int FsCreate (struct TBlkDev * dev) {
	return FS::MakeFS(dev);
}
int FsMount (struct TBlkDev * dev) {
	g_FS = new FS(dev);
	return 1;	
}
int FsUmount (void) {
	delete g_FS;
	g_FS = NULL;
	return 1;
}

int FileSize (const char * fileName) {
	return g_FS->FileSize(fileName);
}

int FileFindFirst (struct TFile * info) {
	return g_FS->FindFirst(info);
}

int FileFindNext (struct TFile * info) {
	return g_FS->FindNext(info);
}

int FileOpen (const char * fileName, int writeMode) {
	return g_FS->Open(fileName, writeMode);
}

int FileClose (int fd) {
	return g_FS->Close(fd);
}

int FileDelete (const char * fileName) {
	return g_FS->Delete(fileName);
}

int FileWrite (int fd, const void * buffer, int len) {
	return g_FS->Write(fd, buffer, len);
}

int FileRead (int fd, void * buffer, int len) {
	return g_FS->Read(fd, buffer, len);
}
