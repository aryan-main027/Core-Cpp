## How C++ Works
```
    #include <iostream>
    int main()
    {
        std::cout << "Hello World" << std::endl;
        std::cin.get();
    }

```
### working
1. `#include <iostream>`: This line includes the iostream header file, which is necessary for input and output operations (like cout and cin) it is a preprocesser directive which tells the compiler to include the iostream header file before compiling the program.

2. `int main()`: This is the main function, where the program execution begins. and it is the entry point of the program .

3. `std::cout << "Hello World" << std::endl;`: This line prints "Hello World" to the console.
    - `std::cout`: This is the standard output stream.
    - `<<`: This is the insertion operator.
    - `"Hello World"`: This is the string to be printed.
    - `std::endl`: This is the end-of-line character.

4. `std::cin.get();`: This line waits for the user to press the Enter key before the program exits. 

Compiler compliles c++ code into object file (.o) and then linker links the object file
with the necessary libraries and creates an executable file .

source.cpp → preprocessor → translation unit → compiler → object file → linker →
executable → loader → running process

## Preprocessor
Imagine you’re writing a C++ program. You quickly realise two things:

1. You want to share code between multiple files. For example, you write a function add(int, int) once and use it in many different .cpp files. You don’t want to copy-paste that function definition every time.

2. You want to write different code depending on the situation. Maybe you have debug printouts that should only exist in a debug build, or platform-specific code for Windows vs Linux.

One way to solve both problems is to create a smart text processor that sits before the real compiler. This text processor can:

Pull in the contents of other files (#include).

Replace one piece of text with another (#define).

Decide which parts of the text to keep or throw away, based on conditions (#ifdef).

That’s the C++ preprocessor. It’s exactly that: a program that runs before compilation
and transforms your source code into a single, self-contained chunk of text that is then
fed to the actual compiler.

Principle: Preprocessing is text-in, text-out. It doesn’t know C++. It doesn’t know what
int means. It only sees characters.

So, from first principles, the preprocessor is a pure text transformation engine.

#include <iostream>
#include "myheader.h"

1. The angle brackets <> tell the preprocessor to search in standard system directories.

2. The double quotes "" tell it to search in the current directory first, then system
directories.

#define creates a macro. A macro is a rule that says: “Whenever you encounter token X in
the text, replace it with token(s) Y.”

```
#define PI 3.14159
#define MAX(a,b) ((a) > (b) ? (a) : (b))
```

After these definitions, the preprocessor scans the rest of the file. Every time it sees
the word PI, it erases PI and puts 3.14159. Every time it sees MAX(x, y), it replaces it
with ((x) > (y) ? (x) : (y)).

File: main.cpp

```
#define MSG "Hello"
#include "greet.h"

int main() {
    say(MSG);
}
```
File: greet.h

```cpp
#include <iostream>
void say(const char* s) {
    std::cout << s << std::endl;
}
```

### step-by-step

The preprocessor starts with main.cpp, line by line:

1. It sees #define MSG "Hello" and records the macro MSG → "Hello".

2. It sees #include "greet.h". It opens greet.h, reads all of it.

3. Inside greet.h, it encounters #include <iostream>. It opens the standard file iostream, reads
its thousands of lines, and pastes them all. Then it continues with greet.h: pastes the void
say(...) function definition.

4. After finishing greet.h, it returns to main.cpp, pastes the remaining lines. The macro MSG in sa
(MSG) is expanded to "Hello".

5. The final output (simplified, ignoring iostream details) will look like:

```cpp
... (thousands of lines from iostream) ...
void say(const char* s) {
    std::cout << s << std::endl;
}

int main() {
    say("Hello");
}
```

This single massive text is called a translation unit and is what the compiler actually compiles.

Because #include does copy-paste, including the same file twice pastes it twice, leading to duplicate definitions and errors. To prevent this, we use include guards:

```cpp
#ifndef MYHEADER_H
#define MYHEADER_H
// content of header
#endif
```
First time the file is included: MYHEADER_H is not defined, so the #ifndef succeeds, the 
content is pasted, and inside it we #define MYHEADER_H. Second time: MYHEADER_H is already
defined, so the #ifndef fails, and everything up to #endif is skipped. The header’s
content appears only once. This is a pure preprocessor trick.

Modern compilers also support #pragma once, which does the same thing with less typing, 
but it’s not part of the standard (though widely supported).

By using `g++ -E Demo.cpp -o main.i` we can see the preprocessed output
OR 
What actually compiler recives by using include `<iostream>` 

## So what actually this `main.i`/`Demo.i` is ?

It is the code which is actually fed into the `g++ compiler`

## Why Preprocessors are Bad

```cpp
#define SQUARE(x) x*x
int a = SQUARE(1+2);   // becomes 1+2*1+2 = 1 + 2 + 2 = 5, not 9!
```


## C++ does not need preprocessing

C++ doesn't *need* a preprocessor. The language itself provides everything the preprocessor does,
but with proper C++ tools instead of crude text substitution.

1. It lives outside the C++ language
    C++ has well-defined concepts: scopes (namespace, class, function), types, overload resolution,
    template instantiation, name lookup rules. The preprocessor respects none of them because it
    runs before any of that exists.

    From first principles, a good language abstraction should let you express intent in a way the
    compiler can fully verify. Macros bypass all verification: they are just a separate
    text-munging language layered on top.

    Consequence: The compiler cannot give you meaningful errors for macro misuse. It can only
    report problems in the expanded code, often with confusing location information or completely
    obscured meaning.

2.  It bypasses C++ type system

```cpp

    function function_name(int,int,......){
        #define X 20;
    }

```

now according to c++ scope rule this x must be accessible only inside the function `function_name`
**but** 
preprocessor doesnt know C++ so it will replace X with 20 everywhere in the file

3. Macros work on tokens, not values. They don’t know about types, conversions, or anything that
makes C++ safer.


```cpp
#define MAX(a,b) ((a)>(b)?(a):(b))

MAX(3, "hello")   // compiles? Preprocessor doesn't care. Error later: compares int and const char*
MAX(++x, y)       // if x is larger, ++x evaluated twice — side effect duplicated.
You can never get an error that “MAX expects arithmetic types” because the macro has no concept of
types. A template or constexpr function will catch these misuses cleanly.
``` 

4. 