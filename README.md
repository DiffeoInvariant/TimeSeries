# TimeSeries
A C++ library for time series analysis.


## Developer Guide

### Style

For this project, refer to Effective Modern C++ by Scott Meyers as a style guide. Unless the definition is completely unambiguous, with parameters named in a clear, obvious manner after common mathematical objects (for example, in a function that fits OLS regression, X is understood to be the data matrix and y the dependent variable), function definitions and implementations should be commented with the author's name, parameter names and acceptable values, and the return type. Function comments should be in the following style:

```
/**
* @author: your name here
* @param x1: first object to add
* @param x2: second object to add
* @return: x1 + x2 if T is an arithmetic type, else throws
an exception
*/
template<typename T>
T add(T x1, T x2);
```
