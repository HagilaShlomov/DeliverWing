# Drone Project Server

## Overview

This repository contains the server-side implementation for a drone delivery simulation project. The server is built in C++ and exposes a REST API for client login, order submission, path planning, weather retrieval, and integration with a database/DAL layer.

The server uses `cpp-httplib` for HTTP handling, `nlohmann/json` for JSON parsing, `libcurl` for external weather requests, and MySQL connector logic to send user and order data to a DAL backend.

## Key Features

- HTTP server running on `http://localhost:8080`
- REST endpoints for:
  - user login (`/login`)
  - order submission (`/AddOrder`)
- CORS support for browser or frontend clients
- Integration with a DAL server on `http://localhost:8081`
- Order forwarding to a backend database service
- Drone path planning using graph search algorithms
- Weather API support via external HTTP requests
- Simulation-oriented modules for sensors and LiDAR

## Architecture

### Main entry point

- `Server/Server/Server.cpp`
  - Initializes the graph and server
  - Calls route setup functions: `handleOptions`, `handleLogin`, `handleAddOrder`
  - Starts listening on `0.0.0.0:8080`

### HTTP route handlers

- `Server/Server/DB_connection.h`
  - Configures CORS and default headers
  - Registers `/login` and `/AddOrder`
  - Contacts the DAL server at `localhost:8081`

- `Server/Server/clientAPI.h`
  - Contains route definitions for:
    - `OPTIONS` requests -- CORS preflight handling
    - `/login` -- accepts username/password JSON
    - `/AddOrder` -- accepts order details and coordinates

### Data and model modules

- `Server/Server/Order.h` / `Order.cpp`
  - Order data model and storage logic
- `Server/Server/User.h` / `User.cpp`
  - User credentials model
- `Server/Server/Coordinate.h` / `Coordinate.cpp`
  - GPS coordinate model used by the drone simulation
- `Server/Server/DataStructures.h` / `DataStructures.cpp`
  - Shared structures used across modules
- `Server/Server/CostCalculate.h` / `CostCalculate.cpp`
  - Cost and path calculation helper logic

### Graph and routing

- `Server/Server/InitGraph.h` / `InitGraph.cpp`
  - Graph initialization and map setup
- `Server/Server/Graph.h`
  - Graph data structure representing nodes and edges
- `Server/Server/Node.h`
  - Graph node definition
- `Server/Server/Dijkstra.h`
  - Dijkstra shortest-path algorithm support
- `Server/Server/AStar.h`
  - A* path-finding algorithm support

### Simulation and sensors

- `Server/Server/Simulation.h`
  - Simulation driver for the drone mission
- `Server/Server/HandleDinamic.h`
  - Dynamic conditions handling and environment updates
- `Server/Server/Sensors.cpp` / `Sensors.h`
  - Sensor abstractions for drone state
- `Server/Server/liD.h`, `Server/Server/LiDar.cpp`, `Server/Server/LiDar.h`
  - LiDAR sensor logic and point-cloud-like processing
- `Server/Server/sensorsAPI.h`
  - Example HTTP client calls for sensor-related endpoints

### Weather support

- `Server/Server/Weather.h` / `Server/Server/Weather.cpp`
  - `WeatherAPI` class for fetching weather data from an external weather service
  - Parses temperature, humidity, wind, pressure, and weather descriptions
  - Can load additional weather / wind-direction data from local files

## Dependencies

The project depends on the following libraries and tools:

- C++11 compiler (Visual Studio / MSVC or compatible)
- CMake 3.10+
- `libcurl` for HTTP requests
- `cpp-httplib` (header-only HTTP server/client library)
- `nlohmann/json` for JSON serialization/parsing
- MySQL connector/JDBC support for DAL integration (`mysql/jdbc.h` references)
- Optional: `PCL` and other vendor libraries included in the repository for sensor or point-cloud processing

## Project Layout

```
Server/
  Server.cpp              # Main application entry
  Server.h                # Server declarations and utility helpers
  clientAPI.h             # HTTP route handlers
  DB_connection.h         # DAL and database communication helpers
  Weather.h / Weather.cpp # External weather retrieval module
  InitGraph.h / InitGraph.cpp
  Graph.h
  Node.h
  Dijkstra.h
  AStar.h
  Simulation.h
  HandleDinamic.h
  Sensors.h / Sensors.cpp
  liD.h / LiDar.cpp / LiDar.h
  Order.h / Order.cpp
  User.h / User.cpp
  Coordinate.h / Coordinate.cpp
  CostCalculate.h / CostCalculate.cpp
  DataStructures.h / DataStructures.cpp
  runDrone.h
  httplib.h
  json.hpp
  packages.config
  weather_data.json*      # Weather data files and caches
```

Additional third-party bundles in the repository:

- `cpp-httplib-master/`
- `curl-8.13.0_3-win64-mingw/`
- `libxl-4.6.0/`
- `pcl-master/`
- `vcpkg/`

## Build Instructions

### Option 1: Visual Studio

1. Open `Server.sln` in Visual Studio.
2. Make sure the include directories contain the required libraries:
   - `curl` headers and libs
   - MySQL connector headers and libs
   - `nlohmann/json.hpp`
3. Build the `Server` project.

### Option 2: CMake

1. Create a build directory in the repository root:
   ```powershell
   cd "c:\Users\חגילה\Desktop\לימודים\פרויקט גמר\drone project\Server\Server"
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

> Note: The existing `CMakeLists.txt` currently targets a `WeatherApp` executable built from `Server/Weather.cpp`. If you want to build the full server project, update `CMakeLists.txt` to include `Server.cpp` and the other source files.

### Build notes

- Ensure `libcurl` and `MySQL` libraries are installed and visible to the compiler/linker.
- If using `vcpkg`, integrate it with your Visual Studio toolchain and install required ports.

## Runtime Usage

### Starting the server

Run the compiled executable. The application will print:

```
Server is running on http://localhost:8080
```

### Required external service

The server expects a DAL backend service on `http://localhost:8081`.
This service handles:

- `/register` for user registration
- `/orders` for storing order data

### Sample request formats

#### Login

Endpoint: `POST http://localhost:8080/login`

JSON body:

```json
{
  "username": "admin",
  "password": "good123"
}
```

Expected success response:

```json
{
  "message": "Login successful"
}
```

#### Add Order

Endpoint: `POST http://localhost:8080/AddOrder`

JSON body:

```json
{
  "source": [32.32928, 34.85561],
  "destination": [32.32387, 34.86207],
  "city": "Tel Aviv",
  "street": "Example Street",
  "houseNumber": "123",
  "itemCount": "2",
  "itemWeight": "1.5"
}
```

This request will:

- validate source/destination coordinates
- create a `Coordinate` and `Order` object
- insert order data into the local `addressSet`
- send the order to the DAL server
- execute the drone path planning routine via `run(g, source, destination)`

## Important Notes

- The `/login` handler currently uses a hard-coded credential check for `admin` / `good123`.
- `registerUserInDB()` sends registration data to `localhost:8081/register`.
- `sendOrderToDB()` sends order data to `localhost:8081/orders`.
- The weather module requires a valid weather API key and fetches current weather from an external API endpoint.
- There may be placeholder or development code in the DAL and sensor modules that requires a live backend to work fully.

## Troubleshooting

- If the server fails to start, verify the build succeeded and libraries were linked.
- If `/AddOrder` returns errors, check the JSON payload and ensure `source`/`destination` arrays contain exactly two numeric values.
- If the DAL calls fail, confirm the DAL service is available on `http://localhost:8081`.
- If weather requests fail, verify network access and the API key configuration in `Weather.cpp`.

---

This README provides a full description of the project structure, dependencies, build options, runtime usage, and key implementation details for the drone server backend.
