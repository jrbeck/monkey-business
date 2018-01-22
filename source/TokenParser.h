#pragma once

#include <vector>

#define MAX_TOKEN_LENGTH (255)
#define MAX_TOKEN_LENGTH_BUFFER_SIZE (MAX_TOKEN_LENGTH + 1)

class TokenParser {
public:
  TokenParser(const char* sourceString, char separator);
  ~TokenParser();

  bool getString(size_t tokenIndex, char* pChar) const;
  bool getInt(size_t tokenIndex, int* pInt) const;
  bool getFloat(size_t tokenIndex, float* pFloat) const;

  bool compare(size_t tokenIndex, const char* otherString) const;

private:
  void parseSourceString(const char* sourceString, char separator);

  std::vector<char*> mTokens;
};
