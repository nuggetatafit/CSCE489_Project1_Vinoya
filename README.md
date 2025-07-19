# AFIT-CSCE489-PROJ1 - Building a Linux Shell

![AFIT](https://www.afit.edu/images/seals/fp_ENG_w.png)

**By 2d Lt Nathaniel Vinoya**

**CSCE489 - Project #1**

**18 July 2025**

------------------

## Introduction

The purpose of this project was to get introduced to creating a shell in Linux that executes the following commands within its terminal:

`create <name>` - creates new files in the current directory

`update <name> <number> <text>` - update said files with a line of text a certain number of times

`list <name>` - lists out the contents of the file and throws an error if the file does not exist

`dir` - lists out what is within the current directory (which is to include the newly created files)

`halt` - terminates the shell


The files of interest include the following:
`myshell.c` - this .c file houses the main function that operates the command loop that is written out in `shellfuncts.c`

`shellfuncts.c` - this .c file contains all of the code logic that makes the shell work; I would say that this is where a lot of the meat for this project is

`shellfuncts.h` - this .h file is the header file that is used to declare the functions that are within `shellfuncts.c`

I am proud to say that this is the first ever shell and big project that I have done using Linux. I hope you enjoy!

------------------

## Components of `shellfuncts.c`

This code starts off by including all of the required c libraries to make the overall functionality work. It also includes the `shellfuncts.h` file accordingly. I then defined the required constants and buffers accordingly. The following are what the actual functions do themselves:

`trim_whitespace` - This helper function trims the whitespace from the end of a string for parsing, making command matching easier.

`cmd_loop` - This is the main command loop for the shell; it is what makes the shell interactive. It starts by prompting the user with the `nutshell> `, reading the input using `fgets()` and handling end-of-file signals accordingly, parsing and deciding using `sscanif` what the user wants to do, forking a child process for valid commands, delegating the child process with a command to execute, and then the parent process waits for the child to finish its task (unless it is a background command). This cycle is repeated until the program is terminated.

`cmd_create` - This function tries to first see if the file exists. It then creates a new file if that is not the case.

`cmd_update` - This function first opens the file, then it writes text in a certain amount of times, and afterwards closes. If there is trouble opening a certain file to append it, it throws an error.

`cmd_list` - This function opens the file in read mode and runs `/bin/cat` and `cat` commands. If the file does not exist, it throws an error.

`cmd_dir` - This function is essentially executing the `/bin/ls/` and `ls` commands to open the directory.


------------------

## How to Use

You can use this shell by doing the following:

1. Download the contents within this GitHub repository
2. Type `cd ./CSCE489_Project1`
3. Compile the code using `make clean; make`
4. Run `./myshell`

------------------

## Limitations

After testing out the new shell, I learned that it if I run the `update <name> <number> <text>` command without ever creating the file, it automatically creates a new file and updates it without me ever creating the file in the first place. In addition, I had a hard time using the tokenization approach, so I opted to use the `sscanf` approach; I know the tokenization `strtok()` approach could make this code a lot more efficient.

------------------

## Questions and Answers

*1. Why was it necessary that your shell use the fork command before running the exec() system call (5 pts)?*

This was necessary in order to preserve the original shell process, or keep it running after it completes a command. The `fork()` system call creates a child process with its own PID that runs an exact process that is copied from the parent. The `exec()` system call is used to overwrite the cloned child process; it replaces the memory space of the process when called with a new program. Thus, using the `exec()` system call before the `fork()` system call would essentially terminate the original process after command execution.

*2. Explain, in as much detail as possible, what happened when you called the fprintf function (10 pts)? Keep in mind, I'm looking for an answer that discusses the process from when the function is entered, through the different "layers" until the function is complete.*

To my understanding, the `fprintf` function from an OS I/O library (called `<stdio>`) that prints out a specific message to file stream (I like to think of it as analogous to sending an email to, say, my boss). The output string is formatted according to the specified format string and arguments (kind of like how emails have signature blocks, greetings, and CUI/Distro A, etc.).

In theory, it goes through different layers, in particular from the user space all the way up to the hardware, given that it relates to the I/O system. At the user space, it formats my string and writes it into a memory buffer within the process's own memory space for efficiency (buffered I/O). After being in the memory buffer for some time, it is flushed into the kernel (in my code I used `fflush`), which ultimately makes a system call (typically `write()`) that requests the kernel to perform a privileged action that allows the data to transition to the kernel. After transitioning to the kernel mode, the OS determines which device the data should be sent to, in which it is sent to the device driver that has the capability to communicate with the hardware. The device driver then sends the commands to the hardware, which in turn writes the passed data to the disk. This sends a signal all the way back to the user-space, where `fprintf` is completed.

------------------

## Final Note

If you have any questions, comments, or concerns, feel free to contact me through the following means:

**AU Teams:** nathaniel.vinoya.1@au.af.edu

**.mil Email:** nathaniel.vinoya.1@us.af.mil

**AFIT Email:** nathaniel.vinoya@afit.edu

------------------

## References

[Markdown Cheat Sheet](https://data.templateroller.com/pdf_docs_html/2637/26371/2637133/page_1_thumb_950.png)

[Markdown Math Notations](https://www.upyesp.org/posts/makrdown-vscode-math-notation/)

[GeeksforGeeks for C Programming](https://www.geeksforgeeks.org)

[w3schools for C Programming](https://www.w3schools.com/)

[man7 for C Programming](https://man7.org/linux/man-pages/index.html)

[GNU for C Programming](https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html)

[Medium for stdin/stdout/stderr](https://medium.com/@c2w.extras/mastering-input-output-and-error-handling-in-c-programming-using-stdin-stdout-and-stderr-3b7cc426ef51)

[Why device driver programs can use fprintf (for Question #2)](https://www.linuxquestions.org/questions/linux-software-2/why-device-driver-programs-can-use-fprintf-4175443108/)

[Scaler fprintf() in C (for Question #2)](https://www.scaler.com/topics/fprintf-in-c/)

[OS Textbook (for Questions #1 and #2)](https://drive.google.com/file/d/1AFRyycszmrdGeOywOkMrV1CxjNg0Qj7P/view?usp=sharing)

[GeeksforGeeks for System Calls in C (for Question #2)](https://www.geeksforgeeks.org/operating-systems/introduction-of-system-call/)

[man7 for fprintf (for Question #2)](https://man7.org/linux/man-pages/man3/fprintf.3.html)

