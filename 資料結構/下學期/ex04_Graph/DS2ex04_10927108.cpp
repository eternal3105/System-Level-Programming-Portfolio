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
struct Data {
	Str10 putID ;
	Str10 getID ;
	float weight ;
};

struct ForPointer {
	Str10 putID ;
	Str10 getID ;
	float weight ;
	int influence = 0 ;
	
	ForPointer *right ;
	ForPointer *down ;
};

typedef struct Data SData ;
typedef struct ForPointer *FP ;

//---------------class Queue---------------

class Queue {             
	public : 
		void push( vector<string> &list, Str10 temp ) { // push temp into queue
			list.push_back( temp ) ;	
		} // Push 
		
		void pop( vector<string> &list ) { // pop out the front of queue
			list.erase( list.begin() ) ;
		} // Pop
		
		int size( vector<string> list ) { // queue size
			return list.size() ;
		} // Size
		
		string front( vector<string> list ) { // front of the queue
			return list[0] ;
		} // Front
		
}; // Queue

//---------------class Queue---------------

class Stack {             
	public : 
		void push( vector<string> &list, Str10 temp ) { // push temp into stack
			list.push_back( temp ) ;	
		} // Push 
		
		void pop( vector<string> &list ) { // pop out the back of stack
			list.pop_back() ;
		} // Pop
		
		int size( vector<string> list ) { // stack size
			return list.size() ;
		} // Size
		
		string back( vector<string> list ) { // back of the stack
			return list[list.size()-1] ;
		} // Front
		
}; // Queue

//---------------class Graph---------------

class Graph {
	public:
		FP mission1, mission2, mission3, walkRoot, lastWalkRoot ; // pointer
		bool haveInsert ;
		// ----------mission1----------
		FP Insert( vector<SData> dataVector, int i  )	;
		FP CreateRoot( ForPointer *root, vector<SData> dataVector, int num ) ;
		FP CreateLeaf( ForPointer *mission1, vector<SData> dataVector, int num ) ;
		FP Insert( FP leaf ) ;
		FP CreateRoot( FP root, FP leaf ) ;
		// ----------mission2----------
		FP CreateBFS( int count ) ;
		FP CreateHead( ForPointer *root, ForPointer *mission1 ) ;
		FP insertGetID( ForPointer *temp, Str10 ID ) ;
		bool checkAlready( Str10 ID, ForPointer *walk ) ;
		// ----------mission3----------
		FP CreateDFS( float rnum, int max ) ;
};

//---------------function---------------

void Initialize( Str50 &openfile1, Str50 &openfile2, Str50 &tempfile, bool &havefile, int &command ) ; // ��l��

//---------------main---------------
int main() {
	int command = -1 ;
	Str50 openfile1 = {'\0'}, openfile2 = {'\0'}, tempfile = {'\0'} ;
	Str50 fileNum = {'\0'} ;
	bool havefile = false ;
	SData filedata ; // struct
	FILE * fin ;
	vector<SData> dataVector ;
	Graph graph ;
	bool runone = false ;
	int count = 0 ;
	int max = 0 ;
	do {
		printf( "\n***** Influence on a graph  ****\n" ) ;
		printf( "* 0. QUIT                      *\n" ) ;
		printf( "* 1. Build adjacency lists     *\n" ) ;
		printf( "* 2. Compute influence values  *\n" ) ;
		printf( "********************************\n" ) ;
		printf( "Input a choice(0, 1, 2, 3 ): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
		if ( command == 0 ) return 0 ;
		dataVector.clear() ; // initialize vector
		//---------------------------確認有無txt或bin檔----------------------------------
		if ( command > 0 && command < 4 ) { 
			if ( command == 1 ) {
				Initialize( openfile1, openfile2, tempfile, havefile, command ) ; // initialize data.
				count = 0 ;
				printf( "\nInput a file number ([0] Quit): " ) ;
				scanf( "%s", tempfile ) ;
				if ( strcmp( tempfile, "0" ) != 0 ) {
					strcpy( openfile1, "pairs" ) ;
					strcat( openfile1, tempfile ) ;
					strcat( openfile1, ".bin" ) ;
									
					if ( ( fin = fopen( openfile1, "r" ) ) ) {  // 如果有bin檔
						while ( !feof( fin ) ) {
							fread( &filedata, sizeof( Data ), 1, fin ) ;  // 要讀入的地方，讀入類型，讀入多少筆，哪個資料夾
							dataVector.push_back( filedata ) ;
						} // while()
				
						dataVector.pop_back() ;  // 尾端會多最後一筆同樣資料的
						havefile = true ;
						fclose( fin ) ;
					} // if
					else {
						printf( "\n### %s does not exist! ###\n", openfile1 ) ;
						havefile = false ;
						runone = false ;
						fclose( fin ) ;
					} // else		
				} // if	
				else {
					Initialize( openfile1, openfile2, tempfile, havefile, command ) ; // initialize data.
					runone = false ;
				} 
			} // if
		} // if
		else printf( "\nCommand does not exist!\n" ) ;

		int count = 0 ;
		//---------------------------Mission---------------------------
		if ( command == 1 ) {  // Mission1 
			if ( havefile ) {
				runone = true ; 
				graph.mission1 = graph.CreateRoot( graph.mission1, dataVector, 0 ) ;
				graph.mission1->right = graph.CreateLeaf( graph.mission1->right, dataVector, 0 ) ;
				graph.walkRoot = graph.mission1 ;
				graph.lastWalkRoot = NULL ;
				graph.haveInsert = false ;
				for ( int i = 1 ; i < dataVector.size() ; i++ ) {
					graph.Insert( dataVector, i ) ;
					
					graph.walkRoot = graph.mission1 ;
					graph.lastWalkRoot = NULL ;
					graph.haveInsert = false ;
				} // for
				//------------------檢查是否有<無發訊者>------------------
				bool find = false ;
				for ( FP root = graph.mission1 ; root != NULL ; root = root->down ) {
					for ( FP leaf = root->right ; leaf != NULL ; leaf = leaf->right ) {
						for ( FP walk = graph.mission1 ; walk != NULL ; walk = walk->down ) {
							if ( strcmp( walk->putID, leaf->getID ) == 0 ) {
								find = true ;
							} // if
							
						} // for
						
						if ( !find ) {
							graph.Insert( leaf ) ;
						} // if
						
						find = false ;
					} // for
					
				} // for
				
				//------------------計算與輸出.adj------------------
				// count = 0 ;
				for ( FP root = graph.mission1 ; root != NULL ; root = root->down ) {
					count++ ;
				} // for
				
				strcpy( openfile2, "pairs" ) ;
				strcat( openfile2, tempfile ) ;
				strcat( openfile2, ".adj" ) ;
				
				FILE * fin = fopen( openfile2, "w+" ) ;
				fprintf( fin, "<<< There are %d IDs in total. >>>\n", count ) ;
				int i = 1, j = 1 ;
				for ( FP root = graph.mission1 ; root != NULL ; root = root->down ) {
					fprintf( fin, "[%3d] %s: \n", i, root->putID ) ;
					for ( FP leaf = root->right ; leaf != NULL ; leaf = leaf->right ) {
						fprintf( fin, "\t(%2d) %s,  %g", j, leaf->getID, leaf->weight ) ;
						if ( j == 10 ) fprintf( fin, "\n" ) ;
						j++ ;
					} // for
					
					fprintf( fin, "\n" ) ;
					i++ ;
					j = 1 ;
				} // for
				fprintf( fin, "<<< There are %d nodes in total. >>>\n", dataVector.size() ) ;
				fclose( fin ) ;
				
				printf( "\n<<< There are %d IDs in total. >>>\n", count ) ;
				printf( "\n<<< There are %d nodes in total. >>>\n", dataVector.size() ) ;
				strcpy( fileNum, tempfile ) ;
				max = count ;
			} // if	
		} // if
		else if ( command == 2 ) { // Mission2
			if ( runone ) {
				graph.mission2 = graph.CreateBFS( max ) ;		
				//------------------計算與輸出.cnt------------------

				strcpy( openfile2, "pairs" ) ;
				strcat( openfile2, fileNum ) ;
				strcat( openfile2, ".cnt" ) ;
				
				FILE * fin = fopen( openfile2, "w+" ) ;
				fprintf( fin, "<<< There are %d IDs in total. >>>\n", max ) ;
				
				int i = 1, j = 1 ;
				for ( FP root = graph.mission2 ; root != NULL ; root = root->down ) {
					
					fprintf( fin, "[%3d] %s(%d): \n", i, root->putID, root->influence ) ;
					int k = 1 ;
					for ( FP leaf = root->right ; leaf != NULL ; leaf = leaf->right ) {
						fprintf( fin, "\t(%2d) %s   ", j, leaf->getID ) ;
						if ( k == 10 ) {
							fprintf( fin, "\n" ) ;
							k = 0 ;
						}
						j++ ;
						k++ ;
					} // for
					
					fprintf( fin, "\n" ) ;
					
					i++ ;
					j = 1 ;
				} // for
				
				fclose( fin ) ;
				
				printf( "\n<<< There are %d IDs in total. >>>\n", max ) ;
			} // if
			else printf( "\n### There is no graph and choose 1 first. ###\n" ) ;
		} // else if
		else if ( command == 3 ) {
			if ( runone ) {
				float rnum = -1 ;
				printf( "\nInput a real number in [0,1]: " ) ;
				scanf( "%f", &rnum ) ;
				while ( rnum < 0 || rnum > 1 ) {
					printf( "\n### It is NOT in [0,1] ###\n" ) ;
					printf( "\nInput a real number in [0,1]: " ) ;
					scanf( "%f", &rnum ) ;
				}
				
				graph.mission3 = graph.CreateDFS( rnum, max ) ;
					
				//------------------計算與輸出.inf------------------

				strcpy( openfile2, "pairs" ) ;
				strcat( openfile2, fileNum ) ;
				strcat( openfile2, ".inf" ) ;
			
				FILE * fin = fopen( openfile2, "w+" ) ;
				fprintf( fin, "<<< There are %d IDs in total. >>>\n", max ) ;
			
				int i = 1, j = 1 ;
				for ( FP root = graph.mission3 ; root != NULL ; root = root->down ) {
				
					fprintf( fin, "[%3d] %s(%d): \n", i, root->putID, root->influence ) ;
					int k = 1 ;
					for ( FP leaf = root->right ; leaf != NULL ; leaf = leaf->right ) {
						fprintf( fin, "\t(%3d) %s   ", j, leaf->getID ) ;
						if ( k == 10 ) {
							fprintf( fin, "\n" ) ;
							k = 0 ;
						}
						j++ ;
						k++ ;
					} // for
					
					fprintf( fin, "\n" ) ;
				
					i++ ;
					j = 1 ;
				} // for
				
				fclose( fin ) ;
				
				printf( "\n<<< There are %d IDs in total. >>>\n", max ) ;
				
			} // if
			else printf( "\n### There is no graph and choose 1 first. ###\n" ) ;
				
		} // else if
		
	} while ( command != 0 ) ;
	
} // main()

//---------------function---------------
void Initialize( Str50 &openfile1, Str50 &openfile2, Str50 &tempfile, bool &havefile, int &command ) { // initialize data
	havefile = false ;
	
	for ( int i = 0 ; i < 10 ; i++ ) {
		openfile1[i] = '\0' ;
		openfile2[i] = '\0' ;
		tempfile[i] = '\0' ;
	} // for()
	
} // Initialize()

//---------------Graph class Function---------------

//---------------MISSION1---------------

FP Graph::Insert( vector<SData> dataVector, int i ) {
	while ( !haveInsert && walkRoot != NULL ) {
		if ( strcmp( dataVector[i].putID, walkRoot->putID ) < 0 ) {  // vector的putID小於pointer的ID
			if ( lastWalkRoot == NULL ) {  // 當walkRoot為第一個pointer
				FP temp = walkRoot ;
				walkRoot = CreateRoot( walkRoot, dataVector, i ) ;
				walkRoot->right = CreateLeaf( walkRoot->right, dataVector, i ) ;
				walkRoot->down = temp ;
				mission1 = walkRoot ;  // 要把head接上去
			} // if
			else {
				FP temp = walkRoot ;
				lastWalkRoot->down = CreateRoot( lastWalkRoot->down, dataVector, i ) ;
				lastWalkRoot->down->right = CreateLeaf( lastWalkRoot->down->right, dataVector, i ) ;
				lastWalkRoot->down->down = temp ;
			} // else
							
				haveInsert = true ;
		} // if
		else if ( strcmp( dataVector[i].putID, walkRoot->putID ) == 0 ) {  // vector與pointer的ID相同
			FP walkLeaf = walkRoot->right ;
			FP lastWalkLeaf = NULL ;
			bool haveInsert2 = false ;
			while ( !haveInsert2 && walkLeaf != NULL ) {
				if ( dataVector[i].weight == walkLeaf->weight ) {  // vector與pointer的weight相同 
					while ( walkLeaf->right != NULL && dataVector[i].weight == walkLeaf->weight ) {
						lastWalkLeaf = walkLeaf ;
						walkLeaf = walkLeaf->right ;
					} // while()
					if ( lastWalkLeaf != NULL ) {
						FP temp = walkLeaf ;
						lastWalkLeaf->right = CreateLeaf( lastWalkLeaf->right, dataVector, i ) ;
						lastWalkLeaf->right->right = temp ;
						haveInsert2 = true ;
					} // if
					else {  // walkLeaf為第一個right 
						FP temp = walkLeaf ;
						walkRoot->right = CreateLeaf( walkRoot->right, dataVector, i ) ;
						walkRoot->right->right = temp ;
					} // else
				} // if
				else if ( dataVector[i].weight > walkLeaf->weight ) {
					if ( lastWalkLeaf != NULL ) {
						FP temp = walkLeaf ;
						lastWalkLeaf->right = CreateLeaf( lastWalkLeaf->right, dataVector, i ) ;
						lastWalkLeaf->right->right = temp ;
					} // if
					else {  //walkLeaf為第一個right 
						FP temp = walkLeaf ;
						walkRoot->right = CreateLeaf( walkRoot->right, dataVector, i ) ;
						walkRoot->right->right = temp ;
					} // else
									
						haveInsert2 = true ;
				} // else if
								
				lastWalkLeaf = walkLeaf ;
				walkLeaf = walkLeaf->right ;
			} // while()
							
			if ( !haveInsert2 ) {  // getID到最後都沒有插入(表示最小，放在最尾)
				lastWalkLeaf->right = CreateLeaf( lastWalkLeaf->right, dataVector, i ) ;
			} // if
							
			haveInsert = true ;
		} // else if 
						
			lastWalkRoot = walkRoot ;
			walkRoot = walkRoot->down ;
	} // while()
					
	if ( !haveInsert ) {  // putID到最後都沒有插入(表示最大，放在最尾)
		lastWalkRoot->down = CreateRoot( lastWalkRoot->down, dataVector, i ) ;
		lastWalkRoot->down->right = CreateLeaf( lastWalkRoot->down->right, dataVector, i ) ;
	} // if
	
} // Insert()

FP Graph::CreateRoot( ForPointer *root, vector<SData> dataVector, int num ) {
	root = new ForPointer ;
	strcpy( root->putID, dataVector[num].putID ) ;
	root->right = NULL ;
	root->down = NULL ;
	
	return root ;
} // CreateRoot()

FP Graph::CreateLeaf( ForPointer *leaf, vector<SData> dataVector, int num ) {
	leaf = new ForPointer ;
	strcpy( leaf->getID, dataVector[num].getID ) ;
	leaf->weight = dataVector[num].weight ;
	leaf->right = NULL ;
	leaf->down = NULL ;
	
	return leaf ;
} // CreateLeave()

FP Graph::Insert( FP leaf ) {
	bool haveInsert = false ;
	lastWalkRoot = NULL ; 
	for ( FP walk = mission1 ; walk != NULL ; walk = walk->down ) {
		if ( strcmp( walk->putID, leaf->getID ) > 0 ) {
			if ( lastWalkRoot == NULL ) {  // 位於第一個 
				mission1 = CreateRoot( mission1, leaf ) ;  
				mission1->down = walk ;
			} // if
			else {  // 位於中間
				lastWalkRoot->down = CreateRoot( lastWalkRoot->down, leaf ) ;
				lastWalkRoot->down->down = walk ;
			} // else
			
			haveInsert = true ;
		} // if()
		
		if ( haveInsert ) break ;
		lastWalkRoot = walk ;
	} // for
	
	if ( !haveInsert ) {
		lastWalkRoot->down = CreateRoot( lastWalkRoot->down, leaf ) ;
	} // if
	
} // Insert

FP Graph::CreateRoot( FP root, FP leaf ) {
	root = new ForPointer ;
	strcpy( root->putID, leaf->getID ) ;
	root->right = NULL ;
	root->down = NULL ;
	
	return root ;
} // CreateRoot

//---------------MISSION2---------------

FP Graph::CreateBFS( int max ) {
	Queue queue ;
	
	ForPointer *walk = NULL ;
	walk = new ForPointer ;
	strcpy ( walk->putID, mission1->putID ) ;
	walk->right = NULL ;
	walk->down = NULL ;
	int comIn = -1 ;
	Str10 comPutID ;
	ForPointer *cmpRight = NULL ;
	ForPointer *cmpDown = NULL ;
	int have = -1 ;
	
	int count = 0 ;
	for ( FP root = mission1 ; root != NULL ; root = root->down ) {  // 從mission1的頭跑到尾
		if ( count != 0 ) {
			walk->down = new ForPointer ;
			strcpy ( walk->down->putID, root->putID ) ;  // 將傳訊者存入mission2 
			walk->down->right = NULL ;
			walk->down->down = NULL ;
			walk = walk->down ;
			count++ ;
		}
		else {
			mission2 = walk ;
			count++ ;
		}

		vector<string> q ;
		
		ForPointer *temp = walk ;
		queue.push( q, walk->putID ) ;

		string ID ;
		have = -1 ;
		while ( queue.size(q) > 0 ) {  // queue裡還有ID 
			ForPointer *test = mission1 ;
			ID = queue.front( q ) ;
			queue.pop( q ) ;
			if ( walk->influence == max - 1 ) break ; 
			for ( ; test != NULL ; test = test->down ) { 
				
				if ( test->putID == ID ) {  // 找到ID在mission1的位置 
					ForPointer *test2 = test->right ;
					for ( ; test2 != NULL ; test2 = test2->right ) {  // 確認此ID的收訊者有甚麼 
						if ( walk->influence == max - 1 ) break ;
						if ( ! checkAlready( test2->getID, walk ) ) {  // 若還沒被存入mission2 
							
							temp = insertGetID( temp, test2->getID ) ;  // 存入mission2的子串列 
							walk->influence++ ; 
							
							if ( have != -1 ) {  // 排序子串列 
								ForPointer *run = walk->right ;
								ForPointer *grab = walk->right ;
								bool isRoot = true ;						
									
								for ( ; run != temp && strcmp( run->getID , temp->getID ) < 0  ; run = run->right ) {  // 跑到要跟新插入的值換的位置後就跳出  
									grab = run ;
									isRoot = false ;
								}
								
								if ( run == temp ) {  // 不須交換 
									;
								}
								else if ( !isRoot ) {  // 中間跟尾巴換 
									grab->right = temp ;

									temp->right = run ;
									temp = cmpRight ;
									temp->right = NULL ;
								}
								else {  // 頭跟尾巴換 
									walk->right = temp ;

									temp->right = run ;
									temp = cmpRight ;
									temp->right = NULL ;
								}
							} 

							have++ ;
							cmpRight = temp ;
							
							queue.push( q, test2->getID ) ;
						}
					}
					
					break ;
				}
			}	
		}
		
		if ( comIn != -1 ) {  // 排序主串列 
			ForPointer *run = mission2 ;
			ForPointer *grab = mission2 ;
			bool isRoot = true ;
			
			for ( ; run != walk && ( run->influence > walk->influence || ( run->influence == walk->influence && strcmp( run->putID , walk->putID ) < 0 ) ) ; run = run->down ) {  // 跑到要跟新插入的值換的位置後就跳出 
				grab = run ;
				isRoot = false ;
			}
			
			if ( run == walk ) {  // 不須交換
				;
			}
			else if ( !isRoot ) {  // 中間跟尾巴換
				grab->down = walk ;
								
				walk->down = run ;
				walk = cmpDown ;
				walk->down = NULL ;
			}
			else {  // 頭跟尾巴換 
				mission2 = walk ;

				walk->down = run ;
				walk = cmpDown ;
				walk->down = NULL ;
			}
		} 
		
		comIn = walk->influence ;
		cmpDown = walk ;

	} // for
	
	return mission2 ;
	
} // CreateBFS()

FP Graph::insertGetID( ForPointer *temp, Str10 ID ) {   // 存入子串列 

	temp->right = new ForPointer ;
	temp->right->right = NULL ;
	temp->right->down = NULL ;
	temp = temp->right ;
	strcpy( temp->getID, ID ) ;	
	
	return temp ;
	
} // insertGetID

bool Graph::checkAlready( Str10 ID, ForPointer *walk ) { // check if already have 
	
	if ( strcmp( walk->putID, ID ) == 0 ) return true ;  // 為此子串的傳訊者 
	ForPointer *test = walk->right ;
	for ( ; test != NULL ; test = test->right ) {  // 子串內已有此ID 
		if ( strcmp( test->getID, ID ) == 0 ) {
			return true ;
		}
	}
	return false ;
}

//---------------MISSION3---------------

FP Graph::CreateDFS( float rnum, int max ) {
	Stack stack ;

	ForPointer *walk = NULL ;
	walk = new ForPointer ;
	strcpy ( walk->putID, mission1->putID ) ;
	walk->right = NULL ;
	walk->down = NULL ;
	int comIn = -1 ;
	Str10 comPutID ;
	ForPointer *cmpRight = NULL ;
	ForPointer *cmpDown = NULL ;
	int have = -1 ;
	
	int count = 0 ;
	for ( FP root = mission1 ; root != NULL ; root = root->down ) {  // 從mission1的頭跑到尾
		if ( count != 0 ) {
			walk->down = new ForPointer ;
			strcpy ( walk->down->putID, root->putID ) ;  // 將傳訊者存入mission3 
			walk->down->right = NULL ;
			walk->down->down = NULL ;
			walk = walk->down ;
			count++ ;
		}
		else {
			mission3 = walk ;
			count++ ;
		}
		
		vector<string> q ;
		
		ForPointer *temp = walk ;
		stack.push( q, walk->putID ) ;

		string ID ;
		have = -1 ;
		while ( stack.size(q) > 0 ) {  // stack裡還有ID
			ForPointer *test = mission1 ;
			ID = stack.back( q ) ;
			stack.pop( q ) ;
			if ( walk->influence == max - 1 ) break ;
			for ( ; test != NULL ; test = test->down ) { 
				
				if ( test->putID == ID ) {  // 找到ID在mission1的位置  
					ForPointer *test2 = test->right ;
					for ( ; test2 != NULL ; test2 = test2->right ) {  // 確認此ID的收訊者有甚麼 
						if ( ! checkAlready( test2->getID, walk ) && test2->weight >= rnum ) {  // 若還沒被存入mission3 
							
							temp = insertGetID( temp, test2->getID ) ;  // 存入mission3的子串列 
							walk->influence++ ; 
							if ( walk->influence == max - 1 ) break ; 
							if ( have != -1 ) {  // 排序子串列 
								ForPointer *run = walk->right ;
								ForPointer *grab = walk->right ;
								bool isRoot = true ;						
									
								for ( ; run != temp && strcmp( run->getID , temp->getID ) < 0  ; run = run->right ) {  // 跑到要跟新插入的值換的位置後就跳出
									grab = run ;
									isRoot = false ;
								}
								
								if ( run == temp ) {  // 不須交換 
									;
								}
								else if ( !isRoot ) {  // 中間跟尾巴換 
									grab->right = temp ;

									temp->right = run ;
									temp = cmpRight ;
									temp->right = NULL ;
								}
								else {  // 頭跟尾巴換 
									walk->right = temp ;

									temp->right = run ;
									temp = cmpRight ;
									temp->right = NULL ;
								}
							} 

							have++ ;
							cmpRight = temp ;
							
							stack.push( q, test2->getID ) ;
						}
					}
					
					break ;
				}
			}	
		}
		
		if ( comIn != -1 ) {  // 排序主串列
			ForPointer *run = mission3 ;
			ForPointer *grab = mission3 ;
			bool isRoot = true ;
			
			for ( ; run != walk && ( run->influence > walk->influence || ( run->influence == walk->influence && strcmp( run->putID , walk->putID ) < 0 ) ) ; run = run->down ) {  // 跑到要跟新插入的值換的位置後就跳出 
				grab = run ;
				isRoot = false ;
			}
			
			if ( run == walk ) {  // 不須交換
				;
			}
			else if ( !isRoot ) {  // 中間跟尾巴換
				grab->down = walk ;
								
				walk->down = run ;
				walk = cmpDown ;
				walk->down = NULL ;
			}
			else {  // 頭跟尾巴換
				mission3 = walk ;

				walk->down = run ;
				walk = cmpDown ;
				walk->down = NULL ;
			}
		} 
		
		comIn = walk->influence ;
		cmpDown = walk ;

	} // for
	
	return mission3 ;
	
} // CreateDFS()
