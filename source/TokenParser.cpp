#include "TokenParser.h"

TokenParser::TokenParser(const char* sourceString, char separator) {
  parseSourceString(sourceString, separator);
}

TokenParser::~TokenParser() {
  size_t numTokens = mTokens.size();
  for (size_t i = 0; i < numTokens; ++i) {
    delete [] mTokens[i];
  }
}

bool TokenParser::getString(size_t tokenIndex, char* pChar) const {
  if (tokenIndex >= mTokens.size()) {
    return false;
  }
  strcpy(pChar, mTokens[tokenIndex]);
  return true;
}

bool TokenParser::getInt(size_t tokenIndex, int* pInt) const {
  if (tokenIndex >= mTokens.size()) {
    return false;
  }
  sscanf(mTokens[tokenIndex], "%d", pInt);
  return true;
}

bool TokenParser::getFloat(size_t tokenIndex, float* pFloat) const {
  if (tokenIndex >= mTokens.size()) {
    return false;
  }
  sscanf(mTokens[tokenIndex], "%f", pFloat);
  return true;
}

bool TokenParser::compare(size_t tokenIndex, const char* otherString) const {
  if (tokenIndex >= mTokens.size()) {
    return false;
  }
  return strcmp(otherString, mTokens[tokenIndex]) == 0;
}

void TokenParser::parseSourceString(const char* sourceString, char separator) {
  char tempToken[MAX_TOKEN_LENGTH_BUFFER_SIZE];
  int sourceIndex = 0;
  int tempIndex = 0;

  while (true) {
    char currentChar = sourceString[sourceIndex];
    if (currentChar == separator || currentChar == '\0') {
      tempToken[tempIndex] = '\0';
      char* newToken = new char[tempIndex + 1];
      memcpy(newToken, tempToken, tempIndex + 1);
      mTokens.push_back(newToken);
      if (currentChar == '\0') {
        break;
      }
      tempIndex = 0;
    }
    else if (tempIndex < MAX_TOKEN_LENGTH) {
      tempToken[tempIndex] = currentChar;
      ++tempIndex;
    }
    ++sourceIndex;
  }
}
