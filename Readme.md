# About otvar (library)

otvar is an environment variable library that implements a dictionary.   

The current version has the following implementation details

* UTHASH is the backend for the dictionary, which is a hash-table library.
* Strings are uses as keys/names for variables.  The maximum name length is 15 characters.
* Variables may have string values, integer values, double values, or arbitrary length binary values.



