/**
 @author: Zane Jakobs
 @brief: type_traits-like templates for the TimeSeries
 library
 */
#ifndef TS_TYPE_TRAITS_HPP
#define TS_TYPE_TRAITS_HPP

#include <type_traits>
#include <vector>
#include <Eigen/Core>

namespace TimeSeries
{
    using namespace std;
    /**
     *@author: Zane Jakobs
     *@param T: type to determine if it's an
     STL-like 1-d container
     */
    template<typename T, typename _ = void>
    struct is_1d_container : false_type {};
    
    
    template<typename T>
    struct is_1d_container<
            T,
            conditional_t<
                false,
                void_t<
                    typename T::value_type,
                    typename T::size_type,
                    typename T::allocator_type,
                    typename T::iterator,
                    typename T::const_iterator,
                    decltype(declval<T>().size()),
                    decltype(declval<T>().begin()),
                    decltype(declval<T>().end()),
                    decltype(declval<T>().cbegin()),
                    decltype(declval<T>().cend())
                    >,
                void
                >
    > : public true_type {};
    
    /**
     *@author: Zane Jakobs
     *@param T: element type of vector
     *@param V: type to determine if it's a std::vector<T>
     */
    template<typename V, typename T, typename _ = void>
    struct is_std_vector_t : false_type {};
    
    template<typename V, typename T>
    struct is_std_vector_t< V, T,
                        typename enable_if<
                            is_same<V,
                                vector<T, typename V::allocator_type >
                            >::value
                        >::type
                    > : public true_type {};
    
    
    /**
     *@author: Zane Jakobs
     *@param T: type to determine if it's a container type,
     including STL-like containers and C-arrays
     
     NOTE: CURRENTLY UNSUPPORTED, REQUIRES C++20
    template<typename T>
    struct is_container_t
    {
        bool value = (is_1d_container<T>::value or
                      is_bounded_array<T>::value or
                      is_unbounded_array<T>::value);
    };
    */
    //gets number of elements in a C-array
    template<typename T, size_t N>
    constexpr size_t C_array_size(T(&)[N])
    {
        return N;
    }
    

}//end namespace TimeSeries
#endif //TS_TYPE_TRAITS_HPP
