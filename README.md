# Socket Server with SSL and Database Logging

## Description

This project is a basic C++ socket server that utilizes SSL (Secure Sockets Layer) for secure communication between the server and clients. The server also logs the IP address and connection time of each client into an SQLite3 database. This project serves as a foundation for further development, such as adding request handling, authentication mechanisms, or multi-client support.

The current implementation focuses on secure socket communication, logging client connection details, and providing a base structure to build upon.

## Key Features

- **SSL-encrypted communication**: The server establishes secure connections with clients using SSL/TLS encryption to ensure data privacy and integrity.
- **SQLite3 Database Logging**: Client IP address and connection timestamp are logged into an SQLite3 database for auditing purposes.
- **Socket Communication**: The server listens for incoming client connections through a socket interface and supports basic functionality for handling these connections.

## Requirements

To run this project on a Windows machine, you'll need the following:

- **C++ compiler** (e.g., MinGW or Visual Studio).
- **OpenSSL**: A library for SSL/TLS support in C++.
- **SQLite3**: A lightweight database engine for logging client connections.

### Installing Dependencies

1. **OpenSSL**:
   - Download OpenSSL for Windows from [here](https://slproweb.com/products/Win32OpenSSL.html).
   - Make sure to install the OpenSSL libraries and development files.
   - Add the OpenSSL bin directory to your system’s `PATH` environment variable so that you can use the OpenSSL command line tools.

2. **SQLite3**:
   - Download SQLite3 for Windows from the [official website](https://www.sqlite.org/download.html).
   - Extract the SQLite3 DLL (`sqlite3.dll`) to your project folder or system directory.

3. **C++ Compiler**:
   - If you are using MinGW, you can download it from [here](https://sourceforge.net/projects/mingw/).
   - Alternatively, you can use **Visual Studio** (ensure to install the C++ development tools during installation).

## Installation Instructions

1. Clone the repository to your local machine:
   ```bash
   git clone [https://github.com/your-repository.git](https://github.com/warjohn/Remote-managment-Server.git)

## Certs
    server.crt: Server cert, signed with the root certificate.
    server.pub: The server's public key.
    server.key: Server's private key.
    server.csr: Certificate Signing Request.
   
    ca.key: The private key of the certification authority (CA).
    ca.crt: The root certificate of the certification authority (CA).
