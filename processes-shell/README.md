From observation of the test cases, the .in files contains the real programs used to test the wish.c program. Hence, when the .run files are passed to **wish.c**, it contains the direction to the .in file for that test case. The point is, my program is not able to give the required output if the .run file is passed to it first. However, if the .in files are passed to it, it works just fine.

### BATCH MODE

Below is the command to test for the batch mode of the program. The batch mode does not take any input. When the program runs, one can now try out the in-built commands like the "cd", "exit" and "path".

```
prompt> ./wish

```

### INTERACTIVE MODE

To test the interactive mood of the **wish.c** program, pass the .in files to it directly. Below is an example of how I tested it with 1.in file.

```
prompt> ./wish tests/1.in
```

When you test it with the .in files for all the 22 files, it runs as expected. 