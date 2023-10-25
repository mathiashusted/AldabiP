<h1 align="center">AlDaBi Praktikum</h1>
<p align="center"><strong>Algorithms and Data Structures for Bioinformatics</strong></p>

![GitHub](https://img.shields.io/github/license/mathiashusted/AldabiP?style=flat-square)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/mathiashusted/AldabiP/CI?style=flat-square)
![GitHub last commit](https://img.shields.io/github/last-commit/mathiashusted/AldabiP?style=flat-square)
![GitHub repo size](https://img.shields.io/github/repo-size/mathiashusted/AldabiP?style=flat-square)

![header](images/header.png)


## Table of Contents
- [About](#about)
- [Course Structure](#course-structure)
  - [Topics](#topics)
- [Implementations](#implementations)
- [Testing](#testing)
- [Installation](#installation)
- [Continuous Integration](#continuous-integration)
- [Contributors](#contributors)
- [License](#license)


## About
This repository contains the code implementations for the Algorithms and Data Structures for Bioinformatics Practical course at [Freie Universität Berlin](https://www.fu-berlin.de/). The main focus is on implementing various algorithms using C++. Each algorithm is implemented as a final assignment, organized in a structured way for easy navigation and comprehension.


## Course Structure
The course is divided into 2 modules running in parallel:
1. Algorithms and Data Structures for Bioinformatics
2. Algorithms and Data Structures for Bioinformatics Practical

In the first, we will acquire a theoretical understanding of the algorithms and data structures used. In the latter, we will have a go ourselves in groups of 2 at implementing them in an efficient manner - this is the code you will find in our repository. These will undergo exhaustive testing to ensure proper performance.


### Topics
- Exact string matching - [(e.g. Boyer-Moore algorithm)]
- Theme 2 - [Description]
- Theme 3 - [Description]
...
[More themes will be added as the course progresses]


## Implementations
Each final assignment corresponding to a theme is placed in a folder named `aufgabe1`, `aufgabe2`, etc., where `aufgabe` means task in German. For instance, for the first theme, you will find the implementation inside the `aufgabe1` folder.

The real development for each of these assignments takes place under `src/*` directory. For example, the code for the first theme would be under `src/a1_dzima/*` and `src/a1_mathias/*`.


## Testing
Each assignment folder contains a test file named `a*_test.cpp.` To run the tests, navigate to the corresponding folder and execute the following command:
```bash
g++ -std=c++17 -Wall -pedantic -O3 aufgabe1.cpp aufgabe1_test.cpp -o aufgabe1_test

```
This command will compile the test file using the C++17 standard, apply all warnings (`-Wall`), enforce strict compliance to the C++ standard (`-pedantic`), and optimize the code (`-O3`). It will then generate an executable file named `aufgabe1_test`.



## Installation
To get started with the development, clone this repository using the following command:
```bash
git clone https://github.com/your-username/Aldabe-Praktikum.git
```
After cloning, navigate to the project's root directory and follow the instructions specific to each task under the [Implementations](#installation) section.


## Continuous Integration
This repository utilizes GitHub Actions for Continuous Integration (CI). The CI pipeline automatically triggers every time a new commit is pushed to the repository. It compiles and runs the tests as specified, ensuring that each change to the codebase maintains the integrity of the algorithms implemented.


## Contributors
- [Dzmitry Hramyka](https://github.com/gromdimon) 
- [Mathias Husted](https://github.com/mathiashusted)


## Acknowledgements
- [Prof. Dr. Knut Reinert](https://www.mi.fu-berlin.de/en/inf/groups/abi/members/Professors/reinert.html)


## References
- Algorithms and Data Structures for Bioinformatics
- Algorithms and Data Structures for Bioinformatics Practical
- GitHub Actions
- [DALLE3](https://openai.com/dall-e-3)

## License
This repository is released under the <a href="LICENSE">GPL v3</a> license. Feel free to tweak it to your own likings.