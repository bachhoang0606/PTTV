/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

FILE* f;

void skipBlank() {
    while (currentChar != -1 && charCodes[currentChar] == CHAR_SPACE)
    readChar();    
}

void skipComment() {
    while (1) {
        // Read next character
        readChar();

        if (currentChar == -1) { // End of File
            error(ERR_ENDOFCOMMENT, lineNo, colNo, f); // Comment end with EOF
        } else if (charCodes[currentChar] == CHAR_TIMES) { // Next is asterick character
            readChar(); // Get next character

            if (currentChar == -1) { // End of File
                error(ERR_ENDOFCOMMENT, lineNo, colNo, f); // Comment end with EOF
            } else if (charCodes[currentChar] == CHAR_RPAR) { // Next is right parenthesis
                // End of comment
                readChar();
                return;
            }
        }
    }
}

//Skip commnet after double slash '//'
void skipCommentLine(){
    
    while (currentChar != '\n' && currentChar != EOF) {
        
        // Get next character in file
        readChar();
        
    }
}

// Convert normal string to flower string
void overrrideIdent(char* string){

    int count = 0;
    while (string[count] != '\0'){
        
        string[count] = toupper(string[count]);
        count++;
    }
}

Token* readIdentKeyword(void) {
    int count = 0;
    Token* token = makeToken(TK_IDENT, lineNo, colNo);
    while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT) {
      
        // get 15 char without error
        if(count < MAX_IDENT_LEN) {

            // Add current character to identifier
            token->string[count] = currentChar;
        }
        
        // Increase identifier length
        count++;

        // Get next character
        readChar();
    }
    // End string
    token->string[count] = '\0';

    // Limit identifier length
    if (count > MAX_IDENT_LEN) {
        // Announce error
        // error(ERR_IDENTTOOLONG, lineNo, colNo - count, f);

    } else {
        // If this identifier is a keyword
        TokenType type = checkKeyword(token->string);

        // Otherwise
        if (type != TK_NONE) {
            token->tokenType = type;
        }

    }

    // Convert normal string to flower string
    overrrideIdent(token->string);
    
    return token;
}

Token* readNumber(void) {
    int count = 0;
    Token* token = makeToken(TK_NUMBER, lineNo, colNo);

    while (charCodes[currentChar] == CHAR_DIGIT) {
        if (count > MAX_NUMBER_LEN) {
            error(ERR_NUMBERTOOLONG, token->lineNo, token->colNo, f); 
        }
            // Add current character to the number
            token->string[count] = currentChar;

            // Increase string index
            count++;

            // Read next character
            readChar();
        }

    // End string
    token->string[count] = '\0';

    // Convert current number to string
    token->value = atoi(token->string);

    return token;
}

Token* readConstChar(void) {
    Token *token = makeToken(TK_CHAR, lineNo, colNo);

    readChar();
    if (currentChar == EOF) {
        token->tokenType = TK_NONE;
        error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo, f);
        return token;
    }
      
    token->string[0] = currentChar;
    token->string[1] = '\0';

    readChar();
    if (currentChar == EOF) {
        token->tokenType = TK_NONE;
        error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo, f);
        return token;
    }

    if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
        readChar();
        return token;
    } else {
        token->tokenType = TK_NONE;
        error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo, f);
        return token;
    }
}

// Read a const string in double singlequote 'string'
Token* readConstString(){
    Token *token = makeToken(TK_CHAR, lineNo, colNo);

    readChar();
    
    int count = 0;
    while (1) {
      
        // The next character is end of file make an error invalid char constant 
        if (currentChar == EOF) {
            token->tokenType = TK_NONE;
            error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo, f);
            return token;
        }
        
        if (count > MAX_STRING_LEN-1) error(ERR_CONSTSTRINGTOOLONG, token->lineNo, token->colNo, f);

        // Assign character value to token string 
        token->constString[count++] = currentChar;

        readChar();

        if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {

            readChar();

            // After ' is ' convert to one ' and continue string
            if (charCodes[currentChar] == CHAR_SINGLEQUOTE) continue;

            // After ' is not ' end read const string
            else token->constString[count] = '\0'; return token;
        }
    }

    
}

Token* getToken(void) {
    Token *token;
    int ln, cn;

    if (currentChar == EOF) 
        return makeToken(TK_EOF, lineNo, colNo);

    switch (charCodes[currentChar]) {

        // If charcode is ' ' 
        case CHAR_SPACE: skipBlank(); return getToken();

        // If charcode is '('
        case CHAR_LPAR: 

            token = makeToken(SB_LPAR, lineNo, colNo);
            readChar();

            // If next charcode is '*' became '(*' start comment
            if (charCodes[currentChar] == CHAR_TIMES) { skipComment(); return getToken(); }
            
            // If next charcode is '.' became '(.'
            // if (charCodes[currentChar] == CHAR_PERIOD) { token->tokenType = SB_LSEL; return token; } 
            
            return token;

        // If first charcode is letter 'a|b|c|d|..'
        case CHAR_LETTER: return readIdentKeyword();

        // If charcode is digit
        case CHAR_DIGIT: return readNumber();

        // If charcode is '+'
        case CHAR_PLUS: token = makeToken(SB_PLUS, lineNo, colNo); readChar(); return token;

        // If charcode is '-'
        case CHAR_MINUS: token = makeToken(SB_MINUS, lineNo, colNo); readChar(); return token;

        // If charcode is '*'
        case CHAR_TIMES: token = makeToken(SB_TIMES, lineNo, colNo); readChar(); return token;

        // if charcode is '/'
        case CHAR_SLASH:

            // token is slash token
            token = makeToken(SB_SLASH, lineNo, colNo);
            readChar();

            // if have double slash '//' , skip comment after '//
            if (charCodes[currentChar] == CHAR_SLASH) { skipCommentLine(); return getToken(); }
            return token;


        // If charcode is '='
        case CHAR_EQ: token = makeToken(SB_EQ, lineNo, colNo); readChar(); return token;

        // If charcode is ','
        case CHAR_COMMA: token = makeToken(SB_COMMA, lineNo, colNo); readChar(); return token;

        // If charcode is ';'
        case CHAR_SEMICOLON: token = makeToken(SB_SEMICOLON, lineNo, colNo); readChar(); return token;

        // If charcode is '.'
        case CHAR_PERIOD:

            token = makeToken(SB_PERIOD, lineNo, colNo);
            readChar();

            // If next charcode is ')', became '.)'
            // if (charCodes[currentChar] == CHAR_RPAR) token->tokenType = SB_RSEL; 
            return token;

        // If charcode is '>'
        case CHAR_GT:

            token = makeToken(SB_GT, lineNo, colNo);
            readChar();

            // If next charcode is '=', became '>='
            if (charCodes[currentChar] == CHAR_EQ) token->tokenType = SB_GE;
            return token;

        // If charcode is '<'
        case CHAR_LT:

            token = makeToken(SB_LT, lineNo, colNo);
            readChar();

            // If next charcode is '=', became '<='
            if (charCodes[currentChar] == CHAR_EQ) token->tokenType = SB_LE;
            return token;

        // If charcode is '!'
        case CHAR_EXCLAIMATION:

            // return token '!='
            token = makeToken(SB_NEQ, lineNo, colNo);
            readChar();

            // If next charcode is not '=' ----> error INVALID SYMBOL
            if (charCodes[currentChar] != CHAR_EQ) error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo, f);

            readChar();
            return token;

        // If charcode is ':'
        case CHAR_COLON:

            token = makeToken(SB_COLON, lineNo, colNo);
            readChar();

            // If next charcode is '=', become ':='
            if (charCodes[currentChar] == CHAR_EQ) { token->tokenType = SB_ASSIGN; readChar(); }
            return token;

        // If charcode is ')'
        case CHAR_RPAR: token = makeToken(SB_RPAR, lineNo, colNo); readChar(); return token;
        
        // If charcode is '/'', read const string
        case CHAR_SINGLEQUOTE: return readConstString(); //readConstChar();

        // If charcode is '['
        case CHAR_LSEL: token = makeToken(SB_LSEL, lineNo, colNo); readChar(); return token;

        // If charcode is ']'
        case CHAR_RSEL: token = makeToken(SB_RSEL, lineNo, colNo); readChar(); return token;
        
        // If charcode is '\t'
/*      case CHAR_HT: token = makeToken(SB_HT, lineNo, colNo); readChar(); return token;

        // If charcode is '\n'
        case CHAR_LF: token = makeToken(SB_LF, lineNo-1 , colNo); readChar(); return token;        */

          // ....
          // TODO
          // ....
        
        default:
            token = makeToken(TK_NONE, lineNo, colNo);
            error(ERR_INVALIDSYMBOL, lineNo, colNo, f);
            readChar(); 
            return token;
    }
}


/******************************************************************/

void printToken(Token *token, FILE* f) {

    fprintf(f, "%d-%d:", token->lineNo, token->colNo);

    switch (token->tokenType) {
        case TK_NONE: fprintf(f, "TK_NONE\n"); break;
        case TK_IDENT: fprintf(f, "TK_IDENT(%s)\n", token->string); break;
        case TK_NUMBER: fprintf(f, "TK_NUMBER(%s)\n", token->string); break;
        case TK_CHAR: fprintf(f, "TK_CHAR(\'%s\')\n", token->constString); break;
        case TK_EOF: fprintf(f, "TK_EOF\n"); break;

        case KW_PROGRAM: fprintf(f, "KW_PROGRAM\n"); break;
        case KW_CONST: fprintf(f, "KW_CONST\n"); break;
        case KW_TYPE: fprintf(f, "KW_TYPE\n"); break;
        case KW_VAR: fprintf(f, "KW_VAR\n"); break;
        case KW_INTEGER: fprintf(f, "KW_INTEGER\n"); break;
        case KW_CHAR: fprintf(f, "KW_CHAR\n"); break;
        case KW_ARRAY: fprintf(f, "KW_ARRAY\n"); break;
        case KW_OF: fprintf(f, "KW_OF\n"); break;
        case KW_FUNCTION: fprintf(f, "KW_FUNCTION\n"); break;
        case KW_PROCEDURE: fprintf(f, "KW_PROCEDURE\n"); break;
        case KW_BEGIN: fprintf(f, "KW_BEGIN\n"); break;
        case KW_END: fprintf(f, "KW_END\n"); break;
        case KW_CALL: fprintf(f, "KW_CALL\n"); break;
        case KW_IF: fprintf(f, "KW_IF\n"); break;
        case KW_THEN: fprintf(f, "KW_THEN\n"); break;
        case KW_ELSE: fprintf(f, "KW_ELSE\n"); break;
        case KW_WHILE: fprintf(f, "KW_WHILE\n"); break;
        case KW_DO: fprintf(f, "KW_DO\n"); break;
        case KW_FOR: fprintf(f, "KW_FOR\n"); break;
        case KW_TO: fprintf(f, "KW_TO\n"); break;

        case SB_SEMICOLON: fprintf(f, "SB_SEMICOLON\n"); break;
        case SB_COLON: fprintf(f, "SB_COLON\n"); break;
        case SB_PERIOD: fprintf(f, "SB_PERIOD\n"); break;
        case SB_COMMA: fprintf(f, "SB_COMMA\n"); break;
        case SB_ASSIGN: fprintf(f, "SB_ASSIGN\n"); break;
        case SB_EQ: fprintf(f, "SB_EQ\n"); break;
        case SB_NEQ: fprintf(f, "SB_NEQ\n"); break;
        case SB_LT: fprintf(f, "SB_LT\n"); break;
        case SB_LE: fprintf(f, "SB_LE\n"); break;
        case SB_GT: fprintf(f, "SB_GT\n"); break;
        case SB_GE: fprintf(f, "SB_GE\n"); break;
        case SB_PLUS: fprintf(f, "SB_PLUS\n"); break;
        case SB_MINUS: fprintf(f, "SB_MINUS\n"); break;
        case SB_TIMES: fprintf(f, "SB_TIMES\n"); break;
        case SB_SLASH: fprintf(f, "SB_SLASH\n"); break;
        case SB_LPAR: fprintf(f, "SB_LPAR\n"); break;
        case SB_RPAR: fprintf(f, "SB_RPAR\n"); break;
        case SB_LSEL: fprintf(f, "SB_LSEL\n"); break;
        case SB_RSEL: fprintf(f, "SB_RSEL\n"); break;
      /*case SB_HT: fprintf(f, "SB_HT\n"); break;
        case SB_LF: fprintf(f, "SB_LF\n"); break;*/
        default: break;
    }
}



int scan(char *fileName) {
    Token *token;
    f = fopen("./test/predict4.txt", "w");
    if (openInputStream(fileName) == IO_ERROR)
        return IO_ERROR;

    token = getToken();
    while (token->tokenType != TK_EOF) {
        printToken(token, f);
        free(token);
        token = getToken();
    }

    free(token);
    fclose(f);
    closeInputStream();
    return IO_SUCCESS;
}

/******************************************************************/

/*int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(f, "scanner: no input file.\n");
        return -1;
    }

    if (scan(argv[1]) == IO_ERROR) {
        printf("Can\'t read input file!\n");
        return -1;
    }

    return 0;
}*/

int main()
{

    if (scan("./test/example4.kpl") == IO_ERROR) {
        printf("Can\'t read input file!\n");
    }
    return 0;

}


