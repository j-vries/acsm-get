Dependencies
------------

Building requires the following libraries to be available:

**Internal**
* libgourou
* uPDFParser

**External**
* libcurl
* libgtkmm 3.0
* libpugixml
* libssl
* libzip
* zlib

The internal libraries are available as submodules in this repository.

The external libraries you can install using the package manager provided by your operating system. For example on Ubuntu:

```
sudo apt install libcurl4-openssl-dev libgtkmm-3.0-dev libpugixml-dev libssl-dev libzip-dev zlib1g-dev 
```


Building
--------

To build acsm-get including the internal libraries, run:

```
make all
```

Once the libraries are built, you can also build just acsm-get with:

```
make acsm-get
```

To build the deb package:

```
make dpkg
```

To clean:

```
make clean
```
