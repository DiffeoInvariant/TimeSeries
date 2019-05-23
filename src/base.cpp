/**
 @author: Zane Jakobs
 @brief: implementation of base.hpp
 */
#include "../include/base.hpp"

namespace TimeSeries
{
    using namespace TimeSeries;
    
    
    
    
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date,
        typename Con,
        typename T
        >
    ts<Series_t, DateTime_t>::ts(Con& _data, T& _times) {
        //check that Series_t is arithmetic
        if(not std::is_arithmetic<Series_t>::value) {
            throw TimeSeries::NonArithmeticTypeError;
        }
        
        if(TimeSeries::is_1d_container<Con>::value) {
            
            length = _data.size();
            //is it convertible to a Vec<Series_t>?
            if(std::is_convertible<Con, Vec<Series_t>>::value){
                data = _data;
            } else {
                //_data is iterable, but not convertible to a Vec<Series_t>
                size_t i = 0;
                data = Vec<Series_t>(length);
                for(auto it : _data){
                    data[i] = *it;
                    i++;
                }//end for
            }//end else convertible to Vec<Series_t>
        }//end if is_1d_container<Con>
        else {
            //error: _data is not a container type
            throw TimeSeries::NonContainerTypeError;
        }//end data assignment/initialization
        
        if(TimeSeries::is_1d_container<T>::value){
            auto dateLen = _times.size();
            //T is a stl-like container of strings or datetimes
            if(std::is_convertible<typename T::value_type, DateTime_t>::value) {
                /* if _times is the correct size assign, else treat
                 the first value as a start DateTime_t and iterate by one
                 up to the length of the series*/
                if(dateLen == length) {
                    if(std::is_convertible<T, DateVec<DateTime_t>>::value){
                        times = _times;
                    } else {
                        for(auto it : _times){
                            times.insert(*it);
                        }
                    }//end else dateLen == length
                } else {
                    //initialize datetime from start
                    times = DateRange(_times[0], length);
                }
            }//end if convertible
            else {
                //not convertible type
                throw TimeSeries::NonConvertibleDateTimeError;
            }
        }//end if container
        else {
            //if _times is not a container, it must be a single start DateTime_t
            if(std::is_convertible<T, DateTime_t>::value) {
                times = DateRange(_times[0], length);
            } else {
                //not convertible type
                throw TimeSeries::NonConvertibleDateTimeError;
            }
        }//end case _times is not container
        
    };
    
    //construct from data only
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date,
        typename Con
        >
    ts(Con& _data)
    {
        //check that Series_t is arithmetic
        if(not std::is_arithmetic<Series_t>::value) {
            throw TimeSeries::NonArithmeticTypeError;
        }
        
        if(TimeSeries::is_1d_container<Con>::value) {
            
            length = _data.size();
            //is it convertible to a Vec<Series_t>?
            if(std::is_convertible<Con, Vec<Series_t>>::value){
                data = _data;
            } else {
                //_data is iterable, but not convertible to a Vec<Series_t>
                size_t i = 0;
                data = Vec<Series_t>(length);
                for(auto it : _data){
                    data[i] = *it;
                    i++;
                }//end for
            }//end else convertible to Vec<Series_t>
        }//end if is_1d_container<Con>
        else {
            //error: _data is not a container type
            throw TimeSeries::NonContainerTypeError;
        }
    };
    
    
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    ts<Series_t, DateTime_t>::Vec<Series_t>
    getData()
    const noexcept
    {
        return data;
        
    }
    
    
    //returns an iterator to data.begin()
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    ts<Series_t, DateTime_t>::Vec<Series_t>::iterator
    dbegin()
    const
    {
        return data.begin();
    }
    
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    ts<Series_t, DateTime_t>::Vec<Series_t>::iterator
    dend()
    const
    {
        return data.end();
    }
}
