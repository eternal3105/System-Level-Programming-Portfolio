// 10927108 劉珈彤

# include <conio.h>
# include <iostream>
# include <string.h>
# include <fstream>
# include <vector>
# include <memory.h>
# include <time.h>
# include <iomanip>

using namespace std ;

typedef char Str50[50] ;

class Queue {
	private:
		struct CPU {
			//----------------input----------------
			int oid ;
			int arrive ;
			int duration ;
			int timeout ;
			//----------------output----------------
			int abort ;
			int departure ;
			int delay ;
		};
		
		struct CPU2 {
			//----------------input----------------
			int oid ;
			int arrive ;
			int duration ;
			int timeout ;
			//----------------output----------------
			int abort ;
			int departure ;
			int delay ;
			bool leave ;
			CPU2 *next ;
		};
		

		typedef vector<CPU> vecCPU ;
		vecCPU data ;
		
		typedef CPU2 *ptrCPU ;
		ptrCPU done, headdone ;
		ptrCPU abort, headabort ;
		ptrCPU lineup, headlineup ;
		ptrCPU walk ;
		
		CPU doing, waiting[3] ;
		
		int timewalk, timeout ;

	public:
		
		void OpenAndStore( Str50 openfile, Str50 &title, bool &havefile ) {
			FILE * fin;
			fin = fopen( openfile, "r" ) ;
			int tempOID = 0, tempArrive = 0, tempDuration = 0 ;
			int tempTimeout = 0, i = 0 ;
			timeout = 0 ;
			
			if ( fin ) {
				int check = 0 ;
        fgets( title, 50, fin ) ;
        while ( !feof( fin ) ) {
          i++ ;
          if ( fscanf( fin, "%d", &tempOID ) == 1 ) {
            fscanf( fin, "%d%d%d", &tempArrive, &tempDuration, &tempTimeout ) ;
            Pushback( tempOID, tempArrive, tempDuration, tempTimeout, i ) ;
          } // if()

          if ( tempTimeout > timeout ) timeout = tempTimeout ;
        } // while
                
				havefile = true ;
			} // if()
			else {
				printf( "\n### %s does not exist! ###\n\n", openfile ) ;
			} // else

		} // OpenAndRead
		
		void Pushback( int oid, int arrive, int duration, int timeout, int i ) {
			data.resize( i ) ;
			data[i-1].oid = oid ;
			data[i-1].arrive = arrive ;
			data[i-1].duration = duration ;
			data[i-1].timeout = timeout ;
			data[i-1].abort = 0 ;
			data[i-1].departure = 0 ;
			data[i-1].delay = 0 ;
			
		} // Pushback()
			
		void ShellSort() {
			int len = data.size()/2 ;
			while ( len > 0 ) {  // 改變間距
				for( int i = len ; i < data.size() ; i++ ) {  // 改變組別的號碼ex:1-7->2-8
					int j = i ;
					while ( j >= len  ) {  // 有交換就往回推
						if ( data[j-len].arrive > data[j].arrive )
							Swap( j-len, j ) ;
						
						else if ( data[j-len].arrive == data[j].arrive ) {
							if ( data[j-len].oid > data[j].oid ) Swap( j-len, j ) ;
							
						} // else if()
							
						j = j - len ;
					} // while()
					
				} // for()
				
				len = len/2 ;  // 間距/2
			} // while()

		} // ShellSort()
		
		void Swap( int i, int j ) {
			int oidTEMP = 0, arriveTEMP = 0, durationTEMP = 0 ;
			int timeoutTEMP = 0 ;
			oidTEMP	= data[j].oid ;
			arriveTEMP = data[j].arrive ;
			durationTEMP = data[j].duration ;
			timeoutTEMP = data[j].timeout ;
			
			data[j].oid = data[i].oid ;
			data[j].arrive = data[i].arrive ;
			data[j].duration = data[i].duration ;
			data[j].timeout = data[i].timeout ;
			
			data[i].oid	= oidTEMP ;
			data[i].arrive = arriveTEMP ;
			data[i].duration = durationTEMP ;
			data[i].timeout = timeoutTEMP ;
			
		} // Swap()
		
		void StoreToSortedfile( Str50 newfile, Str50 title ) {
      ofstream fout( newfile ) ;

      fout << title ;
      for ( int i = 0 ; i < data.size() ; i++ ) {
				fout << data[i].oid << "\t" ;
        fout << data[i].arrive << "\t" ;
      	fout << data[i].duration << "\t" ;
        fout << data[i].timeout << "\n" ;
			} // for()

      fout.close() ;
    } // StoreToSortedfile()
		
		void PrintAll() {
			printf( "\n\tOID	Arrival	Duration	TimeOut\n" ) ;
			for ( int i = 0 ; i < data.size() ; i++ ) {
				printf( "(%d)\t%d\t%d\t%d\t%d\n", i, data[i].oid, data[i].arrive, data[i].duration, data[i].timeout ) ;
			} // for()

		} // PrintAll()
		
		void ChangeVectorToPointer() {
			for ( int i = 0 ; i < data.size() ; i++ ) {
				if ( headlineup == NULL ) {
					lineup = new CPU2 ;
					headlineup = lineup	;
					lineup -> oid = data[i].oid ;
					lineup -> arrive = data[i].arrive ;
					lineup -> duration = data[i].duration ;
					lineup -> timeout = data[i].timeout ;
					lineup -> abort = -1 ;
					lineup -> departure = -1 ;
					lineup -> delay = -1 ;
					lineup -> leave = false ;
					lineup -> next = NULL ;
				} // if()
				else {
					lineup -> next = new CPU2 ;
					lineup = lineup -> next ;
					lineup -> oid = data[i].oid ;
					lineup -> arrive = data[i].arrive ;
					lineup -> duration = data[i].duration ;
					lineup -> timeout = data[i].timeout ;
					lineup -> abort = -1 ;
					lineup -> departure = -1 ;
					lineup -> delay = -1 ;
					lineup -> leave = false ;
					lineup -> next = NULL ;
				} // else()
				
			} // for()
			
		} // ChangeVectorToPointer()
		
		void Work( Str50 openfile, Str50 newfile ) {
			headlineup = NULL ;
			headabort = NULL ;
			headdone = NULL ;
			int timeInDoing = 0 ;
			ChangeVectorToPointer() ;
			Initialize() ;
			timewalk = 1 ;
			walk = headlineup ;
			while ( timewalk <= timeout ) {
				// -----------------確認 done 或 abort -----------------
				if ( doing.duration == 0 && doing.oid != -1 ) {
					Finish( timeInDoing ) ;
					MoveWaiting() ;
					MoveWaitToDo() ;  // 排隊的人排到執行
					MoveWaiting() ;	 // 排隊的人往前排
					timeInDoing = 0 ;
				} // if()
				
				while ( timewalk >= doing.timeout && doing.oid != -1 ) {
					Abort( 3 ) ;
					MoveWaiting() ;
					MoveWaitToDo() ;  // 排隊的人排到執行
					MoveWaiting() ;	 // 排隊的人往前排
					timeInDoing = 0 ;
				} // while()
				
				for ( ptrCPU temp = walk ; temp != NULL ; temp = temp->next ) {  // 檢查所有pointer 
					if ( temp->timeout == timewalk && temp->leave == false ) { 
						if ( headabort == NULL ) {
							abort = new CPU2 ;
							headabort = abort ;			
						} // if()
			
						else {
							abort -> next = new CPU2 ;
							abort = abort -> next ;
						} // else()
						
						abort->oid = temp->oid ;
						abort->abort = timewalk ;
						abort->delay = 0 ;
						abort->next = NULL ;
						temp->leave = true ;
					} // if()
					
				} // for()
					
				// -----------------確認各個東西有沒有缺 -----------------
				MoveWaiting() ;
				MoveWaitToDo() ;  // 排隊的人排到執行
				MoveWaiting() ;	 // 排隊的人往前排
				Saving() ;
				
				// -----------------時間往下跑 -----------------
				doing.duration-- ;
				doing.delay++ ;
				waiting[0].delay++ ;
				waiting[1].delay++ ;
				waiting[2].delay++ ;
				timewalk++ ;
				if ( doing.oid != -1 ) timeInDoing++ ;
			} // while()
			
			StoreToOutputfile( newfile ) ;
		} // Work()
		
		void Finish( int timeInDoing ) {  // 把完成的doing放進done-pointer 
			if ( headdone == NULL ) {
				done = new CPU2 ;
				headdone = done ;
				done -> oid = doing.oid ;
				done -> departure = timewalk ;
				done -> delay = 0 ;
				done -> next = NULL ;			
			} // if()
			
			else {
				done -> next = new CPU2 ;
				done = done -> next ;
				done -> oid = doing.oid ;
				done -> departure = timewalk ;
				done -> delay = doing.delay - timeInDoing ;
				if ( done -> delay == -1 ) done -> delay++ ;
				done -> next = NULL ;
			} // else()
			
			doing.oid = -1 ;
			doing.arrive = -1 ;
			doing.duration = -1 ;
			doing.timeout = -1 ;
			doing.abort = -1 ;
			doing.departure = -1 ;
			doing.delay = -1 ;
			
		} // Finish()
	
		void Abort() {  // 把無法接收的顧客放進abort-pointer
			if ( headabort == NULL ) {
				abort = new CPU2 ;
				
				abort->oid = walk->oid ;
				abort->abort = timewalk ;
				abort->delay = 0 ;
				
				abort->next = NULL ;
				headabort = abort ;
			} // if()
			else{
				abort->next = new CPU2 ;
				abort = abort->next ;
				
				abort->oid = walk->oid ;
				abort->abort = timewalk ;
				abort->delay = 0 ;
				
				abort->next = NULL ;
			} // else
			
			walk->leave = true ;
		} // Abort
		
		void Abort( int i ) {  // 從doing裡來的 

			if ( headabort == NULL ) {
				abort = new CPU2 ;
				headabort = abort ;
				abort -> oid = doing.oid ;
				abort -> abort = timewalk ;
				abort -> delay = doing.delay ;
				abort -> next = NULL ;			
			} // if()
			
			else {
				abort -> next = new CPU2 ;
				abort = abort -> next ;
				abort -> oid = doing.oid ;
				abort -> abort = timewalk ;
				abort -> delay = doing.delay ;
				abort -> next = NULL ;
			} // else()
				
			doing.oid = -1 ;
			doing.arrive = -1 ;
			doing.duration = -1 ;
			doing.timeout = -1 ;
			doing.abort = -1 ;
			doing.departure = -1 ;
			doing.delay = -1 ;
			
			
		} // Abort()
		
		void Initialize() {
			doing.oid = -1 ;
			doing.arrive = -1 ;
			doing.duration = -1 ;
			doing.timeout = -1 ;
			doing.abort = -1 ;
			doing.departure = -1 ;
			doing.delay = -1 ;

			for ( int i = 0 ; i < 3 ; i++ ) {
				waiting[i].oid = -1 ;
				waiting[i].arrive = -1 ;
				waiting[i].duration = -1 ;
				waiting[i].timeout = -1 ;
				waiting[i].abort = -1 ;
				waiting[i].departure = -1 ;
				waiting[i].delay = -1 ;
			} // for()
			
		} // Initialize()

		void MoveWaiting() {  // 排隊的人往前排 
			for ( int i = 0 ; i < 2 ; i++ ) {
				//---------------排隊第二人往前移---------------
				if ( waiting[0].oid == -1 ) {
					waiting[0].oid = waiting[1].oid ;
					waiting[0].arrive = waiting[1].arrive ;
					waiting[0].duration = waiting[1].duration ;
					waiting[0].timeout = waiting[1].timeout ;
					waiting[0].delay = waiting[1].delay ;
					waiting[1].oid = -1 ;
					waiting[1].arrive = -1 ;
					waiting[1].duration = -1 ;
					waiting[1].timeout = -1 ;
					waiting[1].delay = -1 ;
				} // if()
				//---------------排隊第三人往前移---------------
				if ( waiting[1].oid == -1 ) {
					waiting[1].oid = waiting[2].oid ;
					waiting[1].arrive = waiting[2].arrive ;
					waiting[1].duration = waiting[2].duration ;
					waiting[1].timeout = waiting[2].timeout ;
					waiting[1].delay = waiting[2].delay ;
					waiting[2].oid = -1 ;
					waiting[2].arrive = -1 ;
					waiting[2].duration = -1 ;
					waiting[2].timeout = -1 ;
					waiting[2].delay = -1 ;
				} // if()
				
			} // for()
			
		} // MoveWaiting()

		void MoveWaitToDo() {  // 排隊的人排到執行 
			if ( doing.oid == -1 ) {
				int n = 0 ;
				doing.oid = waiting[n].oid ;
				doing.arrive = waiting[n].arrive ;
				doing.duration = waiting[n].duration ;
				doing.timeout = waiting[n].timeout ;
				doing.delay = waiting[n].delay ;
				waiting[n].oid = -1 ;
				waiting[n].arrive = -1 ;
				waiting[n].duration = -1 ;
				waiting[n].timeout = -1 ;
				waiting[n].delay = -1 ;
			} // if()
			
			
		} // MoveWaitToDo
		
		void Saving() {
			while ( walk != NULL && walk->arrive == timewalk ) {
				if ( walk->arrive == timewalk && walk->leave == false ) {
					if ( doing.oid == -1 ) {
						doing.oid = walk->oid ;
						doing.arrive = walk->arrive ;
						doing.duration = walk->duration ;
						doing.timeout = walk->timeout ;
						walk->leave = true ;
					} // if()
					else {
						if ( waiting[0].oid == -1 ) {
							waiting[0].oid = walk->oid ;
							waiting[0].arrive = walk->arrive ;
							waiting[0].duration = walk->duration ;
							waiting[0].timeout = walk->timeout ;
							waiting[0].delay = 0 ;
							walk->leave = true ;
						} // if()
						else if ( waiting[1].oid == -1 ) {
							waiting[1].oid = walk->oid ;
							waiting[1].arrive = walk->arrive ;
							waiting[1].duration = walk->duration ;
							waiting[1].timeout = walk->timeout ;
							waiting[1].delay = 0 ;
							walk->leave = true ;
						} // else if()
						else if ( waiting[2].oid == -1 ) {
							waiting[2].oid = walk->oid ;
							waiting[2].arrive = walk->arrive ;
							waiting[2].duration = walk->duration ;
							waiting[2].timeout = walk->timeout ;
							waiting[2].delay = 0 ;
							walk->leave = true ;
						} // else if()
						else Abort() ;
					
					} // else
					
				} // if()
				
				walk = walk->next ;
			} // while()
			
		} // Saving
		
		void Reset( int command ) {
			if( command == 2 ) {
				delete headdone ;
				delete headabort ;
				headdone = NULL ;
				done = NULL ;
				headabort = NULL ;
				abort = NULL ;
				
				delete headlineup ;
				lineup = NULL ;
				headlineup = NULL ;
				walk = NULL ;
			} // if()

			data.clear() ;
		} // Reset()
		
		void StoreToOutputfile( Str50 newfile ) {
			int i = 1 ;
      ofstream fout( newfile ) ;

      fout << "\t[Abort Jobs]\n" ;
      fout << "\tOID\tAbort\tDelay\n" ;
      for ( ptrCPU walk = headabort ; walk != NULL ; walk = walk->next ) {
        fout << "[" << i << "]\t" ;
				fout << walk->oid << "\t" ;
        fout << walk->abort << "\t" ;
        fout << walk->delay << "\n" ;
        i++ ;
			} // for()

			i = 1 ;
			fout << "\t[Jobs Done]\n" ;
			fout << "\tOID\tDeparture\tDelay\n" ;
			for ( ptrCPU walk = headdone ; walk != NULL ; walk = walk->next ) {
        fout << "[" << i << "]\t" ;
				fout << walk->oid << "\t" ;
        fout << walk->departure << "\t" ;
        fout << walk->delay << "\n" ;
        i++ ;
			} // for()
			
			double Adelay = (double)Caldelay() / (double)data.size() ;
			fout << "[Average Delay]\t" << fixed << setprecision(2) << Adelay << " ms\n" ;
			double success = (double)Caldone() / (double)data.size() * 100 ;
			fout << "[Success Rate]\t" << fixed << setprecision(2) << success << " %\n" ;

      fout.close() ;
    } // StoreToOutputfile
		
		int Caldone() {
			int i = 0 ;
			for ( ptrCPU walk = headdone ; walk != NULL ; walk = walk->next )
				i++ ;
				
			return i ;
		} //Caldone
		
		int Caldelay() {
			int i = 0 ;
			for( ptrCPU walk = headabort ; walk != NULL ; walk = walk->next ) i = i + walk->delay ;
			for( ptrCPU walk = headdone ; walk != NULL ; walk = walk->next ) i = i + walk->delay ;				
			
			return i ;
		} // Adelay
		
}; // Queue

int main() {
	Queue queue ; 
	int command = -1 ;
	int start = 0, end = 0 ;
	int rtime = 0, stime = 0, wtime = 0 ;
	Str50 openfile = {'\0'}, temp = {'\0'}, newfile = {'\0'} ;
	Str50 title = {'\0'} ;
	bool havefile = false ;
	printf( "*** Simulate FIFO Queues by SQF ***\n" ) ;
	printf( "* 0. Quit                         *\n" ) ;
	printf( "* 1. Sort a file                  *\n" ) ;
	printf( "* 2. Simulate one FIFO queue      *\n" ) ;
	printf( "********************\n" ) ;
	printf( "Input a command(0, 1, 2): " ) ;
	if ( scanf( "%d", &command ) == 0 ) return 1 ;
	
	
	while ( command != 0 ) {
		while ( command < 0 || command > 2 ) {
			printf( "\n\nCommand does not exist!\n\n" ) ;
			printf( "*** Simulate FIFO Queues by SQF ***\n" ) ;
			printf( "* 0. Quit                         *\n" ) ;
			printf( "* 1. Sort a file                  *\n" ) ;
			printf( "* 2. Simulate one FIFO queue      *\n" ) ;
			printf( "********************\n" ) ;
			printf( "Input a command(0, 1, 2): " ) ;
			if ( scanf( "%d", &command ) == 0 ) return 1 ; 
		} // while()
		
		if ( command == 1 ) {
			printf( "\nInput a file number (e.g., 401, 402, 403, ...):" ) ;
			scanf( "%s", temp ) ;  // don't throw it away
			strcpy( openfile, "input" ) ;
			strcat( openfile, temp ) ;
			strcat( openfile, ".txt" ) ;
	
			strcpy( newfile, "sorted" ) ;
			strcat( newfile, temp ) ;
			strcat( newfile, ".txt" ) ;
			
			start = clock() ;
			queue.OpenAndStore( openfile, title, havefile ) ;
			end = clock() ;
			rtime = end - start ;
			
			if( havefile ) {
				start = clock() ;
				queue.ShellSort() ;
				end = clock() ;
				stime = end - start ;
				
				start = clock() ;
				queue.StoreToSortedfile( newfile, title ) ;
				end = clock() ;
				wtime = end - start ;
				
				queue.PrintAll() ;
				printf( "請按任意鍵繼續 . . ." ) ;
				getch() ;
				printf( "\n\nReading data: %d clocks (%d.00 ms).\n", rtime, rtime ) ;
				printf( "Sorting data: %d clocks (%d.00 ms).\n", stime, stime ) ;
				printf( "Writing data: %d clocks (%d.00 ms).\n\n", wtime, wtime ) ;
				printf( "\nSee sorted%s.txt\n\n", temp ) ;
			} // if()
		
		} // if()
		else if ( command == 2 ) {
			printf( "\nInput a file number (e.g., 401, 402, 403, ...):" ) ;
			scanf( "%s", temp ) ;
			strcpy( openfile, "sorted" ) ;
			strcat( openfile, temp ) ;
			strcat( openfile, ".txt" ) ;
			
			strcpy( newfile, "output" ) ;
			strcat( newfile, temp ) ;
			strcat( newfile, ".txt" ) ;
			
			queue.OpenAndStore( openfile, title, havefile ) ;
			if ( havefile ) {
				queue.Work( openfile, newfile ) ;
				printf( "\nThe simulation is running...\n" ) ;
				printf( "\nSee output%s.txt\n\n", temp ) ;
			} // if()
			
		} // else if()
	
		if ( havefile ) queue.Reset( command ) ;
		for ( int i = 0 ; i < strlen( openfile ) ; i++ ) openfile[i] = '\0' ;
		for ( int i = 0 ; i < strlen( temp ) ; i++ ) temp[i] = '\0' ;
		for ( int i = 0 ; i < strlen( newfile ) ; i++ ) newfile[i] = '\0' ;
		for ( int i = 0 ; i < strlen( title ) ; i++ ) title[i] = '\0' ;
		havefile = false ; 
		
		command = 0 ;
		printf( "*** Simulate FIFO Queues by SQF ***\n" ) ;
		printf( "* 0. Quit                         *\n" ) ;
		printf( "* 1. Sort a file                  *\n" ) ;
		printf( "* 2. Simulate one FIFO queue      *\n" ) ;
		printf( "********************\n" ) ;
		printf( "Input a command(0, 1, 2): " ) ;
		scanf( "%d", &command ) ;
	} // while()
		
} // main





