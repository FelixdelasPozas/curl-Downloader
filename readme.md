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
Main dialog with a console output dialog of one of the files being downloaded. The progress of each download is represented in the green background. 

For each download a widget with the name, status (downloading, retrying, etc...), remaining time, speed (in bytes usually, uses curl units), progress value and buttons to pause/restart the download, show the console output and cancel the download is shown.

![maindialog](https://github.com/FelixdelasPozas/curl-Downloader/assets/12167134/75049dbc-cc80-413b-a9cf-ff700c8cee01)

Add item dialog. If the server is left empty the file will be downloaded using no proxy. Otherwise the protocols SOCKS4 and SOCKS5 are available for download. Once a file is being downloaded the server, port and output name on local disk can be changed by right-clicking the item information widget in the main dialog. 

![additem](https://github.com/user-attachments/assets/cd78db32-2cc1-47a5-ae4b-a36cc210a717)

Configuration dialog. The curl executable is checked to ensure the configuration is valid, same with the download folder. An optional temporal extension can be set, and the file will be renamed to the original file name when finished downloading.

![configuration](https://github.com/FelixdelasPozas/curl-Downloader/assets/12167134/f313bd02-07b8-499e-a828-8ea5e8fe3a26)

The application can be minimized to the tray area. If visible the global progress is shown in the taskbar button.

![tray](https://github.com/user-attachments/assets/03537053-d1ca-4ada-b12d-25bf91db6fb8)

# Repository information

**Version**: 1.3.3

**Status**: finished

**cloc statistics**

| Language                     |files          |blank        |comment           |code  |
|:-----------------------------|--------------:|------------:|-----------------:|-----:|
| C++                          |   8           |  230        |  202             | 943  |
| C/C++ Header                 |   7           |  137        |  398             | 269  |
| CMake                        |   1           |   12        |    3             |  61  |
| **Total**                    | **16**        | **379**     | **603**          | **1273** |
