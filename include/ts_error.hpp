#ifndef TS_ERROR_HPP
#define TS_ERROR_HPP

namespace TimeSeries
{
    enum TSError
    {
        Success                         = 0,
        NonContainerTypeError           = 1,
        NonArithmeticTypeError          = 2,
        NonConvertibleDateTimeError     = 3
    };
}


#endif//TS_ERROR_HPP
