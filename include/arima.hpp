#ifndef TS_ARIMA_HPP
#define TS_ARIMA_HPP

#include "base.hpp"
#include "model_base.hpp"
#include <utility>
#include <vector>
#include <Eigen/Core>

using namespace TimeSeries;

namespace TimeSeries
{
    /* ---------------------------------------------------------------------------------
     ---------------------------------------------------------------------------------
                                namespace-level functions
     ---------------------------------------------------------------------------------
     --------------------------------------------------------------------------------- */
    
    template<typename T>
    extern Vec<double> ACF(Vec<T> series, size_t length);
    
    template<typename T>
    extern Vec<double> pACF(Vec<T> series, size_t length);
    
    
    
    
    
    /* ---------------------------------------------------------------------------------
     ---------------------------------------------------------------------------------
                                model typedefs and classes
     ---------------------------------------------------------------------------------
     --------------------------------------------------------------------------------- */
    /*
     param order is AR coefs then MA coefs, then external regressors' coefs.
     outs contains a pair of vectors of (p,d,q, num_exog) and (sp,sd,sq),
     (seasonal terms) and an Eigen::VectorXd of residuals
     and a vector
     */
    using ARIMAOutput = ModelOutput<double,
                                    std::vector<double>,
                                    std::pair<std::vector<size_t>, std::vector<size_t>>,
                                    Vec<double>
                                    >;
    
    template<typename ts_type = ts<double>>
    class ARMA : protected Model<ARIMAOutput>
    {
        
    protected:
        
        std::vector<size_t> params;
        
        //(p,0,q) order
        std::vector<size_t> order {0,0,0};
        
        bool isFit = false;
        
    public:
        
        ARMA() {};
        
        ARMA(ts_type& tseries) : series(tseries);
        
        ARMA(const ts_type& series);
        
        ARIMAOutput fit();
        
        ARIMAOutput fit(std::vector<size_t> fixedOrder);
        
        //fit on a subset of the series
        ARIMAOutput fit(size_t start_id,
                        size_t end_id) const;
        
        //log likelihood
        double logLik() const;
        
        void summary() const;
        
        std::vector<double> ARMA_params() const;
        
        std::vector<size_t> ARIMA_order() const;
        
        std::optional<std::vector<size_t>>
        getOrder() const;
        
        void setOrder(vector<size_t> newOrder);
        
        Vec<double> resids();
        
        template<typename T>
        T characteristic_poly(T x);
        
        //returns a triple (p,0,q)
        std::vector<size_t>
        estimate_order(std::optional<std::vector<size_t>> maxOrders,
                      std::optional<std::vector<size_t>> fixedOrders)
        const;
        
        ARIMAOutput
        forecast(size_t forecastLength,
                 std::optional<size_t> start_id) const;
        
        //RMSE on training indices. Default is whole set
        double RMSE(std::optional<size_t> start_id,
                    std::optional<size_t> end_id) const;
        
        //root mean square forecast error
        double RMSFE(size_t forecastLength,
                     std::optional<size_t> start_id) const;
        
        /*directional accuracy: fraction of the time the
        model predicts the correct direction (positive or
        negative) of the training set*/
        double directional_accuracy(std::optional<size_t> start_id,
                                  std::optional<size_t> end_id) const;
        
        double BoxLjung_stat();
        
        bool BoxLjung_test(double sigLevel = 0.05);
        
        
    };
    
    template<
            ts_type,
            bool fractional_order = false,
            bool is_seasonal = false
            >
    class ARIMA : protected ARMA
    {
        
    protected:
        
        bool fractional_order;
        
        bool is_seasonal;
        
    public:
        
        ARIMA() {};
        
        ARIMA(ts_type series);
        
        ARIMA(ts_type series, bool fractional, bool seasonal);
        
        //T is a size_t or a double
        template<typename T>
        T estimate_difference();
        
        std::vector<size_t> estimate_seasonality();
    };
    
    template<typename ts_type>
    using SARIMA = ARIMA<ts_type, false, true>
    
    template<typename ts_type>
    using ARFIMA = ARIMA<ts_type, true, false>
    
    template<typename ts_type>
    using SARFIMA = ARIMA<ts_type, true, true>
    
}//end namespace TimeSeries

#endif //TS_ARIMA_HPP
