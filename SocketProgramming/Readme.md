# Socket Programming Training

## Introduction
This directory contains practical examples of socket programming on Linux, covering common socket types (Internet, Unix, Raw) and connection handling mechanisms (Blocking, Non-blocking, Poll, Select). It also includes an assignment simulating routing table exchange between two devices.

## Directory Structure

- **TypesOfSocket/**  
  Examples of socket types:
  - `InternetSocket/`: TCP/IP network sockets (StreamSocket, DatagramSocket).
  - `UnixSocket/`: Local system sockets (StreamSocket, DatagramSocket).
  - `RawSocket/`: Raw sockets, direct manipulation of IP packets.

- **Assignment/**  
  Assignment simulating routing table exchange between two devices (deviceA, deviceB) using UDP sockets.

- **Mechanisms/**  
  Connection handling mechanisms:
  - `NonBlockingSocket/`: Examples of non-blocking sockets.
  - `Poll/`: Using the poll function to handle multiple connections, with a group chat implementation.
  - `Select/`: Using the select function to handle multiple connections, with a group chat implementation.

## Build & Run Instructions

Each folder contains a `Makefile` for compilation:
```sh
make           # Compile all .c files into .out executables
make clean     # Remove all