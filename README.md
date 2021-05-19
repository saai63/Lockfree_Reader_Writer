# Lockfree_Reader_Writer

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Requirements
* Provide read and write interfaces to an underlying data structure.
* Access should be thread safe.
* Reads will be frequent while writes will be rare.
* Multiple threads should be able to read data in parallel and should not be blocked by other readers.
* Writers modify the data and hence writes cannot be concurrent.

## Solutions
### Global object and std::shared_ptr
[Reader_Writer_pointer_globalObj.cpp](Reader_Writer_pointer_globalObj.cpp)

### std::shared_ptr and temporary object
[Reader_Writer_pointers_only.cpp](Reader_Writer_pointers_only.cpp)

## Test results
* Both solutions seem to work
* No issues pointed out by thread sanitizer