
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cmath>
#include <ostream>

namespace detail
{
    template<typename T1,typename T2,typename F>
	void transform_matrix1(T1 const& mat1, T2& mat2, F f)
	{
		std::transform(mat1.cbegin(), mat1.cend(), mat2.begin(), f);
	}

	template<typename T1,typename T2,typename T3,typename F>
	void transform_matrix2(T1 const& mat1, T2 const& mat2, T3& mat3, F f)
	{
		std::transform(mat1.cbegin(), mat1.cend(), mat2.cbegin(), mat3.begin(), f);
	}

}

template <typename T>
auto matrix_multiplication( std::vector<T> const& mat1, std::vector<T> const mat2)
{
    std::vector<T> temp_mat;
    int n = std::sqrt(static_cast<int>(mat1.size()));
        temp_mat.resize(mat1.size());
        for(int i{0}; i <= n - 1; i++)
        {
            for(int j{0}; j < n; j++)
            {
                T temp_val {0};
                for(int k{0}; k < n; k++)
                {
                    temp_val += mat1[n * i + k] * mat2[n * k + j];
                }
                temp_mat[n * i + j] = temp_val;
            }   
        } 
        return temp_mat;
} 

auto add = [](auto const& x, auto const& y){return x+y;};
auto sub = [](auto const& x, auto const& y){return x-y;};


template<typename T>
struct matrix
{
    std::vector<T> data;
    int dim = std::sqrt(static_cast<int>(data.size()));

    T&       operator()(int i, int j) { return data[i * dim + j]; }
	T const& operator()(int i, int j) const{ return data[i * dim + j]; }
    
    matrix<T>& operator += (matrix<T> const& m)
    {
        detail::transform_matrix2((*this).data, m.data, (*this).data, add);
        return *this;
    }
    
    matrix<T>& operator -= (matrix<T> const& m)
    {
        detail::transform_matrix2((*this).data, m.data, (*this).data, sub);
        return *this;
    }

    matrix<T>& operator *= (T const& s)
    {
        detail::transform_matrix1((*this).data, (*this).data, [=](T const& x){return x*s;});
        return *this;
    }

    matrix<T>& operator /= (T const& s)
    {
        detail::transform_matrix1((*this).data, (*this).data, [=](T const& x){return x / s;});
        return *this;
    }
    
    matrix<T>& operator *= (matrix<T> const& mat)
    {
        (*this).data.swap(matrix_multiplication((*this).data, mat.data));
        return *this;
    } 
    
	int size() const
	{
		return static_cast<int>(data.size());
	}

    auto begin()
	{
		return data.begin();
	}

	auto cbegin() const
	{
		return data.cbegin();
	}

	auto end()
	{
		return data.end();
	}

	auto cend() const
	{
		return data.cend();
	}

    //a konstruktorok
    matrix() = default;
    matrix(matrix const&) = default;
	matrix(matrix&&) = default;
	matrix<T>& operator = (matrix const&)=default;
	matrix<T>& operator = (matrix&&)=default;
    matrix(std::initializer_list<T> const&  i): data{i}{}
    
    template<typename F>
        matrix(F f,int n)
        {
            data.resize(n);
            dim = std::sqrt(static_cast<int>(data.size()));
            
            for(int i = 0; i < n; i++)
            {
                data[i] = f(i);
            }
        }
    //

    
};

//+
template<typename T>
matrix<T> operator+(matrix<T> const& mat1, matrix<T> const& mat2)
{
    matrix<T> a;
    a.data.resize(mat1.size());
    a.dim = mat1.dim;
    detail::transform_matrix2(mat1.data, mat2.data, a.data, add);
    return a;
}

template<typename T>
matrix<T> && operator+(matrix<T> && mat1, matrix<T> const& mat2)
{
    detail::transform_matrix2(mat1.data, mat2.data, mat1.data, add);
    return std::move(mat1);
}

template<typename T>
matrix<T> && operator+(matrix<T> const& mat1, matrix<T> && mat2)
{
    detail::transform_matrix2(mat1.data, mat2.data, mat2.data, add);
    return std::move(mat2);
}

template<typename T>
matrix<T> && operator+(matrix<T> && mat1, matrix<T> && mat2)
{
    detail::transform_matrix2(mat1.data, mat2.data, mat1.data, add);
    return std::move(mat1);
}

//-

template<typename T>
matrix<T> operator-(matrix<T> const& mat1, matrix<T> const& mat2)
{
    matrix<T> a;
    a.data.resize(mat1.data.size());
    a.dim = mat1.dim;
    detail::transform_matrix2(mat1.data, mat2.data, a.data, sub);
    return a;
}

template<typename T>
matrix<T> && operator-(matrix<T> && mat1, matrix<T> const& mat2)
{
    detail::transform_matrix2(mat1.data, mat2.data, mat1.data, sub);
    return std::move(mat1);
}

template<typename T>
matrix<T> && operator-(matrix<T> const& mat1, matrix<T> && mat2)
{
    detail::transform_matrix2(mat1.data, mat2.data, mat2.data, sub);
    return std::move(mat2);
}

template<typename T>
matrix<T> && operator-(matrix<T> && mat1, matrix<T> && mat2)
{
    detail::transform_matrix2(mat1.data, mat2.data, mat1.data, sub);
    return std::move(mat1);
}

//*
template<typename T>
matrix<T> operator*(matrix<T> const& mat, T const& s)
{
    matrix<T> a;
    a.data.resize(mat.data.size());
    a.dim = mat.dim;
    detail::transform_matrix1(mat.data, a.data, [=](T const& b){return b * s;});
    return a;
}

template<typename T>
matrix<T> && operator*(matrix<T> && mat,T const& s)
{
    detail::transform_matrix1(mat.data, mat.data, [=](T const& a){return a * s;});
    return std::move(mat);
}

template<typename T>
matrix<T> operator*(T const& s, matrix<T> const& mat)
{
    matrix<T> a;
    a.data.resize(mat.data.size());
    a.dim = mat.dim;
    detail::transform_matrix1(mat.data, a.data, [=](T const& b){return s * b;});
    return a;
}

template<typename T>
matrix<T> && operator*(T const& s, matrix<T> && mat)
{
    detail::transform_matrix1(mat.data, mat.data, [=](T const& x){return s * x;});
    return std::move(mat);
}

// /

template<typename T>
matrix<T> operator/(matrix<T> const& mat, T const& s)
{
    matrix<T> a;
    a.data.resize(mat.data.size());
    a.dim = mat.dim;
    detail::transform_matrix1(mat.data, a.data, [=](T const& b){return b / s;});
    return a;
}

template<typename T>
matrix<T> && operator/(matrix<T> && mat, T const& s)
{
    detail::transform_matrix1(mat.data, mat.data, [=](T const& a){return a / s;});
    return std::move(mat);
}

 // matrix * matrix 
template<typename T>
matrix<T> operator*(matrix<T> const& mat1, matrix<T> const& mat2)
{
    matrix<T> r;
    r.data = matrix_multiplication(mat1.data, mat2.data);
    r.dim = mat1.dim;
    return r;
}

template<typename T>
matrix<T> && operator*(matrix<T> const& mat1, matrix<T> && mat2)
{   
    std::vector<T> r = matrix_multiplication(mat1.data, mat2.data);
    mat2.data.swap(r);
    return std::move(mat2);
}

template<typename T>
matrix<T> && operator*(matrix<T> && mat1, matrix<T> const& mat2)
{
    std::vector<T> r = matrix_multiplication(mat1.data, mat2.data);
    mat1.data.swap(r);
    return std::move(mat1);
}

template<typename T>
matrix<T> && operator*(matrix<T> && mat1, matrix<T> && mat2)
{
    std::vector<T> r = matrix_multiplication(mat1.data, mat2.data);
    mat1.data.swap(r);
    return std::move(mat1);
}

//<<
template<typename T>
std::ostream& operator<<(std::ostream& o, matrix<T> const& mat)
{
	int n{mat.dim};
	for(int i = 0; i <= n - 1; i++)
	{
        for(int j = 0; j <= n-1; j++)
        {
            o << mat(i,j) << " ";
        }
        o<<"\n";
	}
	return o;
}
