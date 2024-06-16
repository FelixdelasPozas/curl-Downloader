curl Downloader
===============

# Summary
- [Description](#description)
- [Compilation](#compilation-requirements)
- [Install](#install)
- [Screenshots](#screenshots)
- [Repository information](#repository-information)

# Description
Simple curl frontend focused on downloading files using SOCK4/SOCK5 protocols via proxy servers with infinite retries. 

## Options
Instead of using libcurl an external curl executable is needed and its location must be entered in the configuration dialog, with the download folder and the time between retries. When adding a new item only the download url, proxy server and port of the item can be configured, no other curl options are available.

# Compilation requirements
## To build the tool:
* cross-platform build system: [CMake](http://www.cmake.org/cmake/resources/software.html).
* compiler: [Mingw64](http://sourceforge.net/projects/mingw-w64/) on Windows.

## External dependencies
The following libraries are required:
* [Qt 5 opensource framework](http://www.qt.io/).

# Install
The only current option is build from source as binaries are not provided. 

# Screenshots
Main dialog with a console output dialog of one of the files being downloaded. The progress of each download is represented in the green background. For each download a widget with the name, status (downloading, retrying, etc...), remaining time, speed (in bytes usually, uses curl units), progress value and buttons to show the console output and cancel the download is shown.

![maindialog](https://github.com/FelixdelasPozas/curl-Downloader/assets/12167134/48f1d574-fc60-4546-bf35-3a42bd7f0db3)

Add item dialog. If the server is left empty the file will be downloaded using no proxy. Otherwise the protocols SOCKS4 and SOCKS5 are available for download. Once a file is being downloaded the server and port can be changed by right-clicking the item information widget in the main dialog. 

![additem](https://github.com/FelixdelasPozas/curl-Downloader/assets/12167134/89b25e1c-e1b4-4d4c-84d3-019f1eb78387)

Configuration dialog. The curl executable is checked to ensure the configuration is valid, same with the download folder. 

![configuration](https://github.com/FelixdelasPozas/curl-Downloader/assets/12167134/a12e1a92-e75f-4131-afb8-bbbd619f48c2)

The application can be minimized to the tray area.

![tray](https://github.com/FelixdelasPozas/curl-Downloader/assets/12167134/633e1064-930a-4bc0-ab17-0fb4bae3ef00)

# Repository information

**Version**: 1.0.1

**Status**: finished

**cloc statistics**

| Language                     |files          |blank        |comment           |code  |
|:-----------------------------|--------------:|------------:|-----------------:|-----:|
| C++                          |   8           |  216        |  198             | 811  |
| C/C++ Header                 |   7           |  130        |  373             | 250  |
| CMake                        |   1           |   12        |    3             |  61  |
| **Total**                    | **16**        | **358**     | **574**          | **1122** |
