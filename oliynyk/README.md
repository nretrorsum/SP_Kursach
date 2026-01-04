# O16 Language Translator

Course Project on System Programming
Variant: Oliynyk Bohdan Romanovych

## Compilation

```bash
g++ -std=c++17 -o translator main.cpp lexer.cpp parser.cpp semantic.cpp codegen.cpp
```

## Running the Translator

### Default program (program.o16)
```bash
./translator
```

### Specific file
```bash
./translator <filename.o16>
```

## Test Programs

### Test 1: Linear Algorithm
```bash
./translator test1_linear.o16
```

### Test 2: Branching Algorithm
```bash
./translator test2_branch.o16
```

### Test 3: Loop Algorithm
```bash
./translator test3_loop.o16
```

## Running Generated Program

After successful translation, an executable `output` is created:

```bash
./output
```

## Output Files

- `output.c` - Generated C code
- `output` - Compiled executable
- `tokens.txt` - Token list from lexical analysis
- `errors.txt` - Error report

## Language Features

- **Identifiers**: Up4 with _ prefix (underscore followed by 1-4 uppercase letters, e.g., `_A`, `_MAX`, `_X1`)
- **Data type**: int_2 (-32768 to 32767)
- **Arithmetic operators**: `+`, `-`, `*`, `/`, `%`
- **Comparison operators**: `eq`, `noteq`, `less`, `gr`
- **Logical operators**: `!`, `and`, `or`
- **Assignment**: `::`
- **Statements**: `get`, `put`, `if-else` (Rust style), `while` (Rust style)
- **Block delimiters**: `startblok`, `endblok`
- **Comments**: `/* comment text` (single line, no closing)

## Program Structure

```
startprogram
variable _VAR1, _VAR2;
startblok
    /* statements here
endblok
```

## Example Program

```
startprogram
variable _A, _B, _SUM;
startblok
    get(_A);
    get(_B);
    _SUM :: _A + _B;
    put(_SUM);
endblok
```
