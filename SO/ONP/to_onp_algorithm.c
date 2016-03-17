/**
 * Copyrights Jakub Staroñ 2015
 * numer indeksu 361025
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "to_onp_algorithm.h"

void ConsumeSpaces(InputStream_pointer stream) {
  while (InputStream_hasNext(stream) && InputStream_peekChar(stream) == ' ')
    InputStream_readChar(stream);
}

String_pointer ConsumeToken(InputStream_pointer stream) {
  StringBuilder_pointer token = StringBuilder_createEmpty();
  while (InputStream_hasNext(stream) && InputStream_peekChar(stream) != ' ') {
    char c = InputStream_readChar(stream);
    StringBuilder_appendChar(token, c);
  }

  String_pointer result = StringBuilder_toString(token);
  StringBuilder_free(token);
  return result;
}

String_pointer ReadTokenFromExpression(String_pointer* expression_string) {
  InputStream_pointer expression = InputStream_fromString(*expression_string);

  ConsumeSpaces(expression);
  String_pointer result = ConsumeToken(expression);
  ConsumeSpaces(expression);

  String_free(*expression_string);
  *expression_string = InputStream_readAll(expression);
  InputStream_free(expression);

  return result;
}

Vector_pointer Tokenize(String_pointer string) {
  InputStream_pointer stream = InputStream_fromString(string);
  Vector_pointer result = Vector_new(sizeof(String_pointer), VECTOR_DEFAULT);

  ConsumeSpaces(stream);
  while (InputStream_hasNext(stream)) {
    String_pointer token = ConsumeToken(stream);
    Vector_pushBack(result, &token);
    ConsumeSpaces(stream);
  }

  InputStream_free(stream);
  return result;
}

void StringBuilder_appendStringWithOptionalSpace(StringBuilder_pointer builder, String_pointer string) {
  if (!StringBuilder_empty(builder))
    StringBuilder_appendChar(builder, ' ');

  StringBuilder_appendString(builder, string);
}

String_pointer Untokenize(Vector_pointer tokens) {
  StringBuilder_pointer builder = StringBuilder_createEmpty();

  for (size_t i = 0 ; i < Vector_size(tokens) ; i++) {
    String_pointer token = VECTOR_AT(tokens, i, String_pointer);
    StringBuilder_appendStringWithOptionalSpace(builder, token);
  }

  String_pointer result = StringBuilder_toString(builder);
  StringBuilder_free(builder);
  return result;
}

bool IsOperator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int OperatorPriority(String_pointer operator) {
  assert(String_size(operator) == 1);
  char c = String_at(operator, 0);
  switch (c) {
    case '(':
      return 0;
    case '+':
    case '-':
    case ')':
      return 1;
    case '*':
    case '/':
      return 2;
    case '^':
      return 3;
    default:
      assert(0);
      return -1;
  }
}

bool IsNumber(String_pointer string) {
  InputStream_pointer stream = InputStream_fromString(string);

  bool result = false;

  if (InputStream_hasNext(stream) && InputStream_peekChar(stream) == '-')
    InputStream_readChar(stream);

  while (InputStream_hasNext(stream)) {
    result = false;
    char c = InputStream_readChar(stream);
    if (isdigit(c)) {
      result = true;
    }
    else {
      break;
    }
  }

  InputStream_free(stream);
  return result;
}

enum TokenCategory GetTokenCategory(String_pointer token) {
  assert(String_size(token) >= 1);
  char c = String_at(token, 0);
  if (c == '(') {
    assert(String_size(token) == 1);
    return TOKEN_CATEGORY_LEFT_PARENTHESIS;
  }
  else if (c == ')') {
    assert(String_size(token) == 1);
    return TOKEN_CATEGORY_RIGHT_PARENTHESIS;
  }
  else if ('a' <= c && c <= 'z') {
    assert(String_size(token) == 1);
    return TOKEN_CATEGORY_CONSTANT;
  }
  else if (IsNumber(token)){
    return TOKEN_CATEGORY_NUMBER;
  }
  else if (IsOperator(c) && String_size(token) == 1) {
    assert(String_size(token) == 1);
    return TOKEN_CATEGORY_OPERATOR;
  }
  assert(0 && "Invalid token.");
  return TOKEN_CATEGORY_INVALID;
}

void MakeAlgorithmStep(String_pointer* expression_string, String_pointer* stack_string, String_pointer* partial_result_string) {
  //printf("stack = %s\n", String_toCstring(*stack_string));
  String_pointer token = ReadTokenFromExpression(expression_string);

  Vector_pointer stack = Tokenize(*stack_string);
  StringBuilder_pointer partial_result = StringBuilder_fromString(*partial_result_string);

  AlgorithmStep(token, stack, partial_result);
  String_free(token);

  if (String_isEmpty(*expression_string)) {
    while (Vector_size(stack) > 0) {
      String_pointer string = *((String_pointer*)(Vector_back(stack)));
      Vector_popBack(stack);
      StringBuilder_appendStringWithOptionalSpace(partial_result, string);
      String_free(string);
    }
  }

  // stack
  String_free(*stack_string);
  *stack_string = Untokenize(stack);
  for (size_t i = 0 ; i < Vector_size(stack) ; i++) {
    String_pointer string = VECTOR_AT(stack, i, String_pointer);
    String_free(string);
  }
  Vector_free(stack);

  String_free(*partial_result_string);
  *partial_result_string = StringBuilder_toString(partial_result);
  StringBuilder_free(partial_result);
}

void AlgorithmStep(String_pointer token, Vector_pointer stack, StringBuilder_pointer partial_result) {
  enum TokenCategory category = GetTokenCategory(token);
  switch (category) {
    case TOKEN_CATEGORY_LEFT_PARENTHESIS: {
      String_pointer token_copy = String_clone(token);
      Vector_pushBack(stack, &token_copy);
      break;
    }
    case TOKEN_CATEGORY_RIGHT_PARENTHESIS: {
      assert(Vector_size(stack) > 0);
      String_pointer string = VECTOR_AT(stack, Vector_size(stack) - 1, String_pointer);
      Vector_popBack(stack);

      while (GetTokenCategory(string) != TOKEN_CATEGORY_LEFT_PARENTHESIS) {
        StringBuilder_appendStringWithOptionalSpace(partial_result, string);
        String_free(string);
        assert(Vector_size(stack) > 0);
        string = VECTOR_AT(stack, Vector_size(stack) - 1, String_pointer);
        Vector_popBack(stack);
      }

      String_free(string);
      break;
    }
    case TOKEN_CATEGORY_CONSTANT:
    case TOKEN_CATEGORY_NUMBER: {
      StringBuilder_appendStringWithOptionalSpace(partial_result, token);
      break;
    }
    case TOKEN_CATEGORY_OPERATOR: {
      String_pointer token_copy = String_clone(token);
      int my_priority = OperatorPriority(token_copy);
      while (Vector_size(stack) > 0 && OperatorPriority( VECTOR_BACK(stack, String_pointer) ) >= my_priority) {
        String_pointer string = VECTOR_BACK(stack, String_pointer);
        Vector_popBack(stack);
        StringBuilder_appendStringWithOptionalSpace(partial_result, string);
        String_free(string);
      }
      Vector_pushBack(stack, &token_copy);
      break;
    }
    case TOKEN_CATEGORY_INVALID:
    default:
      assert(0);
  }
}