# Lockfree_Reader_Writer
## Requirements
* Provide read and write interfaces to an underlying data structure.
* Access should be thread safe.
* Reads will be frequent while writes will be rare.
* Multiple threads should be able to read data in parallel and should not be blocked by other readers.
* Writers modify the data and hence writes cannot be concurrent.

## Solutions
### Global object and std::shared_ptr


### std::shared_ptr and temporary object
