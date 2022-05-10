/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __CHARCODE_H__
#define __CHARCODE_H__

typedef enum {
  CHAR_SPACE,                 // char ' '
  CHAR_LETTER,                // char 'abcd_'
  CHAR_DIGIT,                 // char '123' 
  CHAR_PLUS,                  // char '+'
  CHAR_MINUS,                 // char '-'
  CHAR_TIMES,                 // char '*'
  CHAR_SLASH,                 // char '/'
  CHAR_LT,                    // char '<'
  CHAR_GT,                    // char '>'
  CHAR_EXCLAIMATION,          // char '!'
  CHAR_EQ,                    // char '='
  CHAR_COMMA,                 // char ','
  CHAR_PERIOD,                // char '.'
  CHAR_COLON,                 // char ':'
  CHAR_SEMICOLON,             // char ';'
  CHAR_SINGLEQUOTE,           // char " ' "
  CHAR_LPAR,                  // char '('
  CHAR_RPAR,                  // char ')'
  CHAR_LSEL,                  // char '['
  CHAR_RSEL,                  // char ']'
/*CHAR_HT,                    // char '\t'
  CHAR_LF,                    // char '\n'
  CHAR_UNDERSOCRE,*/          // '_'
  CHAR_UNKNOWN                // Other char
} CharCode;

#endif
