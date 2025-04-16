// 止步於project3-10

# include <cmath>
# include <iostream>
# include <stdio.h>
# include <string.h>
# include <string>
# include <vector>

using namespace std;

// -------struct-------
struct Data {
  string name ;
  string token_type ;  // 判別這個token是identifier, constant或其他
  int op_bit ;  // 判別這個operator是幾位元運算子
  int rl ;  // 判別這個PP或MM是前置還是後置

  vector<Data> formula_in_array ;  // 用來存放[]裡的計算式或id
  bool array_need_cal ;

  string num_type ;  // 判別這個identifier是int, float, char, string, bool
  int ivalue ;
  float fvalue ;
  bool bvalue ;
  string svalue ;

  bool isError ;
  string error_mess ;
} ;

// -------global variable-------
/*
gover_read用來當PeekToken()後觸發throw, 需要對出錯的行數校正回歸
*/
int gtest = 0, gline = 0, gisinfunction = 0 ;
int gover_read = 0 ;
bool gisEOF = false ;
string goriginal_line = "" ;
vector<Data> *gfunction_var ;  // 用來存放function裡的變數
vector<Data> *gspecial_var = new vector<Data> ;
vector<Data> *gspecial_function = new vector<Data> ;
vector< vector<Data> > *gvariable_table = new vector< vector<Data> > ;
vector< vector<Data> > *gfunction_table = new vector< vector<Data> > ;
vector<Data> *gvar_array_table = new vector<Data> ;  // 用來存放array的變數


// -------function-------
class Ball {
  private:
  Data mError_token ;
  int mError_type ;

  public:
  Ball( Data token, int type ) {
    mError_token = token ;
    mError_type = type ;
  } // Ball()

  void Print() {
    printf( "> Line %d : ", gline ) ;
    if ( mError_type == 0 ) {
      printf( "Is Safe, %s have Bug\n", mError_token.name.c_str() ) ;
    } // if()
    else if ( mError_type == 1 ) {
      printf( "unrecognized token with first char : '%s'\n", mError_token.name.c_str() ) ;
    } // else if()
    else if ( mError_type == 2 ) {
      printf( "unexpected token : '%s'\n", mError_token.name.c_str() ) ;
    } // else if()
    else if ( mError_type == 3 ) {
      printf( "undefined identifier : '%s'\n", mError_token.name.c_str() ) ;
    } // else if()
    else if ( mError_type == 4 ) {
      printf( "EOF" ) ;
    } // else if()

  } // Print()

};

void User_Input( vector<Data> *token_buffer, vector<Data> *result_data, string &work_type ) ;
bool Definition( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Type_Specifier( Data token ) ;
bool Function_Definition_OR_Declarators( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Declarators( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Function_Definition_Without_ID( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Formal_Parameter_List( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Compound_Statement( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Declaration( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Statement( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Expression( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Basic_Expression( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Identifier_Started_Basic_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_PPMM_Identifier_Started_Basic_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Sign( Data token ) ;
bool Actual_Parameter_List( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Assignment_Operator( Data token ) ;
bool Romce_AND_Romloe( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Logical_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Logical_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Logical_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Bit_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Bit_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Bit_Ex_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Bit_Ex_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Bit_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Bit_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Equality_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Equality_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Relational_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Relational_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Shift_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Shift_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Additive_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Additive_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Maybe_Mult_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Rest_OF_Maybe_Mult_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Unary_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Signed_Unary_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Unsigned_Unary_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) ;
bool Variable_Exist( Data token ) ;
bool Function_Exist( Data token ) ;
bool Function_Var_Exist( Data token ) ;
void ThrowBall( Data token, int type ) ;

void GlobalInit() ;
void Definition_OR_Statement( vector<Data> *result_data, string &work_type, bool &isDone ) ;
void Store_To_Table( vector<Data> *result_data ) ;
void Store_To_Function_Table( vector<Data> *result_data ) ;
bool Update_Function_Table( vector<Data> *result_data ) ;
void Store_To_Variable_Table( vector<Data> *result_data ) ;
bool Update_Variable_Table( vector<Data> *result_data, int pos ) ;
void Store_To_Array_Table( vector<Data> *sentence ) ;
void Sort( vector<Data> *sentence, int type ) ;
bool IsGlobalFunction( vector<Data> *result_data, bool &isDone ) ;
void ListAllVariables() ;
void ListAllFunctions() ;
void ListVariable( vector<Data> *result_data ) ;
void ListFunction( vector<Data> *result_data ) ;
void ParameterInit( vector<Data> *result_data, string &work_type ) ;

void PrepareToCal( vector<Data> *result_data ) ;
void GiveNumType( vector<Data> *formula ) ;
void SloveArray( vector<Data> *formula ) ;
void SloveOP( vector<Data> *formula ) ;
bool IsOperators( Data op ) ;
void InfixToPostfix( vector<Data> *infix ) ;
void SloveThree( vector<Data> *infix, vector<Data> &s, vector<Data> &postfix, int &pos ) ;
bool IsAssigns( Data token ) ;
int GetPriority( Data op ) ;
bool IsRight( Data op ) ;
void CalPostfix( vector<Data> *postfix ) ;
void SloveFormulaInArray( Data &operand ) ;
void UpdateValue( Data operand ) ;
void ServicePPMM( vector<Data> &vec_of_PPMM, int type ) ;
void CoutFormula( vector<Data> *formula ) ;
void SloveIF( vector<Data> *formula ) ;
void RenewFunctionVar( vector<Data> *formula ) ;
void SloveWHILE( vector<Data> *formula ) ;
bool IsInFunction( Data operand ) ;

void PeekToken( vector<Data> *token_buffer, Data &peek_token ) ;
void Get_And_PushBack( vector<Data> *token_buffer, vector<Data> *result_data ) ;
void Transform_to_Token( vector<Data> *token_buffer ) ;
void GetLine() ;
bool IsDelimiter( char currentC ) ;
bool IsDouble_Delimiter( char currentC, int pos ) ;
void SetTokenType( vector<Data> *token_buffer, Data &token ) ;
bool IsConstant( Data &token ) ;
bool IsNumber( Data &token ) ;
bool IsIdentifier( Data &token ) ;
void DataClear( Data &data ) ;
// -------main-------
int main() {
  char garbage ;
  bool isDone = false ;
  string work_type = "" ;  // 用來儲存他是definition還是statement
  gfunction_var = new vector<Data> ;
  vector<Data> *token_buffer = new vector<Data> ;
  vector<Data> *result_data = new vector<Data> ;

  scanf( "%d%c", &gtest, &garbage ) ;
  printf( "Our-C running ...\n" ) ;
  GlobalInit() ;

  do {
    try {
      User_Input( token_buffer, result_data, work_type ) ;
      Definition_OR_Statement( result_data, work_type, isDone ) ;

      if ( gisEOF ) {
        Data temp ;
        DataClear( temp ) ;
        temp.name = "EOF" ;
        isDone = true ;
        throw Ball( temp, 4 ) ;
      } // if()

    } // try()
    catch ( Ball e ) {
      e.Print() ;
      if ( gover_read != 0 ) {
        gover_read = 0 ;
        gline = 1 ;
      } // if()
      else if ( gover_read == 0 ) {
        delete token_buffer ;
        token_buffer = new vector<Data> ;
        gline = 0 ;
      } // else if()

    } // catch
    
    ParameterInit( result_data, work_type ) ;  // 初始化gisinfunction, gfunction_var, result_data, work_type
    result_data = new vector<Data> ;
    if ( token_buffer->empty() ) {
      delete token_buffer ;
      token_buffer = new vector<Data> ;
      gline = 0 ;
    } // if()
    else {
      gline = 1 ; // 當token_buffer不為空時, 代表下一行的開頭
    } // else

  } while ( !isDone ) ;

  printf( "Our-C exited ..." ) ;
  return 0 ;
} // main()

// -------function-------
/*
第一版:把全部的文法實作完成
第二版:追加對於Identifier的處理, 引入gisinfunction與3個確認table的function
第三版:修正throw的方式, 並加入gover_read來校正行數
*/

void User_Input( vector<Data> *token_buffer, vector<Data> *result_data, string &work_type ) {
  /*
  ( definition | statement ) { definition | statement }
  */

  if ( Definition( token_buffer, result_data ) ) {
    work_type = "Definition" ;
  } // if()
  else if ( Statement( token_buffer, result_data ) ) {
    work_type = "Statement" ;
  } // else if()
  else {
    Data temp ;
    PeekToken( token_buffer, temp ) ;
    ThrowBall( temp, 2 ) ;
  } // else

} // User_Input()

bool Definition( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  :           VOID Identifier function_definition_without_ID
  | type_specifier Identifier function_definition_or_declarators
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.token_type == "VOID" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "Identifier" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Function_Definition_Without_ID( token_buffer, result_data ) ) {
        return true ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else if ( Type_Specifier( peek_token ) ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "Identifier" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Function_Definition_OR_Declarators( token_buffer, result_data ) ) {
        return true ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Definition()

bool Type_Specifier( Data token ) {
  /*
  INT | CHAR | FLOAT | STRING | BOOL
  */
  if ( token.token_type == "INT" || token.token_type == "CHAR" || token.token_type == "FLOAT" ||
       token.token_type == "STRING" || token.token_type == "BOOL" ) {
    return true ;
  } // if()

  return false ;
} // Type_Specifier()

bool Function_Definition_OR_Declarators( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  : function_definition_without_ID
  | rest_of_declarators
  */
  if ( Function_Definition_Without_ID( token_buffer, result_data ) ) {
    return true ;
  } // if()
  else if ( Rest_OF_Declarators( token_buffer, result_data ) ) {
    return true ;
  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Function_Definition_OR_Declarators()

bool Rest_OF_Declarators( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  [ '[' Constant ']' ] 
  { ',' Identifier [ '[' Constant ']' ] } ';'
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.name == "[" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "Constant" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      PeekToken( token_buffer, peek_token ) ;
      if ( peek_token.name == "]" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()

  if ( peek_token.name == "," ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "Identifier" ) {
      if ( gisinfunction > 0 ) {
        // 這裡是function內的變數
        gfunction_var->push_back( peek_token ) ;
      } // if()

      Get_And_PushBack( token_buffer, result_data ) ;
      return Rest_OF_Declarators( token_buffer, result_data ) ;
    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()

  if ( peek_token.name == ";" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Declarators()

bool Function_Definition_Without_ID( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  '(' [ VOID | formal_parameter_list ] ')' compound_statement
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.name == "(" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "VOID" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
    } // if()
    else if ( Formal_Parameter_List( token_buffer, result_data ) ) {
      ;
    } // else if()

    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == ")" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Compound_Statement( token_buffer, result_data ) ) {
        return true ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Function_Definition_Without_ID()

bool Formal_Parameter_List( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
  { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( Type_Specifier( peek_token ) ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == "&" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      PeekToken( token_buffer, peek_token ) ;
    } // if()

    if ( peek_token.token_type == "Identifier" ) {
      gfunction_var->push_back( peek_token ) ;  // 這裡是functiont傳入的變數
      Get_And_PushBack( token_buffer, result_data ) ;
      PeekToken( token_buffer, peek_token ) ;
      if ( peek_token.name == "[" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.token_type == "Constant" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          PeekToken( token_buffer, peek_token ) ;
          if ( peek_token.name == "]" ) {
            Get_And_PushBack( token_buffer, result_data ) ;
            PeekToken( token_buffer, peek_token ) ;
          } // if()
          else {
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else {
    return false ;
  } // else

  if ( peek_token.name == "," ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    return Formal_Parameter_List( token_buffer, result_data ) ;
  } // if()

  return true ;
} // Formal_Parameter_List()

bool Compound_Statement( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  '{' { declaration | statement } '}'
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.name == "{" ) {
    // 從這開始為function的內容
    gisinfunction++ ;
    Get_And_PushBack( token_buffer, result_data ) ;
    while ( Declaration( token_buffer, result_data ) || Statement( token_buffer, result_data ) ) {
      ;
    } // while()

    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == "}" ) {
      gisinfunction-- ;
      Get_And_PushBack( token_buffer, result_data ) ;
      return true ;
    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Compound_Statement()

bool Declaration( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  type_specifier Identifier rest_of_declarators
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( Type_Specifier( peek_token ) ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "Identifier" ) {
      gfunction_var->push_back( peek_token ) ;  // 這裡是function內宣告的變數
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Rest_OF_Declarators( token_buffer, result_data ) ) {
        return true ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Declaration()

bool Statement( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  : ';'
  | expression ';'
  | RETURN [ expression ] ';'
  | compound_statement
  | IF '(' expression ')' statement [ ELSE statement ]
  | WHILE '(' expression ')' statement
  | DO statement WHILE '(' expression ')' ';'
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.name == ";" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    return true ;
  } // if()
  else if ( peek_token.token_type == "RETURN" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Expression( token_buffer, result_data ) ) {
      ;
    } // if()

    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == ";" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      return true ;
    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( Compound_Statement( token_buffer, result_data ) ) {
    return true ;
  } // else if()
  else if ( peek_token.token_type == "IF" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == "(" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Expression( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == ")" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          if ( Statement( token_buffer, result_data ) ) {
            PeekToken( token_buffer, peek_token ) ;
            if ( peek_token.token_type == "ELSE" ) {
              Get_And_PushBack( token_buffer, result_data ) ;
              if ( Statement( token_buffer, result_data ) ) {
                return true ;
              } // if()
              else {
                PeekToken( token_buffer, peek_token ) ;
                ThrowBall( peek_token, 2 ) ;
              } // else

            } // if()

          } // if()
          else {
            PeekToken( token_buffer, peek_token ) ;
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( peek_token.token_type == "WHILE" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == "(" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Expression( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == ")" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          if ( Statement( token_buffer, result_data ) ) {
            return true ;
          } // if()
          else {
            PeekToken( token_buffer, peek_token ) ;
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( peek_token.token_type == "DO" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Statement( token_buffer, result_data ) ) {
      PeekToken( token_buffer, peek_token ) ;
      if ( peek_token.token_type == "WHILE" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == "(" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          if ( Expression( token_buffer, result_data ) ) {
            PeekToken( token_buffer, peek_token ) ;
            if ( peek_token.name == ")" ) {
              Get_And_PushBack( token_buffer, result_data ) ;
              PeekToken( token_buffer, peek_token ) ;
              if ( peek_token.name == ";" ) {
                Get_And_PushBack( token_buffer, result_data ) ;
                return true ;
              } // if()
              else {
                ThrowBall( peek_token, 2 ) ;
              } // else

            } // if()
            else {
              ThrowBall( peek_token, 2 ) ;
            } // else

          } // if()
          else {
            PeekToken( token_buffer, peek_token ) ;
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( Expression( token_buffer, result_data ) ) {
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == ";" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      return true ;
    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Statement()

bool Expression( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  basic_expression { ',' basic_expression }
  */
  if ( Basic_Expression( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    if ( peek_token.name == "," ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      return Expression( token_buffer, result_data ) ;
    } // if()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Expression()

bool Basic_Expression( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  : Identifier rest_of_Identifier_started_basic_exp
  | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  | sign { sign } signed_unary_exp romce_and_romloe
  | ( Constant | '(' expression ')' ) romce_and_romloe
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.token_type == "Identifier" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    gover_read = gline ;  // 為了校正行數出錯
    PeekToken( token_buffer, peek_token ) ;  // 為了確定這個ID是Variable還是Function
    if ( Variable_Exist( result_data->at( result_data->size()-1 ) ) ||
         ( peek_token.name == "(" && Function_Exist( result_data->at( result_data->size()-1 ) ) ) ||
         ( gisinfunction > 0 && Function_Var_Exist( result_data->at( result_data->size()-1 ) ) ) ) {
      gover_read = 0 ;
      if ( Rest_OF_Identifier_Started_Basic_Exp( token_buffer, result_data ) ) {
        return true ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      if ( gover_read == gline ) {
        gover_read = 0 ;
      } // if()
      else {
        gline = gover_read ;
      } // else

      ThrowBall( result_data->at( result_data->size()-1 ), 3 ) ;
    } // else
    
  } // if()
  else if ( peek_token.token_type == "PP" || peek_token.token_type == "MM" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "Identifier" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Variable_Exist( result_data->at( result_data->size()-1 ) ) ||
           ( gisinfunction > 0 && Function_Var_Exist( result_data->at( result_data->size()-1 ) ) ) ) {
        if ( Rest_OF_PPMM_Identifier_Started_Basic_Exp( token_buffer, result_data ) ) {
          return true ;
        } // if()
        else {
          PeekToken( token_buffer, peek_token ) ;
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        ThrowBall( result_data->at( result_data->size()-1 ), 3 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( Sign( peek_token ) ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    while ( Sign( peek_token ) ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      PeekToken( token_buffer, peek_token ) ;
    } // while()

    if ( Signed_Unary_Exp( token_buffer, result_data ) ) {
      if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
        return true ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( peek_token.token_type == "Constant" || peek_token.name == "(" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( result_data->at( result_data->size()-1 ).name == "(" ) {
      if ( Expression( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == ")" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else if ( result_data->at( result_data->size()-1 ).token_type == "Constant" ) {
      ;
    } // else if()
    
    if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
      return true ;
    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Basic_Expression()

bool Rest_OF_Identifier_Started_Basic_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  : '(' [ actual_parameter_list ] ')' romce_and_romloe
  | [ '[' expression ']' ]
      ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe )
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.name == "(" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Actual_Parameter_List( token_buffer, result_data ) ) {
      ;
    } // if()

    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == ")" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
        return true ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else if ( peek_token.name == "[" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Expression( token_buffer, result_data ) ) {
      PeekToken( token_buffer, peek_token ) ;
      if ( peek_token.name == "]" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        PeekToken( token_buffer, peek_token ) ;
        if ( Assignment_Operator( peek_token ) ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          if ( Basic_Expression( token_buffer, result_data ) ) {
            return true ;
          } // if()
          else {
            PeekToken( token_buffer, peek_token ) ;
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else if ( peek_token.token_type == "PP" || peek_token.token_type == "MM" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
            return true ;
          } // if()
          else {
            PeekToken( token_buffer, peek_token ) ;
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // else if()
        else if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
          return true ;
        } // else if()
        else {
          PeekToken( token_buffer, peek_token ) ;
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( Assignment_Operator( peek_token ) ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Basic_Expression( token_buffer, result_data ) ) {
      return true ;
    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( peek_token.token_type == "PP" || peek_token.token_type == "MM" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
      return true ;
    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
    return true ;
  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Identifier_Started_Basic_Exp()

bool Rest_OF_PPMM_Identifier_Started_Basic_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  [ '[' expression ']' ] romce_and_romloe
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.name == "[" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Expression( token_buffer, result_data ) ) {
      PeekToken( token_buffer, peek_token ) ;
      if ( peek_token.name == "]" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
          return true ;
        } // if()
        else {
          PeekToken( token_buffer, peek_token ) ;
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else if ( Romce_AND_Romloe( token_buffer, result_data ) ) {
    return true ;
  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_PPMM_Identifier_Started_Basic_Exp()

bool Sign( Data token ) {
  /*
  '+' | '-' | '!'
  */
  if ( token.name == "+" || token.name == "-" || token.name == "!" ) {
    return true ;
  } // if()

  return false ;
} // Sign()

bool Actual_Parameter_List( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  basic_expression { ',' basic_expression }
  */
  if ( Basic_Expression( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    if ( peek_token.name == "," ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      return Actual_Parameter_List( token_buffer, result_data ) ;
    } // if()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Actual_Parameter_List()

bool Assignment_Operator( Data token ) {
  /*
  '=' | TE | DE | RE | PE | ME
  */
  if ( token.name == "=" || token.token_type == "TE" || token.token_type == "DE" ||
       token.token_type == "RE" || token.token_type == "PE" || token.token_type == "ME" ) {
    return true ;
  } // if()

  return false ;
} // Assignment_Operator()

bool Romce_AND_Romloe( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
  */
  Data peek_token ;

  if ( Rest_OF_Maybe_Logical_OR_Exp( token_buffer, result_data ) ) {
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.name == "?" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Basic_Expression( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == ":" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          if ( Basic_Expression( token_buffer, result_data ) ) {
            return true ;
          } // if()
          else {
            PeekToken( token_buffer, peek_token ) ;
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Romce_AND_Romloe()

bool Rest_OF_Maybe_Logical_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
  */
  if ( Rest_OF_Maybe_Logical_AND_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.token_type == "OR" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Logical_AND_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Logical_OR_Exp()

bool Maybe_Logical_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  */
  if ( Maybe_Bit_OR_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.token_type == "AND" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Bit_OR_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Logical_AND_Exp()

bool Rest_OF_Maybe_Logical_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  */
  if ( Rest_OF_Maybe_Bit_OR_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.token_type == "AND" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Bit_OR_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Logical_AND_Exp()

bool Maybe_Bit_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  */
  if ( Maybe_Bit_Ex_OR_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "|" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Bit_Ex_OR_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Bit_OR_Exp()

bool Rest_OF_Maybe_Bit_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  */
  if ( Rest_OF_Maybe_Bit_Ex_OR_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "|" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Bit_Ex_OR_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Bit_OR_Exp()

bool Maybe_Bit_Ex_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  */
  if ( Maybe_Bit_AND_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "^" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Bit_AND_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Bit_Ex_OR_Exp()

bool Rest_OF_Maybe_Bit_Ex_OR_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  */
  if ( Rest_OF_Maybe_Bit_AND_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "^" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Bit_AND_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Bit_Ex_OR_Exp()

bool Maybe_Bit_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_equality_exp { '&' maybe_equality_exp }
  */
  if ( Maybe_Equality_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "&" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Equality_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Bit_AND_Exp()

bool Rest_OF_Maybe_Bit_AND_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_equality_exp { '&' maybe_equality_exp }
  */
  if ( Rest_OF_Maybe_Equality_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "&" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Equality_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Bit_AND_Exp()

bool Maybe_Equality_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
  */
  if ( Maybe_Relational_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.token_type == "EQ" || peek_token.token_type == "NEQ" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Relational_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Equality_Exp()

bool Rest_OF_Maybe_Equality_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
  */
  if ( Rest_OF_Maybe_Relational_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.token_type == "EQ" || peek_token.token_type == "NEQ" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Relational_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Equality_Exp()

bool Maybe_Relational_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  */
  if ( Maybe_Shift_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "<" || peek_token.name == ">" ||
            peek_token.token_type == "LE" || peek_token.token_type == "GE" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Shift_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Relational_Exp()

bool Rest_OF_Maybe_Relational_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  */
  if ( Rest_OF_Maybe_Shift_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "<" || peek_token.name == ">" ||
            peek_token.token_type == "LE" || peek_token.token_type == "GE" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Shift_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Relational_Exp()

bool Maybe_Shift_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  */
  if ( Maybe_Additive_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.token_type == "LS" || peek_token.token_type == "RS" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Additive_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Shift_Exp()

bool Rest_OF_Maybe_Shift_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  */
  if ( Rest_OF_Maybe_Additive_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.token_type == "LS" || peek_token.token_type == "RS" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Additive_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Shift_Exp()

bool Maybe_Additive_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  maybe_multiplicative_exp { ( '+' | '-' ) maybe_multiplicative_exp }
  */
  if ( Maybe_Mult_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "+" || peek_token.name == "-" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Mult_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else
      
    } // while()

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Additive_Exp()

bool Rest_OF_Maybe_Additive_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  rest_of_maybe_multiplicative_exp { ( '+' | '-' ) maybe_multiplicative_exp }
  */
  if ( Rest_OF_Maybe_Mult_Exp( token_buffer, result_data ) ) {
    Data peek_token ;
    PeekToken( token_buffer, peek_token ) ;

    while ( peek_token.name == "+" || peek_token.name == "-" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Maybe_Mult_Exp( token_buffer, result_data ) ) {
        PeekToken( token_buffer, peek_token ) ;
      } // if()
      else {
        PeekToken( token_buffer, peek_token ) ;
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // while()

    return true ;
  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Rest_OF_Maybe_Additive_Exp()

bool Maybe_Mult_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  unary_exp rest_of_maybe_mult_exp
  */
  if ( Unary_Exp( token_buffer, result_data ) ) {
    if ( Rest_OF_Maybe_Mult_Exp( token_buffer, result_data ) ) {
      return true ;
    } // if()
    else {
      Data peek_token ;
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else {
    return false ;
  } // else

  return true ;
} // Maybe_Mult_Exp()

bool Rest_OF_Maybe_Mult_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  { ( '*' | '/' | '%' ) unary_exp }  could be empty
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.name == "*" || peek_token.name == "/" || peek_token.name == "%" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Unary_Exp( token_buffer, result_data ) ) {
      return Rest_OF_Maybe_Mult_Exp( token_buffer, result_data ) ;
    } // if()

  } // if()

  return true ;
} // Rest_OF_Maybe_Mult_Exp()

bool Unary_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  : sign { sign } signed_unary_exp
  | unsigned_unary_exp
  | ( PP | MM ) Identifier [ '[' expression ']' ]
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( Sign( peek_token ) ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    while ( Sign( peek_token ) ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      PeekToken( token_buffer, peek_token ) ;
    } // while()

    if ( Signed_Unary_Exp( token_buffer, result_data ) ) {
      return true ;
    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // if()
  else if ( peek_token.token_type == "PP" || peek_token.token_type == "MM" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    PeekToken( token_buffer, peek_token ) ;
    if ( peek_token.token_type == "Identifier" ) {
      Get_And_PushBack( token_buffer, result_data ) ;
      if ( Variable_Exist( result_data->at( result_data->size()-1 ) ) ||
           ( gisinfunction > 0 && Function_Var_Exist( result_data->at( result_data->size()-1 ) ) ) ) {
        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == "[" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          if ( Expression( token_buffer, result_data ) ) {
            PeekToken( token_buffer, peek_token ) ;
            if ( peek_token.name == "]" ) {
              Get_And_PushBack( token_buffer, result_data ) ;
              return true ;
            } // if()
            else {
              ThrowBall( peek_token, 2 ) ;
            } // else

          } // if()
          else {
            PeekToken( token_buffer, peek_token ) ;
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()

      } // if()
      else {
        ThrowBall( result_data->at( result_data->size()-1 ), 3 ) ;
      } // else

    } // if()
    else {
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else if ( Unsigned_Unary_Exp( token_buffer, result_data ) ) {
    return true ;
  } // else if()
  else {
    return false ;
  } // else if()

  return true ;
} // Unary_Exp()

bool Signed_Unary_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  : Identifier [ '(' [ actual_parameter_list ] ')' 
                 |
                 '[' expression ']'
               ]
  | Constant 
  | '(' expression ')'
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.token_type == "Identifier" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    gover_read = gline ;
    PeekToken( token_buffer, peek_token ) ;
    if ( Variable_Exist( result_data->at( result_data->size()-1 ) ) ||
         ( peek_token.name == "(" && Function_Exist( result_data->at( result_data->size()-1 ) ) ) ||
         ( gisinfunction > 0 && Function_Var_Exist( result_data->at( result_data->size()-1 ) ) ) ) {
      gover_read = 0 ;
      if ( peek_token.name == "[" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        if ( Expression( token_buffer, result_data ) ) {
          PeekToken( token_buffer, peek_token ) ;
          if ( peek_token.name == "]" ) {
            Get_And_PushBack( token_buffer, result_data ) ;
            return true ;
          } // if()
          else {
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else {
          PeekToken( token_buffer, peek_token ) ;
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else if ( peek_token.name == "(" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        if ( Actual_Parameter_List( token_buffer, result_data ) ) {
          ;
        } // if()

        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == ")" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          return true ;
        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // else if()

    } // if()
    else {
      if ( gover_read == gline ) {
        gover_read = 0 ;
      } // if()
      else {
        gline = gover_read ;
      } // else

      ThrowBall( result_data->at( result_data->size()-1 ), 3 ) ;
    } // else

  } // if()
  else if ( peek_token.token_type == "Constant" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    return true ;
  } // else if()
  else if ( peek_token.name == "(" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Expression( token_buffer, result_data ) ) {
      PeekToken( token_buffer, peek_token ) ;
      if ( peek_token.name == ")" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        return true ;
      } // if()
      else {
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Signed_Unary_Exp()

bool Unsigned_Unary_Exp( vector<Data> *token_buffer, vector<Data> *result_data ) {
  /*
  : Identifier [ '(' [ actual_parameter_list ] ')' 
                 |
                 '[' expression ']' [ ( PP | MM ) ]
               ]
  | Constant 
  | '(' expression ')'
  */
  Data peek_token ;
  PeekToken( token_buffer, peek_token ) ;

  if ( peek_token.token_type == "Identifier" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    gover_read = gline ;
    PeekToken( token_buffer, peek_token ) ;
    if ( Variable_Exist( result_data->at( result_data->size()-1 ) ) ||
         ( peek_token.name == "(" && Function_Exist( result_data->at( result_data->size()-1 ) ) ) ||
         ( gisinfunction > 0 && Function_Var_Exist( result_data->at( result_data->size()-1 ) ) ) ) {
      gover_read = 0 ;
      if ( peek_token.name == "[" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        if ( Expression( token_buffer, result_data ) ) {
          PeekToken( token_buffer, peek_token ) ;
          if ( peek_token.name == "]" ) {
            Get_And_PushBack( token_buffer, result_data ) ;
            PeekToken( token_buffer, peek_token ) ;
            if ( peek_token.token_type == "PP" || peek_token.token_type == "MM" ) {
              Get_And_PushBack( token_buffer, result_data ) ;
            } // if()

            return true ;
          } // if()
          else {
            ThrowBall( peek_token, 2 ) ;
          } // else

        } // if()
        else {
          PeekToken( token_buffer, peek_token ) ;
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // if()
      else if ( peek_token.token_type == "PP" || peek_token.token_type == "MM" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
      } // else if()
      else if ( peek_token.name == "(" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        if ( Actual_Parameter_List( token_buffer, result_data ) ) {
          ;
        } // if()

        PeekToken( token_buffer, peek_token ) ;
        if ( peek_token.name == ")" ) {
          Get_And_PushBack( token_buffer, result_data ) ;
          return true ;
        } // if()
        else {
          ThrowBall( peek_token, 2 ) ;
        } // else

      } // else if()

    } // if()
    else {
      if ( gover_read == gline ) {
        gover_read = 0 ;
      } // if()
      else {
        gline = gover_read ;
      } // else

      ThrowBall( result_data->at( result_data->size()-1 ), 3 ) ;
    } // else

  } // if()
  else if ( peek_token.token_type == "Constant" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    return true ;
  } // else if()
  else if ( peek_token.name == "(" ) {
    Get_And_PushBack( token_buffer, result_data ) ;
    if ( Expression( token_buffer, result_data ) ) {
      PeekToken( token_buffer, peek_token ) ;
      if ( peek_token.name == ")" ) {
        Get_And_PushBack( token_buffer, result_data ) ;
        return true ;
      } // if()
      else {
        ThrowBall( peek_token, 2 ) ;
      } // else

    } // if()
    else {
      PeekToken( token_buffer, peek_token ) ;
      ThrowBall( peek_token, 2 ) ;
    } // else

  } // else if()
  else {
    return false ;
  } // else

  return true ;
} // Unsigned_Unary_Exp()

bool Variable_Exist( Data token ) {
  for ( int i = 0 ; i < gvariable_table->size() ; i++ ) {
    if ( gvariable_table->at( i ).at( 1 ).name == token.name ) {
      return true ;
    } // if()

  } // for()

  for ( int i = 0 ; i < gspecial_var->size() ; i++ ) {
    if ( gspecial_var->at( i ).name == token.name ) {
      return true ;
    } // if()

  } // for()

  return false ;
} // Variable_Exist()

bool Function_Exist( Data token ) {
  for ( int i = 0 ; i < gfunction_table->size() ; i++ ) {
    if ( gfunction_table->at( i ).at( 1 ).name == token.name ) {
      return true ;
    } // if()

  } // for()

  for ( int i = 0 ; i < gspecial_function->size() ; i++ ) {
    if ( gspecial_function->at( i ).name == token.name ) {
      return true ;
    } // if()

  } // for()

  return false ;
} // Function_Exist()

bool Function_Var_Exist( Data token ) {
  for ( int i = 0 ; i < gfunction_var->size() ; i++ ) {
    if ( gfunction_var->at( i ).name == token.name ) {
      return true ;
    } // if()

  } // for()

  return false ;
} // Function_Var_Exist()

void ThrowBall( Data token, int type ) {
  cout << "" ;
  throw Ball( token, type ) ;
  cout << "" ;
} // ThrowBall()



void GlobalInit() {
  Data *special = new Data ;
  special->name = "cin" ;
  gspecial_var->push_back( *special ) ;
  special->name = "cout" ;
  gspecial_var->push_back( *special ) ;

  special->name = "Done" ;
  gspecial_function->push_back( *special ) ;
  special->name = "ListAllVariables" ;
  gspecial_function->push_back( *special ) ;
  special->name = "ListAllFunctions" ;
  gspecial_function->push_back( *special ) ;
  special->name = "ListVariable" ;
  gspecial_function->push_back( *special ) ;
  special->name = "ListFunction" ;
  gspecial_function->push_back( *special ) ;

  delete special ;
} // GlobalInit()

void Definition_OR_Statement( vector<Data> *result_data, string &work_type, bool &isDone ) {
  cout << "> " ;
  if ( work_type == "Definition" ) {
    Store_To_Table( result_data ) ;
  } // if()
  else if ( work_type == "Statement" ) {
    if ( IsGlobalFunction( result_data, isDone ) ) {
      if ( isDone ) {
        return ;
      } // if()

    } // if()
    else if ( result_data->at( 0 ).name == "cout" ) {
      PrepareToCal( result_data ) ;
    } // else if()
    else if ( Variable_Exist( result_data->at( 0 ) ) ) {
      GiveNumType( result_data ) ;
    } // else if()
    else if ( result_data->at( 0 ).name == "if" ) {
      SloveIF( result_data ) ;
    } // else if()
    else if ( result_data->at( 0 ).name == "while" ) {
      SloveWHILE( result_data ) ;
    } // else if()

    cout << "Statement executed ...\n" ;
  } // else if()

} // Definition_OR_Statement()

void Store_To_Table( vector<Data> *result_data ) {
  if ( result_data->at( 2 ).name == "(" ) {
    Store_To_Function_Table( result_data ) ;
  } // if()
  else {
    Store_To_Variable_Table( result_data ) ;
  } // else

} // Store_To_Table()

void Store_To_Function_Table( vector<Data> *result_data ) {
  if ( Update_Function_Table( result_data ) ) {
    cout << "New definition of " << result_data->at( 1 ).name << "() entered ...\n" ;
  } // if()
  else {
    Sort( result_data, 1 ) ;
    cout << "Definition of " << result_data->at( 1 ).name << "() entered ...\n" ;
  } // else

} // Store_To_Function_Table()

bool Update_Function_Table( vector<Data> *result_data ) {
  for ( int i = 0 ; i < gfunction_table->size() ; i++ ) {
    if ( gfunction_table->at( i ).at( 1 ).name == result_data->at( 1 ).name ) {
      gfunction_table->at( i ) = *result_data ;
      return true ;
    } // if()

  } // for()

  return false ;
} // Update_Function_Table()

void Store_To_Variable_Table( vector<Data> *result_data ) {
  for ( int i = 0 ; i < result_data->size() ; i++ ) {
    if ( result_data->at( i ).token_type == "Identifier" ) {
      if ( Update_Variable_Table( result_data, i ) ) {
        cout << "New definition of " << result_data->at( i ).name << " entered ...\n" ;
      } // if()
      else {
        vector<Data> *temp = new vector<Data> ;
        temp->push_back( result_data->at( 0 ) ) ;
        for ( int j = i ; result_data->at( j ).name != "," && result_data->at( j ).name != ";" ; j++ ) {
          temp->push_back( result_data->at( j ) ) ;
        } // for()

        temp->at( 1 ).num_type = temp->at( 0 ).token_type ;
        temp->push_back( result_data->at( result_data->size()-1 ) ) ;
        if ( temp->at( 2 ).name == "[" )
          Store_To_Array_Table( temp ) ;
        Sort( temp, 2 ) ;
        delete temp ;
        cout << "Definition of " << result_data->at( i ).name << " entered ...\n" ;
      } // else

    } // if()

  } // for()

} // Store_To_Variable_Table()

bool Update_Variable_Table( vector<Data> *result_data, int pos ) {
  for ( int i = 0 ; i < gvariable_table->size() ; i++ ) {
    if ( gvariable_table->at( i ).at( 1 ).name == result_data->at( pos ).name ) {
      gvariable_table->at( i ).at( 1 ) = result_data->at( pos ) ;
      return true ;
    } // if()

  } // for()

  return false ;
} // Update_Variable_Table()

void Store_To_Array_Table( vector<Data> *sentence ) {
  for ( int i = 0 ; i < sentence->at( 3 ).ivalue ; i++ ) {
    vector<Data> temp ;
    temp.push_back( sentence->at( 1 ) ) ;
    temp.at( 0 ).name = temp.at( 0 ).name + "[" ;
    temp.at( 0 ).name = temp.at( 0 ).name + to_string( i ) ;
    temp.at( 0 ).name = temp.at( 0 ).name + "]" ;
    Sort( &temp, 3 ) ;
    temp.clear() ;
  } // for()

} // Store_To_Array_Table()

void Sort( vector<Data> *sentence, int type ) {
  if ( type == 1 ) {
    for ( int i = 0 ; i < gfunction_table->size() ; i++ ) {
      if ( gfunction_table->at( i ).at( 1 ).name > sentence->at( 1 ).name ) {
        gfunction_table->insert( gfunction_table->begin()+i, *sentence ) ;
        return ;
      } // if()

    } // for

    gfunction_table->push_back( *sentence ) ;  // 假如前面都沒有insert, 就放在最後面
  } // if()
  else if ( type == 2 ) {
    for ( int i = 0 ; i < gvariable_table->size() ; i++ ) {
      if ( gvariable_table->at( i ).at( 1 ).name > sentence->at( 1 ).name ) {
        gvariable_table->insert( gvariable_table->begin()+i, *sentence ) ;
        return ;
      } // if()

    } // for()

    gvariable_table->push_back( *sentence ) ;  // 假如前面都沒有insert, 就放在最後面
  } // else if()
  else if ( type == 3 ) {
    for ( int i = 0 ; i < gvar_array_table->size() ; i++ ) {
      if ( gvar_array_table->at( i ).name > sentence->at( 0 ).name ) {
        gvar_array_table->insert( gvar_array_table->begin()+i, sentence->at( 0 ) ) ;
        return ;
      } // if()

    } // for()

    gvar_array_table->push_back( sentence->at( 0 ) ) ;
  } // else if()

} // Sort()

bool IsGlobalFunction( vector<Data> *result_data, bool &isDone ) {
  if ( result_data->at( 0 ).name == "Done" ) {
    isDone = true ;
    return true ;
  } // if()
  else if ( result_data->at( 0 ).name == "ListAllVariables" ) {
    ListAllVariables() ;
    return true ;
  } // else if()
  else if ( result_data->at( 0 ).name == "ListAllFunctions" ) {
    ListAllFunctions() ;
    return true ;
  } // else if()
  else if ( result_data->at( 0 ).name == "ListVariable" ) {
    result_data->at( 2 ).name.erase( 0, 1 ) ;
    result_data->at( 2 ).name.erase( result_data->at( 2 ).name.size()-1 ) ;
    // 因為"name"是整體, 所以要把前後的"去掉
    ListVariable( result_data ) ;
    return true ;
  } // else if()
  else if ( result_data->at( 0 ).name == "ListFunction" ) {
    result_data->at( 2 ).name.erase( 0, 1 ) ;
    result_data->at( 2 ).name.erase( result_data->at( 2 ).name.size()-1 ) ;
    // 因為"name"是整體, 所以要把前後的"去掉
    ListFunction( result_data ) ;
    return true ;
  } // else if()

  return false ;
} // IsGlobalFunction()

void ListAllVariables() {
  for ( int i = 0 ; i < gvariable_table->size() ; i++ ) {
    cout << gvariable_table->at( i ).at( 1 ).name << "\n" ;
  } // for()

} // ListAllVariables()

void ListAllFunctions() {
  for ( int i = 0 ; i < gfunction_table->size() ; i++ ) {
    cout << gfunction_table->at( i ).at( 1 ).name << "()\n" ;
  } // for()

} // ListAllFunctions()

void ListVariable( vector<Data> *result_data ) {
  for ( int i = 0 ; i < gvariable_table->size() ; i++ ) {
    if ( gvariable_table->at( i ).at( 1 ).name == result_data->at( 2 ).name ) {
      for ( int j = 0 ; j < gvariable_table->at( i ).size() ; j++ ) {
        if ( j == gvariable_table->at( i ).size()-1 ) {
          cout << gvariable_table->at( i ).at( j ).name << "\n" ;
        } // if()
        else if ( j+1 < gvariable_table->at( i ).size() && 
                  ( gvariable_table->at( i ).at( j+1 ).name == "[" || gvariable_table->at( i ).at( j+1 ).name == "," ) ) {
          cout << gvariable_table->at( i ).at( j ).name ;
        } // else if()
        else {
          cout << gvariable_table->at( i ).at( j ).name << " " ;
        } // else

      } // for()

      return ;
    } // if()

  } // for()

} // ListVariable()

void ListFunction( vector<Data> *result_data ) {
  int white_space = 0 ;
  bool first = true ;
  for ( int i = 0 ; i < gfunction_table->size() ; i++ ) {
    if ( gfunction_table->at( i ).at( 1 ).name == result_data->at( 2 ).name ) {
      for ( int j = 0 ; j < gfunction_table->at( i ).size() ; j++ ) {
        if ( gfunction_table->at( i ).at( j ).name == "{" ) {
          white_space = white_space + 2 ;
        } // if()
        else if ( gfunction_table->at( i ).at( j ).name == "}" ) {
          white_space = white_space - 2 ;
        } // else if()

        for ( int k = 0 ; first && k < white_space ; k++ ) {
          cout << " " ;
        } // for

        if ( gfunction_table->at( i ).at( j ).name == "{" || 
             gfunction_table->at( i ).at( j ).name == ";" ||
             gfunction_table->at( i ).at( j ).name == "}" ) {
          cout << gfunction_table->at( i ).at( j ).name << "\n" ;
          first = true ;
        } // if()
        else {
          if ( j < gfunction_table->at( i ).size()-1 &&
               ( gfunction_table->at( i ).at( j ).token_type == "Identifier" && gfunction_table->at( i ).at( j+1 ).name == "(" ) ||
               ( gfunction_table->at( i ).at( j ).name == "(" && gfunction_table->at( i ).at( j+1 ).name == ")" ) ||
               gfunction_table->at( i ).at( j+1 ).name == "," ||
               gfunction_table->at( i ).at( j+1 ).name == "[" || 
               ( ( gfunction_table->at( i ).at( j ).token_type == "PP" || gfunction_table->at( i ).at( j ).token_type == "MM" ) &&
               gfunction_table->at( i ).at( j+1 ).token_type == "Identifier" ) ||
               ( gfunction_table->at( i ).at( j ).token_type == "Identifier" &&
                 ( gfunction_table->at( i ).at( j+1 ).token_type == "PP" || gfunction_table->at( i ).at( j+1 ).token_type == "MM" ) ) ) {
            cout << gfunction_table->at( i ).at( j ).name ;
          } // if()
          else {
            cout << gfunction_table->at( i ).at( j ).name << " " ;
          } // else

          first = false ;
        } // else

      } // for()

      return ;
    } // if()

  } // for

} // ListFunction()

void ParameterInit( vector<Data> *result_data, string &work_type ) {
  gisinfunction = 0 ;
  work_type = "" ;

  delete gfunction_var ;
  gfunction_var = new vector<Data> ;

  delete result_data ;
} // ParameterInit()



void PrepareToCal( vector<Data> *result_data ) {
  result_data->erase( result_data->begin() ) ;  // 移除cout
  result_data->erase( result_data->begin() ) ;  // 移除<<

  vector<Data> *formula = new vector<Data> ;
  int haveParen = 0 ;
  for ( int i = 0 ; i < result_data->size() ; i++ ) {
    if ( haveParen == 0 && ( result_data->at( i ).name == "<<" || result_data->at( i ).name == ";" ) ) {
      if ( formula->at( 0 ).name == "(" && formula->at( formula->size()-1 ).name == ")" ) {
        formula->erase( formula->begin() ) ;
        formula->erase( formula->end()-1 ) ;
      } // if()

      GiveNumType( formula ) ;
      CoutFormula( formula ) ;
      delete formula ;
      formula = new vector<Data> ;
    } // if
    else {
      formula->push_back( result_data->at( i ) ) ;
    } // else

    if ( result_data->at( i ).name == "(" ) {
      haveParen++ ;
    } // if()
    if ( result_data->at( i ).name == ")" ) {
      haveParen-- ;
    } // else if()

  } // for()

} // PrepareToCal()

void GiveNumType( vector<Data> *formula ) {
  for ( int i = 0 ; i < formula->size() ; i++ ) {
    if ( formula->at( i ).token_type == "Identifier" ) {
      if ( !gfunction_var->empty() && IsInFunction( formula->at( i ) ) ) {
        // 區域變數, 因應if與while功能而新增
        for ( int j = 0 ; j < gfunction_var->size() ; j++ ) {
          if ( formula->at( i ).name == gfunction_var->at( j ).name ) {
            formula->at( i ).num_type = gfunction_var->at( j ).num_type ;
          } // if()

        } // for()

      } // if()
      else {
        // 全域變數, 因為array還沒合起來, 所以全都可以在variable_table找到
        for ( int j = 0 ; j < gvariable_table->size() ; j++ ) {
          if ( formula->at( i ).name == gvariable_table->at( j ).at( 1 ).name ) {
            formula->at( i ).num_type = gvariable_table->at( j ).at( 1 ).num_type ;
          } // if()

        } // for()

      } // else

    } // if()

  } // for()

  if ( formula->at( formula->size()-1 ).name == ";" ) {
    formula->erase( formula->end()-1 ) ;
  } // if()

  SloveArray( formula ) ;  // 把分散的array token合成一個token
  SloveOP( formula ) ;  // 解決運算子是幾位元, 並判斷PP或MM是在id的左或右
  InfixToPostfix( formula ) ;
  CalPostfix( formula ) ;
} // GiveNumType()

void SloveArray( vector<Data> *formula ) {
  for ( int i = 0 ; i < formula->size() ; i++ ) {
    if ( formula->at( i ).token_type == "Identifier" ) {
      if ( i < formula->size()-1 && formula->at( i+1 ).name == "[" ) {
        // 把id [ constant ]合在一個token裡, constant可能是一個計算式
        while ( i+1 < formula->size() && formula->at( i+1 ).name != "]" ) {
          if ( formula->at( i+1 ).name != "[" ) {
            formula->at( i ).formula_in_array.push_back( formula->at( i+1 ) ) ;
          } // if()

          formula->at( i ).name = formula->at( i ).name + formula->at( i+1 ).name ;
          formula->erase( formula->begin()+i+1 ) ;
        } // while()

        // 把"]"加到後面
        formula->at( i ).name = formula->at( i ).name + formula->at( i+1 ).name ;
        formula->erase( formula->begin()+i+1 ) ;
        // 確認[]裡的東西是不是計算式或id
        if ( formula->at( i ).formula_in_array.size() > 1 || 
             formula->at( i ).formula_in_array.at( 0 ).token_type == "Identifier" ) {
          formula->at( i ).array_need_cal = true ;
        } // if()

      } // if()

    } // if()

  } // for()

} // SloveArray()

void SloveOP( vector<Data> *formula ) {
  for ( int i = 0 ; i < formula->size() ; i++ ) {
    if ( Sign( formula->at( i ) ) &&
         ( i == 0 || formula->at( i-1 ).name == "(" || IsOperators( formula->at( i-1 ) ) ) ) {
      formula->at( i ).op_bit = 1 ;
    } // if()
    else if ( formula->at( i ).token_type == "PP" || formula->at( i ).token_type == "MM" ) {
      formula->at( i ).op_bit = 1 ;
    } // else if()
    else if ( IsOperators( formula->at( i ) ) ) {
      formula->at( i ).op_bit = 2 ;
    } // else if()
    else if ( formula->at( i ).name == "?" || formula->at( i ).name == ":" ) {
      formula->at( i ).op_bit = 3 ;
    } // else if()

    if ( i > 0 && ( formula->at( i ).token_type == "PP" || formula->at( i ).token_type == "MM" ) &&
         formula->at( i-1 ).token_type == "Identifier" ) {
      formula->at( i ).rl = 1 ;
    } // if()
    else if ( i < formula->size()-1 && ( formula->at( i ).token_type == "PP" || formula->at( i ).token_type == "MM" ) &&
              formula->at( i+1 ).token_type == "Identifier" ) {
      formula->at( i ).rl = 2 ;
    } // else if()

  } // for()

} // SloveOP()

bool IsOperators( Data op ) {
  if ( op.name == "+" || op.name == "-" || op.name == "*" || op.name == "/" || op.name == "%" ||
       op.name == "?" || op.name == ":" || op.name == "=" || op.name == "+=" || op.name == "-=" ||
       op.name == "*=" || op.name == "/=" || op.name == "%=" || op.name == "&&" || op.name == "||" ||
       op.name == "==" || op.name == "!=" || op.name == "<" || op.name == "<=" || op.name == ">" ||
       op.name == ">=" || op.name == "<<" || op.name == ">>" )
    return true ;
  else
    return false ;
} // IsOperators()

void InfixToPostfix( vector<Data> *infix ) {
  vector<Data> s ;
  vector<Data> postfix ;

  for ( int i = 0 ; i < infix->size() ; i++ ) {
    if ( infix->at( i ).token_type == "Identifier" || infix->at( i ).token_type == "Constant"  ) {
      postfix.push_back( infix->at( i ) ) ;
    } // if()
    else if ( infix->at( i ).name == "(" ) {
      s.push_back( infix->at( i ) ) ;
    } // else if()
    else if ( infix->at( i ).name == ")" ) {
      while ( !s.empty() && s.back().name != "(" ) {
        postfix.push_back( s.back() ) ;
        s.pop_back() ;
      } // while()

      s.pop_back() ;  // 上面清到")"就停止，這邊要把")"清掉
    } // else if()
    else if ( infix->at( i ).name == "?" ) {
      SloveThree( infix, s, postfix, i ) ;  // 解決三元運算子的問題
    } // else if()
    else if ( infix->at( i ).name == "," ) {
      while ( !s.empty() && s.back().name != "(" ) {
        postfix.push_back( s.back() ) ;
        s.pop_back() ;
      } // while()

      CalPostfix( &postfix ) ;
      postfix.clear() ;
    } // else if()
    else {
      while ( !s.empty() && ( GetPriority( s.back() ) > GetPriority( infix->at( i ) ) ||
                              ( GetPriority( s.back() ) == GetPriority( infix->at( i ) ) && !IsRight( infix->at( i ) ) ) ) ) {
        postfix.push_back( s.back() ) ;
        s.pop_back() ;
      } // while()

      s.push_back( infix->at( i ) ) ;
    } // else

  } // for()

  while ( !s.empty() ) {
    postfix.push_back( s.back() ) ;
    s.pop_back() ;
  } // while()

  infix->clear() ;
  for ( int i = 0 ; i < postfix.size() ; i++ ) {
    infix->push_back( postfix.at( i ) ) ;
  } // for()

} // InfixToPostfix()

void SloveThree( vector<Data> *infix, vector<Data> &s, vector<Data> &postfix, int &pos ) {
  vector<Data> true_banch, false_banch;
  vector<Data> assign ;  // 在condition前可能會有id =的情況
  int equal = 0 ;  // 用來計算true_banch到false_banch間的?:數量

  /*
  1. 首先檢查condition(postfix)前是否有id =的情況，有的話就把它加進assign
  2. 把condition "?"後到":"的東西加進true_banch
  3. 把true_banch ":"後的東西加進false_banch
  4. 把condition(postfix)丟去運算, 根據結果選擇要true_banch還是false_banch
  p.s. 在false_banch後可能會有",", 要清除","前面的東西並塞入true_banch或false_banch
  */

  if ( !s.empty() && IsAssigns( s.back() ) ) {
    for ( int i = 0 ; i < s.size() ; i++ ) {  // 可能會有複數id =的情況
      assign.push_back( postfix.at( 0 ) ) ;
      postfix.erase( postfix.begin() ) ;
    } // for()

  } // if()

  pos++ ;  // 跳過"?"
  for ( ; pos < infix->size() ; pos++ ) {
    if ( infix->at( pos ).name == ":" && equal == 0 )
      break ;

    if ( infix->at( pos ).name == "?" )
      equal++ ;
    else if ( infix->at( pos ).name == ":" )
      equal-- ;

    true_banch.push_back( infix->at( pos ) ) ;
  } // for()

  pos++ ;  // 跳過":"
  int haveParen = 0 ;  // false_banch可能由()包起來，但()裡面可能會有","
  for ( ; pos < infix->size() ; pos++ ) {
    if ( infix->at( pos ).name == "," && haveParen == 0 )
      break ;

    if ( infix->at( pos ).name == "(" )
      haveParen++ ;
    else if ( infix->at( pos ).name == ")" )
      haveParen-- ;

    false_banch.push_back( infix->at( pos ) ) ;
  } // for()

  if ( pos != infix->size() ) {  // 代表停止在","
    for ( int i = 0 ; i < pos ; i++ ) {
      infix->erase( infix->begin() ) ;
    } // for()

  } // if()
  else if ( pos == infix->size() ) {
    infix->clear() ;
  } // else if()

  CalPostfix( &postfix ) ;
  if ( postfix.at( 0 ).bvalue == true ) {
    while ( !infix->empty() ) {  // 解決後面是","的問題
      true_banch.push_back( infix->at( 0 ) ) ;
      infix->erase( infix->begin() ) ;
    } // while()

    while ( !assign.empty() ) {  // 把上面拆除的id =放回來
      infix->push_back( assign.at( 0 ) ) ;
      assign.pop_back() ;
    } // while()

    for ( int i = 0 ; i < true_banch.size() ; i++ ) {
      infix->push_back( true_banch.at( i ) ) ;
    } // for()

  } // if()
  else if ( postfix.at( 0 ).bvalue == false ) {
    while ( !infix->empty() ) {  // 解決後面是","的問題
      false_banch.push_back( infix->at( 0 ) ) ;
      infix->erase( infix->begin() ) ;
    } // while()

    while ( !assign.empty() ) {  // 把上面拆除的id =放回來
      infix->push_back( assign.at( 0 ) ) ;
      assign.pop_back() ;
    } // while()

    for ( int i = 0 ; i < false_banch.size() ; i++ ) {
      infix->push_back( false_banch.at( i ) ) ;
    } // for()

  } // else if()

  postfix.clear() ;
  pos = -1 ;  // 回到InfixToPostfix()的for時會+1，所以要設成-1
} // SloveThree()

bool IsAssigns( Data token ) {
  if ( token.name == "=" || token.name == "+=" || token.name == "-=" || token.name == "*=" ||
       token.name == "/=" || token.name == "%=" )
    return true ;
  else
    return false ;
} // IsAssigns()

int GetPriority( Data op ) {
  if ( op.name == "," )
    return 1 ;
  else if ( op.name == "=" || op.token_type == "PE" || op.token_type == "ME" || 
            op.token_type == "TE" || op.token_type == "DE" || op.token_type == "RE" )
    return 2 ;
  else if ( op.name == "?" || op.name == ":" )
    return 3 ;
  else if ( op.token_type == "OR" )
    return 4 ;
  else if ( op.token_type == "AND" )
    return 5 ;
  else if ( op.token_type == "EQ" || op.token_type == "NEQ" )
    return 6 ;
  else if ( op.name == "<" || op.name == ">" || op.token_type == "GE" || op.token_type == "LE" )
    return 7 ;
  else if ( op.token_type == "RS" || op.token_type == "LS" )
    return 8 ;
  else if ( ( op.name == "+" || op.name == "-" ) && op.op_bit == 2 )
    return 9 ;
  else if ( op.name == "*" || op.name == "/" || op.name == "%" )
    return 10 ;
  else if ( op.token_type == "PP" || op.token_type == "MM" || Sign( op ) )
    return 11 ;
  return 0 ;
} // GetPriority()

bool IsRight( Data op ) {
  if ( op.name == "=" || op.name == "+=" || op.name == "-=" || op.name == "/=" ||
       op.name == "*=" || op.name == "%=" || op.name == "?" || op.name == ":" ||
       op.name == "++" || op.name == "--" || ( op.op_bit == 1 && Sign( op ) ) )
    return true ;
  else
    return false ;
} // IsRight()

void CalPostfix( vector<Data> *postfix ) {
  vector<Data> ans ;
  vector<Data> vecPP, vecMM ;  // 用於解決PP, MM在id右方的問題
  /*
  for裡先分成兩種情況, 運算元與運算符
  運算元裡分成三種, id_array, id, constant, 去各自對應的table取值
  運算符裡分成兩種, 一元運算子, 二元運算子(三元運算會在前面先被拆成只剩二元)
  */

  for ( int i = 0 ; i < postfix->size() ; i++ ) {
    if ( postfix->at( i ).token_type == "Identifier" || postfix->at( i ).token_type == "Constant" ) {
      if ( postfix->at( i ).token_type == "Identifier" && 
           postfix->at( i ).name.at( postfix->at( i ).name.size()-1 ) == ']' ) {
        SloveFormulaInArray( postfix->at( i ) ) ;
        for ( int j = 0 ; j < gvar_array_table->size() ; j++ ) {
          if ( postfix->at( i ).name == gvar_array_table->at( j ).name ) {
            ans.push_back( gvar_array_table->at( j ) ) ;
          } // if()

        } // for()

      } // if()
      else if ( postfix->at( i ).token_type == "Identifier" ) {
        if ( !gfunction_var->empty() && IsInFunction( postfix->at( i ) ) ) {
          // 區域變數
          for ( int j = 0 ; j < gfunction_var->size() ; j++ ) {
            if ( postfix->at( i ).name == gfunction_var->at( j ).name ) {
              ans.push_back( gfunction_var->at( j ) ) ;
            } // if()

          } // for()

        } // if()
        else {
          // 全域變數
          for ( int j = 0 ; j < gvariable_table->size() ; j++ ) {
            if ( postfix->at( i ).name == gvariable_table->at( j ).at( 1 ).name ) {
              ans.push_back( gvariable_table->at( j ).at( 1 ) ) ;
            } // if()

          } // for()

        } // else

      } // else if()
      else {
        ans.push_back( postfix->at( i ) ) ;
      } // else

    } // if()
    else {
      Data operand1, operand2 ;
      DataClear( operand1 ) ;
      DataClear( operand2 ) ;

      if ( postfix->at( i ).op_bit == 1 ) {
        operand1 = ans.back() ;
        ans.pop_back() ;
        if ( operand1.num_type == "INT" ) {
          if ( postfix->at( i ).token_type == "PP" ) {
            if ( postfix->at( i ).rl == 1 ) {
              vecPP.push_back( operand1 ) ;
            } // if()
            else if ( postfix->at( i ).rl == 2 ) {
              operand1.ivalue++ ;
              UpdateValue( operand1 ) ;
            } // else if()

          } // if()
          else if ( postfix->at( i ).token_type == "MM" ) {
            if ( postfix->at( i ).rl == 1 ) {
              vecMM.push_back( operand1 ) ;
            } // if()
            else if ( postfix->at( i ).rl == 2 ) {
              operand1.ivalue-- ;
              UpdateValue( operand1 ) ;
            } // else if()

          } // else if()
          else if ( postfix->at( i ).name == "-" ) {
            operand1.ivalue = -operand1.ivalue ;
          } // else if()

        } // if()
        else if ( operand1.num_type == "FLOAT" ) {
          if ( postfix->at( i ).token_type == "PP" ) {
            if ( postfix->at( i ).rl == 1 ) {
              vecPP.push_back( operand1 ) ;
            } // if()
            else if ( postfix->at( i ).rl == 2 ) {
              operand1.fvalue++ ;
              UpdateValue( operand1 ) ;
            } // else if()

          } // if()
          else if ( postfix->at( i ).token_type == "MM" ) {
            if ( postfix->at( i ).rl == 1 ) {
              vecMM.push_back( operand1 ) ;
            } // if()
            else if ( postfix->at( i ).rl == 2 ) {
              operand1.fvalue-- ;
              UpdateValue( operand1 ) ;
            } // else if()

          } // else if()
          else if ( postfix->at( i ).name == "-" ) {
            operand1.fvalue = -operand1.fvalue ;
          } // else if()
        } // else if()
        else if ( operand1.num_type == "BOOL" ) {
          if ( operand1.bvalue )
            operand1.bvalue = false ;
          else
            operand1.bvalue = true ;
        } // else if()

      } // if()
      else if ( postfix->at( i ).op_bit == 2 ) {
        operand2 = ans.back() ;
        ans.pop_back() ;
        operand1 = ans.back() ;
        ans.pop_back() ;

        if ( operand1.num_type == "INT" && operand2.num_type == "INT" ) {
          if ( postfix->at( i ).name == "+" )
            operand1.ivalue = operand1.ivalue + operand2.ivalue ;
          if ( postfix->at( i ).name == "-" )
            operand1.ivalue = operand1.ivalue - operand2.ivalue ;
          if ( postfix->at( i ).name == "*" )
            operand1.ivalue = operand1.ivalue * operand2.ivalue ;
          if ( postfix->at( i ).name == "/" )
            operand1.ivalue = operand1.ivalue / operand2.ivalue ;
          if ( postfix->at( i ).name == "%" )
            operand1.ivalue = operand1.ivalue % operand2.ivalue ;
          if ( postfix->at( i ).name == "<<" )
            operand1.ivalue = operand1.ivalue << operand2.ivalue ;
          if ( postfix->at( i ).name == ">>" )
            operand1.ivalue = operand1.ivalue >> operand2.ivalue ;
          if ( postfix->at( i ).name == "=" || postfix->at( i ).name == "+=" || postfix->at( i ).name == "-=" ||
               postfix->at( i ).name == "*=" || postfix->at( i ).name == "/=" || postfix->at( i ).name == "%=" ) {
            if ( postfix->at( i ).name == "=" )
              operand1.ivalue = operand2.ivalue ;
            else if ( postfix->at( i ).name == "+=" )
              operand1.ivalue = operand1.ivalue + operand2.ivalue ;
            else if ( postfix->at( i ).name == "-=" )
              operand1.ivalue = operand1.ivalue - operand2.ivalue ;
            else if ( postfix->at( i ).name == "*=" )
              operand1.ivalue = operand1.ivalue * operand2.ivalue ;
            else if ( postfix->at( i ).name == "/=" )
              operand1.ivalue = operand1.ivalue / operand2.ivalue ;
            else if ( postfix->at( i ).name == "%=" )
              operand1.ivalue = operand1.ivalue % operand2.ivalue ;
            ServicePPMM( vecPP, 1 ) ;
            ServicePPMM( vecMM, 2 ) ;
            UpdateValue( operand1 ) ;
          } // if()
          if ( postfix->at( i ).name == "<" || postfix->at( i ).name == "<=" ||
               postfix->at( i ).name == ">" || postfix->at( i ).name == ">=" ||
               postfix->at( i ).name == "==" || postfix->at( i ).name == "!=" ) {
            if ( postfix->at( i ).name == "<" ) {
              if ( operand1.ivalue < operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "<=" ) {
              if ( operand1.ivalue <= operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">" ) {
              if ( operand1.ivalue > operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">=" ) {
              if ( operand1.ivalue >= operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "==" ) {
              if ( operand1.ivalue == operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "!=" ) {
              if ( operand1.ivalue != operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()

            operand1.num_type = "BOOL" ;
            operand1.ivalue = 0 ;
          } // if()

        } // if()
        if ( operand1.num_type == "INT" && operand2.num_type == "FLOAT" ) {
          if ( postfix->at( i ).name == "+" )
            operand1.ivalue = operand1.ivalue + operand2.fvalue ;
          if ( postfix->at( i ).name == "-" )
            operand1.ivalue = operand1.ivalue - operand2.fvalue ;
          if ( postfix->at( i ).name == "*" )
            operand1.ivalue = operand1.ivalue * operand2.fvalue ;
          if ( postfix->at( i ).name == "/" )
            operand1.ivalue = operand1.ivalue / operand2.fvalue ;
          if ( postfix->at( i ).name == "=" || postfix->at( i ).name == "+=" || postfix->at( i ).name == "-=" ||
               postfix->at( i ).name == "*=" || postfix->at( i ).name == "/=" ) {
            if ( postfix->at( i ).name == "=" )
              operand1.ivalue = operand2.fvalue ;
            else if ( postfix->at( i ).name == "+=" )
              operand1.ivalue = operand1.ivalue + operand2.fvalue ;
            else if ( postfix->at( i ).name == "-=" )
              operand1.ivalue = operand1.ivalue - operand2.fvalue ;
            else if ( postfix->at( i ).name == "*=" )
              operand1.ivalue = operand1.ivalue * operand2.fvalue ;
            else if ( postfix->at( i ).name == "/=" )
              operand1.ivalue = operand1.ivalue / operand2.fvalue ;
            ServicePPMM( vecPP, 1 ) ;
            ServicePPMM( vecMM, 2 ) ;
            UpdateValue( operand1 ) ;
          } // if()
          if ( postfix->at( i ).name == "<" || postfix->at( i ).name == "<=" ||
               postfix->at( i ).name == ">" || postfix->at( i ).name == ">=" ||
               postfix->at( i ).name == "==" || postfix->at( i ).name == "!=" ) {
            if ( postfix->at( i ).name == "<" ) {
              if ( operand1.ivalue < operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "<=" ) {
              if ( operand1.ivalue <= operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">" ) {
              if ( operand1.ivalue > operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">=" ) {
              if ( operand1.ivalue >= operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "==" ) {
              if ( operand1.ivalue == operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "!=" ) {
              if ( operand1.ivalue != operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()

            operand1.num_type = "BOOL" ;
            operand1.ivalue = 0 ;
          } // if()

        } // if()
        if ( operand1.num_type == "FLOAT" && operand2.num_type == "INT" ) {
          if ( postfix->at( i ).name == "+" )
            operand1.fvalue = operand1.fvalue + operand2.ivalue ;
          if ( postfix->at( i ).name == "-" )
            operand1.fvalue = operand1.fvalue - operand2.ivalue ;
          if ( postfix->at( i ).name == "*" )
            operand1.fvalue = operand1.fvalue * operand2.ivalue ;
          if ( postfix->at( i ).name == "/" )
            operand1.fvalue = operand1.fvalue / operand2.ivalue ;
          if ( postfix->at( i ).name == "=" || postfix->at( i ).name == "+=" || postfix->at( i ).name == "-=" ||
               postfix->at( i ).name == "*=" || postfix->at( i ).name == "/=" ) {
            if ( postfix->at( i ).name == "=" )
              operand1.fvalue = operand2.ivalue ;
            else if ( postfix->at( i ).name == "+=" )
              operand1.fvalue = operand1.fvalue + operand2.ivalue ;
            else if ( postfix->at( i ).name == "-=" )
              operand1.fvalue = operand1.fvalue - operand2.ivalue ;
            else if ( postfix->at( i ).name == "*=" )
              operand1.fvalue = operand1.fvalue * operand2.ivalue ;
            else if ( postfix->at( i ).name == "/=" )
              operand1.fvalue = operand1.fvalue / operand2.ivalue ;
            ServicePPMM( vecPP, 1 ) ;
            ServicePPMM( vecMM, 2 ) ;
            UpdateValue( operand1 ) ;
          } // if()
          if ( postfix->at( i ).name == "<" || postfix->at( i ).name == "<=" ||
               postfix->at( i ).name == ">" || postfix->at( i ).name == ">=" ||
               postfix->at( i ).name == "==" || postfix->at( i ).name == "!=" ) {
            if ( postfix->at( i ).name == "<" ) {
              if ( operand1.fvalue < operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "<=" ) {
              if ( operand1.fvalue <= operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">" ) {
              if ( operand1.fvalue > operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">=" ) {
              if ( operand1.fvalue >= operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "==" ) {
              if ( operand1.fvalue == operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "!=" ) {
              if ( operand1.fvalue != operand2.ivalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()

            operand1.num_type = "BOOL" ;
            operand1.ivalue = 0 ;
          } // if()

        } // if()
        if ( operand1.num_type == "FLOAT" && operand2.num_type == "FLOAT" ) {
          if ( postfix->at( i ).name == "+" )
            operand1.fvalue = operand1.fvalue + operand2.fvalue ;
          if ( postfix->at( i ).name == "-" )
            operand1.fvalue = operand1.fvalue - operand2.fvalue ;
          if ( postfix->at( i ).name == "*" )
            operand1.fvalue = operand1.fvalue * operand2.fvalue ;
          if ( postfix->at( i ).name == "/" )
            operand1.fvalue = operand1.fvalue / operand2.fvalue ;
          if ( postfix->at( i ).name == "=" || postfix->at( i ).name == "+=" || postfix->at( i ).name == "-=" ||
               postfix->at( i ).name == "*=" || postfix->at( i ).name == "/=" ) {
            if ( postfix->at( i ).name == "=" )
              operand1.fvalue = operand2.fvalue ;
            else if ( postfix->at( i ).name == "+=" )
              operand1.fvalue = operand1.fvalue + operand2.fvalue ;
            else if ( postfix->at( i ).name == "-=" )
              operand1.fvalue = operand1.fvalue - operand2.fvalue ;
            else if ( postfix->at( i ).name == "*=" )
              operand1.fvalue = operand1.fvalue * operand2.fvalue ;
            else if ( postfix->at( i ).name == "/=" )
              operand1.fvalue = operand1.fvalue / operand2.fvalue ;
            ServicePPMM( vecPP, 1 ) ;
            ServicePPMM( vecMM, 2 ) ;
            UpdateValue( operand1 ) ;
          } // if()
          if ( postfix->at( i ).name == "<" || postfix->at( i ).name == "<=" ||
               postfix->at( i ).name == ">" || postfix->at( i ).name == ">=" ||
               postfix->at( i ).name == "==" || postfix->at( i ).name == "!=" ) {
            if ( postfix->at( i ).name == "<" ) {
              if ( operand1.fvalue < operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "<=" ) {
              if ( operand1.fvalue <= operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">" ) {
              if ( operand1.fvalue > operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == ">=" ) {
              if ( operand1.fvalue >= operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "==" ) {
              if ( operand1.fvalue == operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()
            if ( postfix->at( i ).name == "!=" ) {
              if ( operand1.fvalue != operand2.fvalue )
                operand1.bvalue = true ;
              else
                operand1.bvalue = false ;
            } // if()

            operand1.num_type = "BOOL" ;
            operand1.ivalue = 0 ;
          } // if()

        } // if()
        if ( operand1.num_type == "BOOL" && operand2.num_type == "BOOL" ) {
          if ( postfix->at( i ).name == "=" ) {
            operand1.bvalue = operand2.bvalue ;
            UpdateValue( operand1 ) ;
          } // if()
          if ( postfix->at( i ).name == "==" ) {
            if ( operand1.bvalue == operand2.bvalue )
              operand1.bvalue = true ;
            else
              operand1.bvalue = false ;
          } // if()
          if ( postfix->at( i ).name == "!=" ) {
            if ( operand1.bvalue != operand2.bvalue )
              operand1.bvalue = true ;
            else
              operand1.bvalue = false ;
          } // if()
          if ( postfix->at( i ).name == "&&" ) {
            operand1.bvalue = operand1.bvalue && operand2.bvalue ;
          } // if()
          if ( postfix->at( i ).name == "||" ) {
            operand1.bvalue = operand1.bvalue || operand2.bvalue ;
          } // if()

        } // if()
        if ( ( operand1.num_type == "CHAR" && ( operand2.num_type == "CHAR" || operand2.num_type == "STRING" ) ) ||
             ( operand1.num_type == "STRING" && ( operand2.num_type == "CHAR" || operand2.num_type == "STRING" ) ) ) {
          if ( postfix->at( i ).name == "=" || postfix->at( i ).name == "+=" ) {
            if ( postfix->at( i ).name == "=" )
              operand1.svalue = operand2.svalue ;
            else if ( postfix->at( i ).name == "+=" )
              operand1.svalue = operand1.svalue + operand2.svalue ;
            UpdateValue( operand1 ) ;
          } // if()
          else {
            operand1.svalue = operand1.svalue + operand2.svalue ;
          } // else

        } // if()

      } // else if()

      ans.push_back( operand1 ) ;
    } // else

  } // for()

  ServicePPMM( vecPP, 1 ) ;
  ServicePPMM( vecMM, 2 ) ;
  postfix->clear() ;
  postfix->push_back( ans.back() ) ;
} // CalPostfix()

void SloveFormulaInArray( Data &operand ) {
  if ( operand.array_need_cal ) {
    vector<Data> formula ;
    Data temp ;
    DataClear( temp ) ;
    for ( int i = 0 ; i < operand.formula_in_array.size() ; i++ ) {
      formula.push_back( operand.formula_in_array.at( i ) ) ;
    } // for()

    GiveNumType( &formula ) ;
    while ( operand.name.at( 0 ) != '[' ) {
      temp.name = temp.name + operand.name.at( 0 ) ;
      operand.name.erase( operand.name.begin() ) ;
    } // while()

    temp.name = temp.name + operand.name.at( 0 ) ;  // 把"["加進去
    temp.name = temp.name + to_string( formula.at( 0 ).ivalue ) ;
    temp.name = temp.name + "]" ;
    operand.name = temp.name ;
  } // if()

} // SloveFormulaInArray()

void UpdateValue( Data operand ) {
  if ( operand.name.at( operand.name.size()-1 ) == ']' ) {
    for ( int i = 0 ; i < gvar_array_table->size() ; i++ ) {
      if ( operand.name == gvar_array_table->at( i ).name ) {
        gvar_array_table->at( i ) = operand ;
      } // if()

    } // for()

  } // if()
  else {
    if ( !gfunction_var->empty() && IsInFunction( operand ) ) {
      // 區域變數
      for( int i = 0 ; i < gfunction_var->size() ; i++ ) {
        if ( operand.name == gfunction_var->at( i ).name ) {
          gfunction_var->at( i ) = operand ;
        } // if()

      } // for()

    } // if()
    else {
      // 全域變數
      for ( int i = 0 ; i < gvariable_table->size() ; i++ ) {
        if ( operand.name == gvariable_table->at( i ).at( 1 ).name ) {
          gvariable_table->at( i ).at( 1 ) = operand ;
        } // if()

      } // for()

    } // else


  } // else

} // UpdateValue()

void ServicePPMM( vector<Data> &vec_of_PPMM, int type ) {
  while ( !vec_of_PPMM.empty() ) {
    if ( vec_of_PPMM.at( 0 ).num_type == "INT" ) {
      if ( type == 1 )
        vec_of_PPMM.at( 0 ).ivalue++ ;
      else if ( type == 2 )
        vec_of_PPMM.at( 0 ).ivalue-- ;
    } // if()
    else if ( vec_of_PPMM.at( 0 ).num_type == "FLOAT" ) {
      if ( type == 1 )
        vec_of_PPMM.at( 0 ).fvalue++ ;
      else if ( type == 2 )
        vec_of_PPMM.at( 0 ).fvalue-- ;
    } // else if()

    UpdateValue( vec_of_PPMM.at( 0 ) ) ;
    vec_of_PPMM.erase( vec_of_PPMM.begin() ) ;
  } // while()

} // ServicePPMM()

void CoutFormula( vector<Data> *formula ) {
  if ( formula->at( 0 ).num_type == "INT" ) {
    cout << formula->at( 0 ).ivalue ;
  } // if()
  else if ( formula->at( 0 ).num_type == "FLOAT" ) {
    cout << formula->at( 0 ).fvalue ;
  } // else if()
  else if ( formula->at( 0 ).num_type == "BOOL" ) {
    if ( formula->at( 0 ).bvalue )
      cout << "true" ;
    else
      cout << "false" ;
  } // else if()
  else if ( formula->at( 0 ).num_type == "CHAR" ) {
    cout << formula->at( 0 ).svalue ;
  } // else if()
  else if ( formula->at( 0 ).num_type == "STRING" ) {
    for ( int i = 0 ; i < formula->at( 0 ).svalue.size() ; i++ ) {
      if ( formula->at( 0 ).svalue.at( i ) == '\\' && 
           ( i+1 < formula->at( 0 ).svalue.size() && formula->at( 0 ).svalue.at( i+1 ) == 'n' ) ) {
        i++ ;
        cout << "\n" ;
      } // if()
      else {
        cout << formula->at( 0 ).svalue.at( i ) ;
      } // else

    } // for(

  } // else if()

} // CoutFormula()

void SloveIF( vector<Data> *result_data ) {
  int pos = 1 ;
  vector<Data> *formula = new vector<Data> ;
  for ( ; result_data->at( pos ).name != "{" ; pos++ ) {
    formula->push_back( result_data->at( pos ) ) ;
  } // for()

  pos++ ; // 跳過"{"
  GiveNumType( formula ) ;
  if ( formula->at( 0 ).bvalue ) {
    delete formula ;
    formula = new vector<Data> ;
    for ( ; result_data->at( pos ).name != "}" ; pos++ ) {
      formula->push_back( result_data->at( pos ) ) ;  // 計算式需要包含";", 所以可以直接push_back
      if ( result_data->at( pos ).name == ";" ) {
        if ( Type_Specifier( formula->at( 0 ) ) ) {
          RenewFunctionVar( formula ) ;
        } // if()
        else if ( formula->at( 0 ).name == "cout" ) {
          PrepareToCal( formula ) ;
        } // else if()
        else {
          GiveNumType( formula ) ;
        } // else


        delete formula ;
        formula = new vector<Data> ;
      } // if()

    } // for()


  } // if()
  else {
    delete formula ;
    formula = new vector<Data> ;
    for ( ; result_data->at( pos ).name != "else" ; pos++ ) ;  // 不做事 單純讓pos指到else
    pos = pos + 2 ;  // 跳過"else"與"{"
    for ( ; result_data->at( pos ).name != "}" ; pos++ ) {
      formula->push_back( result_data->at( pos ) ) ;  // 計算式需要包含";", 所以可以直接push_back
      if ( result_data->at( pos ).name == ";" ) {
        if ( Type_Specifier( formula->at( 0 ) ) ) {
          RenewFunctionVar( formula ) ;
        } // if()
        else if ( formula->at( 0 ).name == "cout" ) {
          PrepareToCal( formula ) ;
        } // else if()
        else {
          GiveNumType( formula ) ;
        } // else


        delete formula ;
        formula = new vector<Data> ;
      } // if()

    } // for()


  } // else


} // SloveIF()

void RenewFunctionVar( vector<Data> *formula ) {
  delete gfunction_var ;
  gfunction_var = new vector<Data> ;
  for ( int i = 0 ; i < formula->size() ; i++ ) {
    if ( formula->at( i ).token_type == "Identifier" ) {
      formula->at( i ).num_type = formula->at( 0 ).token_type ;
      gfunction_var->push_back( formula->at( i ) ) ;
    } // if()

  } // for()

} // RenewFunctionVar()

void SloveWHILE( vector<Data> *result_data ) {
  int pos = 1 ;
  vector<Data> *formula = new vector<Data> ;
  vector<Data> *condition = new vector<Data> ;  // 用來存放while的條件式
  for ( ; result_data->at( pos ).name != "{" ; pos++ ) {
    formula->push_back( result_data->at( pos ) ) ;
    condition->push_back( result_data->at( pos ) ) ;
  } // for()

  pos++ ;  // 跳過"{"
  GiveNumType( formula ) ;
  while ( formula->at( 0 ).bvalue ) {
    delete formula ;
    formula = new vector<Data> ;
    for ( int i = pos ; result_data->at( i ).name != "}" ; i++ ) {
      formula->push_back( result_data->at( i ) ) ;  // 計算式需要包含";", 所以可以直接push_back
      if ( result_data->at( i ).name == ";" ) {
        if ( Type_Specifier( formula->at( 0 ) ) ) {
          RenewFunctionVar( formula ) ;
        } // if()
        else if ( formula->at( 0 ).name == "cout" ) {
          PrepareToCal( formula ) ;
        } // else if()
        else {
          GiveNumType( formula ) ;
        } // else


        delete formula ;
        formula = new vector<Data> ;
      } // if()

    } // for()

    for ( int i = 0 ; i < condition->size() ; i++ ) {
      formula->push_back( condition->at( i ) ) ;
    } // for()

    GiveNumType( formula ) ;
  } // while()

} // SloveWHILE()

bool IsInFunction( Data operand ) {
  for ( int i = 0 ; i < gfunction_var->size() ; i++ ) {
    if ( operand.name == gfunction_var->at( i ).name ) {
      return true ;
    } // if()

  } // for()

  return false ;
} // IsInFunction()



void PeekToken( vector<Data> *token_buffer, Data &peek_token ) {
  while ( token_buffer->empty() && !gisEOF ) {
    Transform_to_Token( token_buffer ) ;
  } // while()

  DataClear( peek_token ) ;
  peek_token = token_buffer->at( 0 ) ;
  if ( peek_token.isError ) {
    ThrowBall( peek_token, 1 ) ;
  } // if()

} // PeekToken()

void Get_And_PushBack( vector<Data> *token_buffer, vector<Data> *result_data ) {
  while ( token_buffer->empty() && !gisEOF ) {
    Transform_to_Token( token_buffer ) ;
  } // while()

  result_data->push_back( token_buffer->at( 0 ) ) ;
  token_buffer->erase( token_buffer->begin() ) ;
} // Get_And_PushBack()

void Transform_to_Token( vector<Data> *token_buffer ) {
  int pos = 0, many_dot = 0 ;
  char currentC = '\0' ;
  bool isChar = false, isString = false ;
  Data token ;
  goriginal_line = "" ;
  DataClear( token ) ;
  GetLine() ;

  for ( ; pos < goriginal_line.size() ; pos++ ) {
    currentC = goriginal_line.at( pos ) ;

    if ( !isChar && !isString ) {
      if ( currentC == '/' && pos+1 < goriginal_line.size() && goriginal_line.at( pos+1 ) == '/' ) {
        pos = goriginal_line.size() ;
      } // if()
      else if ( IsDelimiter( currentC ) ) {
        if ( !token.name.empty() ) {
          SetTokenType( token_buffer, token ) ;
          if ( many_dot > 0 ) {
            many_dot-- ;
          } // if()
          
        } // if()

        token.name = currentC ;
        if ( IsDouble_Delimiter( currentC, pos ) ) {
          pos++ ;
          token.name = token.name + goriginal_line.at( pos ) ;
        } // if()

        SetTokenType( token_buffer, token ) ;
      } // else if()
      else if ( currentC == ' ' || currentC == '\'' || currentC == '"' ) {
        if ( !token.name.empty() ) {
          SetTokenType( token_buffer, token ) ;
          if ( many_dot > 0 ) {
            many_dot-- ;
          } // if()
          
        } // if()

        if ( currentC == '\'' ) {
          token.name = currentC ;
          isChar = true ;
        } // if()
        else if ( currentC == '"' ) {
          token.name = currentC ;
          isString = true ;
        } // else if()

      } // else if()
      else {
        if ( currentC == '.' ) {
          many_dot++ ;
        } // if()

        if ( many_dot > 1 ) {
          SetTokenType( token_buffer, token ) ;
          many_dot-- ;
        } // if()

        token.name = token.name + currentC ;
      } // else

    } // if()
    else if ( isChar ) {
      token.name = token.name + currentC ;
      if ( currentC == '\'' ) {
        SetTokenType( token_buffer, token ) ;
        isChar = false ;
      } // if()

    } // else if()
    else if ( isString ) {
      token.name = token.name + currentC ;
      if ( currentC == '"' ) {
        SetTokenType( token_buffer, token ) ;
        isString = false ;
      } // if()

    } // else if()
    
  } // for()

  if ( !token.name.empty() ) {
    SetTokenType( token_buffer, token ) ;
  } // if()
  
} // Transform_to_Token()

void GetLine() {
  char temp = getchar() ;
  while ( temp != EOF && temp != '\n' ) {
    goriginal_line = goriginal_line + temp ;
    temp = getchar() ;
  } // while()

  if ( temp == EOF ) {
    gisEOF = true ;
  } // if()

  gline++ ;
} // GetLine()

bool IsDelimiter( char currentC ) {
  if ( currentC == '(' || currentC == ')' || currentC == '[' || currentC == ']' ||
       currentC == '{' || currentC == '}' || currentC == '+' || currentC == '-' ||
       currentC == '*' || currentC == '/' || currentC == '%' || currentC == '^' ||
       currentC == '>' || currentC == '<' || currentC == '&' || currentC == '|' ||
       currentC == '=' || currentC == '!' || currentC == ';' || currentC == ',' ||
       currentC == '?' || currentC == ':' ) {
    return true ;
  } // if

  return false ;
} // IsDelimiter()

bool IsDouble_Delimiter( char currentC, int pos ) {
  if ( pos+1 < goriginal_line.size() ) {
    if ( goriginal_line.at( pos+1 ) == '=' ) {
      if ( currentC == '>' || currentC == '<' || currentC == '=' || currentC == '!' ||
           currentC == '+' || currentC == '-' || currentC == '*' || currentC == '/' ||
           currentC == '%' ) {
        return true ;
      } // if()

    } // if()
    else if ( goriginal_line.at( pos+1 ) == '&' ) {
      if ( currentC == '&' ) {
        return true ;
      } // if()

    } // else if()
    else if ( goriginal_line.at( pos+1 ) == '|' ) {
      if ( currentC == '|' ) {
        return true ;
      } // if()

    } // else if()
    else if ( goriginal_line.at( pos+1 ) == '+' ) {
      if ( currentC == '+' ) {
        return true ;
      } // if()

    } // else if()
    else if ( goriginal_line.at( pos+1 ) == '-' ) {
      if ( currentC == '-' ) {
        return true ;
      } // if()

    } // else if()
    else if ( goriginal_line.at( pos+1 ) == '>' ) {
      if ( currentC == '>' ) {
        return true ;
      } // if()

    } // else if()
    else if ( goriginal_line.at( pos+1 ) == '<' ) {
      if ( currentC == '<' ) {
        return true ;
      } // if()

    } // else if()

  } // if()

  return false ;
} // IsDouble_Delimiter()

void SetTokenType( vector<Data> *token_buffer, Data &token ) {
  if ( token.name == "int" ) {
    token.token_type = "INT" ;
  } // if()
  else if ( token.name == "float" ) {
    token.token_type = "FLOAT" ;
  } // else if()
  else if ( token.name == "char" ) {
    token.token_type = "CHAR" ;
  } // else if()
  else if ( token.name == "bool" ) {
    token.token_type = "BOOL" ;
  } // else if()
  else if ( token.name == "string" ) {
    token.token_type = "STRING" ;
  } // else if()
  else if ( token.name == "void" ) {
    token.token_type = "VOID" ;
  } // else if()
  else if ( token.name == "if" ) {
    token.token_type = "IF" ;
  } // else if()
  else if ( token.name == "else" ) {
    token.token_type = "ELSE" ;
  } // else if()
  else if ( token.name == "while" ) {
    token.token_type = "WHILE" ;
  } // else if()
  else if ( token.name == "do" ) {
    token.token_type = "DO" ;
  } // else if()
  else if ( token.name == "return" ) {
    token.token_type = "RETURN" ;
  } // else if()
  else if ( token.name == ">=" ) {
    token.token_type = "GE" ;
  } // else if()
  else if ( token.name == "<=" ) {
    token.token_type = "LE" ;
  } // else if()
  else if ( token.name == "==" ) {
    token.token_type = "EQ" ;
  } // else if()
  else if ( token.name == "!=" ) {
    token.token_type = "NEQ" ;
  } // else if()
  else if ( token.name == "&&" ) {
    token.token_type = "AND" ;
  } // else if()
  else if ( token.name == "||" ) {
    token.token_type = "OR" ;
  } // else if()
  else if ( token.name == "+=" ) {
    token.token_type = "PE" ;
  } // else if()
  else if ( token.name == "-=" ) {
    token.token_type = "ME" ;
  } // else if()
  else if ( token.name == "*=" ) {
    token.token_type = "TE" ;
  } // else if()
  else if ( token.name == "/=" ) {
    token.token_type = "DE" ;
  } // else if()
  else if ( token.name == "%=" ) {
    token.token_type = "RE" ;
  } // else if()
  else if ( token.name == "++" ) {
    token.token_type = "PP" ;
  } // else if()
  else if ( token.name == "--" ) {
    token.token_type = "MM" ;
  } // else if()
  else if ( token.name == ">>" ) {
    token.token_type = "RS" ;
  } // else if()
  else if ( token.name == "<<" ) {
    token.token_type = "LS" ;
  } // else if()
  else if ( token.name == "(" || token.name == ")" || token.name == "[" || token.name == "]" ||
            token.name == "{" || token.name == "}" || token.name == "+" || token.name == "-" ||
            token.name == "*" || token.name == "/" || token.name == "%" || token.name == "^" ||
            token.name == ">" || token.name == "<" || token.name == "&" || token.name == "|" ||
            token.name == "=" || token.name == "!" || token.name == ";" || token.name == "," ||
            token.name == "?" || token.name == ":"  ) {
    ;
  } // else if()
  else if ( IsConstant( token ) ) {
    token.token_type = "Constant" ;
  } // else if()
  else if ( IsIdentifier( token ) ) {
    token.token_type = "Identifier" ;
  } // else if()
  else {
    token.isError = true ;
  } // else

  token_buffer->push_back( token ) ;
  DataClear( token ) ;
} // SetTokenType()

bool IsConstant( Data &token ) {
  if ( token.name.at( 0 ) == '\'' && token.name.size() == 3 && token.name.at( 2 ) == '\'' ) {
    token.num_type = "CHAR" ;
    token.svalue = token.name.at( 1 ) ;
    return true ;
  } // if()
  else if ( token.name.at( 0 ) == '\"' && token.name.at( token.name.size()-1 ) == '\"' ) {
    token.num_type = "STRING" ;
    for ( int i = 1 ; i < token.name.size()-1 ; i++ ) {
      token.svalue = token.svalue + token.name.at( i ) ;
    } // for()

    return true ;
  } // else if()
  else if ( token.name == "true" || token.name == "false" ) {
    token.num_type = "BOOL" ;
    if ( token.name == "true" )
      token.bvalue = true ;
    else
      token.bvalue = false ;
    return true ;
  } // else if()
  else if ( IsNumber( token ) ) {
    return true ;
  } // else if()

  if ( token.name.at( 0 ) == '\'' ) {
    token.name = token.name.at( 0 ) ;
  } // if()
  
  return false ;
} // IsConstant()

bool IsNumber( Data &token ) {
  int dot_pos = 0, inum = 0 ;
  float fnum1 = 0.0, fnum2 = 0.0 ;
  bool isFloat = false, notzero = false ;
  string temp = "" ;

  for ( int i = 0 ; i < token.name.size() ; i++ ) {
    if ( token.name.at( i ) == '.' ) {
      isFloat = true ;
      dot_pos = i ;
    } // if()
    else if ( token.name.at( i ) < '0' || token.name.at( i ) > '9' ) {
      return false ;
    } // else if()

  } // for

  if ( isFloat ) {
    if ( token.name.at( 0 ) == '.' ) {
      temp = "0" ;
      temp = temp + token.name ;
      token.name = temp ;
      dot_pos++ ;
      notzero = true ;
    } // if()

    for ( int i = 0 ; token.name.at( i ) != '.' ; i++ ) {
      fnum1 = fnum1 * 10 ;
      fnum1 = fnum1 + ( token.name.at( i ) - '0' ) ;
    } // for()

    for ( int i = token.name.size() - 1 ; token.name.at( i ) != '.' ; i-- ) {
      fnum2 = fnum2 + ( token.name.at( i ) - '0' ) ;
      fnum2 = fnum2 / 10 ;
    } // for()

    fnum1 = fnum1 + fnum2 ;
    token.fvalue = fnum1 ;
    token.num_type = "FLOAT" ;
    if ( notzero )
      token.name.erase( token.name.begin() ) ;
  } // if ()
  else {
    for ( int i = 0 ; i < token.name.size() ; i++ ) {
      inum = inum * 10 ;
      inum = inum + ( token.name.at( i ) - '0' ) ;
    } // for()

    token.ivalue = inum ;
    token.num_type = "INT" ;
  } // else

  return true ;
} // IsNumber()

bool IsIdentifier( Data &token ) {
  for ( int i = 0 ; i < token.name.size() ; i++ ) {
    if ( i == 0 ) {
      if ( token.name.at( i ) >= 'a' && token.name.at( i ) <= 'z' )
        ;
      else if ( token.name.at( i ) >= 'A' && token.name.at( i ) <= 'Z' )
        ;
      else if ( token.name.at( i ) == '_' )
        ;
      else {
        token.name = token.name.at( i ) ;
        return false ;
      } // else

    } // if()
    else {
      if ( token.name.at( i ) >= 'a' && token.name.at( i ) <= 'z' )
        ;
      else if ( token.name.at( i ) >= 'A' && token.name.at( i ) <= 'Z' )
        ;
      else if ( token.name.at( i ) >= '0' && token.name.at( i ) <= '9' )
        ;
      else if ( token.name.at( i ) == '_' )
        ;
      else {
        token.name = token.name.at( i ) ;
        return false ;
      } // else

    } // else

  } // for()

  return true ;
} // IsIdentifier()

void DataClear( Data &data ) {
  data.name = "" ;
  data.token_type = "" ;  // 判別這個token是identifier, constant或其他
  data.op_bit = 0 ;  // 判別這個operator是幾位元運算子
  data.rl = 0 ;  // 判別這個PP或MM是前置還是後置

  data.formula_in_array.clear() ;  // 用來存放[]裡的計算式或id
  data.array_need_cal = false ;

  data.num_type = "" ;  // 判別這個identifier是int, float, char, string, bool
  data.ivalue = 0 ;
  data.fvalue = 0 ;
  data.bvalue = false ;
  data.svalue = "" ;

  data.isError = false ;
  data.error_mess = "" ;
} // DataClear()
