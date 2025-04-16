// 10927108 劉珈彤

# include <iostream>
# include <fstream>
# include <string>
# include <string.h>
# include <vector>
# include <memory.h>
# include <time.h>

#define NUM 200 

using namespace std ;
typedef char Str10[10] ;
typedef char Str50[50] ;

//---------------struct---------------
struct Data {
	Str10 putID ;
	Str10 getID ;
	float weight ;
};

struct Table {
	float index = 0.0 ;
	int offset ;
};

typedef struct Data SData ;

//---------------function---------------
void Initialize( string &openfile1, string &openfile2, string &openfile3, string &tempfile, bool &havefile, int &command ) ; // ��l��
void Bubble_sort( vector<SData> &dataVector ) ;
void Primary_index( vector<SData> &dataVector, Table primaryIndex[100], int &address, int numRead ) ;

//---------------main---------------
int main() {
	int command = -1 ;
	FILE *fin1, *fin2, *fin3 ;
	string openfile1 = "0", openfile2 = "0", openfile3 = "0", tempfile = "0" ;
	bool havefile = false ;
	SData filedata1, filedata2 ;
	vector<SData> dataVector ;  // 儲存200筆bin檔資料
	vector<SData> primaryVector ;  // 儲存排序檔 
	vector<SData> sortedVector ;  // 儲存排序檔 
	vector<SData> dataVector1, dataVector2 ;  // 緩衝區 
	clock_t start, middle, end ;
	bool runOne = false ;  
	float threshold ;
	
	do {
		cout << endl << "***********************************************" << endl ;
		cout << " On-machine Exercise 05                       *" << endl ;
		cout << " Mission 1: External Merge Sort on a Big File *" << endl ;
		cout << " Mission 2: Construction of Primary Index     *" << endl ;
		cout << "***********************************************" << endl ;
		cout << "########################################################" << endl ;
		cout << "Mission 1: External merge sort" << endl ;
		cout << "########################################################" << endl ;

		runOne = false ;
		while(1) {
			cout << endl << "Input the file name ([0]Quit) : " ;
			cin >> tempfile ;
			//--------------判斷有無此bin檔---------------
			if ( tempfile == "0" ) {
				cout << endl << "[0]Quit or [Any other key]continue? : " ;
				cin >> tempfile ;
				if ( tempfile == "0" ) return 0 ;
				else break ;
			} // if
			else if ( tempfile != "0" ) {
				openfile1 = "pairs" + tempfile + ".bin" ;
				if ( ( fin1 = fopen( openfile1.c_str(), "rb" ) ) ) {
					havefile = true ;
					break ;
				} // if
				else {
					cout << endl << "###" << openfile1 << "does not exist! ###\n" ;
					havefile = false ;
					fclose( fin1 ) ;
				} // else		
			} // else if
		} // while
		
		// ------------------MISSION 1---------------------
		if ( havefile ) {
			runOne = true ;
			int theNumOfFile = 0 ;
			start = clock() ;  // 開始時間 
			while ( 1 ) {
				dataVector.clear() ;
				dataVector.resize( NUM ) ;
				//--------------讀檔&放入vector--------------
				int numRead = fread( (char*)&dataVector[0], sizeof( Data ), dataVector.capacity(), fin1 ) ;  // 要讀入的地方，讀入類型，讀入多少筆，哪個資料夾
				if ( numRead == 0 ) break ;
				
				//--------------泡沫排序--------------
				Bubble_sort( dataVector ) ;
					
				//--------------開新檔&放入--------------
				openfile2 = "sorted" + tempfile + "_0_" + to_string( theNumOfFile ) + ".bin" ;
				fin2 = fopen( openfile2.c_str(), "wb+" ) ;
				fwrite( (char*)&dataVector[0], sizeof( Data ), numRead, fin2 ) ;
					
				//--------------關檔&初始化--------------
				fclose( fin2 ) ;
				theNumOfFile++ ;
			} // while()
			
			dataVector.clear() ;
			fclose( fin1 ) ;
			cout << "\nThe internal sort is completed. Check the initial sorted runs!\n" ;
			middle = clock() ;  // 外部排序結束時間 
			
			//--------------併檔--------------
			int count = 0, times = 0 ;   //  合併第count次, 此筆file為第times個 
			while ( theNumOfFile != 1 ) {  // 合併直到file只有一個為止
				for ( int x = 0 ; x < theNumOfFile ; x = x + 2 ) {  // 倆倆合併
					openfile1 = "sorted" + tempfile + "_" + to_string( count ) + "_" + to_string( x ) + ".bin" ;
					openfile2 = "sorted" + tempfile + "_" + to_string( count ) + "_" + to_string( x+1 ) + ".bin" ;
					if ( x < theNumOfFile - 1 ) {   // 此筆file為普通偶數
						//--------------開檔--------------
						fin1 = fopen( openfile1.c_str(), "rb" ) ;
						fin2 = fopen( openfile2.c_str(), "rb" ) ;
						openfile3 = "sorted" + tempfile + "_" + to_string( count + 1 ) + "_" + to_string( times ) + ".bin" ; 
						fin3 = fopen( openfile3.c_str(), "wb+" ) ;
						
						//--------------初始化vector&&讀第一筆--------------
						int i = 0, j = 0 ;
						dataVector1.clear() ;
						dataVector2.clear() ;
						dataVector1.resize( NUM ) ;
						dataVector2.resize( NUM ) ;
						int numRead1 = fread( (char*)&dataVector1[0], sizeof( Data ), dataVector1.capacity(), fin1 ) ;
						int numRead2 = fread( (char*)&dataVector2[0], sizeof( Data ), dataVector2.capacity(), fin2 ) ;
						
						while ( 1 ) {
							while ( i < numRead1 && j < numRead2 ) {
								if ( dataVector1[i].weight >= dataVector2[j].weight ) {
									fwrite( &dataVector1[i], sizeof( Data ), 1, fin3 ) ;
									i++ ;
								} // if
								else {
									fwrite( &dataVector2[j], sizeof( Data ), 1, fin3 ) ;
									j++ ;
								} // else
								
							} // while()
							
							if ( i == numRead1 ) {  // fin1跑完了 
								dataVector1.clear() ;
								dataVector1.resize( NUM ) ;
								numRead1 = fread( (char*)&dataVector1[0], sizeof( Data ), dataVector1.capacity(), fin1 ) ;
								i = 0 ;
								
								if ( numRead1 == 0 ) {  // 只剩fin2 
									while ( j < numRead2 ) {
										fwrite( &dataVector2[j], sizeof( Data ), 1, fin3 ) ;
										j++ ;
									} // while
									
									dataVector2.clear() ;
									dataVector2.resize( NUM ) ;
									numRead2 = fread( (char*)&dataVector2[0], sizeof( Data ), dataVector2.capacity(), fin2 ) ;
									while ( numRead2 != 0 ) {
										fwrite( (char*)&dataVector2[0], sizeof( Data ), numRead2, fin3 ) ;
										dataVector2.clear() ;
										dataVector2.resize( NUM ) ;
										numRead2 = fread( (char*)&dataVector2[0], sizeof( Data ), dataVector2.capacity(), fin2 ) ;
									} // while
									
									break ;
								} // if
								
							} // if
							else if ( j == numRead2 ) {  // fin2跑完了 
								dataVector2.clear() ;
								dataVector2.resize( NUM ) ;
								numRead2 = fread( (char*)&dataVector2[0], sizeof( Data ), dataVector2.capacity(), fin2 ) ;
								j = 0 ;
								
								if ( numRead2 == 0 ) {  // 只剩fin1
									while ( i < numRead1 ) {
										fwrite( &dataVector1[i], sizeof( Data ), 1, fin3 ) ;
										i++ ;
									} // while
									
									dataVector1.clear() ;
									dataVector1.resize( NUM ) ;
									numRead1 = fread( (char*)&dataVector1[0], sizeof( Data ), dataVector1.capacity(), fin1 ) ;
									while ( numRead1 != 0 ) {
										fwrite( (char*)&dataVector1[0], sizeof( Data ), numRead1, fin3 ) ;
										dataVector1.clear() ;
										dataVector1.resize( NUM ) ;
										numRead2 = fread( (char*)&dataVector1, sizeof( Data ), dataVector1.capacity(), fin1 ) ;
									} // while
									
									break ;
								} // if
								
							} // else if
							
						} // while()
						
						fclose( fin2 ) ;
						remove( openfile2.c_str() ) ;
					} // if
					else if ( x == theNumOfFile - 1 ) {  // 此筆file為最後基數
						fin1 = fopen( openfile1.c_str(), "rb" ) ;
						openfile3 = "sorted" + tempfile + "_" + to_string( count + 1 ) + "_" + to_string( times ) + ".bin" ; 
						fin3 = fopen( openfile3.c_str(), "wb+" ) ;

						while ( 1 ) {
							dataVector1.clear() ;
							dataVector1.resize( NUM ) ;
							int numRead1 = fread( (char*)&dataVector1[0], sizeof( Data ), dataVector.capacity(), fin1 ) ;
							if ( numRead1 == 0 ) break ;
							fwrite( (char*)&dataVector1[0], sizeof( Data ), numRead1, fin3 ) ;
						} // while

					} // else if
					
					fclose( fin1 ) ;
					fclose( fin3 ) ;
					if ( remove( openfile1.c_str() ) ) cout << openfile1 << "\n" ;
					times++ ;
				} // for
				
					
				theNumOfFile = ( theNumOfFile + 1 ) / 2 ;
				cout << "\nNow there are " << theNumOfFile << "runs\n" ;
				times = 0 ;
				count++ ;
			} // while
			
			dataVector1.clear() ;
			dataVector2.clear() ;
			end = clock() ;  // 內部排序結束時間 
			
			//--------------重新命名檔案--------------
			openfile1 = "sorted" + tempfile + ".bin" ;
			fin1 = fopen( openfile1.c_str(), "rb" ) ; 
			if ( fin1 ) {  // 資料夾內有相同檔案
				fclose( fin1 ) ;
				remove( openfile1.c_str() ) ; 
			} // if
			else {
				fclose( fin1 ) ;
			} // else
				
			rename( openfile3.c_str(), openfile1.c_str() ) ;
				
			//--------------cout時間--------------
			cout << "\nThe execution time\n" ;
			cout << "Internal Sort = " << middle - start << " ms\n" ;
			cout << "External Sort = " << end - middle << " ms\n" ;
			cout << "Total Execution Time = " << end - start << " ms\n" ;
		} // if	
		
		Initialize( openfile1, openfile2, openfile3, tempfile, havefile, command ) ; // initialize data.
		
		// ------------------MISSION 2---------------------
		
		if ( runOne ) {
			cout << endl ;
			system("pause") ;
			cout << endl << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl ;
			cout << "Mission 2: Build the primary index" << endl ;
			cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl ;
			
			// --------------判斷有無此bin檔---------------
			if ( tempfile != "0" ) {
				openfile1 = "sorted" + tempfile + ".bin" ;
				if ( ( fin1 = fopen( openfile1.c_str(), "rb" ) ) ) {  // 如果有bin檔
					havefile = true ;
				} // if
				else {
					cout << "\n\n###" << openfile1 << "does not exist! ###\n" ;
					havefile = false ;
					fclose( fin1 ) ;
				} // else

			} // if

		} // if
		
		Table primaryIndex[100] ;
		if ( havefile ) {
			int address = 0 ;
			while ( 1 ) {
				primaryVector.clear() ;
				primaryVector.resize( NUM ) ;
				
				//--------------讀檔&放入vector--------------
				int numRead = fread( (char*)&primaryVector[0], sizeof( Data ), primaryVector.capacity(), fin1 ) ;  // �nŪ�J���a��AŪ�J�����AŪ�J�h�ֵ��A���Ӹ�Ƨ�
				if ( numRead == 0 ) break ;
				//--------------建立主索引--------------
				
				Primary_index( primaryVector, primaryIndex, address, numRead ) ;
			} // while()
			
			cout << endl << "<Primary index>: (key, offset)" << endl ;
			for ( int k = 0 ; k < 100 ; k++ ) {
				cout << "[" << k+1 << "] (" ;
				cout << primaryIndex[k].index << ", " << primaryIndex[k].offset << ")" << endl ;
			}
	
		} // if
		
		Initialize( openfile1, openfile2, openfile3, tempfile, havefile, command ) ; // initialize data.

		// ------------------MISSION 3---------------------

		if ( runOne ) {
			cout << endl ;
			system("pause") ;
			cout << endl << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl ;
			cout << "Mission 3: Threshold search on primary index" << endl ;
			cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl ;
			
			do {
				cout << endl << "Please input a threshold in the range [0,1]: " ;
				cin >> threshold ;
				if ( ! ( threshold > 0 && threshold <= 1 ) ) {
					cout << endl << "The threshold is not in the range [0,1]!" << endl ;
					cout << endl << "[0]Quit or [Any other key]continue?: " ;
					cin >> threshold ;
					if ( threshold == 0 ) break ;
				}
				else {
					if ( tempfile != "0" ) {
						openfile1 = "sorted" + tempfile + ".bin" ;
						if ( ( fin1 = fopen( openfile1.c_str(), "rb" ) ) ) {  // 如果有bin檔
							havefile = true ;
						} // if
						else {
							cout << "\n\n###" << openfile1 << "does not exist! ###\n" ;
							havefile = false ;
							fclose( fin1 ) ;
						} // else
					} // if
					
					int rangeOffset ;
					for ( int i = 0 ; i < 100 ; i++ ) {
						if ( primaryIndex[i].index < threshold ) {
							rangeOffset = primaryIndex[i].offset ;  // 找到檔案位址 
							break ;
						}
					}
					
					bool final = false ;
					if ( rangeOffset == 0 ) final = true ;
					
					int index = 1 ;
					while (1) {
						sortedVector.clear() ;
						sortedVector.resize( NUM ) ;
					
						//--------------讀檔&放入vector--------------
						int numRead = fread( (char*)&sortedVector[0], sizeof( Data ), sortedVector.capacity(), fin1 ) ;  // �nŪ�J���a��AŪ�J�����AŪ�J�h�ֵ��A���Ӹ�Ƨ�
						if ( numRead == 0 ) break ;
						//--------------輸出小於檔案位址的資料-------
						for ( int i = 0 ; i < 200 ; i++ ) {
							printf( "[%3d]\t", index ) ;
							printf( "%-11s%-11s%-11g\n", sortedVector[i].putID, sortedVector[i].getID, sortedVector[i].weight ) ;
							rangeOffset-- ;
							index++ ;
							if ( final ) {
								numRead-- ;
								if ( numRead == 0 ) break ;
							}
							if ( rangeOffset == 0 ) break ;
						}
						
						if ( rangeOffset == 0 ) break ;
					} // while()
					
					cout << endl << "[0]Quit or [Any other key]continue? : " ;
					cin >> threshold ;
					if ( threshold == 0 ) break ; 
					
				} // else	
				
			} while (1) ;
			
		} // if
		
		fclose( fin1 ) ;
		cout << endl << "[0]Quit or [Any other key]continue? : " ;
		cin >> tempfile ;
		if ( tempfile == "0" ) break ; 		
		
	} while ( tempfile != "0" ) ;
	
} // main()

//---------------function---------------
void Initialize( string &openfile1, string &openfile2, string &openfile3, string &tempfile, bool &havefile, int &command ) { // initialize data
	command = -1 ;
	havefile = false ;
	openfile1 = "0" ;
	openfile2 = "0" ;
	openfile3 = "0" ;
} // Initialize()

void Bubble_sort( vector<SData> &dataVector ) {
	for ( int i = dataVector.size() - 1 ; i > 0 ; i-- ) {
		for ( int j = 0 ; j <= i - 1 ; j++ ) {
			if ( dataVector[j].weight < dataVector[j+1].weight )
				swap( dataVector[j], dataVector[j+1] ) ;
		}
	}
} // Bubble_sort()

void Primary_index( vector<SData> &primaryVector, Table primaryIndex[100], int &address, int numRead ) {
	int i = 0 ;
	//---------------------------初始化strcut array--------------------------
	for ( ; i < numRead ; i++ ) {
		int j = 0 ;
		for ( ; primaryIndex[j].index != 0.0 ; j++ ) {	
			if( primaryIndex[j].index == primaryVector[i].weight ) break ;
		}
		if ( primaryIndex[j].index == 0.0 ) {	
			primaryIndex[j].index = primaryVector[i].weight ;
			primaryIndex[j].offset = address ;
		}
		
		address++ ;		
	} // for

} // Primary_index()