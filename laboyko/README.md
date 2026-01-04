# L16 Language Translator

Course Project on System Programming
Variant: Laboyko Valeriy Valeriyovych

## Compilation

```bash
g++ -std=c++17 -o translator main.cpp lexer.cpp parser.cpp semantic.cpp codegen.cpp
```

## Running the Translator

### Default program (program.l16)
```bash
./translator
```

### Specific file
```bash
./translator <filename.l16>
```

## Test Programs

### Test 1: Linear Algorithm
```bash
./translator test1_linear.l16
```

### Test 2: Branching Algorithm
```bash
./translator test2_branch.l16
```

### Test 3: Loop Algorithm
```bash
./translator test3_loop.l16
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

- **Identifiers**: Up8 (up to 8 uppercase letters, e.g., `A`, `MAX`, `COUNTER`)
- **Data type**: Integer32_t (32-bit integer)
- **Arithmetic operators**: `+`, `-`, `Mul`, `Div`, `Mod`
- **Comparison operators**: `=`, `<>`, `!>` (not greater, <=), `!<` (not less, >=)
- **Logical operators**: `!`, `And`, `Or`
- **Assignment**: `>>`
- **Statements**: `Get`, `Put`, `If-Then Goto`, `Goto`, `For-To-Next`
- **Block delimiters**: `Start`, `Stop`
- **Comments**: `@@ comment text` (single line, no closing)

## Program Structure

```
#Program NAME;
Start Variable VAR1, VAR2
    @@ statements here
Stop
```

## Example Program

```
#Program DEMO;
Start Variable A, B, SUM

Get(A);
Get(B);
SUM >> A + B;
Put(SUM);

Stop
```

## Control Flow Examples

### If-Then Goto (Basic style)
```
If A !< B Then Goto LABEL;
@@ code if condition is false
Goto END;

LABEL:
@@ code if condition is true

END:
```

### For-To-Next Loop (Basic style)
```
For I >> 1 To 10
    Put(I);
Next I;
```
