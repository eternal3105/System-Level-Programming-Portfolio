// 10927108 劉珈彤

# include <iostream>
# include <string.h>
# include <vector>
# include <fstream>
# include <memory.h>


using namespace std ;
typedef char Str200[200] ;

//---------------struct---------------
struct ForVector {
	int num ;
	//----------------我是分隔線----------------
	int schoolCode ;   
	Str200 schoolName ;
	Str200 departmentCode ;
	Str200 departmentName ;
	Str200 club ;
	Str200 level ;
	//----------------我是分隔線----------------
	int student ;
	int teacher ;
	int graduate ;
	Str200 city ;
	Str200 sport ;
};

struct ForTwoThreeTree {
	int num ;
	Str200 schoolName ;
	//----------------我是分隔線----------------
	vector<ForVector> left ;
	vector<ForVector> right ;	
	vector<ForVector> middle ;
	ForTwoThreeTree *parent ;
	ForTwoThreeTree *one ;
	ForTwoThreeTree *two ;
	ForTwoThreeTree *three ;	
};


struct ForAvlTree {
	int num ;
	Str200 departmentName ;
	//----------------我是分隔線----------------
	ForAvlTree *left ;
	ForAvlTree *right ;
	ForAvlTree *same ;
};

ForAvlTree *aroot ;

//---------------class---------------
class TwoThreeTree {
	public:
		vector<ForVector> usingVector ;
		ForTwoThreeTree *root ;
		TwoThreeTree() {
			root = NULL ;
		} // TwoThreeTree()
		
		void OpenAndStore( Str200, Str200, Str200, bool &, Str200 ) ;
		void ResetStr( Str200 ) ;
		int FindlastElememt() ;
		ForTwoThreeTree *Store( ForTwoThreeTree *, int, Str200, string ) ;
		ForTwoThreeTree *Found( ForTwoThreeTree *, Str200, bool & ) ;
		ForTwoThreeTree *splitUp( ForTwoThreeTree *, ForTwoThreeTree *, ForTwoThreeTree *, 
								  ForTwoThreeTree *, ForTwoThreeTree *, ForTwoThreeTree * ) ;
		int GetHigh( ForTwoThreeTree *) ;
		ForTwoThreeTree *Build() ;
		ForTwoThreeTree *Insert( ForTwoThreeTree *, int, Str200 ) ;
		void PrintRoot( ForTwoThreeTree * ) ;
		ForTwoThreeTree *ReSet( ForTwoThreeTree * ) ;
		ForTwoThreeTree *Search( ForTwoThreeTree *, Str200, bool & ) ;

};

class AvlTree {
	public:
		vector<ForVector> usingVector ;
		ForAvlTree *root ;
		
		void OpenAndStore( Str200, Str200, Str200, bool & ) ;
		void ResetStr( Str200 ) ;
		int FindlastElement() ;
		ForAvlTree *Insert( ForAvlTree *, int ) ;
		ForAvlTree *Balance( ForAvlTree * ) ;
		int LRGap( ForAvlTree *) ;
		int GetHigh( ForAvlTree *) ;
		ForAvlTree *RotationRR( ForAvlTree * ) ;
		ForAvlTree *RotationRL( ForAvlTree * ) ;
		ForAvlTree *RotationLR( ForAvlTree * ) ;
		ForAvlTree *RotationLL( ForAvlTree * ) ;
		void PrintRoot( ForAvlTree * ) ;
		ForAvlTree *ReSet( ForAvlTree * ) ;
		ForAvlTree *Found( ForAvlTree *, Str200, bool & ) ;

		
};


//---------------function---------------
void Initialize( Str200 &openfile, Str200 &tempfile, Str200 &title1, Str200 &title2, bool &havefile ) ;

//---------------main---------------
int main() {
	int command = -1, floor = 0 ;
	Str200 openfile = {'\0'}, tempfile = {'\0'} ;  // 打開資料夾用 
	Str200 title1, title2 ;  // 用於讀入file開頭 
	bool havefile = false ;  // 是否有打開資料夾
	TwoThreeTree tTree ;
	AvlTree aTree ;
	ForTwoThreeTree *troot ;
	bool runOne = false ;
	bool runTwo = false ;
	vector<ForVector> Vector ;
	ForTwoThreeTree *stroot ;
	ForAvlTree *saroot ;
	ForAvlTree *saveAvl ;
	ForTwoThreeTree *saveT ;

	
	printf( "\n*** Search Tree Utilities **\n" ) ;
	printf( "* 0. QUIT                  *\n" ) ;
	printf( "* 1. Build 2-3 tree        *\n" ) ;
	printf( "* 2. Build AVL tree        *\n" ) ;
	printf( "* 3. Intersection Query    *\n" ) ;
	printf( "*************************************\n" ) ;
	printf( "Input a choice(0, 1, 2, 3): " ) ;
	if ( scanf( "%d", &command ) == 0 ) return 1 ;
	
	
	while ( command != 0 ) {
		while ( command < 0 || command > 3 ) {
			printf( "\nCommand does not exist!\n\n" ) ;
			printf( "\n*** Search Tree Utilities **\n" ) ;
			printf( "* 0. QUIT                  *\n" ) ;
			printf( "* 1. Build 2-3 tree        *\n" ) ;
			printf( "* 2. Build AVL tree        *\n" ) ;
			printf( "* 3. Intersection Query    *\n" ) ;
			printf( "*************************************\n" ) ;
			printf( "Input a choice(0, 1, 2, 3): " ) ;
			if ( scanf( "%d", &command ) == 0 ) return 1 ;
		} // while()
		
		if ( command == 0 ) return 1 ;
		
		else if ( command == 1 ) {
			Initialize( openfile, tempfile, title1, title2, havefile ) ;
			printf( "\nInput a file number ([0] Quit):" ) ;
			scanf( "%s", tempfile ) ;
			runOne = false ;
			if ( strcmp( tempfile, "0" ) != 0 ) {
				strcpy( openfile, "input" ) ;
				strcat( openfile, tempfile ) ;
				strcat( openfile, ".txt" ) ;				
				//----------------打開資料夾讀檔----------------
				tTree.OpenAndStore( openfile, title1, title2, havefile, tempfile ) ;
			}
		
			Vector = tTree.usingVector ;
			
			if ( havefile ) {
				troot = tTree.Build() ;
				saveT = troot ;
				troot = new ForTwoThreeTree() ;
				//----------------初始化tTree----------------
				troot = tTree.ReSet( troot ) ;
				runOne = true ;
				runTwo = false ;
			} // if()
			
		} // else if()
		
		else if ( command == 2 ) {
			if ( !runOne ) cout << "### Choose 1 first. ###" << endl ;
			else {
				if ( runTwo ) cout << "### AVL tree has been built. ###" << endl ;
				runTwo = false ;
				if ( strcmp( tempfile, "0" ) != 0 ) {
					strcpy( openfile, "input" ) ;
					strcat( openfile, tempfile ) ;
					strcat( openfile, ".txt" ) ;
					//----------------打開資料夾讀檔----------------
					aTree.OpenAndStore( openfile, title1, title2, havefile ) ;
			
					if ( havefile ) {
						int temp = aTree.FindlastElement() ;  // 判斷Vector有多少 
						for( int n = 0 ; n < temp ; n++ ) {
							aroot = aTree.Insert( aroot, n ) ;
						} // for()
					
						aTree.PrintRoot( aroot ) ;
						saveAvl = aroot ;
						
						aroot = new ForAvlTree() ;
						//----------------初始化aTree----------------
						aroot = aTree.ReSet( aroot ) ;
						runTwo = true ; 
					} // if()
				
				} // if()
			}

		} // else if()
		
		else if ( command == 3 ) {
			if ( !runOne && !runTwo ) cout << "### Choose 1 first. ###" << endl ;
			else if ( !runTwo ) cout << "### Choose 2 first. ###" << endl ;
			else {
				Str200 sName, dName ;
				printf( "\nEnter a college name to search [*]:" ) ;
				scanf( "%s", sName ) ;
				printf( "Enter a department name to search [*]:" ) ;
				scanf( "%s", dName ) ;
				
				bool findS = false, findD = false ;
				
				if ( strcmp( sName, "*" ) != 0 && strcmp( dName, "*" ) == 0 ) {  // search 23Tree
					stroot = saveT ;
					stroot = tTree.Search( stroot, sName, findS ) ;
					if ( findS ) {
						
						if ( strcmp( sName, stroot->left[0].schoolName ) == 0 ) {
							int count = 1 ;
							for ( int n = 0 ; n < stroot->left.size() ; n++ ) {
								printf( "%d: [%d] %s, %s", count, Vector[stroot->left[n].num-1].num, Vector[stroot->left[n].num-1].schoolName, Vector[stroot->left[n].num-1].departmentName ) ;
								printf( ", %s, %s, %d\n", Vector[stroot->left[n].num-1].club, Vector[stroot->left[n].num-1].level, Vector[stroot->left[n].num-1].student ) ;
								count++ ;
							}
						}
						else {
							int count = 1 ;
							for ( int n = 0 ; n < stroot->right.size() ; n++ ) {
								printf( "%d: [%d] %s, %s", count, Vector[stroot->right[n].num-1].num, Vector[stroot->right[n].num-1].schoolName, Vector[stroot->right[n].num-1].departmentName ) ;
								printf( ", %s, %s, %d\n", Vector[stroot->right[n].num-1].club, Vector[stroot->right[n].num-1].level, Vector[stroot->right[n].num-1].student ) ;
								count++ ;
							}
						}
					
						
					}
					
					stroot = saveT ;
				}
				else if ( strcmp( sName, "*" ) == 0 && strcmp( dName, "*" ) != 0 ) {  // search avlTree
					saroot = saveAvl ;
					saroot = aTree.Found( saroot, dName, findD ) ;
					if ( findD ) { 
						int count = 1 ;
						
						for ( ; saroot != NULL ; saroot = saroot->same ) {
							printf( "%d: [%d] %s, %s", count, Vector[saroot->num-1].num, Vector[saroot->num-1].schoolName, Vector[saroot->num-1].departmentName ) ;
							printf( ", %s, %s, %d\n", Vector[saroot->num-1].club, Vector[saroot->num-1].level, Vector[saroot->num-1].student ) ;
							count++ ;
						}
					} 
					
					saroot = saveAvl ;
				}
				else if ( strcmp( sName, "*" ) == 0 && strcmp( dName, "*" ) == 0 ) {
					int count = 1 ;
					for ( int n = 0 ; n < Vector.size() ; n++ ) {
						printf( "%d: [%d] %s, %s", count, Vector[n].num, Vector[n].schoolName, Vector[n].departmentName ) ;
						printf
						( ", %s, %s, %d\n", Vector[n].club, Vector[n].level, Vector[n].student ) ;
						count++ ;
					}
				}
				else if ( strcmp( sName, "*" ) != 0 && strcmp( dName, "*" ) != 0 ) {
					stroot = saveT ;
					stroot = tTree.Search( stroot, sName, findS ) ;
					saroot = saveAvl ;
					saroot = aTree.Found( saroot, dName, findD ) ;
					if ( findD && findS ) {
						if ( strcmp( sName, stroot->left[0].schoolName ) == 0 ) {
							int count = 1 ;
							ForAvlTree *store = saroot ;
							for ( int n = 0 ; n < stroot->left.size() ; n++ ) {
								for ( saroot = store ; saroot != NULL ; saroot = saroot->same ) {
									if ( stroot->left[n].num-1 == saroot->num-1 ) {
										printf( "%d: [%d] %s, %s", count, Vector[stroot->left[n].num-1].num, Vector[stroot->left[n].num-1].schoolName, Vector[stroot->left[n].num-1].departmentName ) ;
										printf( ", %s, %s, %d\n", Vector[stroot->left[n].num-1].club, Vector[stroot->left[n].num-1].level, Vector[stroot->left[n].num-1].student ) ;
										count++ ;
									}
								}
							}
						}
						else {
							int count = 1 ;
							ForAvlTree *store = saroot ;
							for ( int n = 0 ; n < stroot->right.size() ; n++ ) {
								for ( saroot = store ; saroot != NULL ; saroot = saroot->same ) {
									if ( stroot->right[n].num-1 == saroot->num-1 ) {
										printf( "%d: [%d] %s, %s", count, Vector[stroot->right[n].num-1].num, Vector[stroot->right[n].num-1].schoolName, Vector[stroot->right[n].num-1].departmentName ) ;
										printf( ", %s, %s, %d\n", Vector[stroot->right[n].num-1].club, Vector[stroot->right[n].num-1].level, Vector[stroot->right[n].num-1].student ) ;
										count++ ;
									}
								}
								
								
							}
						}
					}
					
					stroot = saveT ;
					saroot = saveAvl ;
				} 

			}

		} // else if()
		
		command = 0 ;
		printf( "\n*** Search Tree Utilities **\n" ) ;
		printf( "* 0. QUIT                  *\n" ) ;
		printf( "* 1. Build 2-3 tree        *\n" ) ;
		printf( "* 2. Build AVL tree        *\n" ) ;
		printf( "* 3. Intersection Query    *\n" ) ;
		printf( "*************************************\n" ) ;
		printf( "Input a choice(0, 1, 2, 3): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
	} // while()
		
} // main

//---------------TwoThreeTree class Function---------------

void TwoThreeTree::OpenAndStore( Str200 openfile, Str200 title1, Str200 title2, bool &havefile, Str200 tempfile ) {
	FILE * fin;
	fin = fopen( openfile, "r" ) ;
	
	int tempCode = 0, num = 1 ;
	Str200 temp ;
	Str200 garbge, temp1, temp2 ;
	char numtemp = '\0' ;
			
	if ( fin ) {
		int check = 0 ;
    fgets( title1, 200, fin ) ;
    fgets( title2, 200, fin ) ;
    fgets( garbge, 200, fin ) ;
                
    while ( !feof( fin ) ) {
      ForVector vectortemp ;
      if ( fscanf( fin, "%d", &tempCode ) == 1 ) {
        vectortemp.num = num ;
        vectortemp.schoolCode = tempCode ;
        fscanf( fin, "%s%s%s", vectortemp.schoolName, vectortemp.departmentCode, vectortemp.departmentName ) ;
						
        fscanf( fin, "%c", temp1 ) ;
        fscanf( fin, "%c", temp1 ) ;
        while ( strcmp( temp1, "\t" ) != 0 ) {
          strcat( vectortemp.club, temp1 ) ;
          fscanf( fin, "%c", temp1 ) ;
				} // while()
						
        fscanf( fin, "%c", temp2 ) ;
        while ( strcmp( temp2, "\t" ) != 0 ) {
          strcat( vectortemp.level, temp2 ) ;
          fscanf( fin, "%c", temp2 ) ;
				} // while()
						
        fscanf( fin, "%c", &numtemp ) ;
        if ( numtemp == '\"' ) {
          fscanf( fin, "%c", &numtemp ) ;
          while ( numtemp != '\"' ) {
            if ( numtemp != ',' ) vectortemp.student = ( 10 * vectortemp.student ) + ( numtemp - '0' ) ;
            fscanf( fin, "%c", &numtemp ) ;
					} // while()

				} // if()
				else {
					while ( numtemp != '\t' ) {
            vectortemp.student = ( 10 * vectortemp.student ) + ( numtemp - '0' ) ;
            fscanf( fin, "%c", &numtemp ) ;
					} // while()
							
				} // else
						
        fscanf( fin, "%d%d", &vectortemp.teacher, &vectortemp.graduate ) ;
        fscanf( fin, "%s%s", &vectortemp.city, &temp ) ;
        strcat( vectortemp.city, temp ) ;
        fscanf( fin, "%s%s", &vectortemp.sport, &temp ) ;
        strcat( vectortemp.sport, temp ) ;
        usingVector.push_back( vectortemp ) ;
        num++ ;
			} // if()

			//------------initialize------------
			vectortemp.student = 0 ;
			for ( int i = 0 ; i < 200 ; i++ ) {
				vectortemp.club[i] = '\0' ;
				vectortemp.level[i] = '\0' ;
			} // for()
					
    } // while

		havefile = true ;
	} // if()
			
	else {
		printf( "\n### %s does not exist! ###\n", openfile ) ;
		printf( "\nThere is no data!\n" ) ;

		for ( int i = 0 ; i < 200 ; i++ ) {
		openfile[i] = '\0' ;
		tempfile[i] = '\0' ;
		title1[i] = '\0' ;
		title2[i] = '\0' ;
		} // for()
		
		printf( "\nInput a file number ([0] Quit):" ) ;
		scanf( "%s", tempfile ) ;
		if ( strcmp( tempfile, "0" ) != 0 ) {
			strcpy( openfile, "input" ) ;
			strcat( openfile, tempfile ) ;
			strcat( openfile, ".txt" ) ;				
			//----------------打開資料夾讀檔----------------
			OpenAndStore( openfile, title1, title2, havefile, tempfile ) ;
		}
		
	} // else

} // OpenAndStore()

void TwoThreeTree::ResetStr( Str200 temp ) {
	for ( int i = 0 ; i < strlen( temp ) ; i++ ) temp[i] = '\0' ;
} // ResetStr

int TwoThreeTree::FindlastElememt() {
	return usingVector.size() ;
} // TwoThreeTree::FindlastElement()

ForTwoThreeTree *TwoThreeTree::Store( ForTwoThreeTree *root, int num, Str200 sname, string site ) {  // Store data
	ForVector temp ;
	
	temp.num = num ;
	strcpy( temp.schoolName, sname ) ;
	
	if ( site == "left" ) {
		root->left.push_back(temp) ;                                                                                   // push into left
	}
	else if ( site == "middle" ) {
		root->middle.push_back(temp) ;                                                                                 // push into middle
	}
	else {                                                                                                             // if ( site == "right" ) {
		root->right.push_back(temp) ;                                                                                  // push into right
	}
			
	return root ;
} // TwoThreeTree::Store(

ForTwoThreeTree *TwoThreeTree::Found( ForTwoThreeTree *root, Str200 sname, bool &hasSame ) {  // found if there has same school in tree
	if ( root == NULL ) {
		hasSame = false ;
		return root ;
	} // if()
	else {
		while ( root->one != NULL ) {  // run until arrive leaf
			if ( root->right.size() == 0 ) {  // only one data in this node
				if ( strcmp( sname, root->left[0].schoolName ) == 0 ) {                                                
					break ;  // find same
				} // if()
				else if ( strcmp( sname, root->left[0].schoolName ) > 0 ) {
					root = root->two ;   // go to children two
				} // else if()
				else {
					root = root->one ;  // go to children one
				} // else
			} // if()
			else {  // two data in node
				if ( strcmp( sname, root->left[0].schoolName ) == 0  || strcmp( sname, root->right[0].schoolName ) == 0  ) { 
					break ;  // find same
				} // if()
				else if ( strcmp( sname, root->right[0].schoolName ) > 0  ) {
					root = root->three ;  // go to children three
				} // else if()
				else if ( strcmp( sname, root->left[0].schoolName ) > 0  &&  strcmp( sname, root->right[0].schoolName ) < 0 ) {
					root = root->two ;  // go to children two
				} // else if()
				else {
					root = root->one ;  // go to children one
				} // else
			} // else
		} // while()
		
		if ( root->right.size() == 0 ) {
			if ( strcmp( sname, root->left[0].schoolName ) == 0 ) {
				hasSame == true ;
			}
			else hasSame = false ;
		}
		else {
			if ( strcmp( sname, root->left[0].schoolName ) == 0  || strcmp( sname, root->right[0].schoolName ) == 0  ) {
				hasSame = true ;
			}
			else hasSame = false ;
		}
		
		return root ;
	} // else
	
} // TwoThreeTree::Found()

ForTwoThreeTree *TwoThreeTree::splitUp( ForTwoThreeTree *root, ForTwoThreeTree *walk, ForTwoThreeTree *ptr1, ForTwoThreeTree *ptr2,
                                        ForTwoThreeTree *ptr3, ForTwoThreeTree *ptr4 ) {  // split up
	if ( root != walk ) {
		swap ( root->middle, walk->middle ) ;  // pull walk->middle to root->middle
	} // if()
	
	if ( strcmp( root->left[0].schoolName, root->middle[0].schoolName ) > 0 ) {  // check left < middle < right
		swap( root->left, root->middle ) ;
	} // if()
	
	else if ( strcmp( root->right[0].schoolName, root->middle[0].schoolName ) < 0 ) {
		swap( root->right, root->middle ) ;
	} // else if()
	
	ForTwoThreeTree *w = root ;
	ForTwoThreeTree *parent = root->parent ;
	ForTwoThreeTree *temp1 = new ForTwoThreeTree() ;
	ForTwoThreeTree *temp2 = new ForTwoThreeTree() ;
	
 	swap( w->left, temp1->left ) ;  // let w->left stroe in temp1->left
	swap( w->right, temp2->left ) ;  // let w->right stroe in temp2->left
	
	if ( w->one != NULL ) {  // not leaf
		temp1->one = ptr1 ;
		temp1->one->parent = temp1 ;
		temp1->two = ptr2 ;
		temp1->two->parent = temp1 ;
		temp2->one = ptr3 ;
		temp2->one->parent = temp2 ;
		temp2->two = ptr4 ;
		temp2->two->parent = temp2 ;
	} // if()
	
	if ( parent == NULL ) {  // means w is root
		parent = new ForTwoThreeTree() ;
		swap( w->middle, parent->left ) ;
		
		parent->one = temp1 ;  // conect temp12 back to parent
		temp1->parent = parent ;
		parent->two = temp2 ;
		temp2->parent = parent ;	

	} // if()
	else if( parent->right.size() == 0 ) {  // only one data in parent
		if ( w == parent->one ) {
			swap( parent->left, parent->right ) ;
			swap( w->middle, parent->left ) ;
			
			parent->three = parent->two ;  // connect temp12 back to parent
			parent->one = temp1 ;
			parent->two = temp2 ;
		} // if()
		else if ( w == parent->two ) {
			swap( w->middle, parent->right ) ;
			
			parent->two = temp1 ;  // connect temp12 back to parent
			parent->three = temp2 ;
		} // else if()
		
		parent->one->parent = parent ;
		parent->two->parent = parent ;
		parent->three->parent = parent ;
	} // else if()
	else {  // two data in parent
		if ( w == parent->one ) {
			parent = splitUp( parent, w, temp1, temp2, parent->two, parent->three ) ; 
		} // if()
		else if ( w == parent->two ) {
			parent = splitUp( parent, w, parent->one, temp1, temp2, parent->three ) ; 
		} // else if()
		else {
			parent = splitUp( parent, w, parent->one, parent->two, temp1, temp2 ) ; 
		} // else
	} // else
	
	
	return parent ;
	
} // TwoThreeTree::splitUp()

int TwoThreeTree::GetHigh( ForTwoThreeTree *root ) {  // find the height of the tree
	
	if ( root == NULL ) {
		return 0 ;
	} // if()
	
	else if ( root->right.size() == 0 ) {  // only one data in this node
		if ( GetHigh(root->one) >= GetHigh(root->two) ) return GetHigh(root->one) + 1 ;  // return the higher side
		else return GetHigh(root->two) + 1 ;
	} // else if()
	else {  // two data in node
		
		if ( GetHigh(root->one) >= GetHigh(root->two) && GetHigh(root->one) >= GetHigh(root->three) ) {  // return the highest side
			return GetHigh(root->one) + 1 ;
		}
		else if ( GetHigh(root->two) > GetHigh(root->one) && GetHigh(root->two) >= GetHigh(root->three) ) {
			return GetHigh(root->two) + 1 ;
		}
		else if ( GetHigh(root->three) > GetHigh(root->one) && GetHigh(root->three) >= GetHigh(root->two) ) {
			return GetHigh(root->three) + 1 ;
		}
		
	} // else	
	
} // TwoThreeTree::GetHigh()

ForTwoThreeTree *TwoThreeTree::Build() {  // start build 23tree 
	
	ForTwoThreeTree *root = NULL ;
	int temp = FindlastElememt() ;
	for( int n = 0 ; n < temp ; n++ ) {
		root = Insert( root, usingVector[n].num, usingVector[n].schoolName ) ;  // insert data into 23tree one by one
	}
	
	int h = GetHigh( root ) ;  // find the height of tree
	printf( "Tree height = %d\n", h ) ;
	PrintRoot( root ) ;  // print the root
	
	return root ;
	
} // TwoThreeTree::Build()

ForTwoThreeTree *TwoThreeTree::Insert( ForTwoThreeTree *root, int num, Str200 sname ) {  // inert data into 23tree
	bool hasSame = true ;
	root = Found( root, sname, hasSame ) ;  // check if there has same scoolname in tree
	if ( ! hasSame ) {
		if ( root == NULL ) {
			root = new ForTwoThreeTree() ;
			root = Store( root, num, sname, "left" ) ;  // store in leftdata
		} // if()
		else if ( root->right.size() == 0 ){
			root = Store( root, num, sname, "right" ) ;  // store in rightdata
			if ( strcmp( root->left[0].schoolName, root->right[0].schoolName ) > 0 ) {  // if left bigger than right, swap them
				swap( root->left, root->right ) ;
			} // if()
		} // else if()
		else {
			root = Store( root, num, sname, "middle" ) ;  // store in middle, prepare to split
			root = splitUp( root, root, NULL, NULL, NULL, NULL ) ;  // SPLIT UP
		} // else
	} // if()
	else {  // hasSame
		if ( strcmp( root->left[0].schoolName, sname ) == 0 ) {  // find is same as leftdata or rightdata
			root = Store( root, num, sname, "left" ) ;  // store in leftdata
		} // if()
		else {
			root = Store( root, num, sname, "right" ) ;  // store in rightdata
		} // else()
	} // else()
	
	for ( ; root->parent != NULL ; root = root->parent ) ;  // return to the root
	
	return root ;
} // TwoThreeTree::Insert()

void TwoThreeTree::PrintRoot( ForTwoThreeTree *root ) {  // print the root
	
	int num = FindlastElememt() ;
	int count = 1, countL = 0, countR ;
	
	for ( int n = 0 ; n < num ; n++ ) {
		if ( root->left[countL].num == usingVector[n].num ) {
			printf( "%d: [%d] %s, %s", count, usingVector[n].num, usingVector[n].schoolName, usingVector[n].departmentName ) ;
			printf( ", %s, %s, %d\n", usingVector[n].club, usingVector[n].level, usingVector[n].student ) ;
			count++ ;
			countL++ ;
		}
		
		if ( root->right.size() != 0 && root->right[countR].num == usingVector[n].num ) {
			printf( "%d: [%d] %s, %s", count, usingVector[n].num, usingVector[n].schoolName, usingVector[n].departmentName ) ;
			printf( ", %s, %s, %d\n", usingVector[n].club, usingVector[n].level, usingVector[n].student ) ;
			count++ ;
			countR++ ;
		}

	} // for()
	
} // TwoThreeTree::PrintRoot()

ForTwoThreeTree *TwoThreeTree::ReSet( ForTwoThreeTree *root ) {
	for ( int i = usingVector.size() ; i > 0 ; i-- ) {
		usingVector.pop_back() ;
	} // for()
	delete root ;
	root = NULL ;
	return root ;
} // *TwoThreeTree::ReSet()

ForTwoThreeTree *TwoThreeTree::Search( ForTwoThreeTree *root, Str200 sname, bool &hasSame ) {  // found if there has same school in tree
	if ( root == NULL ) {
		hasSame = false ;
		return root ;
	} // if()
	else {
		while ( root != NULL ) {  // run until arrive leaf
			if ( root->right.empty() ) {  // only one data in this node
				
				if ( strcmp( sname, root->left[0].schoolName ) == 0 ) {             
					hasSame  = true ;                                   
					return root ;  // find same
				} // if()
				else if ( strcmp( sname, root->left[0].schoolName ) > 0 ) {
					root = root->two ;  // go to children two
				} // else if()
				else {
					root = root->one ;  // go to children one
				} // else
			} // if()
			else {  // two data in node
				if ( strcmp( sname, root->left[0].schoolName ) == 0 || strcmp( sname, root->right[0].schoolName ) == 0  ) { 
					hasSame = true ;
					return root ;  // find same
				} // if()
				else if ( strcmp( sname, root->right[0].schoolName ) > 0  ) {
					root = root->three ;  // go to children three
				} // else if()
				else if ( strcmp( sname, root->left[0].schoolName ) > 0  &&  strcmp( sname, root->right[0].schoolName ) < 0 ) {
					root = root->two ;  // go to children two
				} // else if()
				else {
					root = root->one ;  // go to children one
				} // else
			} // else
		} // while()
		
		
		if ( root == NULL ) {
			hasSame = false ;
			return root ;
		} // if()
		
		hasSame = false ;
		return root ;
	} // else
	
} // TwoThreeTree::Found()

//---------------AvlTree class Function---------------

void AvlTree::OpenAndStore( Str200 openfile, Str200 title1, Str200 title2, bool &havefile ) {
	FILE * fin;
	fin = fopen( openfile, "r" ) ;
	
	int tempCode = 0, num = 1 ;
	Str200 temp ;
	Str200 garbge, temp1, temp2 ;
	char numtemp = '\0' ;
			
	if ( fin ) {
		int check = 0 ;
    fgets( title1, 200, fin ) ;
    fgets( title2, 200, fin ) ;
    fgets( garbge, 200, fin ) ;
                
    while ( !feof( fin ) ) {
      ForVector vectortemp ;
      if ( fscanf( fin, "%d", &tempCode ) == 1 ) {
        vectortemp.num = num ;
        vectortemp.schoolCode = tempCode ;
				fscanf( fin, "%s%s%s", vectortemp.schoolName, vectortemp.departmentCode, vectortemp.departmentName ) ;
						
        fscanf( fin, "%c", temp1 ) ;
        fscanf( fin, "%c", temp1 ) ;
        while ( strcmp( temp1, "\t" ) != 0 ) {
          strcat( vectortemp.club, temp1 ) ;
          fscanf( fin, "%c", temp1 ) ;
				} // while()
						
        fscanf( fin, "%c", temp2 ) ;
        while ( strcmp( temp2, "\t" ) != 0 ) {
          strcat( vectortemp.level, temp2 ) ;
          fscanf( fin, "%c", temp2 ) ;
				} // while()
						
        fscanf( fin, "%c", &numtemp ) ;
        if ( numtemp == '\"' ) {
          fscanf( fin, "%c", &numtemp ) ;
          while ( numtemp != '\"' ) {
            if ( numtemp != ',' ) vectortemp.student = ( 10 * vectortemp.student ) + ( numtemp - '0' ) ;
          	fscanf( fin, "%c", &numtemp ) ;
					} // while()

				} // if()
				else {
					while ( numtemp != '\t' ) {
            vectortemp.student = ( 10 * vectortemp.student ) + ( numtemp - '0' ) ;
            fscanf( fin, "%c", &numtemp ) ;
					} // while()
							
				} // else
						
        fscanf( fin, "%d%d", &vectortemp.teacher, &vectortemp.graduate ) ;
        fscanf( fin, "%s%s", &vectortemp.city, &temp ) ;
        strcat( vectortemp.city, temp ) ;
        fscanf( fin, "%s%s", &vectortemp.sport, &temp ) ;
        strcat( vectortemp.sport, temp ) ;
        usingVector.push_back( vectortemp ) ;
        num++ ;
			} // if()

			//------------initialize------------
			vectortemp.student = 0 ;
			for ( int i = 0 ; i < 200 ; i++ ) {
				vectortemp.club[i] = '\0' ;
				vectortemp.level[i] = '\0' ;
			} // for()
					
    } // while

		havefile = true ;
	} // if()
	else {
		printf( "\n### %s does not exist! ###\n", openfile ) ;
		printf( "\nThere is no data!\n\n" ) ;
	} // else
	
} // AvlTree::OpenAndStore()

void AvlTree::ResetStr( Str200 temp ) {
	for ( int i = 0 ; i < strlen( temp ) ; i++ ) temp[i] = '\0' ;
} // AvlTree::ResetStr

int AvlTree::FindlastElement() {
	return usingVector.size() ;
} // AvlTree::FindlastElement()

ForAvlTree *AvlTree::Insert( ForAvlTree *root, int num ) {
	if ( root == NULL ) {
		root = new ForAvlTree ;
		root->num = usingVector[num].num ;
		strcpy( root->departmentName, usingVector[num].departmentName ) ;
		root->left = NULL ;
		root->right = NULL ;
		root->same = NULL ;
		
		return root ;
	} // if()
	else if ( strcmp( root->departmentName, usingVector[num].departmentName ) > 0 ) {
		root->left = Insert( root->left, num ) ;
		root = Balance( root ) ;
	} // else if()
	else if ( strcmp( root->departmentName, usingVector[num].departmentName ) < 0 ) {
		root->right = Insert( root->right, num ) ;
		root = Balance( root ) ;
	} // else if()
	else if ( strcmp( root->departmentName, usingVector[num].departmentName ) == 0 ) {
		root->same = Insert( root->same, num ) ;
	} // else if()
	
} // if()

ForAvlTree *AvlTree::Balance( ForAvlTree *temp ) {
	int type = LRGap( temp ) ;
	if ( type < -1 ) {	//右邊較重 
		if ( LRGap( temp->right ) > 0 ) {	 //右邊的左邊較重->RR
			temp = RotationRL( temp ) ;
		} // if()
		else {	//右邊的右邊較重->RL
			temp = RotationRR( temp ) ;
		} // else
		
	} // if()
	else if ( type > 1 ) {	//左邊較重
		if ( LRGap( temp->left ) > 0 ) {	//左邊的左邊較重->LR
			temp = RotationLL( temp ) ;
		} // if()
		else {	//左邊的右邊較重->LL
			temp = RotationLR( temp ) ;
		} // esle
		
	} // else if()
	
	return temp ;
} // *AvlTree::Balance()

int AvlTree::LRGap( ForAvlTree *temp ) {
	int leftHigh = GetHigh( temp->left ) ;
	int rightHigh = GetHigh( temp->right ) ;
	int gap = leftHigh - rightHigh ;
	return gap ;
} // *AvlTree::LRGap()

int AvlTree::GetHigh( ForAvlTree *temp ) {
	int h = 0 ;
	if ( temp != NULL ) {
		int left = GetHigh( temp->left ) ;
		int right = GetHigh( temp->right ) ;
		if ( left > right ) {
			h = left + 1 ;
		} // if()
		else {
			h = right + 1 ;
		} // else
		
	} // if()
	
	return h ;
} // AvlTree::GetHigh()

ForAvlTree *AvlTree::RotationRR( ForAvlTree *parent ) {
	ForAvlTree *temp ;
	temp = parent->right ;
	parent->right = temp->left ;
	temp->left = parent ;
	return temp ;
} // *AvlTree::RotationRR()
	
ForAvlTree *AvlTree::RotationRL( ForAvlTree *parent ) {
	ForAvlTree *temp ;
	temp = parent->right ;
	parent->right = RotationLL( temp ) ;
	return RotationRR( parent ) ;
} // *AvlTree::RotationRL()

ForAvlTree *AvlTree::RotationLR( ForAvlTree *parent ) {
	ForAvlTree *temp ;
	temp = parent->left ;
	parent->left = RotationRR( temp ) ;
	return RotationLL( parent ) ;
} // *AvlTree::RotationLR()

ForAvlTree *AvlTree::RotationLL( ForAvlTree *parent ) {
	ForAvlTree *temp ;
	temp = parent->left ;
	parent->left = temp->right ;
	temp->right = parent ;
	return temp ;
} // *AvlTree::RotationLL()

void AvlTree::PrintRoot( ForAvlTree *root ) {
	int num = FindlastElement() ;
	int temp = 1 ;
	printf( "Tree height = %d\n", GetHigh( root ) ) ;
	for ( int n = 0 ; n < num ; n++ ) {
		if ( strcmp( root->departmentName, usingVector[n].departmentName ) == 0 ) {
			printf( "%d: [%d] %s, %s", temp, usingVector[n].num, usingVector[n].schoolName, usingVector[n].departmentName ) ;
			printf( ", %s, %s, %d\n", usingVector[n].club, usingVector[n].level, usingVector[n].student ) ;
			temp++ ;
		} // if()
		
	} // for()
	
} // *AvlTree::PrintRoot()

ForAvlTree *AvlTree::ReSet( ForAvlTree *root ) {
	for ( int i = usingVector.size() ; i > 0 ; i-- ) {
		usingVector.pop_back() ;
	} // for()
	delete root ;
	root = NULL ;
	return root ;
} // *AvlTree::ReSet()

ForAvlTree *AvlTree::Found( ForAvlTree *root, Str200 dname, bool &findD ) {
	if ( root == NULL ) {
		findD = false ;
		return root ;
	} // if()
	else {
		while ( root != NULL ) {
			if ( strcmp( dname, root->departmentName ) == 0 ) {
				findD = true ;
				return root ;
			} // if
			else if ( strcmp( dname, root->departmentName ) > 0 ) {
				root = root->right ;
			}
			else {
				root = root->left ;
			}
		}
		
		findD = false ;
	}
	
} // *AvlTree::Found()

//---------------function---------------
void Initialize( Str200 &openfile, Str200 &tempfile, Str200 &title1, Str200 &title2, bool &havefile ) {
	// initialize bool
	havefile = false ;
	// initialize string
	for ( int i = 0 ; i < 200 ; i++ ) {
		openfile[i] = '\0' ;
		tempfile[i] = '\0' ;
		title1[i] = '\0' ;
		title2[i] = '\0' ;
	} // for()
	
} // Initialize()