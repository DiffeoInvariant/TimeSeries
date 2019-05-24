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
    template<typename ts_type, out_t>
    class Model
    {
    protected:
        ts_type series;
        out_t   result;
        
    public:
        //fitting method
        virtual out_t fit();
        
        //returns log likelihood of model
        virtual double logLik();
        
        //prints a summary
        virtual void summary();
        
        virtual std::vector<Param_t> params();
    };
    
    
}
#endif//TS_BASE_MODEL_HPP
