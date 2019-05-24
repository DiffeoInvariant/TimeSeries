#ifndef TS_MODEL_FIT_HPP
#define TS_MODEL_FIT_HPP

#include <vector>
#include <Eigen/Core>
#include "base.hpp"


namespace TimeSeries
{
    template<typename T>
    using Mat<T> = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    
    template<typename T>
    extern Mat<T> ols_model_matrix(Mat<T>& X);
    
    template<typename T>
    extern std::vector<T> ols_fit(Vec<T>& y, Mat<T>& X);
    
    
    
}



#endif
