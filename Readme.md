# About otvar

`otvar` is an environment variable library, written in C.  It implements a dictionary-type data structure.   

The current version has the following implementation details

* UTHASH is the backend for the dictionary, which is a hash-table library.
* Strings are uses as keys/names for variables.  The maximum name length is 15 characters.
* Variables may have string values, integer values, double values, or arbitrary length binary values.

## Building otvar

`otvar` doesn't have any necessary dependencies, so it is easy to build.

```
$ cd otvar
$ make
```

You can find the binary inside `otvar/bin/`
