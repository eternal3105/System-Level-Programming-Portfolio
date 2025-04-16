// 10927108 劉珈彤

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream> 
#include <string.h>
using namespace std ;
typedef char str20[20] ;

int gSC = 0 ;

class Maze {
	private :
		int row ;
		int column ;
		vector< vector<char> > maze ;
		vector< vector<char> > copy ;
		int path[100][2] ;
		int location[50][2] ;
		// other thing which we don't want other people to use
	public :
		// fuction : replace 
		// many fuction & data member
		Maze( str20 que, int &check ) { //constructor
			int row = 0 ;
			int column = 0 ;
			char ch ;
			ifstream inFile ;
			inFile.open( que, ios::in ) ;	
			if( inFile ) {
				inFile.get( ch ) ;
				while ( ch != ' ' ) {
					column = column * 10 + ( ch - '0' ) ;
					inFile.get( ch ) ;
				} // while()
				
				inFile.get( ch ) ;
				while ( ch != '\n' ) {
					row = row * 10 + ( ch - '0' ) ;
					inFile.get( ch ) ;
				} // while()
			
			    inFile.get( ch ) ;
			    maze.resize( row ) ;
			    for ( int i = 0 ; i < row ; i++ ) maze[i].resize( column ) ;
			
				for ( int i = 0 ; i < row ; i++ ) {
					for ( int j = 0 ; j < column ; j++ ) {
						maze[i][j] = ch ;
						inFile.get( ch ) ;
					} // for()
					inFile.get( ch ) ;

				} // for()
				
				inFile.close() ;
				check = 1 ; // ����ɮפF!! 
		    } // if()
		    
			else {
				printf( "\n\n%s does not exist!\n\n", que ) ;
				check = 0 ;
				inFile.close() ;
				return ;
			} // else
			
			PutRow( row ) ;
			PutColumn( column ) ;
		} // Maze()
		
		void Print( int find ) {
			bool change = false ;
			if ( find == 1 ) {
				for ( int i = 0 ; i < maze.size() ; i++ ) {
					for ( int j = 0 ; j < maze[i].size() ; j++ ) {
						if ( maze[i][j] == 'V' ) {
							maze[i][j] = 'E' ;
							change = true ;
						} // if()
					
						printf( "%c", maze[i][j] ) ;
					
						if ( change ) {
							maze[i][j] = 'V' ;
							change = false ;	
						} // if()
					} // for()
				
					printf( "\n" ) ;
				} // for()
			} // if()
			
			printf( "\n" ) ;
			for ( int i = 0 ; i < maze.size() ; i++ ) {
				for ( int j = 0 ; j < maze[i].size() ; j++ ) {
					if ( copy[i][j] == 'R' ) {
						copy[i][j] = 'V' ;
						change = true ;
					} // if()
					
					printf( "%c", copy[i][j] ) ;
					if ( change ) {
						copy[i][j] = 'R' ;
						change = false ;
					} // if()
					
				} // for()
				
				printf( "\n" ) ;
			} // for()
			
			printf( "\n" ) ;	
		} // Print()
		
		int GetRow() {
			return row ;
		} // GetRow
		
		int GetColumn() {
			return column ;
		} // GetColumn
		
	    void PutRow( int temprow ) {
	    	row = temprow ;
	    } // PutRow()
		
		void PutColumn( int tempcolumn ) {
	    	column = tempcolumn ;
	    } // PutColumn()
		
		void WASD ( int i, int j, int &check, int &find, int command ) {		
			// 1 ���ܱq�����
			// 2 ���ܱq�W����
			// 3 ���ܱq�k���
			// 4 ���ܱq�U���� 
			maze[i][j] = 'R' ;
			if ( check == 1 ) {
				if ( check != 5 ) Search( i, j+1, check = 1, find, command ) ;  // �k 
				if ( check != 5 ) Search( i+1, j, check = 2, find, command ) ;  // �U
				if ( check != 5 ) Search( i-1, j, check = 4, find, command ) ;  // �W 
			} // if() ���䨫��
					
			else if( check == 2 ) {
				if ( check != 5 ) Search( i, j+1, check = 1, find, command ) ;  // �k 
				if ( check != 5 ) Search( i+1, j, check = 2, find, command ) ;  // �U
				if ( check != 5 ) Search( i, j-1, check = 3, find, command ) ;  // �� 
			} // else if() �W������ 
					
			else if ( check == 3 ) {
				if ( check != 5 ) Search( i+1, j, check = 2, find, command ) ;  // �W 
				if ( check != 5 ) Search( i, j-1, check = 3, find, command ) ;  // ��
				if ( check != 5 ) Search( i-1, j, check = 4, find, command ) ;  // �W
			} // else if() �k�䨫�� 
					
			else if ( check == 4 ) {
				if ( check != 5 ) Search( i, j+1, check = 1, find, command ) ;  // �k
				if ( check != 5 ) Search( i, j-1, check = 3, find, command ) ;  // ��
				if ( check != 5 ) Search( i-1, j, check = 4, find, command ) ;  // �W 
			} // else if() �U������ 
					 
			maze[i][j] = 'V' ;
		} // WASD()
		
		void WASD ( int i, int j, int &check, int &find, int &count, int &goals, int &path_x, int &location_x ) {
			// 1 ���ܱq�����
			// 2 ���ܱq�W����
			// 3 ���ܱq�k���
			// 4 ���ܱq�U���� 
			if ( maze[i][j] != 'G' ) maze[i][j] = 'R' ;
			if ( check == 1 ) {
				if ( check != 5 ) MutiSearch( i, j+1, check = 1, find, count, goals, path_x, location_x ) ; // �k
				if ( check != 5 ) MutiSearch( i+1, j, check = 2, find, count, goals, path_x, location_x ) ; // �U
				if ( check != 5 ) MutiSearch( i-1, j, check = 4, find, count, goals, path_x, location_x ) ; // �W 
			} // if() ���䨫��
					
			else if( check == 2 ) {
				if ( check != 5 ) MutiSearch( i, j+1, check = 1, find, count, goals, path_x, location_x ) ; // �k
				if ( check != 5 ) MutiSearch( i+1, j, check = 2, find, count, goals, path_x, location_x ) ; // �U
				if ( check != 5 ) MutiSearch( i, j-1, check = 3, find, count, goals, path_x, location_x ) ; // �� 
			} // else if() �W������ 
					
			else if ( check == 3 ) {
				if ( check != 5 ) MutiSearch( i+1, j, check = 2, find, count, goals, path_x, location_x ) ; // �U
				if ( check != 5 ) MutiSearch( i, j-1, check = 3, find, count, goals, path_x, location_x ) ; // �� 
				if ( check != 5 ) MutiSearch( i-1, j, check = 4, find, count, goals, path_x, location_x ) ; // �W 
			} // else if() �k�䨫�� 
					
			else if ( check == 4 ) {
				if ( check != 5 ) MutiSearch( i, j+1, check = 1, find, count, goals, path_x, location_x ) ; // �k
				if ( check != 5 ) MutiSearch( i, j-1, check = 3, find, count, goals, path_x, location_x ) ; // �� 
				if ( check != 5 ) MutiSearch( i-1, j, check = 4, find, count, goals, path_x, location_x ) ; // �W 
			} // else if() �U������ 
						 
			if ( maze[i][j] != 'G' ) maze[i][j] = 'V' ;
		} // WASD()
		
		void WASD_For_E ( int i, int j, int &check, int &find, int &count, int &goals, int &path_x, int &location_x, int &walk ) {
			copy[i][j] = 'R' ;
			if ( ( j + 1 < GetColumn() && copy[i][j+1] == 'E' ) || ( j + 1 < GetColumn() && copy[i][j+1] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i, j+1, check = 1, find, count, goals, path_x, location_x, walk ) ; // �k
				gSC-- ;
			} // if()
			if ( ( i + 1 < GetRow() && copy[i+1][j] == 'E' ) || ( i + 1 < GetRow() && copy[i+1][j] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i+1, j, check = 2, find, count, goals, path_x, location_x, walk ) ; // �U
				gSC-- ;
			} //if()
			if ( ( j - 1 > -1 && copy[i][j-1] == 'E' ) || ( j - 1 > -1 && copy[i][j-1] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i, j-1, check = 3, find, count, goals, path_x, location_x, walk ) ; // ��
				gSC-- ;
			} // if()
    		if ( ( i - 1 > -1 && copy[i-1][j] == 'E' ) || ( i - 1 > -1 && copy[i-1][j] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i-1, j, check = 4, find, count, goals, path_x, location_x, walk ) ; // �W
				gSC-- ;
			} // if()
			copy[i][j] = 'V' ;
		} // WASD
		
		void WASD_For_Z ( int i, int j, int &check, int &find, int &count, int &goals, int &path_x, int &location_x, int &walk ) {
			copy[i][j] = 'R' ;
			if ( ( j + 1 < GetColumn() && copy[i][j+1] == 'Z' ) || ( j + 1 < GetColumn() && copy[i][j+1] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i, j+1, check = 1, find, count, goals, path_x, location_x, walk ) ; // �k
				gSC-- ;
			} // if()
			if ( ( i + 1 < GetRow() && copy[i+1][j] == 'Z' ) || ( i + 1 < GetRow() && copy[i+1][j] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i+1, j, check = 2, find, count, goals, path_x, location_x, walk ) ; // �U
				gSC-- ;
			} //if()
			if ( ( j - 1 > -1 && copy[i][j-1] == 'Z' ) || ( j - 1 > -1 && copy[i][j-1] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i, j-1, check = 3, find, count, goals, path_x, location_x, walk ) ; // ��
				gSC-- ;
			} // if()
    		if ( ( i - 1 > -1 && copy[i-1][j] == 'Z' ) || ( i - 1 > -1 && copy[i-1][j] == 'G' ) ) {
				gSC++ ;
				if ( check != 5 && gSC < walk ) ShortestPath( i-1, j, check = 4, find, count, goals, path_x, location_x, walk ) ; // �W
				gSC-- ;
			} // if()
			copy[i][j] = 'V' ;
		} // WASD

		void Search( int i, int j, int &check, int &find, int command ) {
			if ( i == GetRow() || j == GetColumn() || i == -1 || j == -1 ) ;
			
			else if ( maze[i][j] == 'O' || maze[i][j] == 'R' ) ;
			
			else if ( maze[i][j] == 'G' ) {
				check = 5 ;
				find = 1 ; 
				CopyVector() ;
				if( find == 1 && command == 1 ) Print( find ) ;
				if( find == 1 && command == 3 ) {
					for ( int i = 0 ; i < maze.size() ; i++ ) {
						for ( int j = 0 ; j < maze[i].size() ; j++ ) {
							if ( maze[i][j] == 'R' ) {
								maze[i][j] = 'Z' ;
							} // if()

						} // for()

					} // for()
					
					CopyVector() ;	
				} // if()
				
				return ;
			} // if()
			
			else WASD ( i, j, check, find, command ) ;
		} // Search() mission1 recursion
		
		void CopyVector() {
			copy.resize( GetRow() ) ;
			for ( int i = 0 ; i < GetRow() ; i++ ) copy[i].resize( GetColumn() ) ;
			
			for ( int i = 0 ; i < GetRow() ; i++ ) {
				for ( int  j= 0 ; j < GetColumn() ; j++ ) copy[i][j] = maze[i][j] ;
			}// for()
			
		} // CopyVector
		
		void MutiSearch( int i, int j, int &check, int &find, int &count, int &goals, int &k, int &location_x ) {
			if ( i == GetRow() || j == GetColumn() || i == -1 || j == -1 ) ;
			
			else if ( maze[i][j] == 'O' || maze[i][j] == 'R' ) ;
			
			else if ( maze[i][j] == 'G' ) {
				if ( !SameGoal( i, j, location_x ) ) goals-- ;
				
				if ( goals == 0 ) {
                	check = 5 ;
					MemoryGoal( i, j, location_x ) ;
					MemoryPath( k ) ;
				} // if() ���Ҧ���G
				
				else {
					if ( !SameGoal( i, j, location_x ) ) {
						MemoryGoal( i, j, location_x ) ;
						MemoryPath( k ) ;
					} // if()

				} // else if() �٨S�䧹������G �� ���@�˪�
				
				WASD( i, j, check, find, count, goals, k, location_x ) ;
				return ;
			} // else if()
			
			else WASD( i, j, check, find, count, goals, k, location_x ) ;
		} // MutiSearch()
		
		void MemoryPath( int &path_x ) {
			for ( int i = 0 ; i < GetRow() ; i++ ) {
				for ( int j = 0 ; j < GetColumn() ; j++ ) {
					if ( !AlreadyWalk( i, j, path_x ) && maze[i][j] == 'R' ) {
						path[path_x][0] = i ;
						path[path_x][1] = j ;
						path_x++ ;
					} // if()
					
				} // for()
				
			} // for()
			
		} // MemoryPath()
		
		bool AlreadyWalk( int i , int j, int path_x ) {
			for ( int m = 0 ; m < path_x ; m++ ) {
				if ( path[m][0] == i && path[m][1] == j ) return true ;
			} // for()
			
			return false ;
		} // AlreadyWalk()
		
		void MutiPrint( int path_x , int goal ) {
			if ( goal == 0 ) {
				for ( int i = 0 ; i < GetRow() ; i++ ) {
					for ( int j = 0 ; j < GetColumn() ; j++ ) printf( "%c", maze[i][j] ) ;
					printf( "\n" ) ;
				} // for()
				
				printf( "\n" ) ;
				// -------------------------------------------------
				
				for ( int i = 0 ; i < GetRow() ; i++ ) {
					for ( int j = 0 ; j < GetColumn() ; j++ ) if ( maze[i][j] == 'V' ) maze[i][j] = 'E' ;
					
				} // for()
				
				// -------------------------------------------------
				
				for ( int m = 0 ; m < path_x ; m++ ) maze[path[m][0]][path[m][1]] = 'R' ;

				
				for ( int i = 0 ; i < GetRow() ; i++ ) {
					for ( int j = 0 ; j < GetColumn() ; j++ ) printf( "%c", maze[i][j] ) ;
					printf( "\n" ) ;
				} // for()

			} // if()
			
			else {
				for ( int i = 0 ; i < GetRow() ; i++ ) {
					for ( int j = 0 ; j < GetColumn() ; j++ ) printf( "%c", maze[i][j] ) ;	
					printf( "\n" ) ;
				} // for()
				
			} // else

		} // MutiPrint()
		
		bool SameGoal( int i, int j, int location_x ) {
			for ( int n = 0 ; n < location_x  ; n++ ) {
				if ( location[n][0] == i && location[n][1] == j ) return true ;
			} // for()
			
			return false ;
		} // SameGoal()
		
		void MemoryGoal( int i, int j, int &location_x ) {
			if ( !SameGoal( i, j, location_x ) ) {
				location[location_x][0] = i ;
				location[location_x][1] = j ;
				location_x++ ;
			} // if()
				
		} // MemoryGoal()
		
		void ShortestPath( int i, int j, int &check, int &find, int &count, int &goals, int &path_x, int &location_x, int walk ) {
			if ( i == GetRow() || j == GetColumn() || i == -1 || j == -1 ) ;
			
			else if ( copy[i][j] == 'O' || copy[i][j] == 'R' ) ;

			else if ( copy[i][j] == 'G' && gSC < walk ) {
				check = 5 ;
				find = 1 ; 
				PrintEx() ;
			} // else if()
			
			else if ( gSC < walk ) {
				if ( gSC < walk && check != 5 ) WASD_For_E( i, j, check, find, count, goals, path_x, location_x, walk ) ;
				if ( gSC < walk && check != 5 ) WASD_For_Z( i, j, check, find, count, goals, path_x, location_x, walk ) ;
			} // else if()

		} //  ShortestPath()
		
		int CountZ() {
			int count = 0 ;
			for ( int i = 0 ; i < maze.size() ; i++ ) {
				for ( int j = 0 ; j < maze[i].size() ; j++ ) {
					if ( copy[i][j] == 'Z' ) {
						count++ ;
					} // if()

				} // for()

			} // for()
			
			return count ;
		} // CountR()
		
		void PrintEx() {
			for ( int i = 0 ; i < GetRow() ; i++ ) {
				for ( int j = 0 ; j < GetColumn() ; j++ ) {	
					printf( "%c", copy[i][j] ) ;
				} // for()
				
				printf( "\n" ) ;
			} // for()
		} // PrintEx() only for test
		
}; // Maze()


int main() {
	printf( "*** Path Finding ***\n" ) ;
	printf( "* 0. Quit          *\n" ) ;
	printf( "* 1. One goal      *\n" ) ;
	printf( "* 2. More goals    *\n" ) ;
	printf( "* 3. Shortest path *\n" ) ;
	printf( "********************\n" ) ;
	printf( "Input a command(0, 1, 2, 3): " ) ;
	
	int command = 0 ;
	str20 garbage ;
	if ( scanf( "%d", &command ) == 0 ) return 1 ;
	
	while ( command != 0 ) {   // Quit���� 
		while ( command < 0 || command > 3 ) {   // command����W�X�d�� 
			printf( "\n\nCommand does not exist!\n\n" ) ;
			printf( "*** Path Finding ***\n" ) ;
			printf( "* 0. Quit          *\n" ) ;
			printf( "* 1. One goal      *\n" ) ;
			printf( "* 2. More goals    *\n" ) ;
			printf( "* 3. Shortest path *\n" ) ;
			printf( "********************\n" ) ;
			printf( "Input a command(0, 1, 2, 3): " ) ;
			if ( scanf( "%d", &command ) == 0 ) return 1 ; 
		} // while()
		
		ifstream inFile ;
		int i = 0, j = 0, check = 1 ;
		int goal = 0, find = 0, count = 0 ;
		int path_x = 0, location_x = 0, walk = 0  ;
		str20 num, que = { '\0' } ;
		printf( "\n\nInput a file name: " ) ;
		strcpy( que, "input" ) ;
		scanf( "%s", num ) ;
		strcat( que, num ) ;
		strcat( que, ".txt" ) ;
	
		Maze sample( que, check ) ;
		while( check == 0 ) {  // �p�G��Ƨ��L��txt�� 
			printf( "\nInput a file name: " ) ;
			strcpy( que, "input" ) ;
			scanf( "%s", num ) ;
			strcat( que, num ) ;
			strcat( que, ".txt" ) ;
			check = 0 ;
			Maze sample( que, check ) ;
		} // while()
		
		if ( command == 1 ) {
			sample.Search( i, j, check, find, command ) ;
			if ( find != 1 ) {
				sample.CopyVector() ;
				sample.Print( find ) ;
			} // if()
			printf( "\n\n" ) ;
		} // if()
		
		if ( command == 2 ) {  // �hG 
			printf( "\nNumber of G (goals): " ) ;
			if ( scanf( "%d", &goal ) == 0 ) scanf( "%s", garbage ) ; 
			while ( goal > 100 || goal < 1 ) {   // ��G�W�X�d�� 
				printf( "\n\n### The number must be in [1,100] ###\n" ) ;
				printf( "\nNumber of G (goals): " ) ;
				if ( scanf( "%d", &goal ) == 0 ) scanf( "%s", garbage ) ;
			} // while()
				
			sample.MutiSearch( i, j, check, find, count, goal, path_x, location_x ) ;
			sample.MutiPrint( path_x , goal ) ;
		} // if()
		
		if ( command == 3 ) {
			sample.Search( i, j, check, find, command ) ;
			//sample.PrintEx() ;
			if ( find == 1 ) {
				i = 0 ;
				j = 0 ;
				check = 1 ;
				walk = sample.CountZ() + 1 ;
				sample.ShortestPath( i, j, check, find, count, goal, path_x, location_x, walk ) ;
			} // if()
			else if ( find != 1 ) {
				sample.CopyVector() ;
				sample.Print( find ) ;
			} // if()
			
		} // if()

		command = 0 ;
		printf( "*** Path Finding ***\n" ) ;
		printf( "* 0. Quit          *\n" ) ;
		printf( "* 1. One goal      *\n" ) ;
		printf( "* 2. More goals    *\n" ) ;
		printf( "* 3. Shortest path *\n" ) ;
		printf( "********************\n" ) ;
		printf( "Input a command(0, 1, 2, 3): " ) ;
		scanf( "%d", &command ) ;
		gSC = 0 ;
	} // while()
	
	

} // main()
