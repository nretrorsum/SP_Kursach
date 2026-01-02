# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Compile the project
g++ -o lab_4_5 main.cpp lexer.cpp parser.cpp

# Run the analyzer
./lab_4_5
```

The program reads source code from `program.txt` and performs lexical and syntax analysis.

## Architecture

This is a lexer and parser for a custom programming language (курсовий проєкт for System Programming course, variant: Запливаний Данііл Валерійович).

### Components

- **token.h** - Token types enum (`TokenType`) and `Token` struct with type, value, and line number. Contains `tokenTypeToString()` helper.
- **lexer.h/cpp** - `Lexer` class performs lexical analysis: tokenizes source code, handles comments (`%% ... %%`), validates identifiers (must start with uppercase, max 4 chars) and numbers (int16_t range: 0-32767).
- **parser.h/cpp** - `Parser` class implements recursive descent parser for syntax analysis.
- **main.cpp** - Entry point, reads `program.txt`, runs lexer, visualizes tokens per line, then runs parser.

### Language Syntax (Variant Specific)

```
program <ID>;
var int16_t <ID>, <ID>, ...
start
    <statements>
finish
```

**Statements:** assignment (`ID <- expr`), input (`get ID`), output (`put expr`), if/else (Swift style), for-in loop (Swift style)

**Operators:**
- Assignment: `<-`
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `eg` (equal), `ne` (not equal), `>=`, `<=`
- Logical: `!`, `and`, `or`
- Range: `..` (for loops)

**Keywords:** `program`, `var`, `start`, `finish`, `get`, `put`, `if`, `else`, `for`, `in`, `int16_t` (all lowercase)

### Lexer Rules

- Identifiers: start with uppercase letter, max 4 characters
- Numbers: int16_t range (0-32767)
- Comments: `%% ... %%` (can be multiline)

### Control Flow Syntax

```
if <expr> start <statements> finish
if <expr> start <statements> finish else start <statements> finish

for <ID> in <expr> .. <expr> start <statements> finish
```
