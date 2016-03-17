/**
 * Copyrights Jakub Staroñ 2015
 * numer indeksu 361025
 */

#ifndef _TO_ONP_ALGORITHM_H
#define _TO_ONP_ALGORITHM_H

#include "better_string.h"
#include "string_builder.h"
#include "input_stream.h"
#include "vector.h"

enum TokenCategory {
  TOKEN_CATEGORY_NUMBER,
  TOKEN_CATEGORY_CONSTANT,
  TOKEN_CATEGORY_LEFT_PARENTHESIS,
  TOKEN_CATEGORY_RIGHT_PARENTHESIS,
  TOKEN_CATEGORY_OPERATOR,
  TOKEN_CATEGORY_INVALID
};

void ConsumeSpaces(InputStream_pointer stream);

void StringBuilder_appendStringWithOptionalSpace(StringBuilder_pointer builder, String_pointer string);

String_pointer ConsumeToken(InputStream_pointer stream);

String_pointer ReadTokenFromExpression(String_pointer* expression_string);

Vector_pointer Tokenize(String_pointer string);

String_pointer Untokenize(Vector_pointer tokens);

bool IsOperator(char c);

bool IsNumber(String_pointer string);

int OperatorPriority(String_pointer operator);

enum TokenCategory GetTokenCategory(String_pointer token);

void MakeAlgorithmStep(String_pointer* expression_string, String_pointer* stack_string, String_pointer* partial_result_string);

void AlgorithmStep(String_pointer token, Vector_pointer stack, StringBuilder_pointer partial_result);

#endif // _TO_ONP_ALGORITHM_H
