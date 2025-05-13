curl Downloader
===============

# Summary
- [Description](#description)
- [Compilation](#compilation-requirements)
- [Install](#install)
- [Screenshots](#screenshots)
- [Repository information](#repository-information)

# Description
Simple curl frontend focused on downloading files using SOCK4/SOCK5 protocols via proxy servers with infinite retries. If the proxy server stops working a new one can be entered by clicking in the item download bar in the main dialog. The download will resume with the new proxy server.

If you want to support this project you can do it on [Ko-fi](https://ko-fi.com/felixdelaspozas).

## Options
Instead of using libcurl an external curl executable is needed and its location must be entered in the configuration dialog, with the download folder, the time between retries and the temporal extension to use while downloading. When adding a new item only the download url, proxy server and port of the item can be configured, no other curl options are available.

# Compilation requirements
## To build the tool:
* cross-platform build system: [CMake](http://www.cmake.org/cmake/resources/software.html).
* compiler: [Mingw64](http://sourceforge.net/projects/mingw-w64/) on Windows.

## External dependencies
The following libraries are required:
* [Qt Library](http://www.qt.io/).

You'll need to have a curl executable for windows, you can download one from [here](https://curl.se/windows/).

# Install

Curl Downloader is available for Windows 10 onwards. You can download the latest release from the [releases page](https://github.com/FelixdelasPozas/curl-Downloader/releases). Neither the application or the installer are digitally signed so the system will ask for approval before running it the first time.

# Screenshots
Main dialog with a console output dialog of one of the files being downloaded. The progress of each download is represented in the green background. 

For each download a widget with the name, status (downloading, retrying, etc...), remaining time, speed (in bytes usually, uses curl units), progress value and buttons to pause/restart the download, show the console output and cancel the download is shown.

![maindialog](https://github.com/user-attachments/assets/abc3013f-749c-461b-8a5b-52db86553002)

Add item dialog. If the server is left empty the file will be downloaded using no proxy. Otherwise the protocols SOCKS4 and SOCKS5 are available for download. Once a file is being downloaded the server, port and output name on local disk can be changed by right-clicking the item information widget in the main dialog. The proxy server can be changed by clicking in the download bar of the item and entering a new one. The download will resume using the new one.

![additem](https://github.com/user-attachments/assets/cd78db32-2cc1-47a5-ae4b-a36cc210a717)

Configuration dialog. The curl executable is checked to ensure the configuration is valid, same with the download folder. An optional temporal extension can be set, and the file will be renamed to the original file name when finished downloading.

![configuration](https://github.com/FelixdelasPozas/curl-Downloader/assets/12167134/f313bd02-07b8-499e-a828-8ea5e8fe3a26)

The application can be minimized to the tray area. If visible the global progress is shown in the taskbar button.

![tray](https://github.com/user-attachments/assets/234d1def-b05f-4b88-aeed-d7d40c31115a)

# Repository information

**Version**: 1.5.4

**Status**: finished

**cloc statistics**

| Language      |files      |blank      |comment    |code      |
|:--------------|----------:|----------:|----------:|---------:|
| C++           |   9       |  273      |  248      | 1157     |
| C/C++ Header  |   9       |  182      |  535      | 578      |
| CMake         |   1       |   14      |    3      |  57      |
| **Total**     | **18**    | **469**   | **786**   | **1792** |
