# Simple C++ TCP Web Server

## Overview

This project implements a feature-rich, single-threaded TCP web server written in C++ using POSIX socket programming. While the original implementation was a basic server serving single requests, this enhanced version introduces sophisticated functionality including configuration management, comprehensive HTTP handling, security features, and a modular architecture designed for educational purposes and real-world application understanding.

## Enhanced Architecture

The server follows a layered, modular architecture pattern:

```
┌─────────────────────────────────────────────┐
│              Application Layer              │
│  ┌─────────────┐ ┌─────────────┐            │
│  │   main.cpp  │ │   config    │            │
│  │   (Server   │ │   system    │            │
│  │    Loop)    │ │             │            │
│  └─────────────┘ └─────────────┘            │
├─────────────────────────────────────────────┤
│                HTTP Layer                   │
│  ┌─────────────┐ ┌─────────────┐            │
│  │   request   │ │  response   │            │
│  │   parsing   │ │ generation  │            │
│  └─────────────┘ └─────────────┘            │
├─────────────────────────────────────────────┤
│              Network Layer                  │
│  ┌─────────────┐ ┌─────────────┐            │
│  │   Server    │ │   Utilities │            │
│  │   Socket    │ │   & Logging │            │
│  └─────────────┘ └─────────────┘            │
├─────────────────────────────────────────────┤
│               System Layer                  │
│  ┌─────────────┐ ┌─────────────┐            │
│  │  POSIX      │ │  File I/O   │            │
│  │  Sockets    │ │  & Security │            │
│  └─────────────┘ └─────────────┘            │
└─────────────────────────────────────────────┘
```

## Core Components and Enhanced Functionality

### 1. Enhanced Server Class (`Networking/server.h/.cpp`)

**Beyond Basic Socket Management:**

**Constructor Enhancement:**
- **Port Configuration**: Dynamically configurable port from config file
- **Socket Options**: Advanced socket configuration including `SO_REUSEADDR` for development
- **Timeout Management**: Configurable receive timeouts (`SO_RCVTIMEO`) for client connections
- **Buffer Management**: Configurable send/receive buffer sizes
- **Address Binding**: Flexible IP address binding (INADDR_ANY for all interfaces)

**Advanced Client Management:**
- **IP Address Extraction**: Uses `inet_ntop()` for human-readable client IP addresses
- **Connection Logging**: Detailed logging of client connections with IP and port information
- **Timeout Handling**: Graceful handling of client connection timeouts
- **Resource Cleanup**: RAII-based automatic resource management

**Security Enhancements:**
- **Error Handling**: Comprehensive error checking with `errno` reporting
- **Resource Protection**: Disabled copy/move constructors to prevent resource issues
- **Socket Validation**: Multiple layers of socket validation and error recovery

### 2. HTTP Request Processing (`Networking/request.h/.cpp`)

**Advanced HTTP Parsing:**
- **Method Extraction**: Parses HTTP methods (GET, POST, etc.) from request line
- **Endpoint Processing**: Extracts and validates URL endpoints
- **Header Parsing**: Structured parsing of HTTP request headers
- **Buffer Management**: 4KB buffer with bounds checking and null termination
- **Request Validation**: Validates request format and structure

**Features:**
- **Carriage Return Handling**: Proper handling of `\r\n` line endings
- **Method Detection**: Supports multiple HTTP methods (framework for POST included)
- **URL Processing**: Safe URL handling with validation

### 3. HTTP Response Generation (`Networking/response.h/.cpp`)

**Comprehensive Response Handling:**

**Content Serving:**
- **Static File Serving**: Serves HTML, CSS, JavaScript, images (PNG, JPG), and text files
- **MIME Type Detection**: Automatic Content-Type headers based on file extensions
- **File Extension Parsing**: Smart extension detection from file paths
- **Root Route Handling**: Special handling for "/" route serving index.html

**Security Features:**
- **Path Traversal Protection**: Blocks `../` patterns to prevent directory traversal attacks
- **File Path Validation**: `checkFilePath()` function validates all file access attempts
- **URL Decoding**: Safe URL decoding with `urlDecode()` function
- **Extension Whitelisting**: Only serves approved file types

**HTTP Protocol Support:**
- **Status Code Management**: Comprehensive HTTP status codes (200, 400, 403, 404, 408, 413, 415, 500, etc.)
- **Header Generation**: Proper HTTP headers including Content-Length, Content-Type, Connection
- **Error Pages**: Structured HTML error pages for all error conditions
- **Content-Length Calculation**: Accurate byte counting for response bodies

**Advanced Response Features:**
- **File Reading**: Efficient file reading with `std::stringstream`
- **Response Assembly**: Structured response building (status + headers + body)
- **Connection Management**: Proper connection closure headers

### 4. Configuration System (`utils/config.h/.cpp`)

**INI-Style Configuration:**
- **Section-Based Organization**: `[server]`, `[development]`, `[document]` sections
- **Type-Safe Parsing**: Automatic detection and conversion of string vs. numeric values
- **Multiple Data Types**: Support for strings, numbers, and boolean values
- **Default Value Handling**: Graceful fallback for missing configuration options

**Configuration Parameters:**
- **Server Settings**: Port, host, buffer sizes, timeouts
- **Development Options**: Address reuse settings
- **Document Paths**: Configurable paths for templates, images, styles
- **Runtime Configuration**: No recompilation needed for configuration changes

### 5. Advanced Logging System (`utils/log.h/.cpp`)

**Structured Logging:**
- **Log Levels**: `[LOG]` for standard operations, `[ERROR]` for error conditions
- **Error Context**: Automatic `errno` reporting for system call failures
- **Detailed Messages**: Comprehensive logging of server operations, client connections, file operations

**Logging Features:**
- **Connection Tracking**: Logs client IP addresses, socket descriptors, connection events
- **File Operations**: Logs file access attempts, successful reads, security violations
- **Error Reporting**: Detailed error messages with system error codes

### 6. MIME Type and Status Code Management (`utils/mappings.h/.cpp`)

**Comprehensive Mappings:**
- **MIME Types**: Support for HTML, CSS, JavaScript, JSON, images (PNG, JPG, GIF), and text files
- **HTTP Status Codes**: Complete implementation of 4xx and 5xx error codes
- **File Path Mapping**: Configuration-driven file path resolution
- **Error Message Mapping**: Structured error message generation

### 7. Enhanced Main Server Loop (`src/main.cpp`)

**Robust Request Handling:**
- **Connection Management**: Infinite loop with proper connection handling
- **Error Recovery**: Graceful handling of client disconnections and timeouts
- **Request Size Validation**: Protection against oversized requests (413 Payload Too Large)
- **Response Assembly**: Complete HTTP response generation and transmission

**Advanced Features:**
- **Client IP Tracking**: Detailed logging of client connections
- **Bytes Sent Tracking**: Monitoring of successful data transmission
- **Connection Cleanup**: Proper socket closure after each request
- **Exception Handling**: Comprehensive try-catch blocks for runtime errors

## Security Enhancements

### Path Security
- **Directory Traversal Prevention**: Blocks attempts to access files outside document root
- **File Extension Validation**: Only serves whitelisted file types
- **Path Canonicalization**: Ensures all file paths are within allowed directories

### Input Validation
- **Request Size Limits**: 4KB buffer limit prevents buffer overflow attacks
- **URL Validation**: Safe handling of encoded URLs with proper decoding
- **Method Validation**: Only processes supported HTTP methods

### Resource Protection
- **RAII Implementation**: Automatic resource cleanup prevents memory leaks
- **Socket Management**: Proper socket closure and error handling
- **File Handle Safety**: Automatic file handle management

## Configuration-Driven Behavior

### Server Configuration (`config/server.conf`)
```ini
[server]
port = 8080                 # Server port
host = "127.0.0.1"         # Bind address
reuse_address = "false"     # Socket reuse option
rcv_buffer_size = 65536     # Receive buffer size
snd_buffer_size = 65536     # Send buffer size
rcv_timeout = 5             # Receive timeout in seconds

[development]
reuse_address = "true"      # Development-specific settings

[document]
document_root = "www/"      # Document root directory
template = "www/template/"  # HTML template directory
image = "www/static/img/"   # Image directory
style = "www/static/style/" # CSS directory
```

## Static Content Organization

### Document Structure
```
www/
├── template/           # HTML pages
│   ├── index.html     # Home page (served for "/" route)
│   ├── about.html     # About page
│   └── test.html      # Test page
└── static/            # Static assets
    ├── img/           # Images (PNG, JPG)
    │   └── image.png
    └── style/         # CSS files
        └── style.css
```

## HTTP Protocol Implementation

### Supported Methods
- **GET**: Full implementation with file serving
- **POST**: Framework implemented (ready for extension)

### Content Types
- `text/html` - HTML pages
- `text/css` - Stylesheets
- `application/javascript` - JavaScript files
- `application/json` - JSON data
- `image/png`, `image/jpeg`, `image/gif` - Images
- `text/plain` - Text files

### Status Codes
- **2xx Success**: 200 OK
- **4xx Client Errors**: 400, 401, 403, 404, 405, 408, 413, 415, 429
- **5xx Server Errors**: 500, 501, 502, 503, 504, 505

## Error Handling and Recovery

### Client Error Handling
- **Connection Timeouts**: Proper handling with 408 Request Timeout
- **Malformed Requests**: 400 Bad Request responses
- **Oversized Requests**: 413 Payload Too Large protection
- **Forbidden Access**: 403 Forbidden for security violations

### Server Error Management
- **File Not Found**: 404 responses with detailed logging
- **Internal Errors**: 500 responses for server-side issues
- **Unsupported Media**: 415 responses for unknown file types

## Performance Considerations

### Resource Management
- **Single-threaded Design**: Sequential client handling (educational focus)
- **Memory Efficiency**: ~2MB base footprint + 4KB per connection
- **File Caching**: File content read per request (suitable for development)
- **Connection Handling**: One connection at a time with proper cleanup

### Network Optimization
- **Buffer Management**: Configurable send/receive buffer sizes
- **Timeout Controls**: Configurable client timeouts
- **Connection Closure**: Immediate connection cleanup after response

## Building and Deployment

### Build System
The project uses CMake for cross-platform building:
```bash
mkdir build && cd build
cmake ..
make
cd ..
build/WebServer
```

### Runtime Dependencies
- POSIX-compliant operating system (Linux, macOS, BSD)
- C++17 compiler (GCC 14+ or Clang 12+)
- Network access for port binding

## Educational Value

### Networking Concepts Demonstrated
- **Socket Programming**: Complete TCP socket lifecycle
- **HTTP Protocol**: Request parsing and response generation
- **Network Security**: Input validation and path protection
- **Resource Management**: RAII and proper cleanup

### Software Engineering Practices
- **Modular Design**: Clear separation of concerns
- **Error Handling**: Comprehensive error checking and recovery
- **Configuration Management**: External configuration with type safety
- **Logging**: Structured logging for debugging and monitoring

### C++ Features Showcased
- **RAII**: Resource Acquisition Is Initialization pattern
- **Namespaces**: Organized code structure
- **STL Containers**: `std::unordered_map`, `std::vector`, `std::string`
- **Exception Handling**: Robust error management

## Future Extension Points

### Planned Enhancements
- **Multi-threading**: Thread pool for concurrent client handling
- **HTTPS Support**: TLS/SSL encryption capabilities
- **Database Integration**: Dynamic content with database backends
- **Caching System**: Static file caching for performance
- **Compression**: gzip/deflate response compression

### Protocol Extensions
- **HTTP/2 Support**: Modern HTTP protocol features
- **WebSocket Support**: Real-time communication
- **Range Requests**: Partial content delivery
- **Keep-Alive**: Persistent connections

This enhanced TCP web server demonstrates a complete evolution from a basic socket server to a feature-rich, production-ready web server with comprehensive security, configuration management, and HTTP protocol implementation. The modular architecture and extensive documentation make it an excellent educational resource for understanding network programming, HTTP protocols, and modern C++ development practices.
