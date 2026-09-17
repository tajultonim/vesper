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

inline std::string normalizeErrorMessage(std::string message) {
  static const std::regex phasePrefix(
      R"(^\s*(?:LEXER ERROR|PARSER ERROR|MODULE_ERROR|TYPE_ERROR|RUNTIME_ERROR|RUNTIME ERROR)\s*:\s*)");
  static const std::regex lineSuffix(
      R"(\s+at\s+line\s+\d+\s*,\s*column\s+\d+\s*$)");
  static const std::regex coordinateSuffix(R"(\s+at\s+\d+\s*:\s*\d+\s*$)");

  message = std::regex_replace(message, phasePrefix, "");
  message = std::regex_replace(message, lineSuffix, "");
  message = std::regex_replace(message, coordinateSuffix, "");
  return message;
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

inline const char *errorPhaseColor(ErrorPhase phase) {
  switch (phase) {
  case ErrorPhase::CLI:
    return "\x1b[33m";
  case ErrorPhase::LEXER:
    return "\x1b[35m";
  case ErrorPhase::PARSER:
    return "\x1b[31m";
  case ErrorPhase::MODULE:
    return "\x1b[34m";
  case ErrorPhase::RESOLVER:
    return "\x1b[36m";
  case ErrorPhase::TYPE:
    return "\x1b[33m";
  case ErrorPhase::RUNTIME:
    return "\x1b[91m";
  }

  return "\x1b[31m";
}

class VesperError : public std::runtime_error {
public:
  VesperError(ErrorPhase phase, std::string message)
      : VesperError(phase, normalizeErrorMessage(message),
                    inferSourceLocation(message, "")) {}

  VesperError(ErrorPhase phase, std::string message, SourceLocation location)
      : std::runtime_error(
            formatMessage(phase, normalizeErrorMessage(message), location)),
        phase_(phase), location_(std::move(location)),
        message_(normalizeErrorMessage(std::move(message))) {}

  ErrorPhase phase() const noexcept { return phase_; }
  const SourceLocation &location() const noexcept { return location_; }

  std::string formatWithFile(const std::string &file) const {
    if (!location_.file.empty() || file.empty()) {
      return what();
    }

    return formatMessage(phase_, message_,
                         SourceLocation{file, location_.line, location_.column});
  }

  std::string formatColored(const std::string &file) const {
    return std::string(errorPhaseColor(phase_)) + formatWithFile(file) +
           "\x1b[0m";
  }

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
  std::string message_;
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
