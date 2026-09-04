#include "lexer.hpp"

Lexer::Lexer(const std::string &input) : source(input) {}

char Lexer::current() const {
  if (position >= source.size()) {
    return '\0';
  }

  return source[position];
}

void Lexer::advance() {
  if (position < source.size()) {
    if (current() == '\n') {
      line++;
      column = 1;
    } else {
      column++;
    }
    position++;
  }
}

Token Lexer::readNumber() {
  std::string c;

  while (current() >= '0' && current() <= '9') {
    c += current();
    advance();
  }

  return Token{TokenType::INTEGER_LITERAL, c, line, column};
}

void Lexer::skipWhitespace() {
  while (current() == ' ' || current() == '\n' || current() == '\t') {
    advance();
  }
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;

  while (position < source.size()) {
    skipWhitespace();

    if (position >= source.size()) {
      break;
    }

    char c = current();

    if (c >= '0' && c <= '9') {
      tokens.push_back(readNumber());
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
      tokens.push_back(readIdentifier());
    }

    else if (c == '<') {
      if (position + 1 < source.size() && source[position + 1] == '=') {
        tokens.push_back(Token{TokenType::LESS_EQUAL, "<=", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::LESS, "<", line, column});
        advance();
      }
    } else if (c == '>') {
      if (position + 1 < source.size() && source[position + 1] == '=') {
        tokens.push_back(Token{TokenType::GREATER_EQUAL, ">=", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::GREATER, ">", line, column});
        advance();
      }
    }

    else if (c == '!') {
      if (position + 1 < source.size() && source[position + 1] == '=') {
        tokens.push_back(Token{TokenType::NOT_EQUAL, "!=", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(
            Token{TokenType::INVALID, std::string(1, c), line, column});
        advance();
      }
    } else if (c == '=') {
      if (position + 1 < source.size() && source[position + 1] == '=') {
        tokens.push_back(Token{TokenType::EQUAL_EQUAL, "==", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::EQUAL, "=", line, column});
        advance();
      }
    }

    else if (c == ';') {
      tokens.push_back(Token{TokenType::SEMICOLON, ";", line, column});
      advance();
    } else if (c == ':') {
      tokens.push_back(Token{TokenType::COLON, ":", line, column});
      advance();
    }

    else if (c == '+') {
      tokens.push_back(Token{TokenType::PLUS, "+", line, column});
      advance();
    } else if (c == '-') {
      tokens.push_back(Token{TokenType::MINUS, "-", line, column});
      advance();
    } else if (c == '*') {
      tokens.push_back(Token{TokenType::STAR, "*", line, column});
      advance();
    } else if (c == '/') {
      tokens.push_back(Token{TokenType::SLASH, "/", line, column});
      advance();
    }

    else if (c == '(') {
      tokens.push_back(Token{TokenType::LPAREN, "(", line, column});
      advance();
    } else if (c == ')') {
      tokens.push_back(Token{TokenType::RPAREN, ")", line, column});
      advance();
    }

    else {
      tokens.push_back(Token{TokenType::INVALID, std::string(1, c), 1,
                             static_cast<int>(position) + 1});
      advance();
    }
  }
  tokens.push_back(Token{TokenType::END_OF_FILE, "", line, column});

  return tokens;
}

Token Lexer::readIdentifier() {
  std::string c;

  const int startLine = line;
  const int startColumn = column;

  while ((current() >= 'a' && current() <= 'z') ||
         (current() >= 'A' && current() <= 'Z') ||
         (current() >= '0' && current() <= '9') || current() == '_') {
    c += current();
    advance();
  }

  if (c == "let") {
    return Token{TokenType::LET, c, startLine, startColumn};
  } else if (c == "mut") {
    return Token{TokenType::MUT, c, startLine, startColumn};
  } else if (c == "int" || c == "bool") {
    return Token{TokenType::TYPE, c, startLine, startColumn};
  } else if (c == "print") {
    return Token{TokenType::PRINT, c, startLine, startColumn};
  } else if (c == "true") {
    return Token{TokenType::TRUE, c, startLine, startColumn};
  } else if (c == "false") {
    return Token{TokenType::FALSE, c, startLine, startColumn};
  }

  return Token{TokenType::IDENTIFIER, c, startLine, startColumn};
}