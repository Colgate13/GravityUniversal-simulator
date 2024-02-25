# Gravity Simulation with Raylib and C

This project is an interactive simulation of Newton's law of universal gravitation, developed using the Raylib library in C. It allows users to visualize and interact with celestial bodies, observing the effects of gravity on their trajectories and velocities.

## Features

- Real-time simulation of gravity between celestial bodies.
- Interactive control of zoom and crosshair to explore different parts of the simulated universe.
- Dynamic addition of meteorites with mouse clicks, each with randomly generated names.
- Collision detection between celestial bodies, with mass merging and size adjustments.

## Dependencies

To compile and run this project, you will need:

- [Raylib](https://www.raylib.com/) for graphical rendering and input control.
- A compatible C compiler `clang`.
- `make` to automate compilation.

## Setup

Before compiling the project, make sure to install Raylib and its dependencies by following the [installation instructions](https://github.com/raysan5/raylib/wiki#installing-raylib) appropriate for your operating system.

## Compilation

With the dependencies installed, you can compile the project using the following command in the terminal:

```bash
make
```

This will generate the `newTon` executable in the `bin` directory.

## Execution

To start the simulation, run the command:

```bash
./bin/newTon
```

## Controls

- `W`, `A`, `S`, `D`: Move the crosshair.
- `Space`: Toggles the meteorite target between Earth and the Sun.
- `Mouse Scroll`: Controls the camera zoom.
- `Left Mouse Button`: Adds a new meteorite at the crosshair's position.

## Contributions

Contributions are always welcome! If you have a suggestion to improve this simulation, feel free to create a pull request or open an issue.

## License

This project is distributed under the MIT license. See the `LICENSE` file for more details.

## Authors

- Gabriel Barros Feitosa Sa (Colgate13) - [GitHub](https://github.com/Colgate13)
