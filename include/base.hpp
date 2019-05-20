/**
 @author: Zane Jakobs
 @brief: containers for the ts library
 */
#ifndef TS_BASE_HPP
#define TS_BASE_HPP

#include <Eigen/Core>
#include <set>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <type_traits>
#include "ts_type_traits.hpp"
#include "ts_error.hpp"

namespace TimeSeries
{
/* ---------------------------------------------------------------------------------
   ---------------------------------------------------------------------------------
                            typedefs for the TimeSeries library
   ---------------------------------------------------------------------------------
   --------------------------------------------------------------------------------- */
    //vectors are column vectors
    template<typename T>
    using Vec = typename Eigen::Matrix<T, Eigen::Dynamic, 1>;
    
    //T must be a date_type type
    template<typename T>
    using DateVec = typename std::multiset<T>;

    
    /* ---------------------------------------------------------------------------------
     ---------------------------------------------------------------------------------
                       container classes and helper functions
     ---------------------------------------------------------------------------------
     --------------------------------------------------------------------------------- */
    
    /**
     @author: Zane Jakobs
     @param start: initial DateTime_t. Passed by value so it
     can be freely modified
     @param length: length of container
     @return: std::multiset of DateTime_ts
     */
    template<typename DateTime_t>
    static auto DateRange(DateTime_t start, size_t length)
    {
        //iniitalize with initializer list
        std::multiset<DateTime_t> dtimes { start };
        for(size_t i = 1; i < length; i++){
            start++;
            dtimes.insert(start);
        }
        return dtimes;
    }
    
    
    /**
     @author: Zane Jakobs
     @brief: General time series class that requires an arithmetic
     type for data and an optional boost/date_time type.
     */
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
            >
    class ts
    {
    protected:
        Vec<Series_t>           data;
        DateVec<DateTime_t>     times;
        size_t                  length = 0;
        bool                    has_times = false;//true iff times is initialized
    public:
        
        //default ctor
        constexpr ts() {};
        /*ctor from given information, must
          compute length and set has_times true
          if _times is a suitable time vector or an initial
          time/date.
         
         T is either a DateTime_t or a stl-like container of the above
         
         Con is a stl-like container holding Series_t objects
         */
        template<typename Con, typename T>
        ts(Con& _data, T& _times)
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
                            has_times = true;
                        } else {
                            for(auto it : _times){
                                times.insert(*it);
                            }
                        }//end else dateLen == length
                    } else {
                        //initialize datetime from start
                        times = DateRange(_times[0], length);
                        has_times = true;
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
                    has_times = true;
                } else {
                    //not convertible type
                    throw TimeSeries::NonConvertibleDateTimeError;
                }
            }//end case _times is not container
            
        };
        
        //construct from data only
        template<typename Con>
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
        
        Vec<Series_t> getData() { return data; }
        
        //TODO: template this like the ctor
        void setData(Vec<Series_t>& newData){
            data = newData;
        }
        
        DateVec<DateTime_t> getTimes() { return times; }

        //T is either a DateTime_t or a vector of one of those
        //TODO: implement this
        template<typename T>
        void setTimes(T& newTimes);
        
        size_t getLength() { return length; }

    };


 }//end namespace ts
#endif//TS_BASE_HPP


