#include "lexer.hpp"
#include "parser.hpp"

Lexer::Lexer(const std::string &input) : source(input) {}

char Lexer::current() const {
  if (position >= source.size()) {
    return '\0';
  }

  return source[position];
}

char Lexer::peek() const {
  if (position + 1 >= source.size()) {
    return '\0';
  }

  return source[position + 1];
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

  int startLine = line;
  int startColumn = column;

  while (current() >= '0' && current() <= '9') {
    c += current();
    advance();
  }
  if (current() == '.' && position + 1 < source.size() && peek() >= '0' &&
      peek() <= '9') {
    c += current();
    advance();

    while (current() >= '0' && current() <= '9') {
      c += current();
      advance();
    }
    return Token{TokenType::FLOAT_LITERAL, c, startLine, startColumn};
  }
  return Token{TokenType::INTEGER_LITERAL, c, startLine, startColumn};
}

Token Lexer::readComment() {
  int commentLine = line;
  int commentColumn = column;

  std::string value; // Multi-line block comment: ## ... ##
  if (peek() == '#') {
    value += current();
    advance();
    value += current();
    advance();
    while (current() != '\0') {
      if (current() == '#' && position + 1 < source.size() && peek() == '#') {
        value += current();
        advance();
        value += current();
        advance();
        return Token{TokenType::COMMENT, value, commentLine, commentColumn};
      }
      value += current();
      advance();
    }
    throw std::runtime_error(
        "LEXER ERROR: Unterminated block comment at line " +
        std::to_string(commentLine) + ", column " +
        std::to_string(commentColumn));
  } // Single-line comment: # ...
  while (current() != '\0' && current() != '\n') {
    value += current();
    advance();
  }
  return Token{TokenType::COMMENT, value, commentLine, commentColumn};
}

Token Lexer::readString() {
  std::string c;

  int startLine = line;
  int startColumn = column;

  advance();

  while (current() != '"' && current() != '\0') {
    if (current() == '\\') {
      advance();

      switch (current()) {
      case 'n':
        c += '\n';
        break;

      case 't':
        c += '\t';
        break;

      case '\\':
        c += '\\';
        break;

      case '"':
        c += '"';
        break;

      default:
        throw std::runtime_error(
            "Unknown escape sequence at line " + std::to_string(line) +
            ", column " + std::to_string(column));
      }

      advance();
      continue;
    }
    c += current();
    advance();
  }

  if (current() == '"') {
    advance();
    return Token{TokenType::STRING_LITERAL, c, startLine, startColumn};
  } else {
    throw std::runtime_error("Unterminated string at " +
                             std::to_string(startLine) + ":" +
                             std::to_string(startColumn));
  }
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
    } else if (c == '"') {
      tokens.push_back(readString());
    }

    else if (c == '<') {
      if (peek() == '=') {
        tokens.push_back(Token{TokenType::LESS_EQUAL, "<=", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::LESS, "<", line, column});
        advance();
      }
    } else if (c == '>') {
      if (peek() == '=') {
        tokens.push_back(Token{TokenType::GREATER_EQUAL, ">=", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::GREATER, ">", line, column});
        advance();
      }
    }

    else if (c == '!') {
      if (peek() == '=') {
        tokens.push_back(Token{TokenType::NOT_EQUAL, "!=", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(
            Token{TokenType::INVALID, std::string(1, c), line, column});
        advance();
      }
    } else if (c == '=') {
      if (peek() == '=') {
        tokens.push_back(Token{TokenType::EQUAL_EQUAL, "==", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::EQUAL, "=", line, column});
        advance();
      }
    }

    else if (c == ',') {
      tokens.push_back(Token{TokenType::COMMA, ",", line, column});
      advance();
    } else if (c == ';') {
      tokens.push_back(Token{TokenType::SEMICOLON, ";", line, column});
      advance();
    } else if (c == ':') {
      tokens.push_back(Token{TokenType::COLON, ":", line, column});
      advance();
    }else if (c == '.') {
      tokens.push_back(Token{TokenType::DOT, ".", line, column});
      advance();
    }

    else if (c == '+') {
      tokens.push_back(Token{TokenType::PLUS, "+", line, column});
      advance();
    } else if (c == '-') {
      tokens.push_back(Token{TokenType::MINUS, "-", line, column});
      advance();
    } else if (c == '*') {
      if (peek() == '*') {
        tokens.push_back(Token{TokenType::STAR_STAR, "**", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::STAR, "*", line, column});
        advance();
      }
    } else if (c == '/') {
      if (peek() == '/') {
        tokens.push_back(Token{TokenType::SLASH_SLASH, "//", line, column});
        advance();
        advance();
      } else {
        tokens.push_back(Token{TokenType::SLASH, "/", line, column});
        advance();
      }
    } else if (c == '%') {
      tokens.push_back(Token{TokenType::PERCENT, "%", line, column});
      advance();
    }

    else if (c == '(') {
      tokens.push_back(Token{TokenType::LPAREN, "(", line, column});
      advance();
    } else if (c == ')') {
      tokens.push_back(Token{TokenType::RPAREN, ")", line, column});
      advance();
    } else if (c == '{') {
      tokens.push_back(Token{TokenType::LBRACE, "{", line, column});
      advance();
    } else if (c == '}') {
      tokens.push_back(Token{TokenType::RBRACE, "}", line, column});
      advance();
    } else if (c == '[') {
      tokens.push_back(Token{TokenType::LBRACKET, "[", line, column});
      advance();
    } else if (c == ']') {
      tokens.push_back(Token{TokenType::RBRACKET, "]", line, column});
      advance();
    }

    else if (current() == '#') {
      tokens.push_back(readComment());
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

  if (c == "import") {
    return Token{TokenType::IMPORT, c, startLine, startColumn};
  } else if (c == "as") {
    return Token{TokenType::AS, c, startLine, startColumn};
  } else if (c == "export") {
    return Token{TokenType::EXPORT, c, startLine, startColumn};
  } else if (c == "extern") {
    return Token{TokenType::EXTERN, c, startLine, startColumn};
  }

  else if (c == "let") {
    return Token{TokenType::LET, c, startLine, startColumn};
  } else if (c == "mut") {
    return Token{TokenType::MUT, c, startLine, startColumn};
  }

  else if (c == "fn") {
    return Token{TokenType::FN, c, startLine, startColumn};
  } else if (c == "return") {
    return Token{TokenType::RETURN, c, startLine, startColumn};
  }

  else if (c == "if") {
    return Token{TokenType::IF, c, startLine, startColumn};
  } else if (c == "else") {
    return Token{TokenType::ELSE, c, startLine, startColumn};
  }

  else if (c == "while") {
    return Token{TokenType::WHILE, c, startLine, startColumn};
  } else if (c == "for") {
    return Token{TokenType::FOR, c, startLine, startColumn};
  } else if (c == "break") {
    return Token{TokenType::BREAK, c, startLine, startColumn};
  }

  else if (c == "int" || c == "bool" || c == "float" || c == "string" ||
           c == "void") {
    return Token{TokenType::TYPE, c, startLine, startColumn};
  } else if (c == "true") {
    return Token{TokenType::TRUE, c, startLine, startColumn};
  } else if (c == "false") {
    return Token{TokenType::FALSE, c, startLine, startColumn};
  }

  return Token{TokenType::IDENTIFIER, c, startLine, startColumn};
}
