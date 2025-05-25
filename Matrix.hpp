#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <chrono>
#include <memory>

template<typename T> struct Matrix{
	int32_t h_numb, v_numb;
	T **data;
	Matrix(int32_t h_numb, int32_t v_numb):h_numb{h_numb}, v_numb{v_numb}, data{new T *[v_numb]} {
		*this->data = new T[h_numb * v_numb]{};
		for (int32_t i{1}; i < v_numb; ++i) {
			this->data[i] = *this->data + h_numb * i;
		}
	}
	Matrix(int32_t h_numb, int32_t v_numb, const std::initializer_list<T> &values): Matrix(h_numb, v_numb) {
		::memcpy(*this->data, values.begin(), std::min(uint64_t(this->h_numb) * this->v_numb, uint64_t(values.size())) * sizeof(T));
	}
	~Matrix() {
		delete[] *this->data;
		delete[] this->data;
	}
};

#endif
