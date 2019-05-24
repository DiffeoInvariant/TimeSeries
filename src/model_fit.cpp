#include "../include/model_fit.hpp"
#include "../include/ts_error.hpp"
#include <type_traits>
#include <Eigen/Dense>

namespace TimeSeries
{
    template<typename T>
    Mat<T> ols_model_matrix(Mat<T>& X){
        
        if constexpr(!std::is_arithmetic<T>::value){
            throw TimeSeries::NonArithmeticTypeError;
        }
        
        return (X.transpose() * X);
    }
    
    template<typename T>
    std::vector<T> ols_fit(Vec<T>& y, Mat<T>& X){
        
        if constexpr(!std::is_arithmetic<T>::value){
            throw TimeSeries::NonArithmeticTypeError;
        }
        
        auto beta = X.colPivHouseholderQr().solve(y);
        
        return beta;
    }
}
