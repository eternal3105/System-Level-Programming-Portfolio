// 10927108 劉珈彤

# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <fstream>
# include <string>
# include <string.h>
# include <vector>
# include <sstream> 

#define hash 100

using namespace std ;
typedef char Str20[20] ;
typedef char Str200[200] ;

//---------------struct---------------
struct TData {  // Table1、2 Data
	char del ;
	Str20 resWord ;
};

struct TIData {  // Table3、4、7 Data
	int integer ;
	float realNum ;
	int infor ;
};

struct TFiveData {  // Table5 Data
	Str20 id ;
	int scope ;
	int type ;
	int poTable ;
	int poLoc ;
	bool haveHash ;
};

struct TMData {  // Token 資料 
	Str200 middle ;
};

struct SData {  // Table6 Data 
	int loc ;
	int operator1_1 ;
	int operator1_2 ;
	int operand1_1 ;
	int operand1_2 ;
	int operand2_1 ;
	int operand2_2 ;
	int result1_1 ;
	int result1_2 ;
	Str200 code ;
};

struct PreFix {
	Str20 op ;
};

//---------------全域變數---------------
vector<TData> gtable1, gtable2 ;
vector<TIData> gtable3, gtable4, gtable7 ;
vector<TFiveData> gtable5 ;
vector<SData> gtable6 ;
vector<PreFix> stack1, stack2 ;
int gLoc, gScope ;  // gLoc代表現在gtable6的size，gScope代表目前所處的勢力範圍 

//---------------function---------------
void ReadTable( Str20 tempfile, int which ) ;  // 把Table1、2放入vector 
void ReadInput( Str20 tempfile, vector<TMData> &main ) ;  // 把輸入檔一行一行讀入vector
bool CmpWithDel( char temp ) ;  // 將字元與table1相比 

void Classify( vector<TMData> main, vector<TMData> forLabel ) ;  // 初步判斷是11個statement裡的哪一個 
void IsProgram( vector<TMData> main, int m, int i ) ;  // statement為program的後續處理 
void IsVariable( vector<TMData> main, int m, int i, int round, int &haveError ) ;  // statement為variable的後續處理 
void IsDimension( vector<TMData> main, int m, int i, int round, int &haveError ) ;  // statement為dimension的後續處理 
void IsLabel( vector<TMData> main, int m, int i, vector<TMData> &forLabel, int round, int &haveError ) ;  // statement為label的後續處理 
void IsIF( vector<TMData> main, int m, int i, int round, int &haveError, int type ) ;
void IsGTO( vector<TMData> main, int m, int i ) ;  // statement為GTO的後續處理 
void IsSubroutine( vector<TMData> main, int m, int i, int round, int &haveError ) ;  // statement為subroutine的後續處理 
void SubOfSubroutine( vector<TMData> main, int m, int i, int round, int type ) ;  // subroutine的附屬程式，用於處理()裡的data tpye與id 
void IsCall( vector<TMData> main, int m, int i, int round, int &haveError ) ;  // statement為call的後續處理 
int SubOfCall( vector<TMData> main, int m, int i, int round ) ;
void IsInput( vector<TMData> main, int m, int i, int round, int &haveError ) ;  // statement為input的後續處理 
void IsOutput( vector<TMData> main, int m, int i, int round, int &haveError ) ;  // statement為output的後續處理 
void IsAssignment( vector<TMData> main, int m, int i, int round, int &haveError ) ;  // statement為assignment的後續處理 

void CheckIdentifier( Str20 string, int &haveError ) ;  // 檢查此字串是否符合id規範 
void CheckNumber( Str20 string, int &haveError ) ;  // 檢查此字串是否符合數字規範 
void CheckArray( vector<TMData> main, int m, int &i, Str20 string, int &haveError, int round ) ; // 檢查此字串是否符合array規範 
void CheckNumber( Str20 string, int &haveError ) ; // 檢查此字串是否符合number規範 
void InsertToLabel( vector<TMData> main, int m, int i, vector<TMData> &forLabel ) ;
void DFLocForLabel( Str20 string ) ;
bool CmpWithLabel( vector<TMData> forLabel, Str20 string ) ;
int WhichTypeOfID( Str20 string ) ;
bool CmpWithResWord( Str20 string ) ;  // 將字串與table2相比 
int CheckDataType( Str20 string ) ;  // 判斷此字串為哪種data type 
void WriteErrorMessage( int m, int ErrorType ) ;  // 寫入錯誤訊息 
void ChangeToIMCode( vector<TMData> main, int m, int i, int type, int classify, int poLoc ) ;
void WriteToTable3( Str20 id ) ;
void WriteToTable4( Str20 id ) ;
void WriteToTable6( Str20 id, int fish, int classify ) ;
int WriteToTable7( vector<TMData> main, int m, int i, int dataType ) ;
void WriteToTable7_2( int type, int theLoc ) ;
int GetASCII( Str20 string ) ;  // 取得ASCII碼 
void HashTable5( Str20 id, int type, int &fish, int classify, int poLoc ) ;  // 將資料hash進table5裡 
void StringClear( Str20 string ) ;  // 將string清空 
void FoutTXT() ;

//---------------main---------------
int main() {
	int command = -1, which = 0 ;
	FILE *fop ;
	Str20 tempfile ;  // 打開資料夾用
	bool havefile = false ;  // 是否有打開資料夾
	vector<TMData> main, forLabel ;  // TXT初步讀入temp 
	
	gLoc = 0 ;
	gScope = 0 ;
	gtable3.resize( 1 ) ;
	gtable4.resize( 1 ) ;
	gtable5.resize( hash ) ;
	gtable6.resize( 1 ) ;
	gtable7.resize( 1 ) ;
	forLabel.resize( 1 ) ;
	stack1.resize( 1 ) ;
	stack2.resize( 1 ) ;
	//---------------把table1、2放入vector---------------
	strcpy( tempfile, "Table1.table" ) ;
	ReadTable( tempfile, which = 1 ) ;
	strcpy( tempfile, "Table2.table" ) ;
	ReadTable( tempfile, which = 2 ) ;
	
	
	do {
		printf( "\n****       Compiler      ****\n" ) ;
		printf( "* 0. QUIT                   *\n" ) ;
		printf( "* 1. Compiler               *\n" ) ;
		printf( "*****************************\n" ) ;
		printf( "Input a choice( 0, 1 ): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
		if ( command == 0 ) return 0 ;
		printf( "\nInput a file name: " ) ;
		scanf( "%s", tempfile ) ;
		strcat( tempfile, ".txt" ) ;
		
		ReadInput( tempfile, main ) ;
		Classify( main, forLabel ) ;
		FoutTXT() ;
	} while ( command != 0 ) ;
	
} // main()


//---------------function---------------
void ReadTable( Str20 tempfile, int which ) {
	FILE *fop ;
	char temp ;
	fop = fopen( tempfile, "r+" ) ;
	
	gtable1.resize( 12 ) ;
	gtable2.resize( 26 ) ;
	for ( int m = 0 ; !feof( fop ) ; m++ ) {
		if ( which == 1 ) {
			fscanf( fop, "%c", &gtable1[m].del ) ;
			fscanf( fop, "%c", &temp ) ;
		} // if
		if ( which == 2 ) fscanf( fop, "%s", gtable2[m].resWord ) ;
	} // while()
	
	fclose( fop ) ;
} // ReadTable()


void ReadInput( Str20 tempfile, vector<TMData> &main ) {
	FILE *fop ;
	int putFence = 0 ;
	char temp ;
	fop = fopen( tempfile, "r+" ) ;
	
	for ( int i = 0 ; fscanf( fop, "%c", &temp ) == 1 && !feof( fop ) ; i++ ) {  //直到讀完整個file
		main.resize( i+1 ) ;  // 擴充vector大小
		for ( int j = 0 ; temp != '\n' ; j++ ) {  // 直到讀完整段資料
			if ( temp == ' ' || temp == '\t' || CmpWithDel( temp ) ) {  // 是空白，以空白隔開token
				if ( j > 0 && main[i].middle[j-1] != ' ' ) {  // 這個空白不位於0，且前面一位不為空白 
					main[i].middle[j] = ' ' ;
					j++ ;
				} // if
				
				if ( CmpWithDel( temp ) ) {  // 是delimiter，以空白隔開token
					main[i].middle[j] = temp ;
					j++ ;
					main[i].middle[j] = ' ' ;
					j++ ;
				} // if
				
				j-- ;
			} // if
			else {
				main[i].middle[j] = temp ;
			} // else
			
			if ( main[i].middle[j] == ' ' ) putFence = j ;
			else putFence = j + 1 ;
			
			if ( fscanf( fop, "%c", &temp ) != 1 ) {
				break ;
			} // if
			
		} // for 
		
		main[i].middle[putFence] = ' ' ;
		main[i].middle[putFence+1] = '|' ;
		main[i].middle[putFence+2] = ' ' ;
		putFence = 0 ;
	} // for
} // ReadInput()


bool CmpWithDel( char temp ) {
	bool haveIt = false ;
	for ( int m = 0 ; m < gtable1.size() ; m++ ) {
		if ( temp == gtable1[m].del ) {
			haveIt = true ;
		} // if
			
	} // for
	
	if ( haveIt ) return true ;
	else return false ;
} // CmpWithDel()


void Classify( vector<TMData> main, vector<TMData> forLabel ) {
	int i = 0, haveError = 0 ;
	Str20 statement ;
	
	for ( int m = 0 ; m < main.size() ; m++ ) {
		for ( i = 0 ; main[m].middle[i] != ' ' ; i++ ) {
			statement[i] = main[m].middle[i] ;
		} // for
		
		if ( strcmp( statement, "PROGRAM" ) == 0 ) IsProgram( main, m, i+1 ) ;
		else if ( strcmp( statement, "ELSE" ) == 0 ) ;
		else if ( strcmp( statement, "SUBROUTINE" ) == 0 ) IsSubroutine( main, m, i+1, 1, haveError ) ;
		else if ( strcmp( statement, "VARIABLE" ) == 0 ) IsVariable( main, m, i+1, 1, haveError ) ;
		else if ( strcmp( statement, "LABEL" ) == 0 ) IsLabel( main, m, i+1, forLabel, 1, haveError ) ;
		else if ( strcmp( statement, "DIMENSION" ) == 0 ) IsDimension( main, m, i+1, 1, haveError ) ;
		else if ( strcmp( statement, "IF" ) == 0 ) IsIF( main, m, i+1, 1, haveError, 0 ) ;
		else if ( strcmp( statement, "GTO" ) == 0 ) ; //IsGTO( main, m, i+1 ) ;
		else if ( strcmp( statement, "CALL" ) == 0 ) IsCall( main, m, i+1, 1, haveError ) ;
		else if ( strcmp( statement, "INPUT" ) == 0 ) IsInput( main, m, i+1, 1, haveError ) ;
		else if ( strcmp( statement, "OUTPUT" ) == 0 ) IsOutput( main, m, i+1, 1, haveError ) ;
		else if ( strcmp( statement, "L92" ) == 0 ) ;
		else {
			if ( CmpWithLabel( forLabel, statement ) ) {
				DFLocForLabel( statement ) ;  // 一開始為L91這種
				for ( int n = 0 ; n < gtable6.size() ; n++ ) {
					if ( gtable6[n].result1_1 == 6 && gtable6[n].result1_2 == 0 ) gtable6[n].result1_2 == gLoc ;
				} // for
				
				if ( main[m].middle[6] == 'S' ) {
					strcpy( statement, "ENS" ) ;
					WriteToTable6( statement, 0, 10 ) ;
				} // if
				else if ( main[m].middle[6] == 'P' ) {
					strcpy( statement, "ENP" ) ;
					WriteToTable6( statement, 0, 11 ) ;
				} // else if
				else ;
			} // if
			else if ( strcmp( statement, "ENP" ) == 0 ) WriteToTable6( statement, 0, 10 ) ;
			else if ( strcmp( statement, "ENS" ) == 0 ) WriteToTable6( statement, 0, 11 ) ;
			else IsAssignment( main, m, i=0, 1, haveError ) ;
		} // else
		
		StringClear( statement ) ;	
		haveError = 0 ;
	} // for
	
} // Classify()


void IsProgram( vector<TMData> main, int m, int i ) {
	int num = 0, haveError = 0 ;
	Str20 id ;
	StringClear( id ) ;
	
	//---------------切token，此token為identifier---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		id[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	//---------------檢查此id是否符合規範---------------
	CheckIdentifier( id, haveError ) ; 
	
	//---------------判斷該行結尾是否有<;>---------------
	if ( main[m].middle[i+1] != ';' && haveError == 0 ) haveError = 2 ;
	
	//---------------寫入判斷後的資料---------------
	if ( haveError == 0 ) {
		int fish = GetASCII( id ) % hash ;
		HashTable5( id, 0, fish, 0, -1 ) ;
	} // if
	else {
		WriteErrorMessage( m, haveError ) ;
	} // else
} // IsProgram()


void IsVariable( vector<TMData> main, int m, int i, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round == 1 ) {  // 第一輪，檢查 <data type> 
		if ( CheckDataType( temp ) == 0 ) haveError = 3 ;
	} // if
	else if ( round == 2 ) {  // 第二輪，檢查 <:> 
		if ( temp[0] != ':' && haveError == 0 ) haveError = 4 ;
	} // else if
	else {  // 二輪以上，檢查為 <id> 或是 <,> 或是 <;> 
		if ( round % 2 == 1 ) {  // 基數輪，檢查 <id>
			CheckIdentifier( temp, haveError ) ;
		} // if
		else {  // 偶數輪，檢查 <,> 或是 <;>
			if ( ( temp[0] != ',' && temp[0] != ';' ) || temp[0] == '|' ) {
				haveError = 5 ;
				return ;
			} // if
			else if ( temp[0] == ';' ) return ;
		} // else
		
	} // else
	
	if ( haveError == 0 ) IsVariable( main, m, i+1, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion 
	if ( haveError == 0 && round == 1 )  ChangeToIMCode( main, m, i+1, CheckDataType( temp ), 2, 0 ) ; 
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsVariable()


void IsDimension( vector<TMData> main, int m, int i, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round == 1 ) {  // 第一輪，檢查 <data type> 
		if ( CheckDataType( temp ) == 0 ) haveError = 3 ;
	} // if
	else if ( round == 2 ) {  // 第二輪，檢查 <:> 
		if ( temp[0] != ':' && haveError == 0 ) haveError = 4 ;
	} // else if
	else {  // 二輪以上，檢查為 <array> 或是 <,> 或是 <;>
		if ( round % 2 == 1 ) {  // 基數輪，檢查 <array>
			i = i + 1 ;
			CheckArray( main, m, i, temp, haveError, 1 ) ;
			i = i - 1 ;
		} // if
		else {  // 偶數輪，檢查 <,> 或是 <;>
			if ( ( temp[0] != ',' && temp[0] != ';' ) || temp[0] == '|' ) {
				haveError = 5 ;
				return ;
			} // if
			else if ( temp[0] == ';' ) return ;
		} // else
	} // else
	
	if ( haveError == 0 ) IsDimension( main, m, i+1, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion 
	if ( haveError == 0 && round == 1 ) ChangeToIMCode( main, m, i+1, 1, 4, WriteToTable7( main, m, i+1, CheckDataType( temp ) ) ) ;
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsDimension()


void IsLabel( vector<TMData> main, int m, int i, vector<TMData> &forLabel, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	
	if ( round % 2 == 1 ) {  // 偶數輪，檢查 <id>
		CheckIdentifier( temp, haveError ) ;
	} // if
	else {  // 基數輪，檢查 <,> 或是 <;>
		if ( ( temp[0] != ',' && temp[0] != ';' ) || temp[0] == '|' ) {
			haveError = 5 ;
			return ;
		} // if
		else if ( temp[0] == ';' ) return ;
			
	} // else
	
	if ( haveError == 0 ) IsLabel( main, m, i+1, forLabel, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion 
	if ( haveError == 0 && round == 1 ) {
		ChangeToIMCode( main, m, i=6, CheckDataType( temp ), 3, 0 ) ;
		InsertToLabel( main, m, i=6, forLabel ) ;
	}  // if
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsLabel()


void IsIF( vector<TMData> main, int m, int i, int round, int &haveError, int type ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( type == 0 ) {
		if ( round % 2 == 1 ) {  // 基數輪，檢查 <id>
			CheckIdentifier( temp, haveError ) ;
		} // if
		else {  // 偶數輪，檢查 <GT> <GE> <LT> <LE> <EQ> <THEN> 
			if ( strcmp( temp, "GT" ) == 0 || strcmp( temp, "GE" ) == 0 || strcmp( temp, "LT" ) == 0 || strcmp( temp, "LE" ) == 0 || strcmp( temp, "EQ" ) == 0 ) ;
			else if ( strcmp( temp, "THEN" ) == 0 ) {
				type = 1 ;
			} // else
			else {
				haveError = 5 ;
			} // else
		
		} // else
		
	} // if
	else if ( type == 1 ) {
		if ( round % 2 == 1 ) {  // 基數輪，為 <id> 或 <array> 或 <number> 
			if ( main[m].middle[i+1] == '(' ) {
				i = i + 1 ;
				CheckArray( main, m, i, temp, haveError, 1 ) ;
				i = i - 1 ;
			} // if
			else {
				CheckIdentifier( temp, haveError ) ;
				if ( haveError != 0 ) CheckNumber( temp, haveError ) ;
			} // else
		
		} // if
		else {  // 偶數輪，檢查 <^> <*> </> <+> <-> <=> <;> <(> <)> <else> 
			if ( temp[0] == '^' || temp[0] == '*' || temp[0] == '/' || temp[0] == '+' || temp[0] == '-' || temp[0] == '=' || temp[0] == '(' || temp[0] == ')' ) ;
			else if ( strcmp( temp, "ELSE" ) == 0 ) ;
			else if ( temp[0] == ';' ) return ;
			else {
				haveError = 5 ;
				return ;
			} // else
		
		} // else
		
	} // else if
	
	if ( haveError == 0 ) IsIF( main, m, i+1, round+1, haveError, type ) ;  // 該輪有錯誤就不繼續recursion
	if ( haveError == 0 && round == 1 ) ;
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsIF()


void IsGTO( vector<TMData> main, int m, int i ) {
	int num = 0, haveError = 0 ;
	Str20 id ;
	StringClear( id ) ;
	
	//---------------切token，此token為identifier---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		id[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	//---------------檢查此id是否符合規範---------------
	CheckIdentifier( id, haveError ) ; 
	
	//---------------判斷該行結尾是否有---------------
	if ( main[m].middle[i+1] != ';' && haveError == 0 ) haveError = 2 ;
	
	//---------------寫入判斷後的資料---------------
	if ( haveError == 0 ) {
		gLoc = gLoc + 1 ;
		gtable6[gtable6.size() - 1].loc = gLoc ;
		gtable6[gtable6.size() - 1].operator1_1 = 2 ;
		gtable6[gtable6.size() - 1].operator1_2 = 11 ;
		gtable6[gtable6.size() - 1].result1_1 = 6 ;
		strcpy( gtable6[gtable6.size() - 1].code, "GTO" ) ;
		strcat( gtable6[gtable6.size() - 1].code, id ) ;
	} // if
	else {
		//WriteErrorMessage( m, haveError ) ;
	} // else
} // IsGTO()


void IsSubroutine( vector<TMData> main, int m, int i, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token，此token為identifier---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round == 1 ) {  // 第一輪，檢查 <id> 
		CheckIdentifier( temp, haveError ) ; 
	} // if
	else if ( round == 2 ) {  // 第二輪，檢查 <(> 
		if ( temp[0] != '(' ) haveError = 7 ;
	} // else if
	else if ( round == 3 ) {  // 第三輪，檢查 <data type> 
		if ( CheckDataType( temp ) == 0 ) haveError = 3 ;
	} // else if
	else if ( round == 4 ) {  // 第四輪，檢查 <:> 
		if ( temp[0] != ':' ) haveError = 4 ;
	} // else if
	else {  // 四輪以上， ，檢查為 <id> 或是 <,> 或是 <)> 或是 <;> 
		if ( round % 2 == 1 ) {  // 基數輪，檢查 <id> 或是 ; 
			if ( temp[0] == ';' ) return ;
			CheckIdentifier( temp, haveError ) ;
			if ( haveError != 0 && temp[0] != ';' ) haveError = 1 ;
			else haveError = 0 ;
		} // if
		else {  // 偶數輪，檢查 <,> 或是 <)>
			if ( ( temp[0] != ',' && temp[0] != ')' ) || temp[0] == '|' ) {
				haveError = 5 ;
				return ;
			} // if
			
		} // else
		
	} // else
	
	if ( haveError == 0 ) IsSubroutine( main, m, i+1, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion
	if ( haveError == 0 && round == 1 ) ChangeToIMCode( main, m, i=11, 0, 1, -1 ) ;
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsSubroutine()


void SubOfSubroutine( vector<TMData> main, int m, int i, int round, int type ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token，此token為identifier---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round == 1 ) {
		type = CheckDataType( temp ) ;
	} // if
	else if ( round == 2 ) ;  // 為<:>不做事 
	else {
		if ( round % 2 == 1 ) {  // 基數輪，為 <id> 寫入table5 
			int fish = GetASCII( temp ) % hash ;
			HashTable5( temp, type, fish, 1, 0 ) ;
			WriteToTable6( temp, fish, 1 ) ;
		} // if
		else {  // 偶數輪，檢查 <,> 或是 <)>
			if ( temp[0] == ',' ) ;
			else if ( temp[0] == ')' ) return ;
		} // else
		
	} // else
	
	SubOfSubroutine( main, m, i+1, round+1, type ) ;
} // SubOfSubroutine


void IsCall( vector<TMData> main, int m, int i, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round == 1 ) {  // 第一輪，檢查 <id> 
		CheckIdentifier( temp, haveError ) ; 
	} // if
	else if ( round == 2 ) {  // 第二輪，檢查 <(> 
		if ( temp[0] != '(' ) haveError = 7 ;
	} // else if
	else {  // 二輪以上， ，檢查為 <id> 或是 <,> 或是 <)> 或是 <;> 
		if ( round % 2 == 1 ) {  // 基數輪，檢查 <id> 
			if ( temp[0] == ';' ) return ;
			CheckIdentifier( temp, haveError ) ;
			if ( haveError != 0 ) CheckNumber( temp, haveError ) ;
		} // if
		else {  // 偶數輪，檢查 <,> 或是 <)>
			if ( ( temp[0] != ',' && temp[0] != ')' ) || temp[0] == '|' ) {
				haveError = 5 ;
				return ;
			} // if
			
		} // else
		
	} // else
	
	if ( haveError == 0 ) IsCall( main, m, i+1, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion
	if ( haveError == 0 && round == 1 ) ChangeToIMCode( main, m, i=5, 0, 7, 0 ) ;
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsCall()


int SubOfCall( vector<TMData> main, int m, int i, int round ) {
	int start = gtable7.size() - 1, num = 0 ;  // 把一開始table7裡面有多少先記錄
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token，此token為identifier---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round % 2 == 1 ) {  // 基數輪，檢查為 <integer> <real number> <id> 哪一個 
		int theLoc = 0, type = 0 ;
		type = WhichTypeOfID( temp ) ;
		
		if ( type == 3 ) {  // 為 <integer>
			theLoc = gtable3.size() - 1 ;
			WriteToTable3( temp ) ;
		} // if
		else if ( type == 4 ) {  // 為 <real number>
			theLoc = gtable4.size() - 1 ;
			WriteToTable4( temp ) ;
		} // else if
		else if ( type == 5 ) {  // 為 <id>
			int fish = GetASCII( temp ) % hash ;
			HashTable5( temp, 0, fish, 1, 0 ) ;
			theLoc = fish ;
		} // else if
	
		if ( round == 1 ) gtable7.resize( gtable7.size() + 1 ) ;  // 預留空位 
		WriteToTable7_2( type, theLoc ) ;
	} // if
	else {  // 偶數輪，檢查 <,> 或是 <)>
		if ( temp[0] == ',' ) ;
		else if ( temp[0] == ')' ) return 1 ;
	} // else
	
	SubOfCall( main, m, i+1, round+1 ) ;
	if ( round == 1 ) {  // 將參數全部寫入各table後，將有多少參數寫入table7一開始 
		int end = gtable7.size() - 1 ;
		int sum = ( end - start ) / 2 ;
		gtable7[start].infor = sum ;
		gtable6[gtable6.size() - 2].result1_2 = start ;
		for ( int n = 0 ; main[m].middle[n] != '|' ; n++ ) {
			gtable6[gtable6.size() - 2].code[n] = main[m].middle[n] ;
		} // for
		
	} // if
	
	return start ;
} // SubOfCall()


void IsInput( vector<TMData> main, int m, int i, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round % 2 == 1 ) {  // 基數輪，檢查 <id>
		CheckIdentifier( temp, haveError ) ;
	} // if
	else {  // 偶數輪，檢查 <,> 或是 <;>
		if ( ( temp[0] != ',' && temp[0] != ';' ) || temp[0] == '|' ) {
			haveError = 5 ;
			return ;
		} // if
		else if ( temp[0] == ';' ) return ;
	} // else
	
	if ( haveError == 0 ) IsInput( main, m, i+1, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion 
	if ( haveError == 0 && round == 1 )  ChangeToIMCode( main, m, i+1, 0, 8, 0 ) ; 
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsInput()


void IsOutput( vector<TMData> main, int m, int i, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round % 2 == 1 ) {  // 基數輪，檢查 <id>
		CheckIdentifier( temp, haveError ) ;
	} // if
	else {  // 偶數輪，檢查 <,> 或是 <;>
		if ( ( temp[0] != ',' && temp[0] != ';' ) || temp[0] == '|' ) {
			haveError = 5 ;
			return ;
		} // if
		else if ( temp[0] == ';' ) return ;
	} // else
	
	if ( haveError == 0 ) IsOutput( main, m, i+1, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion 
	if ( haveError == 0 && round == 1 )  ChangeToIMCode( main, m, i+1, 0, 9, 0 ) ; 
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsOutput()


void IsAssignment( vector<TMData> main, int m, int i, int round, int &haveError ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round % 2 == 1 ) {  // 基數輪，為 <id> 或 <array> 或 <number> 
		if ( temp[0] == '(' || temp[0] == ')' ) round-- ;
		else if ( main[m].middle[i+1] == '(' ) {
			i = i + 1 ;
			CheckArray( main, m, i, temp, haveError, 1 ) ;
			i = i - 1 ;
		} // if
		else {
			CheckIdentifier( temp, haveError ) ;
			if ( haveError != 0 ) CheckNumber( temp, haveError ) ;
		} // else
		
	} // if
	else {  // 偶數輪，檢查 <^> <*> </> <+> <-> <=> <;> <(> <)> 
		if ( temp[0] == '^' || temp[0] == '*' || temp[0] == '/' || temp[0] == '+' || temp[0] == '-' || temp[0] == '=' || temp[0] == '(' || temp[0] == ')' ) {
			if ( temp[0] == ')' ) round-- ;
		} // if
		else if ( temp[0] == ';' ) return ;
		else {
			haveError = 9 ;
			return ;
		} // else
		
	} // else
	
	if ( haveError == 0 ) IsAssignment( main, m, i+1, round+1, haveError ) ;  // 該輪有錯誤就不繼續recursion
	if ( haveError == 0 && round == 1 ) ; // ChangeToPostFix( main, m, i=0, 1 ) ;
	else if ( haveError != 0 && round == 1 ) WriteErrorMessage( m, haveError ) ;
} // IsAssignment()


void ChangeToPostFix( vector<TMData> main, int m, int i, int round ) {
	int num = 0 ;
	Str20 temp ;
	StringClear( temp ) ;
	
	//---------------切token---------------
	for ( ; main[m].middle[i] != ' ' ; i++ ) {
		temp[num] = main[m].middle[i] ;
		num++ ;
	} // for
	
	if ( round%2 == 1 ) {  // round為基數，為ID或Array 
		if ( main[m].middle[i+1] != '('  ) {  // 為ID 
			strcpy( stack1[stack1.size() - 1].op, temp ) ;
		} // if
		else {  // 為Array 
			
		} // else
		
		stack1.resize( stack1.size() + 1 ) ;
	} // if
	else {  // round為偶數，為符號 
		
		
		
		stack2.resize( stack2.size() + 1 ) ;
	} // else
	
	
	
} // ChangeToPostFix()


void CheckIdentifier( Str20 string, int &haveError ) {
	bool correct = true ;
	//---------------與table2相比判斷是否為reserved word---------------
	if ( CmpWithResWord( string ) ) correct = false ;
	
	//---------------判斷identifier是否含有特殊字元---------------
	for ( int m = 0 ; string[m] != '\0' ; m++ ) {
		if ( m == 0 ) {
			if ( ( string[m] >= 'a' && string[m] <= 'z' ) || ( string[m] >= 'A' && string[m] <= 'Z' ) ) ;
			else if ( string[m] == '_' ) ;
			else correct = false ;
		} // if
		else {
			if ( ( string[m] >= 'a' && string[m] <= 'z' ) || ( string[m] >= 'A' && string[m] <= 'Z' ) ) ;
			else if ( string[m] >= '0' && string[m] <= '9' ) ;
			else if ( string[m] == '_' ) ;
			else correct = false ;
		} // else
			
	} // for
	
	if ( correct ) haveError = 0 ;
	else haveError = 1 ;
} // CheckIdentifier()


void CheckArray( vector<TMData> main, int m, int &i, Str20 string, int &haveError, int round ) {
	if ( round == 1 ) CheckIdentifier( string, haveError ) ;  // 第一輪，檢查 <id> 
	else if ( round == 2 ) {  // 第二輪，檢查 <(> 
		if ( string[0] != '(' ) haveError = 7 ;
	} // else if
	else {  // 二輪以上，檢查為 <number> 或是 <,> 或是 <)> 
		if ( round % 2 == 1 ) {  // 基數輪，檢查 <number>
			CheckNumber( string, haveError ) ;
			if ( haveError != 0 ) CheckIdentifier( string, haveError ) ;
		} // if
		else {  // 偶數輪，檢查 <,> 或是 <)>
			if ( ( string[0] != ',' && string[0] != ')' ) || string[0] == '|' ) {
				haveError = 5 ;
				return ;
			} // if
			else if ( string[0] == ')' ) return ;
		} // else
		
	} // else
	
	if ( haveError == 0 ) {
		StringClear( string ) ;
		//---------------切token---------------
		for ( int num = 0 ; main[m].middle[i] != ' ' ; i++ ) {
			string[num] = main[m].middle[i] ;
			num++ ;
		} // for
	
		i = i + 1 ;
		CheckArray( main, m, i, string, haveError, round+1 ) ;
	} // if
} // CheckArray()


void CheckNumber( Str20 string, int &haveError ) {
	bool correct = true, haveDot = false ;
	for ( int m = 0 ; string[m] != '\0' ; m++ ) {
		if ( m == 0 ) {
			if ( string[m] >= '0' && string[m] <= '9' ) ;
			else correct = false ;
		} // if
		else {
			if ( string[m] >= '0' && string[m] <= '9' ) ;
			else if ( string[m] == '.' && !haveDot ) haveDot = true ;
			else correct = false ;
		} // else
		
	} // for
	
	if ( correct ) haveError = 0 ;
	else haveError = 1 ;
} // CheckInteger()


void InsertToLabel( vector<TMData> main, int m, int i, vector<TMData> &forLabel ) {
	int num = 0 ;
	Str20 id ; 
	StringClear( id ) ;
	
	for ( ; main[m].middle[i] != ';' ; i++ ) {
		if ( main[m].middle[i] == ' ' ) ;
		else if ( main[m].middle[i] == ',' ) {
			strcpy( forLabel[forLabel.size() - 1].middle, id ) ;
			forLabel.resize( forLabel.size() + 1 ) ;
			StringClear( id ) ;
		} // else if
		else {
			id[num] = main[m].middle[i] ;
			num++ ;
		} // else
		
	} // for
} // InsertToLabel()


void DFLocForLabel( Str20 string ) {
	for ( int m = 0 ; m < gtable5.size() ; m++ ) {
		if ( strcmp( gtable5[m].id, string ) == 0 ) {
			gtable5[m].poTable = 6 ;
			gtable5[m].poLoc = gLoc ;
		} // if
		
	} // for
} // DFLocForLabel()


bool CmpWithLabel( vector<TMData> forLabel, Str20 string ) {
	bool haveFind = false ;
	for ( int m = 0 ; m < forLabel.size() ; m++ ) {
		if ( strcmp( forLabel[m].middle, string ) == 0 ) haveFind = true ; 
	} // for
	
	return haveFind ;
} // CmpWithLabel()


int WhichTypeOfID( Str20 string ) {
	if ( ( string[0] >= 'a' && string[0] <= 'z' ) || ( string[0] >= 'A' && string[0] <= 'Z' ) ) return 5 ;
	else if ( string[0] >= '0' && string[0] <= '9' ) {
		for ( int m = 1 ; string[m] != '\0' ; m++ ) {
			if ( string[m] == '.' ) return 4 ;
		} // for
		
	} // else if
	
	return 3 ;
} // WhichTypeOfID()


bool CmpWithResWord( Str20 string ) {
	bool haveIt = false ;
	for ( int m = 0 ; m < gtable2.size() ; m++ ) {
		if ( strcmp( string, gtable2[m].resWord ) == 0 ) {
			haveIt = true ;
		} // if
			
	} // for
	
	if ( haveIt ) return true ;
	else return false ;
} // CmpWithResword()


int CheckDataType( Str20 string ) {
	if ( strcmp( string, "ARRAY" ) == 0 ) return 1 ;
	else if ( strcmp( string, "BOOLEAN" ) == 0 ) return 2 ;
	else if ( strcmp( string, "CHARACTER" ) == 0 ) return 3 ;
	else if ( strcmp( string, "INTEGER" ) == 0 ) return 4 ;
	else if ( strcmp( string, "LABEL" ) == 0 ) return 5 ;
	else if ( strcmp( string, "REAL" ) == 0 ) return 6 ;
	else return 0 ;
} // WhichDataType()


void WriteErrorMessage( int m, int ErrorType ) {
	Str20 num ;
	StringClear( num ) ;
	itoa( m+1, num, 10 ) ;  // 將int型別轉換為string型別 
	
	strcat( gtable6[gtable6.size() - 1].code, "Line " ) ;
	strcat( gtable6[gtable6.size() - 1].code, num ) ;
	if ( ErrorType == 1 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此Identifier不符合規範" ) ;
	else if ( ErrorType == 2 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 最後字元非結束指令 ';'" ) ;
	else if ( ErrorType == 3 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此Data Type不符合規範" ) ; 
	else if ( ErrorType == 4 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此處應為<:>" ) ;
	else if ( ErrorType == 5 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此處應為<,>或<;>" ) ;
	else if ( ErrorType == 6 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此number不符合規範" ) ;
	else if ( ErrorType == 7 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此處應為<(>" ) ;
	else if ( ErrorType == 8 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此處應為<GT>之類或<THEN>" ) ;
	else if ( ErrorType == 9 ) strcat( gtable6[gtable6.size() - 1].code, " : Syntax Error! 此處應為operator" ) ;
	
	gtable6.resize( gtable6.size() + 1 ) ;
} // WriteErrorMessage()


void ChangeToIMCode( vector<TMData> main, int m, int i, int type, int classify, int poLoc ) {
	int num = 0 ;
	Str20 id ; 
	StringClear( id ) ;
	
	for ( ; main[m].middle[i] != ';' ; i++ ) {
		if ( main[m].middle[i] == ' ' || main[m].middle[i] == ':' ) ;
		else if ( main[m].middle[i] == '(' ) {
			if ( classify == 1 || classify == 7 ) {  // 為subroutine或是call 
				int fish = GetASCII( id ) % hash ;
				if ( !gtable5[fish].haveHash ) HashTable5( id, type, fish, classify, poLoc ) ;
				
				if ( classify == 1 ) SubOfSubroutine( main, m , i+2, 1, 1 ) ;
				else if ( classify == 7 ) {
					WriteToTable6( id, fish, classify ) ;
					SubOfCall( main, m, i+2, 1 ) ;
				} // if 
				
				return ;
			} // if
			else for ( ; main[m].middle[i] != ')' ; i++ ) ;
		} // else if
		else if ( main[m].middle[i] == ',' ) {
			int fish = GetASCII( id ) % hash ;
			HashTable5( id, type, fish, classify, poLoc ) ;
			WriteToTable6( id, fish, classify ) ;
			StringClear( id ) ;
			num = 0 ;
		} // if
		else {
			id[num] = main[m].middle[i] ;
			num++ ;
		} // else
		
	} // for
	
	int fish = GetASCII( id ) % hash ;
	HashTable5( id, type, fish, classify, poLoc ) ;
	WriteToTable6( id, fish, classify ) ;
} // ChangeToIMCode()


void WriteToTable3( Str20 id ) {
	gtable3[gtable3.size() - 1].integer = atoi( id ) ;
	gtable3.resize( gtable3.size() + 1 ) ;
} // WriteToTable3()


void WriteToTable4( Str20 id ) {
	gtable4[gtable4.size() - 1].realNum = atoi( id ) ;
	gtable4.resize( gtable4.size() + 1 ) ;
} // WriteToTable4()


void WriteToTable6( Str20 id, int fish, int classify ) {
	gLoc = gLoc + 1 ;
	
	gtable6[gtable6.size() - 1].loc = gLoc ;
	if ( classify == 1 || classify == 2 || classify == 3 || classify == 4 || classify == 8 || classify == 9 ) {
		gtable6[gtable6.size() - 1].operator1_1 = 5 ;
		gtable6[gtable6.size() - 1].operator1_2 = fish ;
		strcat( gtable6[gtable6.size() - 1].code, id ) ;
	} // if
	else if ( classify == 7 ) {
		gtable6[gtable6.size() - 1].operator1_1 = 2 ;
		gtable6[gtable6.size() - 1].operator1_2 = 3 ;
		gtable6[gtable6.size() - 1].operand1_1 = 5 ;
		gtable6[gtable6.size() - 1].operand1_2 = fish ;
		gtable6[gtable6.size() - 1].result1_1 = 7 ;
	} // else if
	else if ( classify == 10 || classify == 11 ) {
		gtable6[gtable6.size() - 1].operator1_1 = 2 ;
		if ( classify == 10 ) gtable6[gtable6.size() - 1].operator1_2 = 6 ;
		if ( classify == 11 ) gtable6[gtable6.size() - 1].operator1_2 = 7 ;
		strcat( gtable6[gtable6.size() - 1].code, id ) ;
	} // else if
	
	gtable6.resize( gtable6.size() + 1 ) ;
} // WriteToTable6()


int WriteToTable7( vector<TMData> main, int m, int i, int dataType ) { 
	int start = gtable7.size() - 1 ;
	int num = 0, dimension = 1, startLoc = 0 ;
	Str20 id ; 
	bool in = false ;
	StringClear( id ) ;
	
	gtable7[gtable7.size() - 1].infor = dataType ;
		
		for ( ; main[m].middle[i] != ';' ; i++ ) {
			if ( main[m].middle[i] == ' ' ) ;
			else if ( main[m].middle[i] == '(' ) {
				startLoc = i ;
				for ( ; main[m].middle[startLoc] != ')' ; startLoc++ ) {
					if ( main[m].middle[startLoc] == ',' ) dimension++ ;
				} // for
				
				gtable7.resize( gtable7.size() + 1 ) ;
				gtable7[gtable7.size() - 1].infor = dimension ;
				dimension = 1 ;
				in = true ;
			} // else if 
			else if ( main[m].middle[i] == ')' || ( main[m].middle[i] == ',' && in ) ) {
				gtable7.resize( gtable7.size() + 1 ) ;
				gtable7[gtable7.size() - 1].infor = atoi( id ) ;
				StringClear( id ) ;
				num = 0 ;
				if ( main[m].middle[i] == ')' ) in = false ;
			} // else if
			else {
				if ( in ) {
					id[num] = main[m].middle[i] ;
					num++ ;
				} // if
			
			} // else
		
		} // for
		
		if ( in ) {
			gtable7.resize( gtable7.size() + 1 ) ;
			gtable7[gtable7.size() - 1].infor = atoi( id ) ;
		} // if
		
	gtable7.resize( gtable7.size() + 1 ) ;
	return start ;
} // WriteToTable7()


void WriteToTable7_2( int type, int theLoc ) {
	gtable7[gtable7.size() - 1].infor = type ;
	gtable7.resize( gtable7.size() + 1 ) ;
	gtable7[gtable7.size() - 1].infor = theLoc ;
	gtable7.resize( gtable7.size() + 1 ) ;
} // WriteToTable7_2()


int GetASCII( Str20 string ) {
	int ans = 0 ;
	for ( int m = 0 ; string[m] != '\0' ; m++ ) {
		ans = ans + string[m] ;
	} // for
	
	return ans ;
} // GetASCII()


void HashTable5( Str20 id, int type, int &fish, int classify, int poLoc ) {
	while ( gtable5[fish].haveHash ) {
		fish++ ;
		if ( fish > hash-1 ) fish = 0 ;
	} // while
			
	if ( ( classify == 0 || classify == 1 ) && poLoc == -1 ) gScope = fish ; // 為program或是subroutine才需要改變勢力範圍 
	strcpy( gtable5[fish].id, id ) ;
	gtable5[fish].scope = gScope ;
	gtable5[fish].type = type ;
	gtable5[fish].haveHash = true ;
	if ( classify == 4 ) {  // 為demension需要接入table7之pointer 
		gtable5[fish].poTable = 7 ;
		gtable5[fish].poLoc = poLoc ;
	} // if
} // HashTable5()


void StringClear( Str20 string ) {
	for ( int n = 0 ; n < 20 ; n++ ) {
		string[n] = '\0' ;
	} // for
} // StringClear()


void FoutTXT() {
	FILE *fout ;
	fout = fopen( "Output.txt", "w+" ) ;
	
	for ( int m = 0 ; m < gtable6.size() - 1 ; m++ ) {
		if ( gtable6[m].loc != 0 ) {
			fprintf( fout, "%d\t", gtable6[m].loc ) ;
			fprintf( fout, "((%d,%d)\t,", gtable6[m].operator1_1, gtable6[m].operator1_2 ) ;
			if ( gtable6[m].operand1_1 != 0 && gtable6[m].operand1_2 != 0 ) fprintf( fout, "(%d,%d)\t,", gtable6[m].operand1_1, gtable6[m].operand1_2 ) ;
			else fprintf( fout, "\t," ) ;
			if ( gtable6[m].operand2_1 != 0 && gtable6[m].operand2_2 != 0 ) fprintf( fout, "(%d,%d)\t,", gtable6[m].operand2_1, gtable6[m].operand2_2 ) ;
			else fprintf( fout, "\t," ) ;
			if ( gtable6[m].result1_1 != 0 && gtable6[m].result1_2 != 0 ) fprintf( fout, "(%d,%d)\t)", gtable6[m].result1_1, gtable6[m].result1_2 ) ;
			else fprintf( fout, "\t)" ) ;
			fprintf( fout, "\t%s\n", gtable6[m].code ) ;
		} // if
		else fprintf( fout, "%s\n", gtable6[m].code ) ;
	} // for
	
	fclose( fout ) ;
} // FoutTXT()