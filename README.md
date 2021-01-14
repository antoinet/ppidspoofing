# Parent Process ID (PPID) Spoofing

This is a simple demonstration of PPID spoofing. For details, refer to @zodiacon's blog post
[Parent Process vs Creator Process](https://scorpiosoftware.net/2021/01/10/parent-process-vs-creator-process/).

## Building
Import the solution into Visual Studio Community 2019 and build.

## Running
```
Usage: PPIDSpoofing.exe [ppid] [cmdline]
```

For example, launching notepad under PPID 7000:
```
C:\Users\IEUser\source\repos\PPIDSpoofing\Release>PPIDSpoofing.exe 7000 notepad.exe
```
![screenshot](/screenshot.png)