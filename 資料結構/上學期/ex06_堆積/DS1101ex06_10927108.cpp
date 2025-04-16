// 10927108 劉珈彤

# include <iostream>
# include <math.h>
# include <string.h>
# include <fstream>
# include <vector>
# include <memory.h>


using namespace std ;
typedef char Str200[200] ;

struct School {
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


//--------------------class--------------------
class Heap {
	private:
		vector<School> schoolHeap ;
		vector<School> useHeap ;
	public:
		void OpenAndStore( Str200 openfile, Str200 title1, Str200 title2, bool havefile ) {  // 讀取txt並放入schoolHeap
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
        	School schooltemp ;
          if ( fscanf( fin, "%d", &tempCode ) == 1 ) {
            schooltemp.num = num ;
            schooltemp.schoolCode = tempCode ;
            fscanf( fin, "%s%s%s", schooltemp.schoolName, schooltemp.departmentCode, schooltemp.departmentName ) ;
						
            fscanf( fin, "%c", temp1 ) ;
            fscanf( fin, "%c", temp1 ) ;
            while ( strcmp( temp1, "\t" ) != 0 ) {
              strcat( schooltemp.club, temp1 ) ;
              fscanf( fin, "%c", temp1 ) ;
						} // while()
						
            fscanf( fin, "%c", temp2 ) ;
            while ( strcmp( temp2, "\t" ) != 0 ) {
              strcat( schooltemp.level, temp2 ) ;
              fscanf( fin, "%c", temp2 ) ;
						} // while()
						
            fscanf( fin, "%c", &numtemp ) ;
            if ( numtemp == '\"' ) {
              fscanf( fin, "%c", &numtemp ) ;
              while ( numtemp != '\"' ) {
                if ( numtemp != ',' ) schooltemp.student = ( 10 * schooltemp.student ) + ( numtemp - '0' ) ;
                fscanf( fin, "%c", &numtemp ) ;
							} // while()

						} // if()
						else {
							while ( numtemp != '\t' ) {
                schooltemp.student = ( 10 * schooltemp.student ) + ( numtemp - '0' ) ;
                fscanf( fin, "%c", &numtemp ) ;
							} // while()
							
						} // else
						
            fscanf( fin, "%d%d", &schooltemp.teacher, &schooltemp.graduate ) ;
            fscanf( fin, "%s%s", &schooltemp.city, &temp ) ;
            strcat( schooltemp.city, temp ) ;
            fscanf( fin, "%s%s", &schooltemp.sport, &temp ) ;
            strcat( schooltemp.sport, temp ) ;
            schoolHeap.push_back( schooltemp ) ;
            num++ ;
					} // if()

					//------------initialize------------
					schooltemp.student = 0 ;
					for ( int i = 0 ; i < 200 ; i++ ) {
						schooltemp.club[i] = '\0' ;
						schooltemp.level[i] = '\0' ;
					} // for()
					
        } // while

				havefile = true ;
			} // if()
			else {
				printf( "\n### %s does not exist! ###\n", openfile ) ;
				printf( "\nThere is no data!\n\n" ) ;
			} // else

		} // OpenAndStore()

		void Printf() {
			printf( "學校代碼\t學校名稱\t科系代碼\t科系名稱\t日間/進修別\t等級別\t學生數\t教師數\t上學年度畢業生數\t縣市名稱\t體系別\n" ) ;
			for( int i = 0 ; i < useHeap.size() ; i++ ) {
				printf( "%d\t", useHeap[i].num ) ;
				printf( "%d\t%s\t", useHeap[i].schoolCode, useHeap[i].schoolName ) ;
				printf( "%s\t%s\t", useHeap[i].departmentCode, useHeap[i].departmentName ) ;
				printf( "%s\t%s\t", useHeap[i].club, useHeap[i].level ) ;
				printf( "%d\t%d\t", useHeap[i].student, useHeap[i].teacher ) ;
				printf( "%d\t%s\t", useHeap[i].graduate, useHeap[i].city ) ;
				printf( "%s\n", useHeap[i].sport ) ;
			} //for()
			
		} // printf()

		void Insert( int n ) {  // 將schoolHHeap資料放入useHeap
			School schooltemp ;
			schooltemp.num = schoolHeap[n].num ;
			schooltemp.schoolCode = schoolHeap[n].schoolCode ;
			strcpy( schooltemp.schoolName, schoolHeap[n].schoolName ) ;
			strcpy( schooltemp.departmentCode, schoolHeap[n].departmentCode ) ;
			strcpy( schooltemp.departmentName, schoolHeap[n].departmentName ) ;
			strcpy( schooltemp.club, schoolHeap[n].club ) ;
			strcpy( schooltemp.level, schoolHeap[n].level ) ;
			schooltemp.student = schoolHeap[n].student ;
			schooltemp.teacher = schoolHeap[n].teacher ;
			schooltemp.graduate = schoolHeap[n].graduate ;
			strcpy( schooltemp.city, schoolHeap[n].city ) ;
			strcpy( schooltemp.sport, schoolHeap[n].sport ) ;
			useHeap.push_back( schooltemp ) ;
		} // Insert()

		void Rebuild( int i ) {  // 交換父節點與子節點資料
			int father = ( i - 1 ) / 2 ;
			
			if ( father >= 0 && useHeap[i].student > useHeap[father].student ) {
				int temp = 0 ;
				Str200 tempstr = {"\0" } ;
				//--------------num--------------
				temp = useHeap[father].num ;
				useHeap[father].num = useHeap[i].num ;
				useHeap[i].num = temp ;

				//--------------schoolCode--------------
				temp = useHeap[father].schoolCode ;
				useHeap[father].schoolCode = useHeap[i].schoolCode ;
				useHeap[i].schoolCode = temp ;

				//--------------schoolName--------------
				strcpy( tempstr, useHeap[father].schoolName ) ;
				strcpy( useHeap[father].schoolName, useHeap[i].schoolName ) ;
				strcpy( useHeap[i].schoolName, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------departmentCode--------------
				strcpy( tempstr, useHeap[father].departmentCode ) ;
				strcpy( useHeap[father].departmentCode, useHeap[i].departmentCode ) ;
				strcpy( useHeap[i].departmentCode, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------departmentName--------------
				strcpy( tempstr, useHeap[father].departmentName ) ;
				strcpy( useHeap[father].departmentName, useHeap[i].departmentName ) ;
				strcpy( useHeap[i].departmentName, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------club--------------
				strcpy( tempstr, useHeap[father].club ) ;
				strcpy( useHeap[father].club, useHeap[i].club ) ;
				strcpy( useHeap[i].club, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------level--------------
				strcpy( tempstr, useHeap[father].level ) ;
				strcpy( useHeap[father].level, useHeap[i].level ) ;
				strcpy( useHeap[i].level, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------student--------------
				temp = useHeap[father].student ;
				useHeap[father].student = useHeap[i].student ;
				useHeap[i].student = temp ;

				//--------------teacher--------------
				temp = useHeap[father].teacher ;
				useHeap[father].teacher = useHeap[i].teacher ;
				useHeap[i].teacher = temp ;

				//--------------graduate--------------
				temp = useHeap[father].graduate ;
				useHeap[father].graduate = useHeap[i].graduate ;
				useHeap[i].graduate = temp ;

				//--------------city--------------
				strcpy( tempstr, useHeap[father].city ) ;
				strcpy( useHeap[father].city, useHeap[i].city ) ;
				strcpy( useHeap[i].city, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------sport--------------
				strcpy( tempstr, useHeap[father].sport ) ;
				strcpy( useHeap[father].sport, useHeap[i].sport ) ;
				strcpy( useHeap[i].sport, tempstr ) ;
				ResetStr( tempstr ) ;
				
				temp = 0 ;
				Rebuild( father ) ;
			} // if()
	
		} // Rebuild()
		
		int FindlastElement( int n ) {  // 找到對應vector的資料數量
			if ( n == 1 ) return schoolHeap.size() ;
			return useHeap.size() ;
		} // FindlastElement()
		
		void ResetStr( Str200 temp ) {
			for ( int i = 0 ; i < strlen( temp ) ; i++ ) temp[i] = '\0' ;
		} // ResetStr
		
		void FindRoot() {  // 找出根節點
			printf( "root: [%d] %d\n", useHeap[0].num, useHeap[0].student ) ;
		} // FindRoot()
		
		void FindBottomNode() {  // 找出最底層的節點
			int ans = useHeap.size() - 1 ;
			printf( "bottom: [%d] %d\n", useHeap[ans].num, useHeap[ans].student ) ;
		} // FindBottomNode
		
		void FindLeftNode( int heaphigh ) {  // 找出最左邊的節點
			int muti = 1, ans = 0 ;
			for ( int i = heaphigh - 1 ; i > 0 ; i-- ) {
				ans = ans + muti ;
				muti *= 2 ;
			} // for()
			
			printf( "leftmost bottom: [%d] %d\n", useHeap[ans].num, useHeap[ans].student ) ;
		} // FindLeftNode
		
		void Reset() {
			for ( int i = useHeap.size() ; i > 0 ; i-- ) {
				schoolHeap.pop_back() ;
				useHeap.pop_back() ;
			} // for()
		} // Reset
		
};

class Deap {
	private:
		vector<School> schoolDeap ;
		vector<School> useDeap ;
	public:
		void OpenAndStore( Str200 openfile, Str200 title1, Str200 title2, bool havefile ) {
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
        	School schooltemp ;
          if ( fscanf( fin, "%d", &tempCode ) == 1 ) {
            schooltemp.num = num ;
            schooltemp.schoolCode = tempCode ;
            fscanf( fin, "%s%s%s", schooltemp.schoolName, schooltemp.departmentCode, schooltemp.departmentName ) ;
						
            fscanf( fin, "%c", temp1 ) ;
            fscanf( fin, "%c", temp1 ) ;
            while ( strcmp( temp1, "\t" ) != 0 ) {
              strcat( schooltemp.club, temp1 ) ;
              fscanf( fin, "%c", temp1 ) ;
						} // while()
						
            fscanf( fin, "%c", temp2 ) ;
            while ( strcmp( temp2, "\t" ) != 0 ) {
              strcat( schooltemp.level, temp2 ) ;
              fscanf( fin, "%c", temp2 ) ;
						} // while()
						
            fscanf( fin, "%c", &numtemp ) ;
            if ( numtemp == '\"' ) {
            	fscanf( fin, "%c", &numtemp ) ;
              while ( numtemp != '\"' ) {
              	if ( numtemp != ',' ) schooltemp.student = ( 10 * schooltemp.student ) + ( numtemp - '0' ) ;
                fscanf( fin, "%c", &numtemp ) ;
							} // while()

						} // if()
						else {
							while ( numtemp != '\t' ) {
                schooltemp.student = ( 10 * schooltemp.student ) + ( numtemp - '0' ) ;
                fscanf( fin, "%c", &numtemp ) ;
							} // while()
							
						} // else
						
            fscanf( fin, "%d%d", &schooltemp.teacher, &schooltemp.graduate ) ;
            fscanf( fin, "%s%s", &schooltemp.city, &temp ) ;
            strcat( schooltemp.city, temp ) ;
            fscanf( fin, "%s%s", &schooltemp.sport, &temp ) ;
            strcat( schooltemp.sport, temp ) ;
            schoolDeap.push_back( schooltemp ) ;
            num++ ;
					} // if()

					//------------initialize------------
					schooltemp.student = 0 ;
					for ( int i = 0 ; i < 200 ; i++ ) {
						schooltemp.club[i] = '\0' ;
						schooltemp.level[i] = '\0' ;
					} // for()
					
        } // while

				havefile = true ;
			} // if()
			else {
				printf( "\n### %s does not exist! ###\n", openfile ) ;
				printf( "\nThere is no data!\n\n" ) ;
			} // else

		} // OpenAndStore()

		void Printf() {
			printf( "----------------------------------------------------\n" ) ;
			for( int i = 0 ; i < useDeap.size() ; i++ ) {
				printf( "%d\t", useDeap[i].num ) ;
				printf( "%d\t%s\n", useDeap[i].schoolCode, useDeap[i].schoolName ) ;
			} //for()
			
		} // printf()

		void InsertAEmptyData() {  // 將空的資料放入useDeap
			School schooltemp ;
			useDeap.push_back( schooltemp ) ;
		} // InsertAEmptyData()

		void Insert( int n ) {  // 將schoolDeap資料放入useDeap
			School schooltemp ;
			schooltemp.num = schoolDeap[n].num ;
			schooltemp.schoolCode = schoolDeap[n].schoolCode ;
			strcpy( schooltemp.schoolName, schoolDeap[n].schoolName ) ;
			strcpy( schooltemp.departmentCode, schoolDeap[n].departmentCode ) ;
			strcpy( schooltemp.departmentName, schoolDeap[n].departmentName ) ;
			strcpy( schooltemp.club, schoolDeap[n].club ) ;
			strcpy( schooltemp.level, schoolDeap[n].level ) ;
			schooltemp.student = schoolDeap[n].student ;
			schooltemp.teacher = schoolDeap[n].teacher ;
			schooltemp.graduate = schoolDeap[n].graduate ;
			strcpy( schooltemp.city, schoolDeap[n].city ) ;
			strcpy( schooltemp.sport, schoolDeap[n].sport ) ;
			useDeap.push_back( schooltemp ) ;
		} // Insert()

		void LRChange( int deaphigh, int child ) {
			int max = ChangeHighToMax( deaphigh ) ;
			int spacing = pow( 2, deaphigh-2 ) ;
			
			if ( child + spacing <= max && (child-1)/2 != 0 ) {  // 左子節點與右父節點比較
				if ( useDeap[child].student > useDeap[(child + spacing - 1)/2].student ) {
					int rightFather = (child + spacing - 1)/2 ;
					ChangeData( child, rightFather ) ;
					Rebuild( 2, rightFather ) ;  // 2==max
				} // if()
				else Rebuild( 1, child ) ;
			} // if()
			else if ( child + spacing > max )	{  // 右子節點與左子節點比
				if ( useDeap[child].student < useDeap[child-spacing].student ) {
					int leftChild = child-spacing ;
					ChangeData( child, leftChild ) ;
					Rebuild( 1, leftChild ) ;  // 1==min
				} // if()
				else Rebuild( 2, child ) ;
			} // else if()
			
		} // LRChange()
		
		int ChangeHighToMax( int deaphigh ) {  // 將高度轉換成最大值
			int muti = 1, ans = 0 ;
			for ( int i = deaphigh ; i > 0 ; i-- ) {
				ans = ans + muti ;
				muti *= 2 ;
			} // for()
			
			return ans-1 ;
		} // ChangeHighToMax()
		
		void ChangeData( int n, int m ) {  // 交換資料
			int temp = 0 ;
				Str200 tempstr = {"\0" } ;
				//--------------num--------------
				temp = useDeap[m].num ;
				useDeap[m].num = useDeap[n].num ;
				useDeap[n].num = temp ;

				//--------------schoolCode--------------
				temp = useDeap[m].schoolCode ;
				useDeap[m].schoolCode = useDeap[n].schoolCode ;
				useDeap[n].schoolCode = temp ;

				//--------------schoolName--------------
				strcpy( tempstr, useDeap[m].schoolName ) ;
				strcpy( useDeap[m].schoolName, useDeap[n].schoolName ) ;
				strcpy( useDeap[n].schoolName, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------departmentCode--------------
				strcpy( tempstr, useDeap[m].departmentCode ) ;
				strcpy( useDeap[m].departmentCode, useDeap[n].departmentCode ) ;
				strcpy( useDeap[n].departmentCode, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------departmentName--------------
				strcpy( tempstr, useDeap[m].departmentName ) ;
				strcpy( useDeap[m].departmentName, useDeap[n].departmentName ) ;
				strcpy( useDeap[n].departmentName, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------club--------------
				strcpy( tempstr, useDeap[m].club ) ;
				strcpy( useDeap[m].club, useDeap[n].club ) ;
				strcpy( useDeap[n].club, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------level--------------
				strcpy( tempstr, useDeap[m].level ) ;
				strcpy( useDeap[m].level, useDeap[n].level ) ;
				strcpy( useDeap[n].level, tempstr ) ;
				ResetStr( tempstr ) ;
				
				//--------------student--------------
				temp = useDeap[m].student ;
				useDeap[m].student = useDeap[n].student ;
				useDeap[n].student = temp ;

				//--------------teacher--------------
				temp = useDeap[m].teacher ;
				useDeap[m].teacher = useDeap[n].teacher ;
				useDeap[n].teacher = temp ;

				//--------------graduate--------------
				temp = useDeap[m].graduate ;
				useDeap[m].graduate = useDeap[n].graduate ;
				useDeap[n].graduate = temp ;

				//--------------city--------------
				strcpy( tempstr, useDeap[m].city ) ;
				strcpy( useDeap[m].city, useDeap[n].city ) ;
				strcpy( useDeap[n].city, tempstr ) ;
				ResetStr( tempstr ) ;

				//--------------sport--------------
				strcpy( tempstr, useDeap[m].sport ) ;
				strcpy( useDeap[m].sport, useDeap[n].sport ) ;
				strcpy( useDeap[n].sport, tempstr ) ;
				ResetStr( tempstr ) ;
				
				temp = 0 ;
		} // ChangeData()
		
		void Rebuild( int type, int location ) {  // 遞迴，處理直到依照大小排好
			if ( type == 2 ) {
				int father = ( location - 1 ) / 2 ;
				if ( father > 0 && useDeap[location].student > useDeap[father].student ) {
					ChangeData( location, father ) ;
					Rebuild( 2, father ) ;
				} // if()
				
			} // if()
			else if ( type == 1 ) {
				int father = ( location - 1 ) / 2 ;
				if ( father > 0 && useDeap[location].student < useDeap[father].student ) {
					ChangeData( location, father ) ;
					Rebuild( 1, father ) ;
				} // if()

			} // else if()
			
		} // Rebuild()
		
		int FindlastElement( int n ) {  // 找到對應vector的資料數量
			if ( n == 1 ) return schoolDeap.size() ;
			return useDeap.size() ;
		} // FindlastElement()
		
		void ResetStr( Str200 temp ) {
			for ( int i = 0 ; i < strlen( temp ) ; i++ ) temp[i] = '\0' ;
		} // ResetStr
			
		void FindBottomNode() {
			int ans = useDeap.size() - 1 ;
			printf( "bottom: [%d] %d\n", useDeap[ans].num, useDeap[ans].student ) ;
		} // FindBottomNode
		
		void FindLeftNode( int heaphigh ) {
			int muti = 1, ans = 0 ;
			for ( int i = heaphigh - 1 ; i > 0 ; i-- ) {
				ans = ans + muti ;
				muti *= 2 ;
			} // for()
			
			printf( "leftmost bottom: [%d] %d\n", useDeap[ans].num, useDeap[ans].student ) ;
		} // FindLeftNode
		
		void Reset() {
			for ( int i = schoolDeap.size() ; i > 0 ; i-- ) {
				schoolDeap.pop_back() ;
			} // for()
			
			for ( int i = useDeap.size() ; i > 0 ; i-- ) {
				useDeap.pop_back() ;
			} // for()
		} // Reset
		
};

//--------------------function--------------------
void Initialize( Str200 &openfile, Str200 &tempfile, Str200 &title1, Str200 &title2, bool &havefile ) ;




int main() {
	int command = -1, floor = 0 ;
	Str200 openfile = {'\0'}, tempfile = {'\0'} ;
	Str200 title1, title2 ;
	bool havefile = false ;
	Heap sheap ;
	Deap sdeap ;
	
	printf( "**** Heap Construction *****\n" ) ;
	printf( "* 0. QUIT                  *\n" ) ;
	printf( "* 1. Build a max heap      *\n" ) ;
	printf( "* 2. Build a DEAP          *\n" ) ;
	printf( "*************************************\n" ) ;
	printf( "Input a choice(0, 1, 2): " ) ;
	if ( scanf( "%d", &command ) == 0 ) return 1 ;
	
	
	while ( command != 0 ) {
		while ( command < 0 || command > 2 ) {
			printf( "\n\nCommand does not exist!\n\n" ) ;
			printf( "**** Heap Construction *****\n" ) ;
			printf( "* 0. QUIT                  *\n" ) ;
			printf( "* 1. Build a max heap      *\n" ) ;
			printf( "* 2. Build a DEAP          *\n" ) ;
			printf( "*************************************\n" ) ;
			printf( "Input a choice(0, 1, 2): " ) ;
			if ( scanf( "%d", &command ) == 0 ) return 1 ;
		} // while()
		
		if ( command == 1 ) {
			Initialize( openfile, tempfile, title1, title2, havefile ) ;
			printf( "\nInput a file number ([0] Quit):" ) ;
			scanf( "%s", tempfile ) ;
			if ( strcmp( tempfile, "0" ) != 0 ) {
				strcpy( openfile, "input" ) ;
				strcat( openfile, tempfile ) ;
				strcat( openfile, ".txt" ) ;
				//----------------初始化Vector----------------
				sheap.Reset() ;

				//----------------讀入資料----------------
				sheap.OpenAndStore( openfile, title1, title2, havefile ) ;

				//----------------Insert&Rebuild----------------
				int temp = sheap.FindlastElement( 1 ) ;
				for ( int n = 0 ; n < temp ; n++ ) {
					sheap.Insert( n ) ;
				
					int count = sheap.FindlastElement( 2 ) ;
					sheap.Rebuild( count - 1 ) ;
				} // for()
			
				int heaphigh = 0 ;
				int element = sheap.FindlastElement( 2 ) ;
				while ( element > 0 ) {
					element /= 2 ;
					heaphigh++ ;
				} // while()
			
				//----------------Print Answer----------------
				printf( "<max heap>\n" ) ;
				sheap.FindRoot() ;
				sheap.FindBottomNode() ;
				sheap.FindLeftNode( heaphigh ) ;
			} // if()
			
		} // if()
		
		else if ( command == 2 ) {
			Initialize( openfile, tempfile, title1, title2, havefile ) ;
			printf( "\nInput a file number ([0] Quit):" ) ;
			scanf( "%s", tempfile ) ;
			if ( strcmp( tempfile, "0" ) != 0 ) {
				strcpy( openfile, "input" ) ;
				strcat( openfile, tempfile ) ;
				strcat( openfile, ".txt" ) ;
				//----------------初始化Vector----------------
				sdeap.Reset() ;

				//----------------讀入資料----------------
				sdeap.OpenAndStore( openfile, title1, title2, havefile ) ;

				//----------------Insert&Rebuild----------------
				int temp = sdeap.FindlastElement( 1 ) ;
				sdeap.InsertAEmptyData() ;
				for ( int n = 0 ; n < temp ; n++ ) {
					sdeap.Insert( n ) ;
				
					int count = sdeap.FindlastElement( 2 ) ;
					//----------------計算樹層----------------
					int deaphigh = 0 ;
					int element = sdeap.FindlastElement( 2 ) ;
					while ( element > 0 ) {
						element /= 2 ;
						deaphigh++ ;
					} // while()
					
					sdeap.LRChange( deaphigh, count - 1 ) ;
				} // for()
				
				int heaphigh = 0 ;
				int element = sdeap.FindlastElement( 2 ) ;
				while ( element > 0 ) {
					element /= 2 ;
					heaphigh++ ;
				} // while()
				
				printf( "<DEAP>\n" ) ;
				sdeap.FindBottomNode() ;
				sdeap.FindLeftNode( heaphigh ) ;
			} // if()
			
		} // else if()
		
		command = 0 ;
		printf( "\n**** Heap Construction *****\n" ) ;
		printf( "* 0. QUIT                  *\n" ) ;
		printf( "* 1. Build a max heap      *\n" ) ;
		printf( "* 2. Build a DEAP          *\n" ) ;
		printf( "*************************************\n" ) ;
		printf( "Input a choice(0, 1, 2): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
	} // while()
		
} // main


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
