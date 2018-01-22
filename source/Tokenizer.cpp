#include "Tokenizer.h"

Tokenizer::Tokenizer(char separator, size_t maxMessageLength) :
  mSeparator(separator),
  mMaxMessageLength(maxMessageLength),
  mMessageLength(1),
  mMessage(nullptr),
  mNumTokens(0)
{
  mMessage = new char[mMaxMessageLength];
  mMessage[0] = '\0';
}

Tokenizer::~Tokenizer() {
  delete [] mMessage;
}

void Tokenizer::clear() {
  mMessage[0] = '\0';
  mMessageLength = 1;
  mNumTokens = 0;
}

char* Tokenizer::getMessage() const {
  return mMessage;
}

int Tokenizer::addString(const char* tokenValue) {
  size_t tokenLength = strlen(tokenValue) + 1;

  if (mNumTokens > 0) {
    if ((mMessageLength + tokenLength + 1) > mMaxMessageLength) {
      return TOKENIZER_MESSAGE_TOO_LONG;
    }

    mMessage[mMessageLength - 1] = mSeparator;
    ++mMessageLength;
  }
  else if ((mMessageLength + tokenLength) > mMaxMessageLength) {
    return TOKENIZER_MESSAGE_TOO_LONG;
  }

  memcpy(mMessage + (mMessageLength - 1), tokenValue, tokenLength);
  mMessageLength += (tokenLength - 1);
  ++mNumTokens;

  return TOKENIZER_SUCCESS;
}

int Tokenizer::addInt(int tokenValue) {
  sprintf(mTokenTemp, "%d", tokenValue);
  return addString(mTokenTemp);
}

int Tokenizer::addFloat(float tokenValue) {
  sprintf(mTokenTemp, "%.3f", tokenValue);
  return addString(mTokenTemp);
}
