// 10927108 劉珈彤

# include <stdio.h>
# include <iostream>
# include <fstream>
# include <string>
# include <string.h>
# include <vector>
# include <sstream> 

using namespace std ;
typedef char Str20[20] ;
typedef char Str200[200] ;

//---------------struct---------------
struct TData {  // Table Data
	Str20 mnemonic ;
	int format1 ;
	int format2 ;
	Str20 opcode ;
	int regNum ;
};

struct FData {  // File Data
	Str200 token ;
	Str200 comment ;
};

struct PData {  // Package Token Data
	Str20 loc ;
	Str20 obCode ;
	Str20 obCode2 ;
	Str20 address ;
	Str20 pseudoLoc ;

	Str20 label ;
	char ins_prefix ;
	Str20 ins ;
	
	int amount ;
	char tg_prefix ;
	bool haveEqual ;
	bool isComplex ;
	Str20 token_groups[5] ;
	
	int error ;  // -1表literal 1表syntax error 2表重複定義error 3表未定義error 
};

struct CTData {  // Create Table Data
	Str20 symbol ;
	Str20 num ;
	Str20 string ;
	
	bool haveHash ;
};

struct LTORGData {
	Str20 loc ;	
	Str20 ins ;
	Str20 operand ;
	Str20 obCode ;
};

//---------------Global Variable---------------
vector<TData> gtable1, gtable2, gtable3, gtable4 ;
vector<CTData> gtable5, gtable6, gtable7 ;
vector<LTORGData> gLTORGData ;

//---------------function---------------
void ForTable1( Str20 tempfile ) ;  // 把Table0 & Table1放入vector 
void ReadAndStore( Str20 tempfile, int which ) ;  // 把Table2、3、3.5、4放入vector 
void ChangeToToken( Str20 tempfile, vector<FData> &main ) ;  // 把輸入檔一行一行讀入
void DistinguishTC( vector<FData> &main, vector<FData> &tc_main ) ;  // 把指令跟註解區分開，並在指令後方加入柵欄
void TokenUpper( vector<FData> &tc_main ) ;  // 將除了string外的英文字母全部轉大寫
void ChangeToPackageToken( vector<FData> &tc_main, vector<PData> &pt_Data ) ;  // 把一行指令拆分成label、ins、operand 
void StringClear( Str20 string ) ;
bool CmpWithTable1( Str20 string ) ;
bool CmpWithTable2( Str20 string ) ;
bool CmpWithTable3( Str20 string ) ;
bool CmpWithTable4( Str20 string ) ;
void DistinguishOperand( vector<PData> &pt_Data ) ;  // 細分operand，並把前綴符號放入該放的地方
void String2DClear( Str20 string[] ) ;
void CreateTable( vector<PData> pt_Data ) ;  // 建立Table5、6、7
unsigned long long int GetASCII( vector<PData> pt_Data, int n, int type ) ;  // ASCII編碼相加
bool PutIntoTable( int which, vector<PData> pt_Data, int n, int fish ) ;  // 將資訊放入，並檢查重複定義error、未定義error
//---------------創建address---------------
void CreateLoc( vector<PData> &pt_Data ) ;
void PlusLoc( Str20 &loc, int which, int num ) ; 
void ChangeToHex( Str20 &decToHex, int num ) ;  // 10轉16進位
int GetFormat( vector<PData> pt_Data, int n ) ;
//---------------創建Object Code---------------
void CreateSIC( vector<PData> &pt_Data ) ;
void GetOpCode( vector<PData> pt_Data, int n, Str20 &ans ) ;
void GetLoc( vector<PData> pt_Data, int n, Str20 &ans ) ;

void CreateXE( vector<PData> &pt_Data ) ;
void GetReg( vector<PData> pt_Data, int n, int i, Str20 &ans ) ;
void PlusNI( vector<PData> pt_Data, int n, Str20 &ans ) ;
void CountDisp( vector<PData> pt_Data, int n, Str20 ta, Str20 &tempDisp, bool &usePC ) ;
void ChangeToDec( Str20 hexToDec, int &num ) ;  // 16轉10進位
void FindBASE( vector<PData> pt_Data, int &n ) ;
bool isLiteral( vector<PData> pt_Data, int n ) ; 
int FindLTORG( vector<PData> pt_Data ) ;
void Displacement( vector<PData> pt_Data, int n, int where, int ltNum ) ;

void ChangeObjectCode( vector<PData> &pt_Data ) ;  // 將Object code轉10進位

void FoutTXT( vector<PData> pt_Data, vector<FData> tc_main, int type ) ;
void Initialize( vector<FData> &main, vector<FData> &tc_main, vector<PData> &pt_Data ) ;

//---------------main---------------
int main() {
	int command = -1, which = 0 ;
	FILE *fop ;
	Str20 tempfile ;  // 打開資料夾用
	bool havefile = false ;  // 是否有打開資料夾
	vector<FData> main, tc_main ;  // main為一開始讀file的資料  tc_main為區分token跟comment後的
	vector<PData> pt_Data ;  // pt_Data為細化token的vector
	
	//---------------把table1~4放入vector---------------
	strcpy( tempfile, "Table1.table" ) ;
	ForTable1( tempfile ) ;
	strcpy( tempfile, "Table2.table" ) ;
	ReadAndStore( tempfile, which=2 ) ;
	strcpy( tempfile, "Table3.table" ) ;
	ReadAndStore( tempfile, which=3 ) ;
	strcpy( tempfile, "Table4.table" ) ;
	ReadAndStore( tempfile, which=4 ) ;
	
	
	do {
		printf( "\n****      Assembler      ****\n" ) ;
		printf( "* 0. QUIT                   *\n" ) ;
		printf( "* 1. SIC                    *\n" ) ;
		printf( "* 2. SIC/XE                 *\n" ) ;
		printf( "*****************************\n" ) ;
		printf( "Input a choice( 0, 1, 2 ): " ) ;
		if ( scanf( "%d", &command ) == 0 ) return 1 ;
		if ( command == 0 ) return 0 ;
		printf( "\nInput a file name: " ) ;
		scanf( "%s", tempfile ) ;
		strcat( tempfile, ".txt" ) ;
		
		fop = fopen( tempfile, "r+" ) ;
		gtable5.resize( 100 ) ;
		gtable6.resize( 100 ) ;
		gtable7.resize( 100 ) ;
		//---------------1. SIC---------------
		if ( command == 1 ) {
			ChangeToToken( tempfile, main ) ;  // 將file切成token 
			DistinguishTC( main, tc_main ) ;  // 把token跟comment區分開 
			TokenUpper( tc_main ) ;  // 將除了String外的英文全部轉成大寫 
			ChangeToPackageToken( tc_main, pt_Data ) ;  // 將token細化，並存放在pt_Data 
			DistinguishOperand( pt_Data ) ;  // 把operand切開 
			CreateTable( pt_Data ) ;  // 創立Table5、6、7  Table5為symbol Table6為int Table7為string，此function可以找兩種error 
			CreateLoc( pt_Data ) ;  // 建立每行指令的loc 
			CreateSIC( pt_Data ) ;  // 建立每行指令的Object Code 
			FoutTXT( pt_Data, tc_main, 1 ) ;
		} // if
		//---------------2. SIC/XE---------------
		else if ( command == 2 ) {
			ChangeToToken( tempfile, main ) ;  // 將file切成token 
			DistinguishTC( main, tc_main ) ;  // 把token跟comment區分開 
			TokenUpper( tc_main ) ;  // 將除了String外的英文全部轉成大寫 
			ChangeToPackageToken( tc_main, pt_Data ) ;  // 將token細化，並存放在pt_Data 
			DistinguishOperand( pt_Data ) ;  // 把operand切開 
			CreateTable( pt_Data ) ;  // 創立Table5、6、7  Table5為symbol Table6為int Table7為string，此function可以找兩種error 
			CreateLoc( pt_Data ) ;  // 建立每行指令的loc 
			CreateXE( pt_Data ) ;  // 建立每行指令的Object Code 
			ChangeObjectCode( pt_Data ) ;  // 將Object code轉10進位 
			FoutTXT( pt_Data, tc_main, 2 ) ;
		} // else if
		
		
		printf( "Line\tLoc\tSource\tstatement\t\tObject code\n\n" ) ;
		for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
			printf( "%3d\t", m*5 ) ;
			if ( tc_main[m].comment[0] == '\0' ) {
				printf( "%s\t%s\t", pt_Data[m].loc, pt_Data[m].label ) ;
				if ( pt_Data[m].ins_prefix == '+' ) printf( "%c", pt_Data[m].ins_prefix ) ;
				printf( "%s\t", pt_Data[m].ins ) ;
				if ( pt_Data[m].haveEqual ) printf( "=" ) ;
				if ( pt_Data[m].tg_prefix != '\0') printf( "%c", pt_Data[m].tg_prefix ) ;
				if ( pt_Data[m].tg_prefix == 'C' || pt_Data[m].tg_prefix == 'X' ) printf( "\'" ) ;
				printf( "%s", pt_Data[m].token_groups[0] ) ;
				if ( pt_Data[m].token_groups[1][0] != '\0' ) printf( ",%s", pt_Data[m].token_groups[1] ) ;
				if ( pt_Data[m].tg_prefix == 'C' || pt_Data[m].tg_prefix == 'X' ) printf( "\'" ) ;
				if ( pt_Data[m].error == 0 ) printf( "\t\t%s\n", pt_Data[m].obCode ) ;
				else if ( pt_Data[m].error == 1 ) printf( "\t\tsyntax error\n" ) ;
				else if ( pt_Data[m].error == 2 ) printf( "\t\tduplicate definition\n" ) ;
				else if ( pt_Data[m].error == 3 ) printf( "\t\tundefined symbol\n" ) ;
			} // if
			else {
				printf( "\t%s\n", tc_main[m].comment ) ;
			} // else
			
		} // for 
		
		
		Initialize( main, tc_main, pt_Data ) ;
	} while ( command != 0 ) ;
	
	
} // main()


//---------------function---------------
void ForTable1( Str20 tempfile ) {
	FILE *fop1, *fop2 ;
	Str20 temp ;
	fop1 = fopen( tempfile, "r+" ) ;
	fop2 = fopen( "Table0.table", "r+" ) ;
	
	gtable1.resize(59) ;
	for( int n = 0 ; !feof( fop1 ) ; n++ ) {
		if ( fscanf( fop1, "%s", gtable1[n].mnemonic ) == 1 ) {
			for ( int m = 0 ; gtable1[n].mnemonic[m] >= 'a' && gtable1[n].mnemonic[m] <= 'z' ; m++ ) {
				gtable1[n].mnemonic[m] = gtable1[n].mnemonic[m] - 32 ;
				
			} // for
			
			fscanf( fop2, "%d", &gtable1[n].format1 ) ;
			fscanf( fop2, "%d", &gtable1[n].format2 ) ;
			fscanf( fop2, "%s", gtable1[n].opcode ) ;
			fscanf( fop2, "%d", &gtable1[n].regNum ) ;
		} // if
		
	} // for
	
	fclose( fop1 ) ;
	fclose( fop2 ) ;
} // ForTable1()


void ReadAndStore( Str20 tempfile, int which ) {
	FILE *fop, *fop2 ;
	Str20 temp ;
	fop = fopen( tempfile, "r+" ) ;
	fop2 = fopen( "Table3.5.table", "r+" ) ;
	
	gtable2.resize( 9 ) ;
	gtable3.resize( 9 ) ;
	gtable4.resize( 13 ) ;
	for ( int n = 0 ; !feof( fop ) ; n++ ) {
		if ( which == 2 ) fscanf( fop, "%s", gtable2[n].mnemonic ) ;
		if ( which == 3 ) {
			fscanf( fop, "%s", gtable3[n].mnemonic ) ;
			fscanf( fop2, "%d", &gtable3[n].regNum ) ;
		} // if
		if ( which == 4 ) fscanf( fop, "%s", gtable4[n].mnemonic ) ;
		
	} // while()
	
	fclose( fop ) ;
	fclose( fop2 ) ;
} // ReadAndStore()


void ChangeToToken( Str20 tempfile, vector<FData> &main ) {
	FILE *fop ;
	int putFence = 0 ;
	char temp ;
	fop = fopen( tempfile, "r+" ) ;
	
	for ( int i = 0 ; fscanf( fop, "%c", &temp ) == 1 && !feof( fop ) ; i++ ) {  //直到讀完整個file
		main.resize( i+1 ) ;  // 擴充vector大小
		for ( int j = 0 ; temp != '\n' ; j++ ) {  // 直到讀完整段資料
			if ( temp == ' ' || temp == '\t' ) {  // 是空白，以空白隔開token
				if ( j > 0 && main[i].token[j-1] != ' ' ) {  // 這個空白不位於0，且前面一位不為空白 
					main[i].token[j] = ' ' ;
					j++ ;
				} // if
				
				j-- ;	
			} // if
			else {
				main[i].token[j] = temp ;
			} // else
			
			putFence = j + 1 ;
			if ( fscanf( fop, "%c", &temp ) != 1 ) {
				break ;
			} // if
			
		} // for 
		
		main[i].token[putFence] = ' ' ;
		main[i].token[putFence+1] = '|' ;
		putFence = 0 ;
	} // for
} // ChangeToToken()
	

void DistinguishTC( vector<FData> &main, vector<FData> &tc_main ) {
	tc_main.resize( main.size() ) ;  // 將 tc_main 擴充到跟 main 一樣大小
	for ( int i = 0 ; i < main.size() ; i++ ) {
		if ( main[i].token[0] == '.' ) {
			strcpy( tc_main[i].comment, main[i].token ) ;
			strcpy( tc_main[i].token, " |" ) ;
			
			int count = 0 ;
			for ( int m = 0 ; tc_main[i].comment[m] != '|' ; m++ ) count++ ;
			tc_main[i].comment[count] = '\0' ;
		} // if ()
		else {
			strcpy( tc_main[i].token, main[i].token ) ;
		} // else
		
	} // for
} // DistinguishTC()


void TokenUpper( vector<FData> &tc_main ) {
	for ( int i = 0 ; i < tc_main.size() ; i++ ) {
		if ( strcmp( tc_main[i].token, " |" ) != 0 ) {
			for ( int j = 0 ; tc_main[i].token[j] != '|' ; j++ ) {
				if ( tc_main[i].token[j] != '\'' ) {
					if ( tc_main[i].token[j] >= 'a' && tc_main[i].token[j] <= 'z' ) {
						tc_main[i].token[j] = tc_main[i].token[j] - 32 ;
					} // if
					
				}  // if
				else break ;
				
			} // for
			
		} // if
		
	} // for
} // TokenUpper()

	
void ChangeToPackageToken( vector<FData> &tc_main, vector<PData> &pt_Data ) {
	int num = 0, whichToken = 0 ;  // whichToken: 0表目前還沒有 1表有label 2表有ins 
	char type = '\0' ;
	Str20 temp, temp2 ;
	StringClear( temp ) ;  // 先將temp初始化 
	StringClear( temp2 ) ;  // 先將temp2初始化 
	pt_Data.resize( tc_main.size() ) ;  // 將 pt_Data 擴充到跟 tc_main 一樣大小
	
	for ( int i = 0 ; i < tc_main.size() ; i++ ) {
		for ( int j = 0 ; tc_main[i].token[j] != '|' ; j++ ) {
			if ( tc_main[i].token[j] != ' ' ) {
				temp[num] = tc_main[i].token[j] ;
				num++ ;
			} // if 
			else {
				if ( temp[0] != '+' ) {
					if ( whichToken == 0 ) {  // 處理syntax error
						
						
					} // if
					else if ( whichToken == 1 ) {  // 處理syntax error
						
						
					} // else if
					
					if ( ( !CmpWithTable1( temp ) && !CmpWithTable2( temp ) && !CmpWithTable3( temp ) ) && whichToken == 0 ) {  // 為label
						strcpy( pt_Data[i].label, temp ) ;
						StringClear( temp ) ;
						whichToken = 1 ;
					} // else if
					else if ( CmpWithTable1( temp ) || CmpWithTable2( temp ) ) {  // 為instruction或者pesudo instruction 
						strcpy( pt_Data[i].ins, temp ) ;
						StringClear( temp ) ;
						whichToken = 2 ;
					} // else if
					else {  // 為operand 
						if ( CmpWithTable4( temp ) ) pt_Data[i].isComplex = true ;
						else pt_Data[i].isComplex = false ;
						pt_Data[i].amount = 1 ;
						strcpy( pt_Data[i].token_groups[0], temp ) ;
						StringClear( temp ) ;
					} // else
					
				} // if
				else if ( temp[0] == '+' ) {  // temp的第一個位置為+ 
					type = temp[0] ;
					for ( int n = 1 ; temp[n] != '\0' ; n++ ) {  // 把+號跟ins拆開 
						temp2[n-1] = temp[n] ;
					} // for
					
					if ( CmpWithTable1( temp2 ) || CmpWithTable2( temp2 ) ) {  // 為instruction或者pesudo instruction 
						strcpy( pt_Data[i].ins, temp2 ) ;
						pt_Data[i].ins_prefix = type ;
						StringClear( temp ) ;
						StringClear( temp2 ) ;
						whichToken = 2 ;
					} // if
					else {  // 處理syntax error
						
						
					} // else
					
				} // else
				
				num = 0 ;
			} // else
			
		} // for
		
		whichToken = 0 ;
	} // for
	
} // ChangeToPackageToken()


void StringClear( Str20 string ) {
	for ( int n = 0 ; n < 20 ; n++ ) {
		string[n] = '\0' ;
	} // for
} // StringClear()


bool CmpWithTable1( Str20 string ) {
	bool haveIt = false ;
	for ( int n = 0 ; n < gtable1.size() ; n++ ) {
		if ( strcmp( gtable1[n].mnemonic, string ) == 0 ) {
			haveIt = true ;
		} // if
			
	} // for
	
	if ( haveIt ) return true ;
	else return false ;
} // CmpWithTable1()


bool CmpWithTable2( Str20 string ) {
	bool haveIt = false ;
	for ( int n = 0 ; n < gtable2.size() ; n++ ) {
		if ( strcmp( gtable2[n].mnemonic, string ) == 0 ) {
			haveIt = true ;
		} // if
		
	} // for
	
	if ( haveIt ) return true ;
	else return false ;
} // CmpWithTable2()


bool CmpWithTable3( Str20 string ) {
	bool haveIt = false ;
	for ( int n = 0 ; n < gtable3.size() ; n++ ) {
		if ( strcmp( gtable3[n].mnemonic, string ) == 0 ) {
			haveIt = true ;
		} // if
		
	} // for
	
	if ( haveIt ) return true ;
	else return false ;
} // CmpWithTable3()


bool CmpWithTable4( Str20 string ) {
	bool haveIt = false ;
	for ( int i = 0 ; string[i] != '\0' ; i++ ) {
		for ( int j = 0 ; j < gtable4.size() ; j++ ) {
			if ( string[i] == gtable4[j].mnemonic[0] ) {
				haveIt = true ;
			} // if
			
		} // for
		
	} // for
	
	if ( haveIt ) return true ;
	else return false ;
} // CmpWithTable4()
	

void DistinguishOperand( vector<PData> &pt_Data ) {
	int num1 = 0, num2 = 0 ;  // num1用來記錄該temp的第幾個位置 num2用來記錄為第幾個temp 
	char type ;  // 用來記錄該token的前綴 
	Str20 temp[5] ;  // 用來記錄區分後的operand 
	
	for ( int n = 0 ; n < pt_Data.size() ; n++ ) {
		if ( pt_Data[n].isComplex ) {
			if ( pt_Data[n].token_groups[0][0] == '#' || pt_Data[n].token_groups[0][0] == '@' ) {  // 為 #40 或 @RETARD 的情況 
				type = pt_Data[n].token_groups[0][0] ;
				for ( int m = 1 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {
					temp[0][num1] = pt_Data[n].token_groups[0][m] ;
					num1++ ;
				} // for
				
				pt_Data[n].tg_prefix = type ;
				strcpy( pt_Data[n].token_groups[0], temp[0] ) ;
				String2DClear( temp ) ;
				num1 = 0 ;
			} // if
			else if ( pt_Data[n].token_groups[0][1] == '\'' ) {  // 為 C'   ' 或 X'   ' 的情況 
				type = pt_Data[n].token_groups[0][0] ;
				for ( int m = 2 ; pt_Data[n].token_groups[0][m] != '\'' ; m++ ) {
					temp[0][num1] = pt_Data[n].token_groups[0][m] ;
					num1++ ;
				} // for
				
				pt_Data[n].tg_prefix = type ;
				strcpy( pt_Data[n].token_groups[0], temp[0] ) ;
				
				String2DClear( temp ) ;
				num1 = 0 ;
			} // else if
			else if ( pt_Data[n].token_groups[0][0] == '=' ) {  // 為 =x'b1' 或 =c'EOF' 或 =3277 的情況 
				pt_Data[n].haveEqual = true ;
				type = pt_Data[n].token_groups[0][1] ;
				if ( pt_Data[n].token_groups[0][2] == '\'' ) {  //  =x'b1' 或 =c'EOF'
					for ( int m = 3 ; pt_Data[n].token_groups[0][m] != '\'' ; m++ ) {
						temp[0][num1] = pt_Data[n].token_groups[0][m] ;
						num1++ ;
					} // for
					
				} // if
				else {  // 為 =3277
					for ( int m = 2 ; pt_Data[n].token_groups[0][m] != '\'' ; m++ ) {
						temp[0][num1] = pt_Data[n].token_groups[0][m] ;
						num1++ ;
					} // for
					
				} // else
				
				pt_Data[n].tg_prefix = type ;
				strcpy( pt_Data[n].token_groups[0], temp[0] ) ;
				
				String2DClear( temp ) ;
				num1 = 0 ;
			} // else if
			else {  // 為 A,B 情況 
				for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {  // 把operand用,或+或-區分成兩個
					if ( pt_Data[n].token_groups[0][m] != ',' && pt_Data[n].token_groups[0][m] != '+' && pt_Data[n].token_groups[0][m] != '-' ) {
						temp[num2][num1] = pt_Data[n].token_groups[0][m] ;
						num1++ ;
					} // if()
					else {
						pt_Data[n].amount = pt_Data[n].amount + 1 ;
						num2++ ;
						num1 = 0 ;
					} // else
					
				} // for
				
				for ( int m = 0 ; m < pt_Data[n].amount ; m++ ) {
					strcpy( pt_Data[n].token_groups[m], temp[m] ) ;
				} // for
					
				String2DClear( temp ) ;
				num1 = 0 ;
				num2 = 0 ;
			} // else
			
		} // if
		
	} // for
} // DistinguishTC()


void String2DClear( Str20 string[] ) {
	for ( int n = 0 ; n < 5 ; n++ ) {
		for ( int m = 0 ; m < 20 ; m++ ) {
			string[n][m] = '\0' ;
		} // for
		
	} // for
} // String2DClear() ;


void CreateTable( vector<PData> pt_Data ) {
	for ( int n = 0 ; n < pt_Data.size() ; n++ ) {  // 跑label 
		if ( pt_Data[n].label[0] != '\0' ) {
			int fish = GetASCII( pt_Data, n, 1 ) % 100 ;
			if ( !PutIntoTable( 5, pt_Data, n, fish ) )  pt_Data[n].error = 2 ;
		} // if
		
	} // for
	
	for ( int n = 0 ; n < pt_Data.size() ; n++ ) { // 跑operand 
		if ( pt_Data[n].token_groups[0][0] >= '0' && pt_Data[n].token_groups[0][0] <= '9' ) {
			int fish = GetASCII( pt_Data, n, 2 ) % 100 ;
			PutIntoTable( 6, pt_Data, n, fish ) ;
		} // if
		else {
			if ( pt_Data[n].tg_prefix == '\0' ) {
				int fish = GetASCII( pt_Data, n, 1 ) % 100 ;
				if ( !PutIntoTable( 5, pt_Data, n, fish ) )  pt_Data[n].error = 3 ;
			} // if
			else {
				int fish = GetASCII( pt_Data, n, 1 ) % 100 ;
				PutIntoTable( 7, pt_Data, n, fish ) ;
			} // else
			
		} // else
		
	} // for
	
} // CreateTable()


unsigned long long int GetASCII( vector<PData> pt_Data, int n, int type ) {
	int ans = 0 ;
	if ( type == 1 ) {
		for ( int i = 0 ; pt_Data[n].label[i] != '\0' ; i++ ) {
			ans = ans + pt_Data[n].label[i] ;
		} // for
		
	} // if
	else if ( type == 2 ) {
		for ( int i = 0 ; pt_Data[n].token_groups[0][i] != '\0' ; i++ ) {
			ans = ans + pt_Data[n].token_groups[0][i] ;
		} // for
		
	} // else if
	
	return ans ;
} // GetASCII()


bool PutIntoTable( int which, vector<PData> pt_Data, int n, int fish ) {
	if ( which == 5 ) {
		if ( !gtable5[fish].haveHash ) {
			strcpy( gtable5[fish].symbol, pt_Data[n].label ) ;
			gtable5[fish].haveHash = true ;
		} // if
		else {
			if ( strcmp( gtable5[fish].symbol, pt_Data[n].label ) == 0 ) return false ;  // 重複定義error 
			
			while ( gtable5[fish].haveHash ) {
				fish++ ;
				if ( fish > 99 ) fish = 0 ;
				if ( strcmp( gtable5[fish].symbol, pt_Data[n].label ) == 0 ) return false ;  // 重複定義error 
			} // while
			
			strcpy( gtable5[fish].symbol, pt_Data[n].label ) ;
			gtable5[fish].haveHash = true ;
		} // if()
		
	} // if
	else if ( which == 6 ) {
		if ( !gtable6[fish].haveHash ) {
			strcpy( gtable6[fish].num, pt_Data[n].label ) ;
			gtable6[fish].haveHash = true ;
		} // if
		else {
			if ( strcmp( gtable6[fish].num, pt_Data[n].label ) == 0 ) ;  // 重複
			
			while ( gtable6[fish].haveHash ) {
				fish++ ;
				if ( fish > 99 ) fish = 0 ;
			} // while
			
			strcpy( gtable6[fish].num, pt_Data[n].label ) ;
			gtable6[fish].haveHash = true ;
		} // if()
		
	} // else if
	else if ( which == 7 ) {
		if ( !gtable7[fish].haveHash ) {
			strcpy( gtable7[fish].string, pt_Data[n].label ) ;
			gtable7[fish].haveHash = true ;
		} // if
		else {
			if ( strcmp( gtable7[fish].string, pt_Data[n].label ) == 0 ) ;  // 重複
			
			while ( gtable7[fish].haveHash ) {
				fish++ ;
				if ( fish > 99 ) fish = 0 ;
			} // while
			
			strcpy( gtable7[fish].string, pt_Data[n].label ) ;
			gtable7[fish].haveHash = true ;
		} // if()
		
	} // else if
	
	return true ;
} // PutIntoTable()


void CreateLoc( vector<PData> &pt_Data ) {
	Str20 tempLoc ;
	StringClear( tempLoc ) ;
	for ( int n = 0 ; n < pt_Data.size() ; n++ ) {
		if ( pt_Data[n].ins[0] != '\0' ) {
			if ( pt_Data[n].error > 0 ) ;  // 有錯誤，此行指令不做事 
			else if ( strcmp( pt_Data[n].ins, "START" ) == 0 ) {
				int count = 0 ;
				while ( pt_Data[n].token_groups[0][count] != '\0'  ) count++ ;
				
				count = 4 - count ;
				for ( int m = 0 ; m < count ; m++ ) {  // 將不足4位的補0
					strcat( tempLoc, "0" ) ;
					strcat( pt_Data[n].loc, "0" ) ;
				} // for
				
				strcat( tempLoc, pt_Data[n].token_groups[0] ) ;
				strcat( pt_Data[n].loc, pt_Data[n].token_groups[0] ) ;
			} // else if
			else if ( strcmp( pt_Data[n].ins, "END" ) == 0 ) ;  // don't do anything
			else if ( strcmp( pt_Data[n].ins, "BYTE" ) == 0 ) {
				int num = 0 ;
				for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {
					num++ ;
				} // for
				
				strcpy( pt_Data[n].loc, tempLoc ) ;
				if ( pt_Data[n].tg_prefix == 'C' ) PlusLoc( tempLoc, 3, num ) ;
				else if ( pt_Data[n].tg_prefix == 'X' ) PlusLoc( tempLoc, 3, num/2 ) ;
			} // else if
			else if ( strcmp( pt_Data[n].ins, "WORD" ) == 0 ) {
				strcpy( pt_Data[n].loc, tempLoc ) ;
				PlusLoc( tempLoc, 3, 3 ) ;
			} // else if
			else if ( strcmp( pt_Data[n].ins, "RESB" ) == 0 || strcmp( pt_Data[n].ins, "RESW" ) == 0 ) {
				int num = 0 ;
				strcpy( pt_Data[n].loc, tempLoc ) ;
				for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {  // 把此行指令的char型別數字轉int型別  
					num = num * 10 + pt_Data[n].token_groups[0][m] - '0' ;
				} // for
				
				if ( strcmp( pt_Data[n].ins, "RESW" ) == 0 ) num = num * 3 ;  // Word佔3個byte，所以*3 
				Str20 decToHex ;
				StringClear( decToHex ) ;
				ChangeToHex( decToHex, num ) ;  // 將num轉成16進位至decToHex後回傳
				int count = 0 ;
				while ( decToHex[count] != '\0' ) count++ ;  // 計算此string有多少個char 
				 
				for ( int m = 3 ; count > 0 ; m-- ) {
					if ( decToHex[count-1] >= 'a' && decToHex[count-1] <= 'z' ) decToHex[count-1] = decToHex[count-1] - 32 ;
					int a = decToHex[count-1] - '0' ;
					PlusLoc( tempLoc, m, a ) ;
					count-- ;
				} // for
				
			} // else if
			else if ( strcmp( pt_Data[n].ins, "EQU" ) == 0 ) {
				if ( pt_Data[n].token_groups[0][0] >= '0' && pt_Data[n].token_groups[0][0] <= '9' ) {
					int num = 0 ;
					for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {  // 把此行指令的char型別數字轉int型別 
						num = num * 10 + pt_Data[n].token_groups[0][m] - '0' ;
					} // for
					
					Str20 decToHex ;
					StringClear( decToHex ) ;
					ChangeToHex( decToHex, num ) ;  // 將num轉成16進位至decToHex後回傳
					int count = 0 ;
					while ( decToHex[count] != '\0' ) {  // 計算此string有多少個char 
						if ( decToHex[count] >= 'a' && decToHex[count] <= 'z' ) decToHex[count] = decToHex[count] - 32 ;  // 轉大寫 
						count++ ;
					} // while
					
					count = 4 - count ;
					for ( int m = 0 ; m < count ; m++ ) {  // 將不足4位的補0 
						strcat( pt_Data[n].loc, "0" ) ;
					} // for
					
					strcat( pt_Data[n].loc, decToHex ) ;
				} // if
				else {  // label、label+label、label-label 
					Str20 ans ;
					StringClear( ans ) ;
					GetLoc( pt_Data, n, ans ) ;
					strcat( pt_Data[n].loc, ans ) ;
				} // else
				
			} // else if
			else if ( strcmp( pt_Data[n].ins, "BASE" ) == 0 ) ;  // BASE在建Loc時不做事 
			else if ( strcmp( pt_Data[n].ins, "LTORG" ) == 0 ) {
				strcpy( pt_Data[n].pseudoLoc, tempLoc ) ;
			} // else if
			else {
				strcpy( pt_Data[n].loc, tempLoc ) ;
				PlusLoc( tempLoc, 3, GetFormat( pt_Data, n ) ) ;
			} // else
			
		} // if
		
	} // for
	
} // CreateLoc()


void PlusLoc( Str20 &loc, int which, int num ) {
	loc[which] = loc[which] + num ;
	if ( loc[which] > 57 && loc[which] < 65 ) {  // 9~A之間還隔著其他符號  
		loc[which] = loc[which] + 7 ;
	} // if
	
	if ( loc[which] > 70 ) {  // 尾數>F，需進位 
		loc[which] = loc[which] - 23 ;
		PlusLoc( loc, which-1, 1 ) ;  // Recursion進位問題 
	} // if
} // PlusLoc()


void ChangeToHex( Str20 &decToHex, int num ) {
	string temp ;
    stringstream ss ;
    ss << hex << num ;  // 10進位制轉成十六進位制讀入流中，再以字串輸出
    ss >> temp ; 
    strcpy( decToHex, temp.c_str() ) ;
} // ChangeToHex()


int GetFormat( vector<PData> pt_Data, int n ) {
	if ( pt_Data[n].ins_prefix == '+' ) return 4 ;
	else {
		for ( int m = 0 ; m < gtable1.size() ; m++ ) {
			if ( strcmp( pt_Data[n].ins, gtable1[m].mnemonic ) == 0 ) {
				if ( gtable1[m].format1 == 0 ) return gtable1[m].format2 ;
				else return gtable1[m].format1 ;
			} // if
			
		} // for
		
	} // else
	
	return 0 ;
} // FindFromat()


void CreateSIC( vector<PData> &pt_Data ) {
	Str20 ans ;
	for ( int n = 0 ; n < pt_Data.size() ; n++ ) {
		if ( pt_Data[n].ins[0] != '\0' ) {
			if ( strcmp( pt_Data[n].ins, "START" ) == 0 || strcmp( pt_Data[n].ins, "RESW" ) == 0 || strcmp( pt_Data[n].ins, "RESB" ) == 0
			     || strcmp( pt_Data[n].ins, "END" ) == 0  || pt_Data[n].error > 0 ) ;  // don't do anything
			else if ( strcmp( pt_Data[n].ins, "RSUB" ) == 0 ) {
				GetOpCode( pt_Data, n, ans ) ;
				strcat( pt_Data[n].obCode, ans ) ;
				StringClear( ans ) ;
				strcat( pt_Data[n].obCode, "0000" ) ;
			} // if
			else if ( strcmp( pt_Data[n].ins, "WORD" ) == 0 ) {
				int num = 0, count = 0 ;
				for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {
					num = num * 10 + pt_Data[n].token_groups[0][m] - '0' ;
					count++ ;
				} // for
				
				Str20 decToHex ;
				StringClear( decToHex ) ;
				ChangeToHex( decToHex, num ) ;  // 將num轉成16進位至decToHex後回傳
				
				count = 6 - count ;
				for( ; count > 0 ; count-- ) {
					strcat( pt_Data[n].obCode, "0" ) ;
				} // for
				
				strcat( pt_Data[n].obCode, decToHex ) ;
			} // else if
			else if ( pt_Data[n].tg_prefix == 'C' ) {
				int ans = 0 ;
				Str20 decToHex ;
				for ( int i = 0 ; pt_Data[n].token_groups[0][i] != '\0' ; i++ ) {
					ans = pt_Data[n].token_groups[0][i] ;
					StringClear( decToHex ) ;
					ChangeToHex( decToHex, ans ) ;
					for ( int j = 0 ; decToHex[j] != '\0' ; j++ ) {
						if ( decToHex[j] >= 'a' && decToHex[j] <='z' ) {
							decToHex[j] = decToHex[j] - 32 ;
						} // if
						
					}  // for
					
					strcat( pt_Data[n].obCode, decToHex ) ;
				} // for
				
			} // else if
			else if ( pt_Data[n].tg_prefix == 'X' ) {
				strcat( pt_Data[n].obCode, pt_Data[n].token_groups[0] ) ;
			} // else if
			else {
				GetOpCode( pt_Data, n, ans ) ;
				strcat( pt_Data[n].obCode, ans ) ;
				StringClear( ans ) ;
				GetLoc( pt_Data, n, ans ) ;
				//printf( "%d:%s\n", n, ans ) ;
				if ( pt_Data[n].token_groups[1][0] == 'X' ) PlusLoc( ans, 0, 8 ) ;
				strcat( pt_Data[n].obCode, ans ) ;
				StringClear( ans ) ;
			} // else
			
		} // if
		
	} // for
} // void CreateSIC()


void GetOpCode( vector<PData> pt_Data, int n, Str20 &ans ) {
	for ( int m = 0 ; m < gtable1.size() ; m++ ) {
		if ( strcmp( pt_Data[n].ins, gtable1[m].mnemonic ) == 0 ) {
			strcpy( ans, gtable1[m].opcode ) ;
			return ;
		} // if
		
	} // for
} // GetOpCode()


void GetLoc( vector<PData> pt_Data, int n, Str20 &ans ) {
	for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
		if ( strcmp( pt_Data[n].token_groups[0], pt_Data[m].label ) == 0 && pt_Data[n].error == 0 ) {
			strcpy( ans, pt_Data[m].loc ) ;
			return ; 
		} // if
		
	} // for
} // GetOpCode()


void CreateXE( vector<PData> &pt_Data ) {
	int ltNum = 1 ;
	Str20 ans ;
	for ( int n = 0 ; n < pt_Data.size() ; n++ ) {
		if ( pt_Data[n].ins[0] != '\0' ) {
			if ( strcmp( pt_Data[n].ins, "START" ) == 0 || strcmp( pt_Data[n].ins, "RESW" ) == 0 || strcmp( pt_Data[n].ins, "RESB" ) == 0
			     || strcmp( pt_Data[n].ins, "EQU" ) == 0 || strcmp( pt_Data[n].ins, "LTORG" ) == 0
				 || strcmp( pt_Data[n].ins, "END" ) == 0 || pt_Data[n].error > 0 ) ;  // don't do anything
			else if ( strcmp( pt_Data[n].ins, "BASE" ) == 0 ) {
				for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
					if ( strcmp( pt_Data[m].label, pt_Data[n].token_groups[0] ) == 0 ) {
						strcpy( pt_Data[n].pseudoLoc, pt_Data[m].loc ) ;
					} // if
					
				} // for
				
			} // else if
			else if ( strcmp( pt_Data[n].ins, "WORD" ) == 0 ) {
				int num = 0, count = 0 ;
				for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {
					num = num * 10 + pt_Data[n].token_groups[0][m] - '0' ;
					count++ ;
				} // for
				
				Str20 decToHex ;
				StringClear( decToHex ) ;
				ChangeToHex( decToHex, num ) ;  // 將num轉成16進位至decToHex後回傳
				
				count = 6 - count ;
				for( ; count > 0 ; count-- ) {
					strcat( pt_Data[n].obCode, "0" ) ;
				} // for
				
				strcat( pt_Data[n].obCode, decToHex ) ;
			} // else if
			else if ( pt_Data[n].haveEqual && isLiteral( pt_Data, n ) ) {
				gLTORGData.resize( ltNum ) ;
				Displacement( pt_Data, n, FindLTORG( pt_Data ), ltNum ) ;  // 找到LTORG，並將LTORG後的全部往後移
				GetOpCode( pt_Data, n, ans ) ;  // 只有format3、4會有
				ltNum++ ;
			} // else if
			else if ( pt_Data[n].tg_prefix == 'C' ) {
				int ans = 0 ;
				Str20 decToHex ;
				for ( int i = 0 ; pt_Data[n].token_groups[0][i] != '\0' ; i++ ) {
					ans = pt_Data[n].token_groups[0][i] ;
					StringClear( decToHex ) ;
					ChangeToHex( decToHex, ans ) ;
					for ( int j = 0 ; decToHex[j] != '\0' ; j++ ) {
						if ( decToHex[j] >= 'a' && decToHex[j] <='z' ) {
							decToHex[j] = decToHex[j] - 32 ;
						} // if
						
					}  // for
					
					strcat( pt_Data[n].obCode, decToHex ) ;
				} // for
				
			} // else if
			else if ( pt_Data[n].tg_prefix == 'X' ) {
				strcat( pt_Data[n].obCode, pt_Data[n].token_groups[0] ) ;
			} // else if
			else {
				GetOpCode( pt_Data, n, ans ) ;
				if ( GetFormat( pt_Data, n ) == 1 ) {
					strcpy( pt_Data[n].obCode, ans ) ;
					StringClear( ans ) ;
				} // if
				else if ( GetFormat( pt_Data, n ) == 2 ) {
					strcpy( pt_Data[n].obCode, ans ) ;
					StringClear( ans ) ;
					for ( int i = 0 ; i < pt_Data[n].amount ; i++ ) {
						GetReg( pt_Data, n, i, ans ) ;
						strcat( pt_Data[n].obCode, ans ) ;
						StringClear( ans ) ;
					} // for
					
					if ( pt_Data[n].amount == 1 ) strcat( pt_Data[n].obCode, "0" ) ;
				} // else if
				else if ( GetFormat( pt_Data, n ) == 3 ) {
					int sum = 0, count = 0 ;
					Str20 ta, tempDisp ;  // ta紀錄operand、tempDisp記錄回傳資料 
					//---------解決Opcode n i--------- 
					PlusNI( pt_Data, n, ans ) ;
					strcpy( pt_Data[n].obCode, ans ) ;
					StringClear( ans ) ;
					//---------解決x b p e--------- 
					if ( pt_Data[n].token_groups[1][0] == 'X' ) ans[0] = '8' ;
					else ans[0] = '0' ;
					
					if ( pt_Data[n].token_groups[0][0] >= '0' && pt_Data[n].token_groups[0][0] <= '9' ) {
						for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) {
							sum = sum * 10 + pt_Data[n].token_groups[0][m] - '0' ;
							count++ ;
						} // for
						
						ChangeToHex( tempDisp, sum ) ;
					} // if
					else {
						bool usePC = true ;
						GetLoc( pt_Data, n, ta ) ;
						CountDisp( pt_Data, n+1, ta, tempDisp, usePC ) ;  // 計算TA-PC的距離 
						for ( int m = 0 ; tempDisp[m] != '\0' ; m++ ) count++ ;
						if ( usePC ) ans[0] = ans[0] + 2 ;
						else ans[0] = ans[0] + 4 ;
					} // else
					
					if ( ans[0] > 57 && ans[0] < 65 ) ans[0] = ans[0] + 7 ;
					strcat( pt_Data[n].obCode, ans ) ;
					for ( int m = 0 ; m < 3-count ; m++ ) strcat( pt_Data[n].obCode, "0" ) ;
					strcat( pt_Data[n].obCode, tempDisp ) ;
					StringClear( ans ) ;
					StringClear( ta ) ;
					StringClear( tempDisp ) ;
				} // else if
				else if ( GetFormat( pt_Data, n ) == 4 ) {
					//---------解決Opcode n i--------- 
					PlusNI( pt_Data, n, ans ) ;
					strcpy( pt_Data[n].obCode, ans ) ;
					StringClear( ans ) ;
					//---------解決x b p e--------- 
					if ( pt_Data[n].token_groups[1][0] == 'X' ) strcat( pt_Data[n].obCode, "9" ) ;
					else strcat( pt_Data[n].obCode, "1" ) ;
					//---------解決address---------
					if ( pt_Data[n].token_groups[0][0] >= '0' && pt_Data[n].token_groups[0][0] <= '9' ) {
						int sum = 0 ;
						for ( int m = 0 ; pt_Data[n].token_groups[0][m] != '\0' ; m++ ) sum = sum * 10 + pt_Data[n].token_groups[0][m] - '0' ;
						ChangeToHex( ans, sum ) ;
					} // if
					else GetLoc( pt_Data, n, ans ) ;
					
					strcat( pt_Data[n].obCode, "0" ) ;
					strcat( pt_Data[n].obCode, ans ) ;
					StringClear( ans ) ;
				} // else if
				
			} // else
			
		} // if
		
	} // for
} // void CreateXE()


void GetReg( vector<PData> pt_Data, int n, int i, Str20 &ans ) {
	for ( int m = 0 ; m < gtable3.size() ; m++ ) {
		if ( strcmp( gtable3[m].mnemonic, pt_Data[n].token_groups[i] ) == 0 ) {
			ans[0] = gtable3[m].regNum + '0' ;
		} // if
		
	} // for
} // GetReg() ;


void PlusNI( vector<PData> pt_Data, int n, Str20 &ans ) {
	if ( pt_Data[n].tg_prefix == '#' ) ans[1] = ans[1] + 1 ;
	else if ( pt_Data[n].tg_prefix == '@' ) ans[1] = ans[1] + 2 ;
	else if ( pt_Data[n].tg_prefix == '\0' ) ans[1] = ans[1] + 3 ;
	
	if ( ans[1] > 57 && ans[1] < 65 ) ans[1] = ans[1] + 7 ;
} // PlusNI()


void CountDisp( vector<PData> pt_Data, int n, Str20 ta, Str20 &tempDisp, bool &usePC ) {
	int pcnum = 0, operandnum = 0 ;
	Str20 tempAns ;
	while ( pt_Data[n].ins == '\0' ) n++ ;  // 跳過是comment的行數
	
	ChangeToDec( pt_Data[n].loc, pcnum ) ;
	ChangeToDec( ta, operandnum ) ;
	
	if ( pcnum < operandnum ) {  // 正數 
		operandnum = operandnum - pcnum ;
		if ( operandnum < 16*16*16*16 ) {  // 沒有超過3byte 
			ChangeToHex( tempAns, operandnum ) ;
		} // if()
		else {  // 使用BASE 
			usePC = false ;
			FindBASE( pt_Data, n ) ; 
			ChangeToDec( pt_Data[n].pseudoLoc, pcnum ) ;
			ChangeToDec( ta, operandnum ) ;
			operandnum = operandnum - pcnum ;
		} // else
		
		strcpy( tempDisp, tempAns ) ;
		for ( int m = 0 ; tempDisp[m] != '\0' ; m++ ) {
			if ( tempDisp[m] >= 'a' && tempDisp[m] <= 'z' ) tempDisp[m] = tempDisp[m] - 32 ;
		} // if
		
	} // if
	else if ( pcnum > operandnum ) {  // 負數  
		pcnum = pcnum - operandnum ;
		if ( pcnum < 16*16*16*16 ) {  // 沒有超過3byte 
			ChangeToHex( tempAns, -pcnum ) ;
		} // if()
		else {  // 使用BASE 
			usePC = false ;
			FindBASE( pt_Data, n ) ; 
			ChangeToDec( pt_Data[n].pseudoLoc, pcnum ) ;
			ChangeToDec( ta, operandnum ) ;
			pcnum = pcnum - operandnum ;
			ChangeToHex( tempAns, -pcnum ) ;
		} // else
		
		tempDisp[0] = tempAns[5] ;
		tempDisp[1] = tempAns[6] ;
		tempDisp[2] = tempAns[7] ;
		for ( int m = 0 ; tempDisp[m] != '\0' ; m++ ) {
			if ( tempDisp[m] >= 'a' && tempDisp[m] <= 'z' ) tempDisp[m] = tempDisp[m] - 32 ;
		} // for
		
	} // else if
} // CountDisp()


void ChangeToDec( Str20 hexToDec, int &num ) {
	string temp ;
    stringstream ss ;
    ss << hexToDec ;
    ss >> temp ;
    ss.clear() ;
    ss << hex << hexToDec ;  // 以16進位制讀入流中
    ss >> num ;  // 10進位制int型輸出
} // ChangeToHex()


void FindBASE( vector<PData> pt_Data, int &n ) {
	for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
		if ( strcmp( pt_Data[m].ins, "BASE" ) == 0 ) {
			n = m ;
		} // if
		
	} // for
} // void


bool isLiteral( vector<PData> pt_Data, int n ) {
	for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
		if ( strcmp( pt_Data[m].token_groups[0], pt_Data[n].token_groups[0] ) == 0 && m != n ) {
			return false ;
		} // if
		
	} // for
	
	return true ;
} // isLiteral()


int FindLTORG( vector<PData> pt_Data ) {
	for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
		if ( strcmp( pt_Data[m].ins, "LTORG" ) == 0 ) {
			return m ;
		} // if
		
	} // for
	
} // FindLTORG()


void Displacement( vector<PData> pt_Data, int n, int where, int ltNum ) {
	if ( pt_Data[n].tg_prefix == 'X' ) {
		
	} // if
	else if ( pt_Data[n].tg_prefix == 'C' ) {
		
	} // if
	else {
		
	} // else
	
	
} // Displacement()


void FoutTXT( vector<PData> pt_Data, vector<FData> tc_main, int type ) {
	FILE *fout ;
	
	if ( type == 1 ) fout = fopen( "SIC_output.txt", "w+" ) ;
	else if ( type == 2 ) fout = fopen( "SICXE_output.txt", "w+" ) ;
	
	fprintf( fout, "Line\tLoc\tSource\tstatement\t\tObject code\n\n" ) ;
	for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
		fprintf( fout, "%3d\t", m*5 ) ;
		if ( tc_main[m].comment[0] == '\0' ) {
			fprintf( fout, "%s\t%s\t", pt_Data[m].loc, pt_Data[m].label ) ;
			if ( pt_Data[m].ins_prefix == '+' ) fprintf( fout, "%c", pt_Data[m].ins_prefix ) ;
			fprintf( fout, "%s\t", pt_Data[m].ins ) ;
			if ( pt_Data[m].haveEqual ) fprintf( fout, "=" ) ;
			if ( pt_Data[m].tg_prefix != '\0') fprintf( fout, "%c", pt_Data[m].tg_prefix ) ;
			if ( pt_Data[m].tg_prefix == 'C' || pt_Data[m].tg_prefix == 'X' ) fprintf( fout, "\'" ) ;
			fprintf( fout, "%s", pt_Data[m].token_groups[0] ) ;
			if ( pt_Data[m].token_groups[1][0] != '\0' ) fprintf( fout, ",%s", pt_Data[m].token_groups[1] ) ;
			if ( pt_Data[m].tg_prefix == 'C' || pt_Data[m].tg_prefix == 'X' ) fprintf( fout, "\'" ) ;
			if ( pt_Data[m].error == 0 ) fprintf( fout, "\t\t%s\n", pt_Data[m].obCode ) ;
			else if ( pt_Data[m].error == 1 ) fprintf( fout, "\t\tsyntax error\n" ) ;
			else if ( pt_Data[m].error == 2 ) fprintf( fout, "\t\tduplicate definition\n" ) ;
			else if ( pt_Data[m].error == 3 ) fprintf( fout, "\t\tundefined symbol\n" ) ;
		} // if
		else {
			fprintf( fout, "\t%s\n", tc_main[m].comment ) ;
		} // else
		
	} // for
		
	fclose( fout ) ;
} // FoutTXT()


void Initialize( vector<FData> &main, vector<FData> &tc_main, vector<PData> &pt_Data ) {
	main.clear() ;
	tc_main.clear() ;
	pt_Data.clear() ;
} // Initialize()


void ChangeObjectCode( vector<PData> &pt_Data ) {
	int len = 0, temp = 0 ;
	for ( int m = 0 ; m < pt_Data.size() ; m++ ) {
		if ( pt_Data[m].obCode[0] != '\0' ) {
			for ( int o = 0 ; pt_Data[m].obCode[o] != '\0' ; o++ ) len++ ;
			
			for ( int j = 0 ; j < len / 2 ; j++ ) {
				
				swap( pt_Data[m].obCode[j], pt_Data[m].obCode[len - j - 1] ) ;
			} // for
			
		} // if
		
		len = 0 ;
		temp = 0 ;
	} // for

} // ChangeObjectCode