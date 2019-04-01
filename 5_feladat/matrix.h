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
auto matrix_multiplication1( int N, std::vector<T> const& mat1, std::vector<T> const& mat2)
{
    std::vector<T> temp;
    temp.resize(N * N);
    for(int i{0}; i < N; i++)
    {
        for(int j{0}; j < N; j++)
        {
            T temp_val {0};
            for(int k{0}; k < N; k++)
            {
                temp_val += mat1[N * i + k] * mat2[N * k + j];
            }
            temp[N * i + j] = temp_val;
        }   
    } 
    return temp;
} 

template <typename T>
void matrix_multiplication2( int N, std::vector<T>& mat1, std::vector<T> const& mat2)
{
    std::vector<T> temp;
    temp.resize(N);
    for(int i{0}; i < N; i++)
    {
        for(int j{0}; j < N; j++)
        {
            T temp_val {0};
            for(int k{0}; k < N; k++)
            {
                temp_val += mat1[N * i + k] * mat2[N * k + j];
            }
            temp[j] = temp_val;
        }

        for (int l{0}; l < N; l++)
        {
            mat1[i * N + l] = temp[l];
        }   
    } 
} 

template <typename T>
void matrix_multiplication3( int N, std::vector<T> const& mat1, std::vector<T>& mat2)
{
    std::vector<T> temp;
    temp.resize(N);
    for(int i{0}; i < N; i++)
    {
        for(int j{0}; j < N; j++)
        {
            T temp_val {0};
            for(int k{0}; k < N; k++)
            {
                temp_val += mat1[N * i + k] * mat2[N * k + j];
            }
            temp[j] = temp_val;
        }

        for (int l{0}; l < N; l++)
        {
            mat2[i * N + l] = temp[l];
        }   
    } 
} 
auto add = [](auto const& x, auto const& y){return x+y;};
auto sub = [](auto const& x, auto const& y){return x-y;};


template<typename T>

class matrix
{
private:
    int dim;
    std::vector<T> data;
   
public:
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

    int N()
    {
        int a = dim;
        return a;
    }
    
	int size()
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
    //

    matrix()
    {
        dim = 0;
        data;
    }

    matrix(int N)
    {
        dim = N;
        data;
        data.resize(N * N);
    }

    matrix(matrix const& cpy): dim{cpy.dim}, data{cpy.data}
    {
        if( dim != cpy.dim)
        {
            exit(-1);
        }
    }
    
    matrix(int N, std::vector<T> m): dim{N}, data{m}
    {
    }

    matrix(matrix&& mv)
    {
        dim = std::move(mv.dim);
        std::swap(data, mv.data);
    }

    matrix<T>& operator = (matrix const& mat)
    {
        dim = mat.dim;
        data = mat.data;
    }

	matrix<T>& operator = (matrix&& mat)
    {
        dim = mat.dim;
        std::swap(data, mat.data);
        
    }

    matrix(int const& d,  std::initializer_list<T> const&  m)
    {   
        if (!(d * d == static_cast<int>(m.size())))
        {
            std::cout << "Error in: size";
            exit(-1);
        }
        dim = d;
        data = m;
    }

    
    template<typename F>
    matrix(F f, int N)
    {
        data.resize(N * N);
        dim = N;
            
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                (*this)(i, j) = f(i, j);
            }
        }
    }
    //

    template<typename T1>
    friend std::ostream& operator<<(std::ostream& o, matrix<T1> const& mat);

    template<typename T1,typename T2,typename F>
    friend void transform_matrix1(T1 const& mat1, T2& mat2, F f);

    template<typename T1,typename T2,typename T3,typename F>
    friend void transform_matrix2(T1 const& mat1, T2 const& mat2, T3& mat3, F f);

    template<typename T1>
    friend matrix<T1> operator+(matrix<T1> const& mat1, matrix<T1> const& mat2);

    template<typename T1>
    friend matrix<T1> && operator+(matrix<T1> && mat1, matrix<T1> const& mat2);

    template<typename T1>
    friend matrix<T1> && operator+(matrix<T1> const& mat1, matrix<T1> && mat2);

    template<typename T1>
    friend matrix<T1> && operator+(matrix<T1> && mat1, matrix<T1> && mat2);

    template<typename T1>
    friend matrix<T1> operator-(matrix<T1> const& mat1, matrix<T1> const& mat2);

    template<typename T1>
    friend matrix<T1> && operator-(matrix<T1> && mat1, matrix<T1> const& mat2);

    template<typename T1>
    friend matrix<T1> && operator-(matrix<T1> const& mat1, matrix<T1> && mat2);

    template<typename T1>
    friend matrix<T1> && operator-(matrix<T1> && mat1, matrix<T1> && mat2);

    template<typename T1>
    friend matrix<T1> operator*(matrix<T1> const& mat, T1 const& s);

    template<typename T1>
    friend matrix<T1> && operator*(matrix<T1> && mat,T1 const& s);

    template<typename T1>
    friend matrix<T1> operator*(T1 const& s, matrix<T1> const& mat);

    template<typename T1>
    friend matrix<T1> && operator*(T1 const& s, matrix<T1> && mat);

    template<typename T1>
    friend matrix<T1> operator/(matrix<T1> const& mat, T1 const& s);

    template<typename T1>
    friend matrix<T1> && operator/(matrix<T1> && mat, T1 const& s);

    template<typename T1>
    friend matrix<T1> operator*(matrix<T1> const& mat1, matrix<T1> const& mat2);

    template<typename T1>
    friend matrix<T1> && operator*(matrix<T1> const& mat1, matrix<T1> && mat2);

    template<typename T1>
    friend matrix<T1> && operator*(matrix<T1> && mat1, matrix<T1> const& mat2);

    template<typename T1>
    friend matrix<T1> && operator*(matrix<T1> && mat1, matrix<T1> && mat2);

    template<typename T1>
    friend bool check(matrix<T1> const& mat1, matrix<T1> const& mat2);
    

};

//+
template<typename T>
matrix<T> operator+(matrix<T> const& mat1, matrix<T> const& mat2)
{
    matrix<T> a(mat1.dim);
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
    matrix<T> a(mat1.dim);
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
    matrix<T> a(mat.dim);
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
    matrix<T> a(mat.dim);
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
    matrix<T> a(mat.dim);
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
    r.dim = mat1.dim;
    r.data = matrix_multiplication1(mat1.dim, mat1.data, mat2.data);
    return r;
}

template<typename T>
matrix<T> && operator*(matrix<T> const& mat1, matrix<T> && mat2)
{   
    matrix_multiplication3(mat1.dim, mat1.data, mat2.data);
    return std::move(mat2);
}

template<typename T>
matrix<T> && operator*(matrix<T> && mat1, matrix<T> const& mat2)
{
    matrix_multiplication2(mat1.dim, mat1.data, mat2.data);
    return std::move(mat1);
}

template<typename T>
matrix<T> && operator*(matrix<T> && mat1, matrix<T> && mat2)
{
    matrix_multiplication2(mat1.dim, mat1.data, mat2.data);
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

template<typename T>
bool check(matrix<T> const& mat1, matrix<T> const& mat2)
{
    if (mat1.dim != mat2.dim)
    {
        return 0;
    }
    int n = mat1.dim;
    double toll{1e-12};
    for(int i = 0; i < n; i++)
    {
        if ((mat1.data[i] - mat2.data[i]) > toll)
        {
            return 0;
        }
            
    }
    return 1;
}
