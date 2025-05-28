# InterPlanetary File System (IPFS) Implementation

This project implements a Distributed Hash Table (DHT) based InterPlanetary File System using C++. The system uses a ring topology for distributed file storage and retrieval.

## Features

- Ring-based Distributed Hash Table (DHT) implementation
- B-tree data structure for efficient file storage
- Custom BigInt implementation for handling large numbers
- Cryptographic operations using Crypto++ library
- File system operations for managing files and directories
- Circular linked list structure for ring topology

## Prerequisites

- C++ Compiler (Visual Studio recommended)
- Crypto++ Library
- Windows OS (for system commands)

## Project Structure

- `Source.cpp` - Main program implementation
- `RingDht.h` - Ring DHT implementation
- `Machine.h` - Machine class implementation
- `MyBigInt.h` - Custom BigInt implementation
- `BTREE.h` - B-tree data structure implementation

## Dependencies

### Standard C++ Libraries
- `<iostream>` - Input/output operations
- `<string>` - String manipulation
- `<cstdlib>` - System commands and utilities
- `<cstdio>` - C-style input/output
- `<fstream>` - File operations
- `<sstream>` - String stream operations
- `<cmath>` - Mathematical operations

### Crypto++ Libraries
- `<sha.h>` - SHA hashing
- `<filters.h>` - Cryptographic filters
- `<hex.h>` - Hexadecimal encoding/decoding

## Building the Project

1. Clone the repository
2. Open the solution in Visual Studio
3. Build the project using Visual Studio's build tools

## Usage

1. Run the compiled executable
2. Enter the identifier space (must be between 1 and 160)
3. Specify the B-tree order
4. Choose the number of machines to create
5. Select between random or personal IDs for machines
6. Follow the prompts to create and manage machines and files

## Features

- Create and manage multiple machines in a ring topology
- Store and retrieve files using DHT
- Automatic file distribution across machines
- B-tree based efficient file storage
- Cryptographic operations for security

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request 