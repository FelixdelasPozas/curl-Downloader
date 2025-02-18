/*
 File: curlErrors.h
 Created on: 11/02/2025
 Author: Felix de las Pozas Alvarez

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CURL_ERROR_CODES_H_
#define _CURL_ERROR_CODES_H_

// Qt
#include <QString>

const QString curlErrorCodeToText(const int errorCode)
{
  switch (errorCode)
  {
  case 0:
    return "Success. The operation completed successfully according to the instructions";
  case 1:
    return "Unsupported protocol. This build of curl has no support for this protocol";
  case 2:
    return "Failed to initialize";
  case 3:
    return "URL malformed. The syntax was not correct";
  case 4:
    return "A feature or option that was needed to perform the desired request was not enabled or was explicitly disabled at build-time. To make curl able to do this, you probably need another build of libcurl";
  case 5:
    return "Could not resolve proxy. The given proxy host could not be resolved";
  case 6:
    return "Could not resolve host. The given remote host could not be resolved";
  case 7:
    return "Failed to connect to host";
  case 8:
    return "Weird server reply. The server sent data curl could not parse";
  case 9:
    return "FTP access denied. The server denied login or denied access to the particular resource or directory you wanted to reach. Most often you tried to change to a directory that does not exist on the server";
  case 10:
    return "FTP accept failed. While waiting for the server to connect back when an active FTP session is used, an error code was sent over the control connection or similar";
  case 11:
    return "FTP weird PASS reply. curl could not parse the reply sent to the PASS request";
  case 12:
    return "During an active FTP session while waiting for the server to connect back to curl, the timeout expired";
  case 13:
    return "FTP weird PASV reply, curl could not parse the reply sent to the PASV request";
  case 14:
    return "FTP weird 227 format. curl could not parse the 227-line the server sent";
  case 15:
    return "FTP cannot use host. Could not resolve the host IP we got in the 227-line";
  case 16:
    return "HTTP/2 error. A problem was detected in the HTTP2 framing layer. This is somewhat generic and can be one out of several problems, see the error message for details";
  case 17:
    return "FTP could not set binary. Could not change transfer method to binary";
  case 18:
    return "Partial file. Only a part of the file was transferred";
  case 19:
    return "FTP could not download/access the given file, the RETR (or similar) command failed";
  case 21:
    return "FTP quote error. A quote command returned error from the server";
  case 22:
    return "HTTP page not retrieved. The requested URL was not found or returned another error with the HTTP error code being 400 or above. This return code only appears if --fail is used";
  case 23:
    return "Write error. curl could not write data to a local filesystem or similar";
  case 25:
    return "Failed starting the upload. For FTP, the server typically denied the STOR command";
  case 26:
    return "Read error. Various reading problems";
  case 27:
    return "Out of memory. A memory allocation request failed";
  case 28:
    return "Operation timeout. The specified time-out period was reached according to the conditions";
  case 30:
    return "FTP PORT failed. The PORT command failed. Not all FTP servers support the PORT command, try doing a transfer using PASV instead";
  case 31:
    return "FTP could not use REST. The REST command failed. This command is used for resumed FTP transfers";
  case 33:
    return "HTTP range error. The range request did not work";
  case 34:
    return "HTTP post error. Internal post-request generation error";
  case 35:
    return "SSL connect error. The SSL handshaking failed";
  case 36:
    return "Bad download resume. Could not continue an earlier aborted download";
  case 37:
    return "FILE could not read file. Failed to open the file. Permissions?";
  case 38:
    return "LDAP cannot bind. LDAP bind operation failed";
  case 39:
    return "LDAP search failed";
  case 41:
    return "Function not found. A required LDAP function was not found";
  case 42:
    return "Aborted by callback. An application told curl to abort the operation";
  case 43:
    return "Internal error. A function was called with a bad parameter";
  case 45:
    return "Interface error. A specified outgoing interface could not be used";
  case 47:
    return "Too many redirects. When following redirects, curl hit the maximum amount";
  case 48:
    return "Unknown option specified to libcurl. This indicates that you passed a weird option to curl that was passed on to libcurl and rejected. Read up in the manual";
  case 49:
    return "Malformed telnet option";
  case 52:
    return "The server did not reply anything, which here is considered an error";
  case 53:
    return "SSL crypto engine not found";
  case 54:
    return "Cannot set SSL crypto engine as default";
  case 55:
    return "Failed sending network data";
  case 56:
    return "Failure in receiving network data";
  case 58:
    return "Problem with the local certificate";
  case 59:
    return "Could not use specified SSL cipher";
  case 60:
    return "Peer certificate cannot be authenticated with known CA certificates";
  case 61:
    return "Unrecognized transfer encoding";
  case 63:
    return "Maximum file size exceeded";
  case 64:
    return "Requested FTP SSL level failed";
  case 65:
    return "Sending the data requires a rewind that failed";
  case 66:
    return "Failed to initialize SSL Engine";
  case 67:
    return "The username, password, or similar was not accepted and curl failed to log in";
  case 68:
    return "File not found on TFTP server";
  case 69:
    return "Permission problem on TFTP server";
  case 70:
    return "Out of disk space on TFTP server";
  case 71:
    return "Illegal TFTP operation";
  case 72:
    return "Unknown TFTP transfer ID";
  case 73:
    return "File already exists (TFTP)";
  case 74:
    return "No such user (TFTP)";
  case 77:
    return "Problem reading the SSL CA cert (path? access rights?)";
  case 78:
    return "The resource referenced in the URL does not exist";
  case 79:
    return "An unspecified error occurred during the SSH session";
  case 80:
    return "Failed to shut down the SSL connection";
  case 82:
    return "Could not load CRL file, missing or wrong format (added in 7.19.0)";
  case 83:
    return "Issuer check failed (added in 7.19.0)";
  case 84:
    return "The FTP PRET command failed";
  case 85:
    return "Mismatch of RTSP CSeq numbers";
  case 86:
    return "Mismatch of RTSP Session Identifiers";
  case 87:
    return "Unable to parse FTP file list";
  case 88:
    return "FTP chunk callback reported error";
  case 89:
    return "No connection available, the session is queued";
  case 90:
    return "SSL public key does not matched pinned public key";
  case 91:
    return "Invalid SSL certificate status";
  case 92:
    return "Stream error in HTTP/2 framing layer";
  case 93:
    return "An API function was called from inside a callback";
  case 94:
    return "An authentication function returned an error";
  case 95:
    return "A problem was detected in the HTTP/3 layer. This is somewhat generic and can be one out of several problems, see the error message for details";
  case 96:
    return "QUIC connection error. This error may be caused by an SSL library error. QUIC is the protocol used for HTTP/3 transfers";
  case 97:
    return "Proxy handshake error";
  case 98:
    return "A client-side certificate is required to complete the TLS handshake";
  case 99:
    return "Poll or select returned fatal error";
  case 100:
    return "A value or data field grew larger than allowed";
  default:
    break;
  }

  return "Unspecified error";
}

#endif