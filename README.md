# MyShell
MyShell - custom Linux shell for process management

## Building and running
`gcc -o myshell myshell.c -std=c99 -Wall`

`./myshell`


## Running a simple start, wait and run example

`./myshell < startScript`

## Running a simple kill, stop, continue example

`./myshell`
`start ./writeProgram`
`stop <pid>`
`continue <pid>`
`kill <pid>`

## Running a simple SIGSEGV, SIGABRT, SIGILL, SIGFPE example

`./myshell < signalScript`


## Authors
- [André Paiva](https://github.com/xornotor)
- [Ariel Andrade](https://github.com/sudoariel)
- [Natan Moura](https://github.com/ntsmoura)
- [Pedro Correia](https://github.com/Chiiip)
