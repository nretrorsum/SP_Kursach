# V03 Language Translator

Course Project on System Programming
Variant: Voitovych Oleksandr Viktorovych

## Compilation

```bash
g++ -std=c++17 -o translator main.cpp lexer.cpp parser.cpp semantic.cpp codegen.cpp
```

## Running the Translator

### Default program (program.v03)
```bash
./translator
```

### Specific file
```bash
./translator <filename.v03>
```

## Test Programs

### Test 1: Linear Algorithm
```bash
./translator test1_linear.v03
```

### Test 2: Branching Algorithm
```bash
./translator test2_branch.v03
```

### Test 3: Loop Algorithm
```bash
./translator test3_loop.v03
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

- **Identifiers**: Up2 (uppercase letters only, max 2 characters)
- **Data type**: Int16 (-32768 to 32767)
- **Operators**: `+`, `-`, `Mul`, `Div`, `Mod`, `>>`, `<<`, `Eg`, `Ne`, `And`, `Or`, `!`
- **Statements**: `Get`, `Put`, `If-Goto`, `Goto`, `For-To-Next`
- **Comments**: `/* comment text`
