# Contributing

## Set Up

> [!IMPORTANT]
> This Guide assumes you are using a UNIX-like System and have `git` already installed!

### Get the Source Code

At first, you have to download the source code from GitHub.
This can be archived by running

```bash
git clone https://github.com/es-ude/elastic-AI.Protocol.git eaip  # Download the Repository
cd eaip                                                            # Move inside the repository
```

in your shell.

### Required Tools

- **[Astral-UV](https://docs.astral.sh/uv/)**
  -> Python Runtime/Package Manager
  Can be installed via your local package manager like dnf, apt or brew.

#### Python Environment

To minimize the potential corruption of your system we recommend installing the dependencies inside
a [virtual environment](https://python.land/virtual-environments/virtualenv#How_to_create_a_Python_venv).
The python environment can be created and prepared by running:

```bash
uv venv .venv              # create a virtual python environment under `.venv/`
source .venv/bin/activate  # load the virtual environment
uv sync --all-groups       # install all python dependencies
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

### Local Test Execution

The local execution of the unit tests is possible via the `pytest` tool as follows:

```bash
uv run pytest
```

## Contribute your Changes

Do **not** push your changes directly to the `main` branch.
Push your modification to a new branch and open a pull request to `main`, so that the maintainers of this repository can
review your modifications and merge them.
