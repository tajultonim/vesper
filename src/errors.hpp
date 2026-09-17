#pragma once

#include <stdexcept>
#include <regex>
#include <string>
#include <utility>

enum class ErrorPhase {
  CLI,
  LEXER,
  PARSER,
  MODULE,
  RESOLVER,
  TYPE,
  RUNTIME,
};

struct SourceLocation {
  std::string file;
  int line = 0;
  int column = 0;
};

inline SourceLocation inferSourceLocation(const std::string &message,
                                          const std::string &file) {
  SourceLocation location{file, 0, 0};
  std::smatch match;

  const std::regex lineColumn(R"(line\s+(\d+)\s*,\s*column\s+(\d+))");
  const std::regex lineColon(R"((?:at\s+)?(\d+)\s*:\s*(\d+))");

  if (std::regex_search(message, match, lineColumn) ||
      std::regex_search(message, match, lineColon)) {
    location.line = std::stoi(match[1].str());
    location.column = std::stoi(match[2].str());
  }

  return location;
}

inline const char *errorPhaseName(ErrorPhase phase) {
  switch (phase) {
  case ErrorPhase::CLI:
    return "CLI";
  case ErrorPhase::LEXER:
    return "LEXER";
  case ErrorPhase::PARSER:
    return "PARSER";
  case ErrorPhase::MODULE:
    return "MODULE";
  case ErrorPhase::RESOLVER:
    return "RESOLVER";
  case ErrorPhase::TYPE:
    return "TYPE";
  case ErrorPhase::RUNTIME:
    return "RUNTIME";
  }

  return "UNKNOWN";
}

class VesperError : public std::runtime_error {
public:
  VesperError(ErrorPhase phase, std::string message)
      : VesperError(phase, std::move(message), {}) {}

  VesperError(ErrorPhase phase, std::string message, SourceLocation location)
      : std::runtime_error(formatMessage(phase, message, location)),
        phase_(phase), location_(std::move(location)) {}

  ErrorPhase phase() const noexcept { return phase_; }
  const SourceLocation &location() const noexcept { return location_; }

private:
  static std::string formatMessage(ErrorPhase phase,
                                   const std::string &message,
                                   const SourceLocation &location) {
    std::string result = "[" + std::string(errorPhaseName(phase)) + "] ";

    if (!location.file.empty()) {
      result += location.file;
      if (location.line > 0) {
        result += ":" + std::to_string(location.line);
        if (location.column > 0) {
          result += ":" + std::to_string(location.column);
        }
      }
      result += ": ";
    }

    result += message;
    return result;
  }

  ErrorPhase phase_;
  SourceLocation location_;
};

class CliError : public VesperError {
public:
  explicit CliError(std::string message)
      : VesperError(ErrorPhase::CLI, std::move(message)) {}

  CliError(std::string message, SourceLocation location)
      : VesperError(ErrorPhase::CLI, std::move(message), std::move(location)) {}
};

class LexerError : public VesperError {
public:
  explicit LexerError(std::string message)
      : VesperError(ErrorPhase::LEXER, std::move(message)) {}

    LexerError(std::string message, SourceLocation location)
      : VesperError(ErrorPhase::LEXER, std::move(message), std::move(location)) {}
};

class ParserError : public VesperError {
public:
  explicit ParserError(std::string message)
      : VesperError(ErrorPhase::PARSER, std::move(message)) {}

    ParserError(std::string message, SourceLocation location)
      : VesperError(ErrorPhase::PARSER, std::move(message), std::move(location)) {}
};

class ModuleError : public VesperError {
public:
  explicit ModuleError(std::string message)
      : VesperError(ErrorPhase::MODULE, std::move(message)) {}

    ModuleError(std::string message, SourceLocation location)
      : VesperError(ErrorPhase::MODULE, std::move(message), std::move(location)) {}
};

class ResolverError : public VesperError {
public:
  explicit ResolverError(std::string message)
      : VesperError(ErrorPhase::RESOLVER, std::move(message)) {}

    ResolverError(std::string message, SourceLocation location)
      : VesperError(ErrorPhase::RESOLVER, std::move(message), std::move(location)) {}
};

class TypeError : public VesperError {
public:
  explicit TypeError(std::string message)
      : VesperError(ErrorPhase::TYPE, std::move(message)) {}

    TypeError(std::string message, SourceLocation location)
      : VesperError(ErrorPhase::TYPE, std::move(message), std::move(location)) {}
};

class RuntimeError : public VesperError {
public:
  explicit RuntimeError(std::string message)
      : VesperError(ErrorPhase::RUNTIME, std::move(message)) {}

  RuntimeError(std::string message, SourceLocation location)
      : VesperError(ErrorPhase::RUNTIME, std::move(message), std::move(location)) {}
};

template <typename Error>
[[noreturn]] inline void rethrowWithSource(const std::exception &error,
                                           const std::string &file) {
  throw Error(error.what(), inferSourceLocation(error.what(), file));
}
