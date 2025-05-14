# Contributing

## Set Up

> [!IMPORTANT]
> This Guide assumes you are using a UNIX-like System and have `git` already installed!

### Get the Source Code

At first, you have to download the source code from GitHub.
This can be archived by running

```bash
git clone https://github.com/es-ude/elastic-ai.Protocol.git eaip  # Download the Repository
cd eaip                                                            # Move inside the repository
```

in your shell.

### Required Tools

- **[GCC](https://gcc.gnu.org/)**
  -> C Compiler
  Can be installed via your local package manager like dnf, apt or brew.
- **[CMake](https://cmake.org)**
  -> Build System
  Can be installed via your local package manager like dnf, apt or brew.
- **[Ninja](https://ninja-build.org/)**
  -> Build Tool
  Can be installed via your local package manager like dnf, apt or brew.
- **[Astral-UV](https://docs.astral.sh/uv/) [OPTIONAL]**
  -> Python Runtime/Package Manager
  Can be installed via your local package manager like dnf, apt or brew.
- **[NodeJS](https://nodejs.org/en) [OPTIONAL]**
  -> Javascript Runtime/Package Manager
  Can be installed via your local package manager like dnf, apt or brew.

#### Python Environment [OPTIONAL]

> [!NOTE]
> The Python environment is only required if you want to use pre-commit.

To minimize the potential corruption of your system we recommend installing the dependencies inside
a [virtual environment](https://python.land/virtual-environments/virtualenv#How_to_create_a_Python_venv).
The python environment can be created and prepared by running:

```bash
uv venv .venv              # create a virtual python environment under `.venv/`
source .venv/bin/activate  # load the virtual environment
uv sync --group pre-commit # install all python dependencies
```

> [!TIP]
> To use pre-commit for [conventional commit checks](documentation/CONTRIBUTION.md#contribution-guidelines) run
>
> ```bash
> uv run pre-commit install --install-hooks
> ```
>
> after initializing the Python environment

> [!IMPORTANT]
> To be able to use pre-commit and the other python tools you have to load the virtual python environment or execute the
> script with uv
>
> > [!TIP]
> > Use a tool like [direnv](https://direnv.net/) to automatically set up your environment when entering the folder
> > inside your shell!

### Compile the Source Code

> [!NOTE]
> This project is based on CMake.

We provide one predefined profile with the [CMakePresets.json](CMakePresets.json):

- host

This profile provide the required settings to build for your local system (_host_) to locally run the tests.

To initialize the CMake run:

```bash
cmake --preset host
```

> [!WARNING]
> This may take a while because all required external dependencies are loaded from the internet.
> See [external dependencies](#external-dependencies).

### Local Test Execution

The local execution of the unit tests is possible via the `ctest` function as follows:

```bash
ctest --preset unit_test
```

> [!NOTE]
> The unit-test executables can then be found under [build/host/test/unit](build/host/test/unit).

## Contribute your Changes

Do **not** push your changes directly to the `main` branch.
Push your modification to a new branch and open a pull request to `main`, so that the maintainers of this repository can
review your modifications and merge them.

## External Dependencies

> [!IMPORTANT]
> These dependencies will automatically be downloaded and initialized by CMake!

Following external dependencies are used by this project:

- [ThrowTheSwitch/CException](https://github.com/ThrowTheSwitch/CException)
- [ThrowTheSwitch/Unity](https://github.com/ThrowTheSwitch/Unity)

## Troubleshooting

- To generate a clean CMake Build without deleting and reinitializing the build directory run
  `cmake --build <build_dir> --target clean`.
