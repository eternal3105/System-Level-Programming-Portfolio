// 10927108 劉珈彤

#include <iostream>
#include <stack>
#include <string.h>

// 1�O�Ʀr
// 2�O+-*/
// 3�O(
// 4�O) 

using namespace std ;
typedef char Str50[50] ;

struct Node {
  int num ;
  char operater ;
  int type ;
  Node *next ;
};

typedef Node *NodePtr ;

class Function {
	public :
		int check ;
		int doubleCheck ;
		int isError ;
		int count ;
		Str50 stack ;
		bool findError ;
		NodePtr infix = NULL ;
		NodePtr head = NULL ;
		NodePtr walk = NULL ;
		NodePtr postfix = NULL ;
		
		void cleartrash( char temp ) {
			while( temp != '\n' ) scanf( "%c", &temp ) ;
		} // cleartrash
		
		void StoreToInfix( NodePtr &infix, NodePtr &head, char temp, int check ) {
			if( infix == NULL ) {
				infix = new Node ;
				if( check == 1 ) {
					infix->num = temp - '0' ;
					infix->type = check ;
					infix->next = NULL ;
					head = infix ;
				} // if()
		
				else if( check == 2 || check == 3 || check == 4 ) {
					infix->operater = temp ;
					infix->type = check ;
					infix->num = -1 ;
					infix->next = NULL ;
					head = infix ;
				} // else if()
	
			} // if()
			else {
				if( check == 1 ) {
					infix->next = new Node ;
					infix = infix->next ;
					infix->num = temp - '0' ;
					infix->type = check ;
					infix->next = NULL ;
				} // if()
		
				else if( check == 2 || check == 3 || check == 4 ) {
					infix->next = new Node ;
					infix = infix->next ;
					infix->operater = temp ;
					infix->num = -1 ;
					infix->type = check ;
					infix->next = NULL ;
				} // else if()
				
			} // else
		
		} // Store
		
		void StoreToPostfix( NodePtr &postfix, NodePtr &head, NodePtr &walk, Str50 &stack, int &count, int &check ) {
			if ( postfix == NULL ) {
  				postfix = new Node ;
				if( check == 1 ) {
					postfix->num = walk->num ;
					postfix->type = check ;
					postfix->next = NULL ;
					head = postfix ;
				} // if()
				
			} // if()
					
			else {
				if ( check == 4 ) {
					if ( count > 0 ) {
						for ( int i = count-1 ; i >= 0 && stack[i] != '(' ; i-- ) {
							postfix->next = new Node ;
							postfix = postfix->next ;
							postfix->operater = stack[i] ;
							postfix->type = 2 ;
							postfix->next = NULL ;
							stack[i] = '\0' ;
							count-- ;	
						} // for()

					} // if()
					
					else {
						postfix->next = new Node ;
						postfix = postfix->next ;
						postfix->operater = stack[count] ;
						postfix->type = 2 ;
						postfix->next = NULL ;
						stack[count] = '\0' ;
						count-- ;	
					} // else

				} // if()
				
				else if ( check == 1 ) {
					postfix->next = new Node ;
					postfix = postfix->next ;
					postfix->num = walk->num ;
					postfix->type = check ;
					postfix->next = NULL ;
				} // else if()
				
				else if ( check == 2 ) {
					postfix->next = new Node ;
					postfix = postfix->next ;
					postfix->operater = stack[count-1] ;
					postfix->type = 2 ;
					postfix->next = NULL ;
					stack[count-1] = walk->operater ;
					count-- ;
					if ( ( stack[count-1] == '+' || stack[count-1] == '-' ) && ( stack[count] == '+' || stack[count] == '-' ) ) {
						postfix->next = new Node ;
						postfix = postfix->next ;
						postfix->operater = stack[count-1] ;
						postfix->type = 2 ;
						postfix->next = NULL ;
						stack[count-1] = stack[count] ;
						stack[count] = '\0' ;
						count-- ;
					} // if()
				
				} // else if()
		
			} // else

		} // StoreToPostfix
		
		void NumUp( NodePtr &infix, char temp ) {
			infix->num = infix->num*10 ;
			infix->num = infix->num + ( temp - '0' ) ;
		}  // NumUp
		
		int ReadAndCheck( char &temp ) {
			if( scanf( "%c", &temp ) == EOF ) return 6 ;
			if ( temp == ' ' ) return 0 ;
			else if ( temp == '+' || temp == '-' || temp == '*' || temp == '/' ) return 2 ;
			else if ( temp == '(' ) return 3 ;
			else if ( temp == ')' ) return 4 ;
			else if ( temp >= '0' && temp <= '9' ) return 1 ;
			return 5 ;
		} // ReadAndCheck
		
		void CheckError2( NodePtr walk, NodePtr head, int &check ) {
			if ( head->type == 4 ) {
				check-- ;
				return ;
			} // if() 
	
			while ( walk != NULL ) {
				if ( walk->type == 3 ) check++ ;
				else if ( walk->type == 4 ) check-- ;
				walk = walk->next ;
			} // while()

		} // CheckError2
		
		void CheckError3( NodePtr walk, NodePtr head, int &isError, int &doubleCheck ) {
			if ( walk != NULL && isError == 0 ) {
				if ( walk->type == 3 ) {
					if ( doubleCheck == 1 ) isError = 2 ;
					else if ( doubleCheck == 4 ) isError = 2 ;
					doubleCheck = 3 ;
				} // if()
				else if ( walk->type == 4 ) {
					if ( doubleCheck == 2 ) isError = 3 ;
					else if ( doubleCheck == 3 ) isError = 3 ;
					doubleCheck = 4 ;
				} // else if()
				else if ( walk->type == 1 ) {
					if ( doubleCheck == 4 ) isError = 2 ;
					doubleCheck = 1 ;
				} // else if()
				else if ( walk->type == 2 ) {
					if ( doubleCheck == 2 ) isError = 1 ;
					else if ( doubleCheck == 3 ) isError = 3 ;
					doubleCheck = 2 ;
				} // else if()
				
				if ( walk == head && doubleCheck == 2 ) {
					isError = 2 ;
				} //if()
				
				walk = walk->next ;
				
				if ( walk == NULL && doubleCheck == 2 ) {
					isError = 2 ;
				} //if()
				
				CheckError3( walk, head, isError, doubleCheck ) ;
			} // if()

		} // CheckPostfix

		void ChangeToPostfix( NodePtr &postfix, NodePtr &head, NodePtr &walk, Str50 &stack, int &count, int &check ) {
			while ( walk != NULL ) {
				if ( walk->type == 3 ) {
					stack[count] = '(' ;
					count++ ;
				} // if()
				else if ( walk->type == 4 ) {
					StoreToPostfix( postfix, head, walk, stack, count, check = 4 ) ;
					stack[count-1] = '\0' ;
					count-- ;
				}  // else if()
				else if ( walk->type == 1 )
					StoreToPostfix( postfix, head, walk, stack, count, check = 1 ) ;
				else if ( walk->type == 2 ) {
					if ( count == 0 || stack[count-1] == '(' ) {
						stack[count] = walk->operater ;
						count++ ;
					} // if()
					else {
						if ( ( stack[count-1] == '*' || stack[count-1] == '/' ) &&
							 ( walk->operater == '+' || walk->operater == '-' ) ) {
							 	StoreToPostfix( postfix, head, walk, stack, count, check = 2 ) ;
							 	count++ ;
						} // if()
					
						// ------------------�e�@�ӹB��Ÿ��j------------------
						else if ( ( stack[count-1] == '+' || stack[count-1] == '-' ) &&
								  ( walk->operater == '*' || walk->operater == '/' ) ) {
							stack[count] = walk->operater ;
							count++ ;
						} // if()
						// ------------------�e�@�ӹB��Ÿ��p------------------
						else if ( ( stack[count-1] == '+' || stack[count-1] == '-' ) &&
								  ( walk->operater == '+' || walk->operater == '-' ) ) {
							StoreToPostfix( postfix, head, walk, stack, count, check = 2 ) ;
							count++ ;
						} // else if()
						else if ( ( stack[count-1] == '*' || stack[count-1] == '/' ) &&
								  ( walk->operater == '*' || walk->operater == '/' ) ) {
							StoreToPostfix( postfix, head, walk, stack, count, check = 2 ) ; 	
							count++ ;
						} // else if()
						// ------------------��ӹB��Ť@�ˤj------------------
				
					} // else

				} // else if()
		
				walk = walk->next ;
			} // while()
	
			if ( count >= 0 ) {
				StoreToPostfix( postfix, head, walk, stack, count, check = 4 ) ;
			} // if()

		} // ChangeToPostFix
		
		void InitData( int &check, int &doubleCheck, int &isError, int &count, bool &findError ) {
			check = 0 ;
			doubleCheck = -1 ;
			isError = 0 ;
			count = 0 ;
			findError = false ;
			for ( int i = 0 ; i < 50 ; i++ ) {
				stack[i] = '\0' ;
			}  // for()
		} // InitData()
		
		void ReleaseMemory() {
			delete infix ;
			delete postfix ;
			delete head ;
			delete walk ;
			infix = NULL ;
			head = NULL ;
			walk = NULL ;
			postfix = NULL ;
		} // ReleaseMemory()
};


int main() {
	printf( "* Arithmetic Expression Evaluator *\n" ) ;
	printf( "* 0. QUIT                         *\n" ) ;
	printf( "* 1. Infix2postfix Evaluation     *\n" ) ;
	printf( "***********************************\n" ) ;
	printf( "Input a choice(0, 1): " ) ;
	int command = 0 ;
	if ( scanf( "%d", &command ) == 0 ) return 1 ;
	Str50 garbage ;
	
	while ( command != 0 ) {
		char temp = 'a' ;
		Function function ;
		function.InitData( function.check, function.doubleCheck, function.isError, function.count, function.findError ) ;
		while ( command < 0 || command > 1 ) {
    		printf( "\n\nCommand does not exist!\n\n" ) ;
			printf( "* Arithmetic Expression Evaluator *\n" ) ;
    		printf( "* 0. QUIT                         *\n" ) ;
    		printf( "* 1. Infix2postfix Evaluation     *\n" ) ;
    		printf( "***********************************\n" ) ;
			printf( "Input a choice(0, 1): " ) ;
			if ( scanf( "%d", &command ) == 0 ) return 1 ; 
		} // while()
		
		if ( command == 1 ) {
			printf( "\nInput an infix expression:" ) ;
			
			scanf( "%c", &temp ) ;
			function.check = function.ReadAndCheck( temp ) ;
			while( function.check != 6 && !function.findError && temp != '\n' ) {
				if ( function.check == 1 ) {
					if ( function.check == function.doubleCheck ) function.NumUp( function.infix, temp ) ;
					else function.StoreToInfix( function.infix, function.head, temp, function.check ) ;
				} // if()
				else if ( function.check == 2 || function.check == 3 || function.check == 4 ) function.StoreToInfix( function.infix, function.head, temp, function.check ) ;
				else if ( function.check == 5 ) {
					function.findError = true ;
					printf( "Error 1: %c is not a legitimate character.\n", temp ) ;
					function.cleartrash( temp ) ;
				} // else if()
				
				if( function.check != 0 ) function.doubleCheck = function.check ;
				if( !function.findError ) function.check = function.ReadAndCheck( temp ) ;
			} // while()
			
			if ( function.head == NULL ) {
				function.findError = true ;
			} // if()
			// ------------------Error1------------------
			function.check = 0 ;
			function.walk = function.head ;
			if ( !function.findError ) function.CheckError2( function.walk, function.head, function.check ) ;
			if ( function.check != 0 ) {
				if ( function.check > 0 ) printf( "Error 2: there is one extra open parenthesis.\n" ) ;
				else if ( function.check < 0 ) printf( "Error 2: there is one extra close parenthesis.\n" ) ;
				function.findError = true ;
			} // if()
			
			// ------------------Error2------------------
			function.check = 0 ;
			function.doubleCheck = -1 ;
			function.walk = function.head ;
			if ( !function.findError ) function.CheckError3( function.walk, function.head, function.isError, function.doubleCheck ) ;
			if ( function.isError != 0 ) {
				if ( function.isError == 1 ) printf( "Error 3: there is one extra operator.\n" ) ;
				else if ( function.isError == 2 ) printf( "Error 3: there is one extra operand.\n" ) ;
				else if ( function.isError == 3 ) printf( "Error 3: it is not infix in the parentheses.\n" ) ;
				function.findError = true ;
			} // if()
			
			// ------------------Error3------------------
			function.check = 0 ;
			function.walk = function.head ;
			if ( !function.findError ) {
				function.ChangeToPostfix( function.postfix, function.head, function.walk, function.stack, function.count, function.check ) ;
			
				function.walk = function.head ;
				printf( "It is a legitimate infix expression.\n" ) ;
				printf( "Postfix expression: " ) ;
				for ( function.walk = function.head ; function.walk != NULL ; function.walk = function.walk->next ) {
					if ( function.walk->type == 1 ) printf( "%d ", function.walk->num ) ;
					if ( function.walk->type == 2 ) printf( "%c ", function.walk->operater ) ;
				}  // for()
				
				printf( "\n" ) ;
				
			} // if()
			
			if ( !function.findError ) function.ReleaseMemory() ;
			
		} // if()

		printf( "* Arithmetic Expression Evaluator *\n" ) ;
		printf( "* 0. QUIT                         *\n" ) ;
		printf( "* 1. Infix2postfix Evaluation     *\n" ) ;
		printf( "***********************************\n" ) ;
		printf( "Input a choice(0, 1): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
  	} // while()
	
} // main()