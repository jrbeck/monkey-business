#pragma once

#include <cstdio>
#include <cstring>

#define TOKEN_TEMP_LENGTH (32)

enum TOKENIZER_RETURNS {
  TOKENIZER_SUCCESS = 0,
  TOKENIZER_MESSAGE_TOO_LONG,
};

class Tokenizer {
public:
  Tokenizer(char separator, size_t maxMessageLength);
  ~Tokenizer();

  void clear();
  char* getMessage() const;

  int addString(const char* tokenValue);
  int addInt(int tokenValue);
  int addFloat(float tokenValue);

private:
  char mTokenTemp[TOKEN_TEMP_LENGTH];
  char mSeparator;
  size_t mMaxMessageLength;
  size_t mMessageLength;
  char* mMessage;
  int mNumTokens;
};
