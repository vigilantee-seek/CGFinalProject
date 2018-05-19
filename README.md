# CGFinalProject
This is the final project of the course Computer Graphics. 

### Project Statement

----

### File Structure

---

### Building Approach

#### Linux

With a series of external libraries: GL, GLU and glut.

```shell
gcc xxx.c -o xxx -l GL -l GLU -l glut
```

or C++ file:

```shell
g++ xxx.cpp -o xxx -l GL -l GLU -l glut
```

**Attention**: You should first launch the X server to enable the graphics display before executing the executable file. Otherwise, you would encounter a "missing display" error.

For a WSL (Windows Subsystem Linux) specially, since no graphics driver software is available, you need to start a similar server (e.g. Xming) in your Windows and set the display port. And add an environment variable in your **.bashrc** file. **Here I suppose the display port is set to 0**.

Then in you last line of you .bashrc, add:

```shell
export DISPLAY=:0
```

 To make it take effect right now:

```shell
source ~/.bashrc
```

#### Windows

Just build the main.cpp/main.c file in your C++/C IDE/compiler (Visual Studio for instance).

---

Welcome you to find bugs and submit PR! Thank you!!