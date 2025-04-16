// 10927108 劉珈彤

# include <iostream>
# include <fstream>
# include <string.h>
# include <vector>
# include <memory.h>

using namespace std ;
typedef char Str10[10] ;
typedef char Str50[50] ;

//---------------struct---------------
struct SData {
	Str10 sid ;
	Str10 sname ;
	
	unsigned char score1 ;
	unsigned char score2 ;
	unsigned char score3 ;
	unsigned char score4 ;
	unsigned char score5 ;
	unsigned char score6 ;
	
	float average ;
};

struct Table {
	Str10 sid ;
	Str10 sname ;
	int fish ;
	
	unsigned char score1 ;
	unsigned char score2 ;
	unsigned char score3 ;
	unsigned char score4 ;
	unsigned char score5 ;
	unsigned char score6 ;
	
	float average ;
};

typedef struct SData SData ;
typedef struct Table Table ;

//---------------function---------------
void Initialize( Str50 &openfile1, Str50 &openfile2, Str50 &tempfile, bool &havefile, int &command ) ;  // 初始化 
void ReadAndStore( Str10 openfile1, FILE* fin, SData data ) ;  // txt轉bin 
int findPrime( int n ) ;  // 找最小質數 
int isPrime( int n ) ;
unsigned long long int GetASCII( vector<SData> dataVector, int i ) ; // vector版的GetASCII
unsigned long long int GetASCII2( Table hashTable[], int i ) ;	// array版的GetASCII
void InputToTableX( Table hashTable[], vector<SData> dataVector, int i, int fish, int temp, int prime ) ;  // 將資料丟進TableX 
void InputToTableY( Table hashTable[], vector<SData> dataVector, int i, int fish, int temp, int prime ) ;  // 將資料丟進TableY
void InputToTableZ( Table hashTable[], vector<SData> dataVector, int i, int fish, int prime ) ;  // 將資料丟進TableZ
void FoutTXT( Str50 openfile2, int num, Table hashTable[], int prime ) ;  // 輸出txt檔 

//---------------main---------------
int main() {
	int command = -1 ;
	int mission2 = 0 ;
	Str50 openfile1 = {'\0'}, openfile2 = {'\0'}, tempfile = {'\0'} ;
	bool havefile = false ;
	SData data ;
	FILE * fin ;
	vector<SData> dataVector ;
	
	do {
		printf( "\n**** Hash Table Builder ****\n" ) ;
		printf( "* 0. QUIT                  *\n" ) ;
		printf( "* 1. Linear Probing        *\n" ) ;
		printf( "* 2. Double Hash           *\n" ) ;
		printf( "* 3. Quadratic Probing     *\n" ) ;
		printf( "****************************\n" ) ;
		printf( "Input a choice(0, 1, 2, 3 ): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
		if ( command == 0 ) return 0 ;
		dataVector.clear() ; // initialize vector
		//---------------------------確認有無txt或bin檔----------------------------------
		if ( command > 0 && command < 4 ) { 
			printf( "\nInput a file number ([0] Quit): " ) ;
			scanf( "%s", tempfile ) ;
			if ( strcmp( tempfile, "0" ) != 0 ) {
				strcpy( openfile1, "input" ) ;
				strcat( openfile1, tempfile ) ;
				strcat( openfile1, ".bin" ) ;
								
				if ( !( fin = fopen( openfile1, "r" ) ) ) {  // 如果沒有bin檔
					strcpy( openfile2, "input" ) ;
					strcat( openfile2, tempfile ) ;
					strcat( openfile2, ".txt" ) ;
					
					if ( fin = fopen( openfile2, "r" ) ) {
						ReadAndStore( openfile1, fin, data ) ;  // 儲成struct轉成bin檔
						havefile = true ;
						fclose( fin ) ;
					} // if
					else {
						printf( "\n### %s does not exist! ###\n", openfile1 ) ;
						printf( "### %s does not exist! ###\n", openfile2 ) ;
					} // else
					
				} // if
				else {
					havefile = true ;
					fclose( fin ) ;
				} // else		
			} // if()		
		} // if()
		
		//---------------------------將bin檔讀成一個個struct並存進vector-----------------
		if ( havefile ) {
			fin = fopen( openfile1, "rb" ) ;
			while ( !feof( fin ) ) {
				fread( &data, sizeof( SData ), 1, fin ) ;  // 要讀入的地方，讀入類型，讀入多少筆，哪個資料夾
				dataVector.push_back( data ) ;
			} // while()
			
			dataVector.pop_back() ;  // 尾端會多最後一筆同樣資料的 
			fclose( fin ) ;
		} // if
		
		//---------------------------Mission---------------------------------------------
		if ( command == 1 ) { // Mission1 
			if ( havefile ) {
				int prime = findPrime( dataVector.size() * 1.2 ) ;  // 大於資料數1.2倍的最小質數 
				Table hashTable[prime] ;
				//---------------------------初始化strcut array--------------------------
				for ( int i = 0 ; i < prime ; i++ ) {
					hashTable[i].fish = -1 ;
				} // for
				//---------------------------linear probe--------------------------------
				for ( int i = 0 ; i < dataVector.size() ; i++ ) {
					unsigned long long int fish = GetASCII( dataVector, i ) % prime ;
					int hash = GetASCII( dataVector, i ) % prime ;
					InputToTableX( hashTable, dataVector, i, fish, hash, prime ) ; // hash into TableX
				} // for()
				//---------------------------算successful--------------------------------
				float success = 0 ;
				for ( int i = 0 ; i < prime ; i++ ) {
					///---------------------------不是原來放的位置------------------------
					if ( hashTable[i].fish != i && hashTable[i].fish != -1 ) {
						//---------------------------計算走了多少------------------------
						int temp = i ;
						while ( hashTable[i].fish != temp ) {
							temp-- ;
							if ( temp < 0 ) {
								temp = prime - 1 ;
							}
							
							success++ ;
						} // while
						
						success++ ;
					} // if
					else if ( hashTable[i].fish == i ) {
						success++ ;
					} // else if	

				} // for

        //---------------------------算unsuccessful------------------------------
				float unSuccess = 0 ;
				for ( int i = 0 ; i < prime ; i++ ) {
					//---------------------------計算走了多少----------------------------
					int temp = i ;
					while ( hashTable[temp].fish != -1 ) {  // temp這格不為空 
						temp++ ;
						if ( temp > prime - 1 ) {
							temp = 0 ;
						}
						
						unSuccess++ ;
					} // while

				} // for				
						
				printf( "Hash Table X has been created.\n" ) ;
				printf( "unsuccessful search: %3.4f comparisons on average\n", unSuccess / prime ) ;
				printf( "successful search: %3.4f comparisons on average\n", success / dataVector.size() ) ;
				
				//---------------------------建txt---------------------------------------
				strcpy( openfile2, "linear" ) ;
				strcat( openfile2, tempfile ) ;
				strcat( openfile2, ".txt" ) ;
				
				FoutTXT( openfile2, 1, hashTable, prime ) ;
			} // if	

		} // if
		else if ( command == 2 ) { // Mission2
			if ( havefile ) {
				int prime = findPrime( dataVector.size() * 1.2 ) ;  // 大於資料數1.2倍的最小質數 
				int step = findPrime( dataVector.size() / 3 ) ;  // 最高步階 
				Table hashTable[prime] ;
				//---------------------------初始化strcut array--------------------------
				for ( int i = 0 ; i < prime ; i++ ) {
					hashTable[i].fish = -1 ;
				} // for
				//---------------------------double hash---------------------------------
				for ( int i = 0 ; i < dataVector.size() ; i++ ) {
					unsigned long long int fish = GetASCII( dataVector, i ) % prime ;
					int temp = GetASCII( dataVector, i ) % step ;
					temp = step - temp ;
					InputToTableY( hashTable, dataVector, i, fish, temp, prime ) ; // hash into TableY
				} // for()
				//---------------------------算successful--------------------------------
				float total = 0 ;
				for ( int i = 0 ; i < prime ; i++ ) {
					//---------------------------不是原來放的位置------------------------
					if ( hashTable[i].fish != i && hashTable[i].fish != -1 ) {
						//---------------------------計算step(key)-----------------------
						mission2 = GetASCII2( hashTable, i ) % step ;
						mission2 = step - mission2 ;
						//---------------------------計算走了多少------------------------
						int temp = hashTable[i].fish ;
						while ( temp != i ) {  // 用key找現在位置 
							temp = ( temp + mission2 ) % prime ; 
							total++ ;
						} // while
						
						total++ ;
					} // if
					else if ( hashTable[i].fish == i ) {
						total++ ;
					} // else if
					
				} // for
				
				printf( "Hash Table Y has been created.\n" ) ;
				printf( "successful search: %3.4f comparisons on average\n", total / dataVector.size() ) ;
				
				//---------------------------建txt---------------------------
				strcpy( openfile2, "double" ) ;
				strcat( openfile2, tempfile ) ;
				strcat( openfile2, ".txt" ) ;
				
				FoutTXT( openfile2, 2, hashTable, prime ) ; 
			} // if
		} // else if
		else if ( command == 3 ) { // Mission3
			if ( havefile ) {
				int prime = findPrime( dataVector.size() * 1.2 ) ;  // 大於資料數1.2倍的最小質數
				Table hashTable[prime] ;
				//---------------------------初始化strcut array--------------------------
				for ( int i = 0 ; i < prime ; i++ ) {
					hashTable[i].fish = -1 ;
				} // for
				//---------------------------Quadratic hash---------------------------------
				for ( int i = 0 ; i < dataVector.size() ; i++ ) {
					unsigned long long int fish = GetASCII( dataVector, i ) % prime ;
					InputToTableZ( hashTable, dataVector, i, fish, prime ) ;  // hash into TableZ
				} // for()
				
				//---------------------------算successful--------------------------------
				float success = 0 ;
				for ( int i = 0 ; i < prime ; i++ ) {
					//---------------------------不是原來放的位置------------------------
					if ( hashTable[i].fish != i && hashTable[i].fish != -1 ) {
						//---------------------------計算走了多少------------------------
						int temp = i ;
						int times = 1 ;
						
						while ( hashTable[i].fish != temp ) {
							temp = ( i - times * times ) ;
							
							if ( temp < 0 ) {
								temp = temp % prime + prime ;
							} // if
							
							times++ ;
							success++ ;
						} // while
						
						success++ ;
					} // if
					else if ( hashTable[i].fish == i ) {
						success++ ;
					} // else if
					
				} // for
				
				//---------------------------算unsuccessful------------------------------
				float unSuccess = 0 ;
				for ( int i = 0 ; i < prime ; i++ ) {
					//---------------------------計算走了多少----------------------------
					int temp = i ;
					int times = 1 ;
					int count = temp ;
					while ( hashTable[count].fish != -1 ) {  // temp這格不為空 
						count = ( temp + times * times ) % prime ;

						times++ ;
						unSuccess++ ;
					} // while

				} // for

				printf( "Hash Table Z has been created.\n" ) ;
				printf( "unsuccessful search: %3.4f comparisons on average\n", unSuccess / prime ) ;
				printf( "successful search: %3.4f comparisons on average\n", success / dataVector.size() ) ;
				
				//---------------------------建txt---------------------------
				strcpy( openfile2, "quadratic" ) ;
				strcat( openfile2, tempfile ) ;
				strcat( openfile2, ".txt" ) ;
				
				FoutTXT( openfile2, 3, hashTable, prime ) ;
				
			} // if
			
		} // else if

		Initialize( openfile1, openfile2, tempfile, havefile, command ) ;  // initialize data
	} while ( command != 0 ) ;
	
} // main()

//---------------function---------------
void Initialize( Str50 &openfile1, Str50 &openfile2, Str50 &tempfile, bool &havefile, int &command ) { // initialize data
	command = -1 ;
	havefile = false ;
	for ( int i = 0 ; i < 10 ; i++ ) {
		openfile1[i] = '\0' ;
		openfile2[i] = '\0' ;
		tempfile[i] = '\0' ;
	} // for()
	
} // Initialize()

void ReadAndStore( Str10 openfile1, FILE* fin, SData data ) {
	FILE* fout ;
	fout = fopen( openfile1, "wb+" ) ;
	Str10 temp = {"\0"} ;
	
	while ( !feof( fin ) ) {
		if ( fscanf( fin, "%s", data.sid ) != EOF ) {
			
			for ( int i = 0 ; i < 10 ; i++ ) data.sname[i] = '\0' ;
			fscanf( fin, "%c", temp ) ;
      fscanf( fin, "%c", temp ) ;
      while ( strcmp( temp, "\t" ) != 0 ) {
        strcat( data.sname, temp ) ;
        fscanf( fin, "%c", temp ) ;
			} // while()
			
			fscanf( fin, "%u%u%u", &data.score1, &data.score2, &data.score3 ) ;
			fscanf( fin, "%u%u%u", &data.score4, &data.score5, &data.score6 ) ;
			fscanf( fin, "%f", &data.average ) ;
			
			fwrite( &data, sizeof( SData ), 1, fout ) ;
		} // if
		
	} // while()
	
	fclose( fout ) ;
} // ReadAndStore()

int findPrime( int n ) {  // 算雜湊表大小 
    int i = n + 1 ;
    while( 1 ) {
        if( isPrime( i ) ) {
        	break;
		} // if
		
        i++;
    } // while()
    
    return i;
    
} // findPrime()

int isPrime( int n ) {
    int i, j = 0 ;
    for( i = 1 ; i <= n ; i++ ) {
        if( n % i == 0 ) {
        	j++;
		} // if
    
    } // for 
    
    if( j == 2 ) {
        return 1 ;
    } // if
    else if( j > 2 ) {
        return 0 ;
    } // else if
    
} // isPrime()

unsigned long long int GetASCII( vector<SData> dataVector, int i ) {  // ASCII編碼相乘 
	int n = 0 ;
	unsigned long long int ans = 1 ;
	
	while ( dataVector[i].sid[n] != '\0' ) {
		ans = ans * dataVector[i].sid[n] ;
		n++ ;
	} // while
	
	
	return ans ;
} // GetASCII()

unsigned long long int GetASCII2( Table hashTable[], int i ) {
	int n = 0 ;
	unsigned long long int ans = 1 ;
	
	while ( hashTable[i].sid[n] != '\0' ) {
		ans = ans * hashTable[i].sid[n] ;
		n++ ;
	} // while
	
	
	return ans ;
} // GetASCII()

void InputToTableX( Table hashTable[], vector<SData> dataVector, int i, int fish, int temp, int prime ) { // linear probing into TableX 
	if ( hashTable[fish].fish == -1 ) {
		strcpy( hashTable[fish].sid, dataVector[i].sid ) ;
		strcpy( hashTable[fish].sname, dataVector[i].sname ) ;
		hashTable[fish].fish = fish ;
		
		hashTable[fish].score1 = dataVector[i].score1 ;
		hashTable[fish].score2 = dataVector[i].score2 ;
		hashTable[fish].score3 = dataVector[i].score3 ;
		hashTable[fish].score4 = dataVector[i].score4 ;
		hashTable[fish].score5 = dataVector[i].score5 ;
		hashTable[fish].score6 = dataVector[i].score6 ;
	
		hashTable[fish].average = dataVector[i].average ;
	} // if
	else {
		int key = fish ;
		
		while ( hashTable[fish].fish != -1 ) {
			fish++ ;
			if ( fish > prime ) fish = 0 ;
		} // while
		
		strcpy( hashTable[fish].sid, dataVector[i].sid ) ;
		strcpy( hashTable[fish].sname, dataVector[i].sname ) ;
		hashTable[fish].fish = key ;
		
		hashTable[fish].score1 = dataVector[i].score1 ;
		hashTable[fish].score2 = dataVector[i].score2 ;
		hashTable[fish].score3 = dataVector[i].score3 ;
		hashTable[fish].score4 = dataVector[i].score4 ;
		hashTable[fish].score5 = dataVector[i].score5 ;
		hashTable[fish].score6 = dataVector[i].score6 ;
	
		hashTable[fish].average = dataVector[i].average ;
	} // else
	
} // InputToTableX()

void InputToTableY( Table hashTable[], vector<SData> dataVector, int i, int fish, int temp, int prime ) { // double hashing into TableY
	if ( hashTable[fish].fish == -1 ) {
		strcpy( hashTable[fish].sid, dataVector[i].sid ) ;
		strcpy( hashTable[fish].sname, dataVector[i].sname ) ;
		hashTable[fish].fish = fish ;
		
		hashTable[fish].score1 = dataVector[i].score1 ;
		hashTable[fish].score2 = dataVector[i].score2 ;
		hashTable[fish].score3 = dataVector[i].score3 ;
		hashTable[fish].score4 = dataVector[i].score4 ;
		hashTable[fish].score5 = dataVector[i].score5 ;
		hashTable[fish].score6 = dataVector[i].score6 ;
	
		hashTable[fish].average = dataVector[i].average ;
	} // if
	else {
		int key = fish ;
		fish = ( fish + temp ) % prime ;
		
		while ( hashTable[fish].fish != -1 ) {
			fish = ( fish + temp ) % prime ;
		} // while
		
		strcpy( hashTable[fish].sid, dataVector[i].sid ) ;
		strcpy( hashTable[fish].sname, dataVector[i].sname ) ;
		hashTable[fish].fish = key ;
		
		hashTable[fish].score1 = dataVector[i].score1 ;
		hashTable[fish].score2 = dataVector[i].score2 ;
		hashTable[fish].score3 = dataVector[i].score3 ;
		hashTable[fish].score4 = dataVector[i].score4 ;
		hashTable[fish].score5 = dataVector[i].score5 ;
		hashTable[fish].score6 = dataVector[i].score6 ;
	
		hashTable[fish].average = dataVector[i].average ;
	} // else
	
} // InputToTableY()

void InputToTableZ( Table hashTable[], vector<SData> dataVector, int i, int fish, int prime ) { // Quadratic hashing into TableZ
	if ( hashTable[fish].fish == -1 ) {
		strcpy( hashTable[fish].sid, dataVector[i].sid ) ;
		strcpy( hashTable[fish].sname, dataVector[i].sname ) ;
		hashTable[fish].fish = fish ;
		
		hashTable[fish].score1 = dataVector[i].score1 ;
		hashTable[fish].score2 = dataVector[i].score2 ;
		hashTable[fish].score3 = dataVector[i].score3 ;
		hashTable[fish].score4 = dataVector[i].score4 ;
		hashTable[fish].score5 = dataVector[i].score5 ;
		hashTable[fish].score6 = dataVector[i].score6 ;
	
		hashTable[fish].average = dataVector[i].average ;
	} // if
	else {
		int times = 1 ;  // 第幾次平方 
		int count = ( fish + times * times ) % prime ;  // 運算需要 
		
		while ( hashTable[count].fish != -1 ) {
			times++ ;
			count = ( fish + times * times ) % prime ;
			
			if ( times == prime - 1 ) {
				printf( "### Failed at [%d]. ###\n", count ) ;
				return ;
			} 
			
		} // while
		
		strcpy( hashTable[count].sid, dataVector[i].sid ) ;
		strcpy( hashTable[count].sname, dataVector[i].sname ) ;
		hashTable[count].fish = fish ;
		
		hashTable[count].score1 = dataVector[i].score1 ;
		hashTable[count].score2 = dataVector[i].score2 ;
		hashTable[count].score3 = dataVector[i].score3 ;
		hashTable[count].score4 = dataVector[i].score4 ;
		hashTable[count].score5 = dataVector[i].score5 ;
		hashTable[count].score6 = dataVector[i].score6 ;
	
		hashTable[count].average = dataVector[i].average ;
	} // else
	
} // InputToTableY()

void FoutTXT( Str50 openfile2, int num, Table hashTable[], int prime ) {
	FILE * fin = fopen( openfile2, "w+" ) ;
	if ( num == 1 ) { // Mission1
		fprintf( fin, " --- Hash Table X --- (linear probing)\n" ) ;
	} // if
	else if ( num == 2 ) { // Mission2
		fprintf( fin, " --- Hash Table Y --- (double hashing)\n" ) ;
	} // else if
	else if ( num == 3 ) { // Mission3
		fprintf( fin, " --- Hash Table Z --- (quadratic probing)\n" ) ;
	} // else if
	
	for ( int i = 0 ; i < prime ; i++ ) {
		fprintf( fin, "[%3d]", i ) ;
		if ( hashTable[i].fish != -1 ) {
			fprintf( fin, "%11d,", hashTable[i].fish ) ;
			fprintf( fin, "%11s,", hashTable[i].sid ) ;
			fprintf( fin, "%11s,", hashTable[i].sname ) ;
			fprintf( fin, "%11g\n", hashTable[i].average ) ;
		} // if()
		else {
			fprintf( fin, "\n" ) ;
		} // else
		
	} // for
				
	fclose( fin ) ;
} // FoutTXT()