# Setting up a Zephyr + Fprime Project
 These notes should walk you through setting up a project for Zephyr and Fprime. By the end you'll have a project that can compile a Zepyr build.

 ## Setting up Fprime
 First you need to create and activate a python virtual environment (.venv). The Nix flake will automatiically make one for you if you are using Nix.

Then, install the fprime package.
```sh
pip install --upgrade fprime-bootstrap
```

Create the project by running:
```sh
fprime-bootstrap project
```

> [!IMPORTANT] Initializing an existing project
> If you are initializing an existing project (after cloning it from github), you'll need to setup git submodule by running `git submodule update --init --recursive`

Then, you'll need to either activate the fprime .venv or install the packages into your current .venv.
```
source fprime-venv/bin/activate

# or

pip install ./lib/fprime/requirements.txt
```

Inside of the project folder, run:
```sh
fprime-util generate
fprime-util build
```

Next, follow setup 2 and 3 [here](https://fprime.jpl.nasa.gov/latest/tutorials-hello-world/docs/hello-world/#2-creating-an-f-component) to create a component and deployment.

## Seting up Zephyr Fprime Integration
First, add the fprime-zephyr git repo as a submodule. You can do this by adding the following to `.gitmodules`

```
[submodule "lib/fprime-zephyr"]
	path = lib/fprime-zephyr
	url = https://github.com/fprime-community/fprime-zephyr
```

Then run:
```sh
git submodule update --init --recursive
```

You'll next need to copy some files from the fprime-zephyr repo into your project. You can find the files [here](https://github.com/fprime-community/fprime-zephyr/tree/main/sample-config). For an example of where to place the files, look at this repository.

Update your `setings.ini` with the `library_locations`
```
[fprime]
project_root: .
framework_path: ./lib/fprime
library_locations: ./lib/fprime-zephyr # <--- add this
```

In your root CMakeLists.txt add the following above the `project` declaration:
```
if (NOT BUILD_TESTING)
    find_package(Zephyr HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/zephyr-workspace")
endif()

# ^^^ add the above

project(HiNamespace C CXX)
```

In your deployment `CMakeLists.txt` replace
register_fprime_deployment(
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/Main.cpp"
    DEPENDS
        ${FPRIME_CURRENT_MODULE}_Top
)
with

```
register_fprime_zephyr_deployment(
        ${FPRIME_CURRENT_MODULE}
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/Main.cpp"
    DEPENDS
        ${FPRIME_CURRENT_MODULE}_Top
)
```