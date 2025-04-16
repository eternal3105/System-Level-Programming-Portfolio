// 10927108 劉珈彤

# include <iostream>
# include <string.h>
# include <fstream>
# include <vector>
# include <memory.h>


using namespace std ;

typedef char Str200[200] ;


struct School {
	int code ;
	//----------------我是分隔線----------------
	Str200 name ;
	Str200 department ;
	Str200 club ;
	Str200 level ;
	//----------------我是分隔線----------------
	int student ;
	int teacher ;
	int graduate ;
	//----------------我是分隔線----------------
	School *next ;
	School *lnext ;
	School *rnext ;
};
		
typedef School *SchoolPtr ;

// ----------------fuction set----------------
void OpenAndStore( Str200 openfile, SchoolPtr &sorigin, SchoolPtr &sowalk, Str200 &title1, Str200 &title2, bool &havefile ) ;  // filedata放入origin-pointer
void PrintAll( SchoolPtr sowalk, Str200 title1, Str200 title2 ) ;
void StoreToSgraduate( SchoolPtr sowalk, SchoolPtr &sgraduate, int &floor ) ;  // origin-pointer放入Sgraduate-pointer
void StoreToSname( SchoolPtr sowalk, SchoolPtr &sname, int &floor ) ;  // origin-pointer放入Scode-pointer
void PrintSgraduate( SchoolPtr sgraduate, int &IsPrint, int num ) ;  // print Sgraduate
void PrintSname( SchoolPtr sname, int &IsPrint, Str200 name ) ;  // print Sname
void Initialize( SchoolPtr &sorigin, SchoolPtr &sowalk, SchoolPtr &sgraduate, SchoolPtr &sname, Str200 &openfile, Str200 &tempfile, Str200 &title1, Str200 &title2, bool &havefile ) ;
void Print( SchoolPtr walk, int &IsPrint ) ;
void DeleteSorigin( SchoolPtr sorigin, SchoolPtr sowalk, SchoolPtr lastwalk, Str200 name ) ;
void FindName( SchoolPtr &origin, SchoolPtr lastwalk,SchoolPtr walk, SchoolPtr temp, SchoolPtr lasttemp, Str200 name, int path, int &IsPrint ) ;
void DeleteName( SchoolPtr &origin, SchoolPtr lastwalk, SchoolPtr &walk, SchoolPtr temp, SchoolPtr lasttemp, int path ) ;
void CountTreeHigh( SchoolPtr mainPtr, int count, int &finalANS ) ;
// ----------------fuction set----------------

		


int main() {
	SchoolPtr sorigin = NULL, sowalk = NULL ;  // 用於讀入filedata的pointer 
	SchoolPtr sgraduate = NULL ;  // 以畢業生為依據的樹 
	SchoolPtr sname = NULL ;  // 以學校名稱為依據的樹 
	int command = -1, floor = 0 ;
	int sghigh = 0, snhigh = 0 ;  // 紀錄兩棵樹樹高 
	int IsPrint = 0 ;  // 紀錄是否找到 
	Str200 openfile = {'\0'}, tempfile = {'\0'} ;
	Str200 title1, title2 ;
	bool havefile = false ;
	
	SchoolPtr lastwalk = NULL, temp = NULL, lasttemp = NULL ;  // 用於mission3
	int path = 0 ;  // 用於mission3
	
	printf( "*** University Graduate Information System ***\n" ) ;
	printf( "* 0. Quit                                    *\n" ) ;
	printf( "* 1. Create Two Binary Search Trees          *\n" ) ;
	printf( "* 2. Search by Number of Graduates           *\n" ) ;
	printf( "* 3. Search by School Name                   *\n" ) ;
	printf( "* 4. Removal by School Name                  *\n" ) ;
	printf( "**********************************************\n" ) ;
	printf( "Input a command(0, 1-4): " ) ;
	if ( scanf( "%d", &command ) == 0 ) return 1 ;
	
	
	while ( command != 0 ) {
		while ( command < 0 || command > 4 ) {
			printf( "\n\nCommand does not exist!\n\n" ) ;
			printf( "*** University Graduate Information System ***\n" ) ;
			printf( "* 0. Quit                                    *\n" ) ;
			printf( "* 1. Create Two Binary Search Trees          *\n" ) ;
			printf( "* 2. Search by Number of Graduates           *\n" ) ;
			printf( "* 3. Search by School Name                   *\n" ) ;
			printf( "* 4. Removal by School Name                  *\n" ) ;
			printf( "**********************************************\n" ) ;
			printf( "Input a command(0, 1-4): " ) ;
			if ( scanf( "%d", &command ) == 0 ) return 1 ; 
		} // while()
		
		if ( command == 1 ) {
			Initialize( sorigin, sowalk, sgraduate, sname, openfile, tempfile, title1, title2, havefile ) ;
			printf( "\nInput a file number:" ) ;
			scanf( "%s", tempfile ) ;
			strcpy( openfile, "input" ) ;
			strcat( openfile, tempfile ) ;
			strcat( openfile, ".txt" ) ;
			
			
			OpenAndStore( openfile, sorigin, sowalk, title1, title2, havefile ) ;
			sowalk = sorigin ;
			
			if( havefile ) {
				PrintAll( sowalk, title1, title2 ) ;
				
				sowalk = sorigin ;  // 初始化walk指向sorigin-pointer
				sgraduate = NULL ;
				while ( sowalk != NULL ) {
					StoreToSgraduate( sowalk, sgraduate, floor = 0 ) ;
					if ( sghigh < floor ) sghigh = floor ;
					sowalk = sowalk->next ;
				} // while()
				
				sowalk = sorigin ;  // 初始化walk指向sorigin-pointer
				sname = NULL ;
				while ( sowalk != NULL ) {
					StoreToSname( sowalk, sname, floor = 0 ) ;
					if ( snhigh < floor ) snhigh = floor ;
					sowalk = sowalk->next ;
				} // while()
			
				printf( "Tree heights:\n" ) ;
				printf( "{Number of graduates} = %d\n", sghigh ) ;
				printf( "{School name} = %d\n", snhigh ) ;
			} // if()
			
		} // if()
		
		else if ( command == 2 ) {
			if ( !havefile ) printf( "\nPlease choose command 1 first!\n\n" ) ;
			else {
				IsPrint = 0 ;
				printf( "Input the number of graduates:" ) ;
				int num = 0 ;
				scanf( "%d", &num ) ;
				if ( num < 0 ) printf( "### the input string %d is not a decimal number! ###\n", num ) ;
				else PrintSgraduate( sgraduate, IsPrint, num ) ;
				if ( IsPrint == 0 ) printf( "There is no match!\n\n" ) ;
			} // else()
			
		} // else if()
		
		else if ( command == 3 ) {
			if ( !havefile ) printf( "\nPlease choose command 1 first!\n\n" ) ;
			else {
				IsPrint = 0 ;
				printf( "Input a school name:" ) ;
				Str200 name ;
				scanf( "%s", name ) ;
				PrintSname( sname, IsPrint, name ) ;
				if ( IsPrint == 0 ) printf( "There is no match!\n\n" ) ;
			} // else()
		
		} // else if()
		
		else if ( command == 4 ) {
			if ( !havefile ) printf( "\nPlease choose command 1 first!\n\n" ) ;
			else {
				IsPrint = 0 ;
				printf( "Input a school name:" ) ;
				Str200 name ;
				scanf( "%s", name ) ;
				FindName( sname, lastwalk, sowalk = sname, temp, lasttemp, name, path = 0, IsPrint ) ;

				if ( IsPrint == 0 ) printf( "There is no match!\n\n" ) ;
				if ( IsPrint == 0) {
					FindName( sgraduate, lastwalk, sowalk = sgraduate, temp, lasttemp, name, path = 0, IsPrint ) ;
					DeleteSorigin( sorigin, sowalk, lastwalk, name ) ;
				} // if()
				
				floor = 0 ;
				snhigh = 0 ;
				CountTreeHigh( sname, floor, snhigh ) ;
				floor = 0 ;
				sghigh = 0 ;
				CountTreeHigh( sname, floor, sghigh ) ;
				printf( "Tree heights:\n" ) ;
				printf( "{Number of graduates} = %d\n", sghigh ) ;
				printf( "{School name} = %d\n", snhigh ) ;
			} // else()
		
		} // else if()
		
		sghigh = 0 ;
		snhigh = 0 ;
		command = 0 ; // command�b�����s 
		printf( "*** University Graduate Information System ***\n" ) ;
		printf( "* 0. Quit                                    *\n" ) ;
		printf( "* 1. Create Two Binary Search Trees          *\n" ) ;
		printf( "* 2. Search by Number of Graduates           *\n" ) ;
		printf( "* 3. Search by School Name                   *\n" ) ;
		printf( "* 4. Removal by School Name                  *\n" ) ;
		printf( "**********************************************\n" ) ;
		printf( "Input a command(0, 1-4): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
	} // while()
		
} // main

void OpenAndStore( Str200 openfile, SchoolPtr &sorigin, SchoolPtr &sowalk, Str200 &title1, Str200 &title2, bool &havefile ) {
	FILE * fin;
	fin = fopen( openfile, "r" ) ;
	
	int tempCode = 0 ;
	Str200 garbge, temp1, temp2 ;
	char numtemp = '\0' ;
	sorigin = NULL ;
	sowalk = NULL ;
			
	if ( fin ) {
		int check = 0 ;
    fgets( title1, 200, fin ) ;
    fgets( title2, 200, fin ) ;
    fgets( garbge, 200, fin ) ;
                
    while ( !feof( fin ) ) {
      if ( fscanf( fin, "%d", &tempCode ) == 1 ) {
        if ( sorigin == NULL ){
          sowalk = new School ;
          fscanf( fin, "%s%s%s", sowalk->name, garbge, sowalk->department ) ;
					
          fscanf( fin, "%c", temp1 ) ;
          fscanf( fin, "%c", temp1 ) ;
          while ( strcmp( temp1, "\t" ) != 0 ) {
            strcat( sowalk->club, temp1 ) ;
            fscanf( fin, "%c", temp1 ) ;
					} // while()

        	fscanf( fin, "%c", temp2 ) ;
          while ( strcmp( temp2, "\t" ) != 0 ) {
            strcat( sowalk->level, temp2 ) ;
            fscanf( fin, "%c", temp2 ) ;
					} // while()
                              
          fscanf( fin, "%c", &numtemp ) ;
          if ( numtemp == '\"' ) {
            fscanf( fin, "%c", &numtemp ) ;
            while ( numtemp != '\"' ) {
              if ( numtemp != ',' ) sowalk->student = ( 10 * sowalk->student ) + ( numtemp - '0' ) ;
              fscanf( fin, "%c", &numtemp ) ;
						} // while()

					} // if()
					else {
						while ( numtemp != '\t' ) {
              sowalk->student = ( 10 * sowalk->student ) + ( numtemp - '0' ) ;
              fscanf( fin, "%c", &numtemp ) ;
						} // while()

					} // else
                            
          fscanf( fin, "%d%d", &sowalk->teacher, &sowalk->graduate ) ;
          fscanf( fin, "%s%s%s%s", garbge, garbge, garbge, garbge ) ;
          sowalk->next = NULL ;
          sorigin = sowalk ;
				} // if()
				else {
        	sowalk->next = new School ;
          sowalk = sowalk->next ;
          fscanf( fin, "%s%s%s", sowalk->name, garbge, sowalk->department ) ;
					
        	fscanf( fin, "%c", temp1 ) ; // ��tabŪ�� 
          fscanf( fin, "%c", temp1 ) ;
          while ( strcmp( temp1, "\t" ) != 0 ) {
            strcat( sowalk->club, temp1 ) ;
            fscanf( fin, "%c", temp1 ) ;
					} // while()

          fscanf( fin, "%c", temp2 ) ;
        	while ( strcmp( temp2, "\t" ) != 0 ) {
            strcat( sowalk->level, temp2 ) ;
            fscanf( fin, "%c", temp2 ) ;
					} // while()
                    
          fscanf( fin, "%c", &numtemp ) ;
        	if ( numtemp == '\"' ) {
            fscanf( fin, "%c", &numtemp ) ;
            while ( numtemp != '\"' ) {
              if ( numtemp != ',' ) sowalk->student = ( 10 * sowalk->student ) + ( numtemp - '0' ) ;
              fscanf( fin, "%c", &numtemp ) ;
						} // while()

					} // if()
					else {
						while ( numtemp != '\t') {
              sowalk->student = ( 10 * sowalk->student ) + ( numtemp - '0' ) ;
              fscanf( fin, "%c", &numtemp ) ;
						} // while()

					} // else
					  
          fscanf( fin, "%d%d", &sowalk->teacher, &sowalk->graduate ) ;
          fscanf( fin, "%s%s%s%s", garbge, garbge, garbge, garbge ) ;
          sowalk->next = NULL ;
				} // else
                    	
      } // if()

    } // while
        
		havefile = true ;
	} // if()	
	else {
		printf( "\n### %s does not exist! ###\n", openfile ) ;
		printf( "\nThere is no data!\n\n" ) ;
	} // else

} // OpenAndRead

void PrintAll( SchoolPtr sowalk, Str200 title1, Str200 title2 ) {
	printf( "\n%s%s", title1, title2 ) ;
	printf( "\t學校名稱\t科系名稱\t日間/進修別\t等級別\t學生數\t教師數\t上學年度畢業生數\n" ) ;
	for( int i = 1 ; sowalk != NULL ; i++ ) {
		printf( "[%d]", i ) ;
		printf( "\t%s\t%s\t%s\t\t", sowalk->name, sowalk->department, sowalk->club ) ;
		printf( "%s\t", sowalk->level ) ;
		printf( "%d\t%d\t%d\n", sowalk->student, sowalk->teacher, sowalk->graduate ) ;
		sowalk = sowalk->next ;
	} //for()
	
} // PrintAll

void StoreToSgraduate( SchoolPtr sowalk, SchoolPtr &sgraduate, int &floor ) {
	if ( sgraduate == NULL ) {
		sgraduate = new School ;
		
		strcpy( sgraduate->name, sowalk->name ) ;
		strcpy( sgraduate->department, sowalk->department ) ;
		strcpy( sgraduate->club, sowalk->club ) ;
		strcpy( sgraduate->level, sowalk->level ) ;
		sgraduate->student = sowalk->student ;
		sgraduate->teacher = sowalk->teacher ;
		sgraduate->graduate = sowalk->graduate ;
		
		sgraduate->rnext = NULL ;
		sgraduate->lnext = NULL ;
	} // if()
	else if ( sowalk->graduate >= sgraduate->graduate )
		StoreToSgraduate( sowalk, sgraduate->rnext, floor ) ;
	else if ( sowalk->graduate < sgraduate->graduate )
		StoreToSgraduate( sowalk, sgraduate->lnext, floor ) ;
	
	floor++ ;
} // StoreToSgraduate

void StoreToSname( SchoolPtr sowalk, SchoolPtr &sname, int &floor ) {
		if ( sname == NULL ) {
		sname = new School ;
		
		strcpy( sname->name, sowalk->name ) ;
		strcpy( sname->department, sowalk->department ) ;
		strcpy( sname->club, sowalk->club ) ;
		strcpy( sname->level, sowalk->level ) ;
		sname->student = sowalk->student ;
		sname->teacher = sowalk->teacher ;
		sname->graduate = sowalk->graduate ;
		
		sname->rnext = NULL ;
		sname->lnext = NULL ;
	} // if()
	else if ( strcmp( sowalk->name, sname->name ) >= 0 )
		StoreToSname( sowalk, sname->rnext, floor ) ;
	else if ( strcmp( sowalk->name, sname->name ) < 0 )
		StoreToSname( sowalk, sname->lnext, floor ) ;
	
	floor++ ;	
} // StoreToSname

void PrintSgraduate( SchoolPtr sgraduate, int &IsPrint, int num ) {
	if ( sgraduate == NULL ) return ;
	
	else if ( sgraduate->graduate >= num ) {
		SchoolPtr walk = sgraduate->rnext ;
		if ( IsPrint == 0 ) printf( "Search results:\n" ) ;
		printf( "[%d]\t", IsPrint + 1 ) ;
		printf( "%s\t", sgraduate->name ) ;
		printf( "%s\t", sgraduate->department ) ;
		printf( "%s\t", sgraduate->club ) ;
		printf( "%s\t", sgraduate->level ) ;
		printf( "%d\t", sgraduate->student ) ;
		printf( "%d\t", sgraduate->teacher ) ;
		printf( "%d\n", sgraduate->graduate ) ;	
		IsPrint++ ;
		Print( walk, IsPrint ) ;
		PrintSgraduate( sgraduate->lnext, IsPrint, num ) ;
	} // else()
	
	else PrintSgraduate( sgraduate->rnext, IsPrint, num ) ;
	
}  // PrintSgraduate()

void PrintSname( SchoolPtr sname, int &IsPrint, Str200 name ) {
	if ( sname == NULL ) return ;
	
	else if ( strcmp( name, sname->name ) >= 0 ) {
		if ( IsPrint == 0 ) printf( "Search results:\n" ) ;
		printf( "[%d]\t", IsPrint + 1 ) ;
		printf( "%s\t", sname->name ) ;
		printf( "%s\t", sname->department ) ;
		printf( "%s\t", sname->club ) ;
		printf( "%s\t", sname->level ) ;
		printf( "%d\t", sname->student ) ;
		printf( "%d\t", sname->teacher ) ;
		printf( "%d\n", sname->graduate ) ;	
		IsPrint++ ;
		PrintSname( sname->rnext, IsPrint, name ) ;
	} // else()
	
	else PrintSname( sname->lnext, IsPrint, name ) ;
	
}  // PrintSgraduate()

void Initialize( SchoolPtr &sorigin, SchoolPtr &sowalk, SchoolPtr &sgraduate, SchoolPtr &sname, Str200 &openfile, Str200 &tempfile, Str200 &title1, Str200 &title2, bool &havefile ) {
	// initialize pointer
	delete sorigin ;
	delete sgraduate ;
	delete sname ;
	sorigin = NULL ;
	sowalk = NULL ;
	sgraduate = NULL ;
	sname = NULL ;
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

void Print( SchoolPtr walk, int &IsPrint ) {
	if ( walk == NULL ) {
		return ;
	} // if()
	else {
		Print( walk->lnext, IsPrint ) ;
		
		printf( "[%d]\t", IsPrint + 1 ) ;
		printf( "%s\t", walk->name ) ;
		printf( "%s\t", walk->department ) ;
		printf( "%s\t", walk->club ) ;
		printf( "%s\t", walk->level ) ;
		printf( "%d\t", walk->student ) ;
		printf( "%d\t", walk->teacher ) ;
		printf( "%d\n", walk->graduate ) ;	
		IsPrint++ ;
		
		Print( walk->rnext, IsPrint ) ;		
	} // else()

} // Print

void FindName( SchoolPtr &origin, SchoolPtr lastwalk,SchoolPtr walk, SchoolPtr temp, SchoolPtr lasttemp, Str200 name, int path, int &IsPrint ) {
	if ( walk == NULL )
		return ;
	
	else {
		if ( strcmp( walk->name, name ) == 0 ) {
			printf( "[%d]\t", IsPrint + 1 ) ;
			printf( "%s\t", walk->name ) ;
			printf( "%s\t", walk->department ) ;
			printf( "%s\t", walk->club ) ;
			printf( "%s\t", walk->level ) ;
			printf( "%d\t", walk->student ) ;
			printf( "%d\t", walk->teacher ) ;
			printf( "%d\n", walk->graduate ) ;	
			DeleteName( origin, lastwalk, walk, temp, lasttemp, path ) ;
			IsPrint++ ;
		} // if() 
		
		lastwalk = walk ;
		FindName( origin, lastwalk, walk->lnext, temp, lasttemp, name, path = 1, IsPrint ) ;
		
		lastwalk = walk ;
		FindName( origin, lastwalk, walk->rnext, temp, lasttemp, name, path = 2, IsPrint ) ;
	} // else()
	
} // DeleteSgraduate

void DeleteName( SchoolPtr &origin, SchoolPtr lastwalk, SchoolPtr &walk, SchoolPtr temp, SchoolPtr lasttemp, int path ) {
	if ( walk->lnext != NULL && walk->rnext != NULL ) {
		temp = walk->rnext ;
		lasttemp = NULL ;
		while ( temp->lnext != NULL ) {
			lasttemp = temp ;
			temp = temp->lnext ;
		} // while()
		
		if ( lasttemp == NULL ) {
			if ( path == 1 ) lastwalk->lnext = temp ;
			else if ( path == 2 ) lastwalk->rnext = temp ;
			
			temp->lnext = walk->lnext ;
			walk->lnext = NULL ;
			walk->rnext = NULL ;
			delete walk ;
			walk = temp ;
		} // if()
		else {
			if ( temp->rnext != NULL ) lasttemp->lnext = temp->rnext ;
			lasttemp->lnext = NULL ;
			
			if ( path == 1 ) lastwalk->lnext = temp ;
			else if ( path == 2 ) lastwalk->rnext = temp ;
			
			temp->lnext = walk->lnext ;
			temp->rnext = walk->rnext ;
			walk->lnext = NULL ;
			walk->rnext = NULL ;
			delete walk ;
			walk = temp ;
		} // else
		
	} // if()
	
	else if ( walk->lnext != NULL && walk->rnext == NULL ) {
		temp = walk ; 
		walk = walk->lnext ;
		
		if ( path == 1 ) lastwalk->lnext = walk ;
		else if ( path == 2 ) lastwalk->rnext = walk ;
		
		temp->rnext = NULL ;
		delete temp ;
		temp = NULL ;
	} // else if()
	
	else if ( walk->lnext == NULL && walk->rnext != NULL ) {
		temp = walk ;
		walk = walk->rnext ;
		
		if ( path == 1 ) lastwalk->lnext = walk ;
		else if ( path == 2 ) lastwalk->rnext = walk ;
		
		temp->lnext = NULL ;
		delete temp ;
		temp = NULL ;
	} // else if()
	
	else if ( walk->lnext == NULL && walk->rnext == NULL ) {
		if ( path == 1 ) lastwalk->lnext = NULL ;
		else if ( path == 2 ) lastwalk->rnext = NULL ;
		
		delete walk ;
		walk = lastwalk ;
	} // else if()
	
} // DeleteName

void DeleteSorigin( SchoolPtr sorigin, SchoolPtr sowalk, SchoolPtr lastwalk, Str200 name ) {
	sowalk =  sorigin ; 
  while ( sowalk != NULL ) {
    if ( strcmp( sowalk->name, name) == 0 ) {
      if ( sorigin == sowalk ) {
        sorigin = sowalk->next ;
        sowalk->next = NULL ;
        delete sowalk ;
        sowalk = sorigin ;
      } // if()
      else {
        lastwalk->next = sowalk->next ;
        sowalk->next = NULL ;
      	delete sowalk ;
        sowalk = lastwalk->next ;
      } // else()

    } // if()

    lastwalk = sowalk ;
    sowalk = sowalk->next ;
  } // while()

} // DeleteSorigin()

void CountTreeHigh( SchoolPtr mainPtr, int count, int &finalANS ) {
	if ( mainPtr == NULL ) {
		if ( finalANS <= count ) finalANS = count ;	
		return ;
	} // if()
	
	else {
		CountTreeHigh( mainPtr->lnext, count+1, finalANS ) ;
			
		CountTreeHigh( mainPtr->rnext, count+1, finalANS ) ;	
	} // else()
	
} // CountTreeHigh()
