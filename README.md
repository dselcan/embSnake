# embSnake

## Short summary

embSnake is a reimplementation of the classic snake game typically provided together with old mobile phones (e.g. Nokia 3310). It is written to be used together with a HX8340-B LCD controller as part of an embedded project.

![embSnake game running on a PC](https://raw.githubusercontent.com/dselcan/embSnake/master/docs/embSnake_PC.png)

## Installation

The project is provided as is: simply extract and run the provided vcc project. Alternatively, the code can be embedded as part of a micro-controller project.

## How to Use

### PC Emulator

The project also features a Windows compatible emulator of the game using the SDL 2 library. It is found in the embSnake_vcc folder, and requires Visual C++ 2010 or later to open.
The game inside the emulator is controlled by the LEFT and RIGHT keyboard keys.

### Graphics

Three Python script to generate the LCD driver compatible graphics are also provided. The scripts can be found in the graphics folder. To use the png_to_carr.py and png_to_color_carr.py scripts, simply drag and drop the required png files onto them (alternatively, run them with the [script] [image_filename] command. The generate_color_lut.py script can be started as as standalone script.

## License

The SDL 2.0.3 library is licensed under a zlib license.
Otherwise, this software is licensed under a BSD license.
