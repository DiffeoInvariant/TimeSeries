/**
 @author: Zane Jakobs
 @brief: containers for the ts library
 */
#ifndef TS_BASE_HPP
#define TS_BASE_HPP

#ifdef TS_USE_MKL
    #ifndef EIGEN_USE_MKL_ALL
    #define EIGEN_USE_MKL_ALL
    #endif
#endif

#include <Eigen/Core>
#include <set>
#include <string>
#include <utility>
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
    auto DateRange(DateTime_t start, size_t length)
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
        Vec<Series_t>                          data;
        std::optional<DateVec<DateTime_t>>     times;
        size_t                                length = 0;

    public:
        
        typename iterator;
        
        typedef Series_t series_type;
        
        typedef DateTime_t datetime_type;
        
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
        ts(Con& _data, T& _times);
        
        ts(ts<Series_t,DateTime_t>& other);
        
        ts(ts<Series_t,DateTime_t>&& other);
        
        Vec<Series_t> getData() const noexcept;
        
        iterator begin() const;
        
        iterator end() const;

        //returns an iterator to data.begin()
        Vec<Series_t>::iterator dbegin() const;
        
        Vec<Series_t>::iterator dend() const;
        
        DateVec<DateTime_t>::iterator tbegin() const;
        
        DateVec<DateTime_t>::iterator tend() const;
        
        //TODO: template this like the ctor
        void setData(Vec<Series_t>& newData);
        
        std::optional<DateVec<DateTime_t>>
        getTimes() const;

        //T is either a DateTime_t or a vector of one of those
        //TODO: implement this
        template<typename T>
        void setTimes(T& newTimes);
        
        size_t getLength();
        
        bool has_time_labels();
        
        /**
         *@param other: another ts object
         *@param index: optional index at which to insert other
         *@brief: attached other to *this
         */
        void
        append(TimeSeries::ts<Series_t, DateTime_t>& other,
                    std::optional<size_t> index);
        
        TimeSeries::ts<Series_t, DateTime_t> lag(int lagLen);
        
        TimeSeries::ts<Series_t, DateTime_t> diff(size_t nDiff = 1);
        
        TimeSeries::ts<Series_t, DateTime_t> integrate(size_t order);
        
        TimeSeries::ts<Series_t, DateTime_t>
        operator+(ts<Series_t, DateTime_t>& other);
        
        //add a scalar to all elements
        TimeSeries::ts<Series_t, DateTime_t>
        operator+(Series_t addQ);
        
        //multiply all elements by a scalar
        TimeSeries::ts<Series_t, DateTime_t>
        operator*(Series_t scale);
        
        

    };
    
    /* ---------------------------------------------------------------------------------
     ---------------------------------------------------------------------------------
                    functions to interface with other container libraries
     ---------------------------------------------------------------------------------
     --------------------------------------------------------------------------------- */
    //reads a csv into a ts object
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    extern TimeSeries::ts<Series_t, DateTime_t>
    read_csv(std::string& filename);
    
    //creates a ts object from an Eigen::Vector
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    extern TimeSeries::ts<Series_t, DateTime_t>
    ts_from_eigen(Vec<Series_t>& series);
    
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    extern Vec<Series_t>
    ts_to_eigen(TimeSeries::ts<Series_t, DateTime_t>& series);
    
    /*
     arma interface--"turn on" compilation of this code
     by compiling with -DTS_USE_ARMA
     */
    #ifdef TS_USE_ARMA
    #include <armadillo>
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    extern TimeSeries::ts<Series_t, DateTime_t>
    ts_from_arma(arma::Col<Series_t>& series);//TODO: implement this
    
    template<
        typename Series_t,
        typename DateTime_t = boost::gregorian::date
        >
    extern arma::Col<Series_t>
    ts_to_arma(TimeSeries::ts<Series_t, DateTime_t>& series);//TODO: implement this
    #endif

 }//end namespace ts
#endif//TS_BASE_HPPs
