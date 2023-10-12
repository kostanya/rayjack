# **C++ Notes**

## Why CMake exists ?

Compiling the library from the source code guarantees that the resulting library is perfectly tailored for your CPU/OS, a luxury pre-compiled binaries don't always provide (sometimes, pre-compiled binaries are not available for your system). The problem with providing source code to the open world however is that not everyone uses the same IDE or build system for developing their application, which means the project/solution files provided may not be compatible with other people's setup. So people then have to setup their own project/solution with the given .c/.cpp and .h/.hpp files, which is cumbersome. Exactly for those reasons there is a tool called CMake.

## Make vs CMake

Make (or rather a Makefile) is a buildsystem - it drives the compiler and other build tools to build your code.

**CMake is a generator of buildsystems.** It can produce Makefiles, it can produce Ninja build files, it can produce KDEvelop or Xcode projects, it can produce Visual Studio solutions. From the same starting point, the same CMakeLists.txt file. So if you have a platform-independent project, CMake is a way to make it buildsystem-independent as well.

## Notable CMake Commands and Variables

**Commands:**

- **add\_executable:** Add an executable to the project using the specified source files.
- **add\_library:** Add a library to the project using the specified source files.
- **add\_subdirectory:** Adds a subdirectory to the build.
- **include\_directories:** Add the given directories to those the compiler uses to search for include files. Relative paths are interpreted as relative to the current source directory.
- **target\_include\_directories:** Specifies include directories to use when compiling a given target. The named \<target\> must have been created by a command such as add\_executable() or add\_library()
- **target\_link\_libraries** : Specify libraries or flags to use when linking a given target and/or its dependents.

**NOTE:** When it comes to [target\_include\_directories](https://cmake.org/cmake/help/latest/command/target_include_directories.html) and [target\_link\_libraries](https://cmake.org/cmake/help/latest/command/target_link_libraries.html), there are several keywords, PUBLIC, PRIVATE, and INTERFACE.

| **Include Inheritance** | **Description** |
| --- | --- |
| PUBLIC | All the directories following PUBLIC will be used for the current target and the other targets that have dependencies on the current target, i.e., appending the directories to INCLUDE\_DIRECTORIES and INTERFACE\_INCLUDE\_DIRECTORIES. |
| PRIVATE | All the include directories following PRIVATE will be used for the current target only, i.e., appending the directories to INCLUDE\_DIRECTORIES. |
| INTERFACE | All the include directories following INTERFACE will NOT be used for the current target but will be accessible for the other targets that have dependencies on the current target, i.e., appending the directories to INTERFACE\_INCLUDE\_DIRECTORIES. |

<br>

| **Link Type** | **Description** |
| --- | --- |
| PUBLIC | All the objects following PUBLIC will be used for linking to the current target and providing the interface to the other targets that have dependencies on the current target. |
| PRIVATE | All the objects following PRIVATE will only be used for linking to the current target. |
| INTERFACE | All the objects following INTERFACE will only be used for providing the interface to the other targets that have dependencies on the current target. |

<br>

- **set\_target\_properties:** Targets can have properties that affect how they are built. Sets properties on targets. The syntax for the command is to list all the targets you want to change, and then provide the values you want to set next.
- **target\_compile\_options:** Adds options to the COMPILE\_OPTIONS or INTERFACE\_COMPILE\_OPTIONS target properties. These options are used when compiling the given \<target\>, which must have been created by a command such as add\_executable() or add\_library()
- **find\_package:** When we need to add an external project as the dependency, we will use command find\_package in CMakeLists.txt to let CMake know where the header files and libraries are located, such that they can be found and linked properly later on. The CMake command find\_package has two modes (Config mode and Module mode) to find and load the CMake configuration of an external project.

**Variables:**

- **CMAKE\_CXX\_STANDARD:** Specifies the C++ standard to be used for compilation (e.g., C++11, C++14, C++17).
- **CMAKE\_BUILD\_TYPE:** Specifies the build type (e.g., Debug, Release, RelWithDebInfo) which affects compiler flags and optimizations.
- **CMAKE\_INSTALL\_PREFIX:** Specifies the installation directory for your project.
- **CMAKE\_SOURCE\_DIR:** The root directory of the source code.
- **CMAKE\_BINARY\_DIR:** The root directory where the build files are generated.
- **CMAKE\_CURRENT\_SOURCE\_DIR:** The directory where the current CMakeLists.txt file is located.
- **CMAKE\_CURRENT\_LIST\_DIR:** Full directory of the listfile currently being processed. As CMake processes the listfiles in your project this variable will always be set to the directory where the listfile which is currently being processed (CMAKE\_CURRENT\_LIST\_FILE) is located. The value has dynamic scope. When CMake starts processing commands in a source file it sets this variable to the directory where this file is located. When CMake finishes processing commands from the file it restores the previous value. Therefore the value of the variable inside a macro or function is the directory of the file invoking the bottom-most entry on the call stack, not the directory of the file containing the macro or function definition.

**Example:** if a CMakeLists.txt is present in a directory project and contains the following directive


``include(src/CMakeLists.txt)``

&rarr; then while ```src/CMakeLists.txt``` is being processed CMAKE\_CURRENT\_LIST\_DIR will refer to ```project/src``` whereas CMAKE\_CURRENT\_SOURCE\_DIR still points to the outer directory project.

&rarr; CMAKE\_CURRENT\_LIST\_DIR comes in handy, when you need to locate resource files like template files or batch scripts that are located next to the CMakeLists.txt file currently being processed.

- **CMAKE\_CURRENT\_BINARY\_DIR:** The directory where the current CMakeLists.txt file generates build files.

## CMake Configure vs Generate

1. **CMake Configure** :
  - The "configure" step is the initial phase of using CMake. This is where CMake analyzes your project's CMakeLists.txt files to gather information about your project, such as compiler settings, build options, and dependencies.
  - During the configure step, CMake checks the availability of required tools and libraries, sets up variables and options, and generates build system files (e.g., Makefiles, Visual Studio project files, or Ninja build files) based on the platform and generator specified by the user.
  - If CMake encounters any issues or missing dependencies during the configure step, it will report errors and ask you to resolve them before proceeding.

<br>

2. **CMake Generate** :
  - The "generate" step is the second phase in the CMake process, and it occurs after the configure step.
  - During the generate step, CMake takes the information gathered during configuration and generates the actual build system files and scripts that will be used to build your project.
  - This step generates the files necessary to compile and link your source code based on the platform and build generator specified during configuration.
  - Once the generate step is complete, you can use the generated build files (e.g., make, MSBuild, or Ninja) to build your project by invoking the appropriate build commands.

<br>

In summary, the "configure" step is where CMake analyzes your project and sets up the build environment, while the "generate" step is where it creates the build system files based on that configuration. These two steps work together to prepare your project for compilation and allow you to build it on **various platforms** and **with different build systems**.

## What is CPack ?

CPack is a powerful, easy to use, cross-platform software packaging tool distributed with CMake. It uses the generators concept from CMake to abstract package generation on specific platforms. It can be used with or without CMake, but it may depend on some software being installed on the system. Using a simple configuration file, or using a CMake module, the author of a project can package a complex project into a simple installer.

## What is Ninja ?

Ninja is a lightweight build system, designed to take advantage of the available CPU cores in your machine, to build executable files and libraries from source code efficiently. The way it works is very similar to GNU Make. That is it takes input files generated by a high-level build system like CMake and turns them into libraries and executables as fast as possible.

## GLFW

GLFW gives us the bare necessities required for rendering goodies to the screen. It allows us to create an OpenGL context, define window parameters, and handle user input.

_Note_: Above operations are specific per operating system and OpenGL purposefully tries to abstract itself from these operations.

## GLAD

Because OpenGL is only really a standard/specification it is up to the driver manufacturer to implement the specification to a driver that the specific graphics card supports. Since there are many different versions of OpenGL drivers, the location of most of its functions is not known at compile-time and needs to be queried at run-time. It is then the task of the developer to retrieve the location of the functions he/she needs and store them in function pointers for later use.

## GLSL vs GLM

- **GLSL (OpenGL Shading Language)** is a language used by OpenGL (syntax based on C) to run programs on the GPU, called _shaders_, which you know the purpose of. They're not even part of your program - instead, they are two files stored in your computer which at runtime are passed to OpenGL and only then they're compiled. It provides advanced math for two reasons: there's no way to load libraries, and because this is _graphics programming_, which is very related to math.
- **GLM (OpenGL Mathematics)** is a C++ library used to extend C++'s math capabilities with functions and types that are commonly used in graphics programming - all this will be executed on the CPU, and it's independent from OpenGL.

## Traditional Libraries (Static or Dynamic) vs Header-only Libraries

- A header-only library in C++ is a library that consists entirely of header files, without any associated source (.cpp) files.

1. **File Structure:**

  - **Traditional Libraries:** Traditional libraries are typically divided into two parts: header files (.h or .hpp) and source files (.cpp). Header files contain declarations of classes, functions, and data structures, while source files contain the actual implementation of these declarations. Users of the library include the necessary header files and link their code against the compiled binary (usually a .lib or .dll file on Windows or a .a or .so file on Unix-like systems).
  - **Header-Only Libraries:** Header-only libraries consist entirely of header files. All the implementation code for the library is in these header files, usually using inline functions and templates. Users include these header files directly in their code.

<br>

2. **Usage:**

  - **Traditional Libraries:** To use a traditional library, users typically include the necessary header files in their code to access the library's interface. They also link their code against the compiled library binary, which provides the actual implementation. This requires the library binary to be built separately and linked during the compilation process.
  - **Header-Only Libraries:** With header-only libraries, users only need to include the header files directly in their code. The implementation code is inlined, so there's no separate binary to link against. This simplifies the build process.

<br>

3. **Build Process:**

  - **Traditional Libraries:** Building and linking traditional libraries involves an additional compilation step to create the library binary. Users need to ensure that they have the correct library binary when compiling their code.
  - **Header-Only Libraries:** Using header-only libraries simplifies the build process because there's no need to create or manage a separate library binary. Users include the headers, and the compiler handles the rest **. Since there is nothing to build to use a header-only library, it increases cross-platform capabilities.**

<br>

4. **Compile Times and Code Size:**

  - **Traditional Libraries:** Traditional libraries can lead to shorter compile times and smaller code size because the implementation code is separate and is only included once in the final binary.
  - **Header-Only Libraries:** Header-only libraries can lead to longer compile times and larger code size because the implementation code is included in every source file that includes the library headers.

<br>

5. **Maintenance:**

  - **Traditional Libraries:** Maintenance of traditional libraries can be more straightforward as changes to the implementation don't require recompiling all code that uses the library. Only the library itself needs recompilation.
  - **Header-Only Libraries:** Maintenance of header-only libraries can be more challenging, as changes to the library headers may necessitate recompilation of all code that includes them. This can lead to longer recompilation times.

<br>

In short, header-only libraries are convenient for small, self-contained libraries but can lead to longer compile times and larger code size. Traditional libraries, on the other hand, provide more modularity and can result in faster compile times and smaller binaries but involve an additional linking step.

## Single-file Libraries

Single-file libraries in C++ are libraries that are designed to be included and used in a C++ project with just a single header file. These libraries aim to simplify the process of adding external functionality to your C++ code by eliminating the need for complex build systems or multi-file setups. Instead, you include the single header file in your C++ code, and you're ready to use the library's features.

## Inline functions

C++ provides inline functions to reduce the function call overhead. An inline function is a function that is expanded in line when it is called. **When the inline function is called, whole code of the inline function gets inserted at the point of the inline function call instead of making a regular function call.** This insertion is performed by the C++ compiler at compile time. An inline function may increase efficiency if it is small.

<br>

**Why Inline Functions are Used?**

For functions that are large and/or perform complex tasks, the overhead of the function call is usually insignificant compared to the amount of time the function takes to run. However, for small, commonly-used functions, the time needed to make the function call is often a lot more than the time needed to actually execute the function's code. This overhead occurs for small functions because the execution time of a small function is less than the switching time.

<br>

**Inlining is Not Guaranteed**

The "inline" keyword is a hint to the compiler, and it's up to the compiler's discretion whether to actually inline the function or not. Modern compilers are quite smart and will make this decision based on optimization settings and the complexity of the function.

## .inl files

In some C++ codebases, developers choose to place the implementations of inline functions or template functions in files with ".inl" extensions. These ".inl" files are typically included at the end of the corresponding header files.

For example, if you have a header file "MyClass.h" with inline functions, you might create a corresponding "MyClass.inl" file that contains the implementations of those functions. Then, in "MyClass.h," you include "MyClass.inl" at the end to provide the function definitions. This separation allows you to keep the declaration and implementation separate while also making the code more readable and maintainable.

When the code is compiled, the ".inl" file is effectively included in the header file, and the inline functions are expanded at their call sites during compilation, just like regular inline functions defined within the header file itself.

## Move Semantics and Smart Pointers

- **unique\_ptr**

A [unique\_ptr](https://learn.microsoft.com/en-us/cpp/standard-library/unique-ptr-class?view=msvc-170) does not share its pointer. It cannot be copied to another unique\_ptr, passed by value to a function, or used in any C++ Standard Library algorithm that requires copies to be made. **A unique\_ptr can only be moved**. This means that the ownership of the memory resource is transferred to another unique\_ptr and the original unique\_ptr no longer owns it. It is recommended that you restrict an object to one owner, because multiple ownership adds complexity to the program logic. Therefore, when you need a smart pointer for a plain C++ object, use unique\_ptr, and when you construct a unique\_ptr, use the [make\_unique](https://learn.microsoft.com/en-us/cpp/standard-library/memory-functions?view=msvc-170#make_unique) helper function.

<br>

- **shared\_ptr**

The shared\_ptr type is a smart pointer in the C++ standard library that is designed for scenarios in which more than one owner might have to manage the lifetime of the object in memory. After you initialize a shared\_ptr you can copy it, pass it by value in function arguments, and assign it to other shared\_ptr instances. All the instances point to the same object, and share access to one "control block" that increments and decrements the reference count whenever a new shared\_ptr is added, goes out of scope, or is reset. When the reference count reaches zero, the control block deletes the memory resource and itself.

<br>

- **weak\_ptr** **(used super rarely)**

Special-case smart pointer for use in conjunction with shared\_ptr. A weak\_ptr provides access to an object that is owned by one or more shared\_ptr instances, but does not participate in reference counting. Use when you want to observe an object, but do not require it to remain alive. Required in some cases to break circular references between shared\_ptr instances.

## \_t data types

In C++, the suffix "\_t" is often used to denote a user-defined type, typically associated with typedef or using declarations. It's a common convention to give such types a name that ends with "\_t" to indicate that they are custom types.

For example, in C++ standard library (STL), you'll often encounter types with names like "size\_t," "int32\_t," "uint64\_t," and so on. These types are typically defined in the **\<cstdint\>** header and are used to represent specific data sizes and types in a **platform-independent** way.

In short, "\_t" suffix doesn't really mean much of anything. It's just how C decided to name things. However, i.e "int32" part allows developers to develop applications with compatibility and consistency between architectures, different compiler versions and different standards.

**size\_t:** It's a type which is used to represent the size of objects in bytes and is therefore used as the return type by the sizeof operator. It is guaranteed to be big enough to contain the size of the biggest object the host system can handle. Basically the maximum permissible size is dependent on the compiler; if the compiler is 32 bit then it is simply a typedef for unsigned int but if the compiler is 64 bit then it would be a typedef for unsigned long long. The size\_t data type is never negative. It is typically used when working with sizes or indices of objects in memory. Some common scenarios where size\_t can be used are array indexing, size and length representations, memory allocation and deallocation, loop counters.

## Using declarations and using directives

First of all, the using keyword is also used to define type aliases, which are unrelated to using-statements.

**Using-declarations**

One way to reduce the repetition of typing std:: over and over is to utilize a using-declaration statement. A using declaration allows us to use an unqualified name (with no scope) as an alias for a qualified name.

For instance, the using-declaration **using std::cout** ; tells the compiler that we're going to be using the object cout from the std namespace. So whenever it sees cout, it will assume that we mean std::cout. If there's a naming conflict between std::cout and some other use of cout, std::cout will be preferred.

<br>

**Using-directives**

Another way to simplify things is to use a using-directive. Slightly simplified, a using directive imports all of the identifiers from a namespace into the scope of the using-directive.

For instance, the using-directive **using namespace std** ; tells the compiler to import all of the names from the std namespace into the current scope. When we then use unqualified identifier cout, it will resolve to the imported std::cout.

<br>

**Best practices for using-statements**

Avoid using-directives (particularly using namespace std;), except in specific circumstances (such as using namespace std::literals to access the s and sv literal suffixes). Using-declarations are generally considered safe to use inside blocks. Limit their use in the global namespace of a code file, and never use them in the global namespace of a header file.

## ``const`` before and after a function

```cpp
const T& get_data() const { return data_; }
```

- **Before a function**

"const" before a function means that the return parameter is const, which only really makes sense if you return a reference or a pointer.

For instance, getters should return by value or const lvalue reference.

```cpp
const std::string& getName() { return m_name; } // getter returns by const reference
```

<br>

- **After a function (const member function)**

"const" after a function means that it will not modify the object or call any non-const member functions (as they may modify the object).

**Note that const member functions can also be called on non-const objects.** Because const member functions can be called on both const and non-const objects, if a member function does not modify the state of the object, it should be made const.

For instance, as getters might be called on const objects, they should really be const member functions.

```cpp
int getDay() const { return m_day; } // const getter member function (can be called on both const and non-const objects)
```

Example:

```cpp
const T& get_data() const { return data_; }
^^^^^
```

means it will return a const reference to T (here data\_)

```cpp
Class c;

T& t = c.get_data() // Not allowed.

const T& tc = c.get_data() // OK.
```

---------------------------------------------

---------------------------------------------

```cpp
const T& get_data() const { return data_; }
                    ^^^^^
```

means the method will not modify any member variables of the class (unless the member is mutable).

```cpp
void Class::get_data() const {

  this->data_ = ... // is not allowed here since get_data() is const (unless 'data_' is mutable)

  this->anything = ... // Not allowed unless the thing is 'mutable'

}
```

## Virtual function vs pure virtual function (abstract function) in C++

**Virtual Function in C++**

A virtual function is a member function which is declared within a base class and is re-defined(Overridden) by a derived class. When you refer to a derived class object using a pointer or a reference to the base class, you can call a virtual function for that object and execute the derived class's version of the function.

<br>

**Pure Virtual Functions in C++**

A pure virtual function (or **abstract function** ) in C++ is a virtual function for which we don't have an implementation, we only declare it. A pure virtual function is declared by assigning 0 in the declaration.

<br>

**Important Note:** In C++, classes or methods are not explicitly declared as abstract. The presence of pure virtual methods is what makes them abstract. In other words, a class is abstract if and only if it contains at least 1 pure virtual function/abstract method.

## #pragma once vs include guards

- **#pragma once** is recommended for new code in C++ as it reduces build times by preventing multiple reads of the same file.
- It's similar in function to include guards, but cleaner and avoids polluting the global namespace.
- Mixing **#pragma once** and include guards is not needed; either one suffices.
- Include guards are useful for portability, consistency, and in complex projects with file system aliasing.
- Be cautious not to use **#pragma once** or include guards in header files meant to be included multiple times, and manage include paths to avoid conflicts.

In essence, prefer **#pragma once** for new code, use include guards for specific scenarios, and be mindful of their usage and include path management.

## Circular Dependency and Forward Declaration

Suppose that you have two classes A and B. If A uses B and conversely then there is a circular dependency. However, the circular dependency maybe subtler. For instance, it may be A that uses B that uses C that uses A.

In C++, if a file "A.h" includes "B.h" then "B.h" cannot include "A.h". The only way for B to use A is to forward declare A, **use pointers or references** on A in the header and finally include "A.h" in "B.cpp".

For example, these three files should compile successfully.

A.h:

```cpp
#pragma once

#include "B.h"

class A {

private:
  B mB;

};
```

B.h:

```cpp
#pragma once

class A; // forward declaration

class B {

private:
  A* mA;

};
```

B.cpp:

```cpp

#include "B.h"

#include "A.h" // We include "A.h" in the cpp file
```

This works well and is often the simpler solution for small projects. Then, why so much hate toward circular dependencies after all?

Several reasons against circular dependencies:

- Compilation time can blow up: when a file in the cycle is changed, all the other files have to be recompiled.
- Prone to errors: as several pieces of code are tightly coupled, a change in one will probably break another.
- Harder to reuse the code: as many files are dependent on each other, if you want to reuse a file in another project, you must also take the other ones.
- Harder to debug: as many pieces of codes are coupled, you will have to look at a lot of files if you want to trace back a bug.

There are many ways to avoid circular dependencies. The most obvious one is to design well the project with independent modules or even to break a big project in smaller libraries. Moreover, it may be okay to have a small circular dependency inside a small module/library but not through the whole project.

## Explicit Type Casting

C++ supports 5 different types of casts: **C-style casts** , **static casts** , **const casts** , **dynamic casts** , and **reinterpret casts**.

1. **C-Style Casts:**
  - The most basic and flexible type of cast.
  - Syntax: **(type)value**.
  - It can perform a wide range of conversions, but it can be risky if used improperly because it doesn't always enforce type safety.

<br>

2. **Static Casts:**
  - Used for basic, well-defined type conversions.
  - Syntax: **static\_cast\<new\_type\>(expression)**.
  - Performs compile-time checks when converting between related types.

<br>

3. **Dynamic Casts:**
  - Used for safe runtime type checking and casting in the context of polymorphism.
  - Syntax: **dynamic\_cast\<new\_type\>(expression)**.
  - Checks if the conversion is valid and returns nullptr for pointers or throws an exception for references if not.

<br>

4. **Const Casts:**
  - Used to add or remove the **const** qualifier from a variable.
  - Syntax: **const\_cast\<new\_type\>(expression)**.
  - Typically used to allow modifying a const object.

<br>

5. **Reinterpret Casts:**
  - Used to perform low-level, unsafe type conversions.
  - Syntax: **reinterpret\_cast\<new\_type\>(expression)**.
  - Often used when converting between unrelated pointer types or working with binary data.

<br>

Generally, you should prefer safer casts like **static\_cast** and **dynamic\_cast** over the less safe **reinterpret\_cast** and use **const\_cast** sparingly to avoid unintended side effects.

<br>

**Note1:** Although a **C-style cast** appears to be a single cast, it can actually perform a variety of different conversions depending on context. This can include a **static cast** , a **const cast** or a **reinterpret cast** (the latter two of which we mentioned above you should avoid). As a result, **C-style casts** are at risk for being inadvertently misused and not producing the expected behavior, something which is easily avoidable by using the C++ casts instead. Also, because **C-style casts** are just a type name, parenthesis, and variable or value, they are both difficult to identify (making your code harder to read) and even more difficult to search for.

<br>

**Note2:** If you need to cast pointers, cast them via **void\*** , and only if absolutely necessary use **reinterpret\_cast** - that means, if you really have to reinterpret the data.

<br>

**How C-style casting works ?**

C-style casting tries to do the following casts, in the following order:

1. **const\_cast**
2. **static\_cast**
3. **static\_cast**  followed by  **const\_cast**
4. **reinterpret\_cast**
5. **reinterpret\_cast** followed by  **const\_cast**

<br>

**Why exactly C-style casts are not safe ?**

Because **they lack compile time type checking** , may lose type information, can be ambiguous, ignore compiler warnings, make code harder to maintain, and can lead to undefined behavior. For safety and clarity, specialized casting operators should be used and C-style casts should be used sparingly and carefully.

## What is extern "C"

In C++, the **extern "C"** declaration is used to indicate that a particular block of code or function should be treated as C code, rather than C++ code. This is primarily used when you want to interface C++ code with code written in other programming languages, especially C.

When you declare a function or code block as **extern "C"** , it affects how the C++ compiler generates the names and linkage for symbols (e.g., functions and variables) within that block. Here's how it works:

1. **Name Mangling:** In C++, function names are often "mangled" by the compiler to include additional information about the function's parameters and return type. This helps distinguish overloaded functions. However, C does not support name mangling, so if you want to call a C++ function from C, you need to disable name mangling. You do this by declaring the function with **extern "C"**.
2. **Calling Convention:** C and C++ compilers may use different calling conventions for functions, which specify how arguments are passed and how the stack is managed during function calls. When you declare a function as **extern "C"** , it tells the C++ compiler to use the C calling convention for that function, ensuring compatibility with C code.

Code example:

```cpp
// C++ code

#ifdef __cplusplus

extern "C" {

#endif

// This function is treated as C code
int add (int a, int b) {
  return a + b;
}

#ifdef __cplusplus

}

#endif
```

## What is ``volatile`` keyword?

The **volatile** keyword is used to indicate that a variable can be changed by some external entity that is not under the control of the program. It informs the compiler that the variable's value may change at any time, even if it appears that there is no code in the program that modifies it and prevents the compiler from applying **aggressive optimization involving the object**.

**Some use-cases**

1. **Hardware Register Access** : When working with embedded systems or device drivers, you often need to access hardware registers. These registers can change their values due to external hardware events, and the **volatile** keyword is used to prevent the compiler from optimizing away reads or writes to these registers.
2. **Interrupt Service Routines (ISRs)**: In real-time systems, ISRs can modify variables that are accessed in the main program. Marking these shared variables as **volatile** ensures that the compiler doesn't optimize away accesses to them.
3. **Multithreading** : In multithreaded programs, when shared variables are accessed by multiple threads, the **volatile** keyword can be used to indicate that a variable is subject to unsynchronized access and should not be optimized by the compiler.
4. **Memory-Mapped I/O** : When working with memory-mapped I/O, where memory locations represent hardware peripherals, the **volatile** keyword is often used to ensure that reads and writes to these locations are not optimized away.

It should be noted that while **volatile** can prevent certain compiler optimizations, it does not provide thread safety or synchronization in multithreaded programs. For thread safety, synchronization mechanisms such as mutexes, locks, or atomic operations should be used.

## What is \_\_declspec and its use-cases?

[\_\_declspec](http://msdn.microsoft.com/en-us/library/dabb5z75.aspx) is a Microsoft-specific attribute that allows you to specify storage-class information. _(However, a number of other compiler vendors—e.g. GCC—now support this language extension for compatibility with the installed base of code that was written targeting Microsoft's compilers. Some even provide additional storage-class attributes.)_

Two of those storage-class attributes that can be specified are  **dllimport**  and  **dllexport**. These indicate to the compiler that a function or object is imported or exported (respectively) from a DLL.

More specifically, they define the DLL's interface to the client without requiring a module-definition (.DEF) file. Most people find it much easier to use these language extensions than to create DEF files.

For obvious reasons, \_\_declspec(dllimport) and \_\_declspec(dllexport) are generally paired with one another. You use dllexport to mark a symbol as exported from a DLL, and you use dllimport to import that exported symbol in another file.

Because of this, and because the same header file is generally used both when compiling the DLL and in client code that consumes the DLL's interface, it is a common pattern to define a macro that automatically resolves to the appropriate attribute specifier at compile-time. For example:

```cpp
#if COMPILING_DLL
  #define DLLEXPORT __declspec(dllexport)
#else
  #define DLLEXPORT __declspec(dllimport)
#endif
```

And then marking all of the symbols that should be exported with DLLEXPORT.

## Storage Classes in C++

A _storage class_ in the context of C++ variable declarations is a type specifier that governs the lifetime, linkage, and memory location of objects. A given object can have only one storage class. Variables defined within a block have automatic storage unless otherwise specified using the  **extern** ,  **static** , or  **thread\_local**  specifiers. Automatic objects and variables have no linkage; they aren't visible to code outside the block. Memory is allocated for them automatically when execution enters the block, and it's de-allocated when the block is exited.

<img src="https://github.com/kostanya/rayjack/assets/73854258/3d1e1814-c1c8-4e65-8c17-905ec0f9628c" height="400" />


**Notes:**

- The [mutable](https://learn.microsoft.com/en-us/cpp/cpp/mutable-data-members-cpp?view=msvc-170) keyword may be considered a storage class specifier. However, it's only available in the member list of a class definition.
- **Visual Studio 2010 and later:**  The  **auto**  keyword is no longer a C++ storage-class specifier, and the  **register**  keyword is deprecated.

## What is CUDA ?

CUDA is a parallel computing platform and programming model that enables developers to write code for NVIDIA GPUs. With CUDA, you can accelerate your code by offloading compute-intensive tasks to the GPU, which can perform many operations in parallel.

**Key Points of CUDA**

1. **Parallel Computing:** CUDA allows developers to write code that can be executed in parallel on NVIDIA GPUs. This is especially useful for computationally intensive tasks, such as scientific simulations, deep learning, image processing, and more.
2. **GPU Acceleration:** By offloading certain parts of a program to the GPU, applications can benefit from significant speedup compared to running the same code on a CPU alone. GPUs are highly parallel processors with thousands of cores, making them well-suited for tasks that can be parallelized.
3. **Programming Model:** CUDA provides a programming model and a set of libraries that enable developers to write code for GPUs using languages like C, C++, and Python. It also includes tools like the CUDA Compiler (nvcc) and the CUDA Runtime, which help manage GPU resources and memory.
4. **Memory Hierarchy:** CUDA offers a specific memory hierarchy on GPUs, including global memory, shared memory, and registers, which developers can use to optimize data access and computation. Efficient memory management is crucial for achieving good GPU performance.
5. **CUDA Cores:** GPUs consist of multiple CUDA cores, which are individual processing units capable of executing threads. The number of CUDA cores varies depending on the GPU model.
6. **Parallelism:** CUDA allows for both data parallelism and task parallelism. Data parallelism involves processing multiple data elements in parallel, while task parallelism involves running multiple independent tasks concurrently.
7. **Ecosystem:** NVIDIA provides a comprehensive ecosystem for CUDA development, including libraries for various domains (e.g., cuBLAS for linear algebra, cuDNN for deep learning, cuFFT for fast Fourier transforms) and developer tools (e.g., NVIDIA Nsight for debugging and profiling).

**How do you use CUDA ?**

With CUDA, developers write programs using an ever-expanding list of supported languages that includes C, C++, Fortran, Python and MATLAB, and incorporate extensions to these languages in the form of a few basic keywords.

These keywords let the developer express massive amounts of parallelism and direct the compiler (or interpreter) to those portions of the application on GPU accelerators.

The simple example below shows how a standard C program can be accelerated using CUDA.

<img src="https://github.com/kostanya/rayjack/assets/73854258/193bdab2-bc37-4fb4-ac3b-540b5eb6ecad" height="350" />

**CUDA Toolkit**

The NVIDIA CUDA Toolkit provides a development environment for creating high performance GPU-accelerated applications. With the CUDA Toolkit, you can develop, optimize, and deploy your applications on GPU-accelerated embedded systems, desktop workstations, enterprise data centers, cloud-based platforms and HPC supercomputers. The toolkit includes GPU-accelerated libraries, debugging and optimization tools, a C/C++ compiler, and a runtime library to deploy your application.

Using built-in capabilities for distributing computations across multi-GPU configurations, scientists and researchers can develop applications that scale from single GPU workstations to cloud installations with thousands of GPUs.

## MinGW vs Cygwin vs MSYS2

Cygwin is an attempt to create a complete UNIX/POSIX environment on Windows. To do this it uses various DLLs. While these DLLs are covered by GPLv3+, their license contains [an exception](http://cygwin.com/licensing.html) that does not force a derived work to be covered by the GPLv3+. MinGW is a C/C++ compiler suite which allows you to create Windows executables without dependency on such DLLs - you only need the normal MSVC runtimes, which are part of any normal Microsoft Windows installation.

You can also get a small UNIX/POSIX like environment, compiled with MinGW called [MSYS](http://www.mingw.org/wiki/msys). It doesn't have anywhere near all the features of Cygwin, but is ideal for programmers wanting to use MinGW.

<br>

1. **MinGW (Minimalist GNU for Windows):**
  - MinGW is a development environment for Windows that focuses on creating native Windows applications.
  - It includes GCC (GNU Compiler Collection) and other GNU development tools.
  - The goal of MinGW is to enable developers to compile and build software that runs natively on Windows without requiring a Unix emulation layer.
  - MinGW provides Windows-specific headers and libraries, allowing developers to create Windows executables -native Windows applications- directly. It therefore acts as an open source alternative to the [Microsoft Visual C++](https://en.wikipedia.org/wiki/Visual_C%2B%2B) suite.

<br>

2. **Cygwin:**
  - Cygwin is a comprehensive Unix-like environment for Windows.
  - It includes a wide range of Unix-like command-line utilities and tools similar to those found in a Unix/Linux environment.
  - Cygwin also provides a Unix-like shell (usually bash) for interactive use.
  - It offers X11 support, which allows running graphical Unix applications on Windows.
  - Cygwin includes a compatibility layer that allows Windows programs to interact with Cygwin programs, making it possible to use both Windows and Unix-like tools together seamlessly.

<br>

3. **MSYS (Minimal SYStem):**
  - MSYS is a lightweight Unix-like environment for Windows.
  - It provides a minimal set of Unix-like command-line utilities, such as bash (a Unix-like shell), ls, cp, rm, etc.
  - Its primary purpose is to facilitate the building and running of Unix-like software on Windows.
  - MSYS doesn't include a full suite of Unix utilities but offers enough to work with common development tasks.
  - It is often used in combination with MinGW to compile Windows applications using GCC, where MSYS provides the Unix-like command-line environment necessary for the build process.

<br>

In essence, **MSYS and Cygwin aim to bring Unix-like functionality to Windows, while MinGW is primarily focused on native Windows application development using Unix tools like GCC**. The choice depends on your specific needs – whether you want a lightweight Unix-like environment, Windows-native development, or a full Unix-like experience on Windows.

## Shallow Copy vs Deep Copy

Depending upon the resources like [dynamic memory](https://www.geeksforgeeks.org/what-is-dynamic-memory-allocation/) held by the object, either we need to perform  **Shallow Copy**  or  **Deep Copy** in order to create a replica of the object. In general, if the variables of an object have been dynamically allocated, then it is required to do a Deep Copy in order to create a copy of the object.

**Shallow Copy:** A shallow copy of an object **shares** some or all of its internal elements with the original object. Changes made to the copied object affect the original object, and vice versa. Shallow copies are often created using copy constructors or assignment operators and are more memory-efficient because they don't duplicate data.

_Note1:_ This works well if none of the variables of the object are defined in the [heap section of memory](https://www.geeksforgeeks.org/stack-vs-heap-memory-allocation/). If some variables are dynamically allocated memory from heap section, then the copied object variable will also reference the same memory location. This will create ambiguity and run-time errors, dangling pointer.

_Note2:_ [C++](https://www.geeksforgeeks.org/c-plus-plus/) compiler implicitly creates a [copy constructor](https://www.geeksforgeeks.org/copy-constructor-in-cpp/) and [overloads assignment operator](https://www.geeksforgeeks.org/assignment-operator-overloading-in-c/) in order to perform shallow copy at compile time.

**Deep Copy:** A deep copy of an object duplicates all of its internal elements, creating independent copies. Changes made to the copied object do not affect the original object, and vice versa. Deep copies require manually copying each element, which can be less memory-efficient but ensures data isolation.
