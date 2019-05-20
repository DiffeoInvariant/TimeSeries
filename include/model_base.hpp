#ifndef TS_MODEL_BASE_HPP
#define TS_MODEL_BASE_HPP
/**
 @author: Zane Jakobs
 @brief: base class for models in TimeSeries
 */
#include "base.hpp"
#include <vector>
#include <utility>

namespace TimeSeries
{
    //stores results of a model; T is the type of parameters
    //RetPars are other model parameters
    template<typename T = double, typename RetPars...>
    struct ModelOutput
    {
        std::vector<T>          params;
        TimeSeries::TSError     status;
        std::tuple<RetPars...>  outs;
    };
    
    
    /**
     ts_type is a time series type, e.g. ts
     */
    template<typename ts_type = TimeSeries::ts,
             typename Param_t = double,
             typename RetPars...>
    class Model
    {
        using out_t = ModelOutput<Param_t, RetPars...>;
    protected:
        ts_type series;
        out_t   result;
        
    public:
        //fitting method
        virtual out_t fit() = 0;
        
        //returns log likelihood of model
        virtual double logLik() = 0;
        
    };
    
    
}
#endif//TS_BASE_MODEL_HPP
