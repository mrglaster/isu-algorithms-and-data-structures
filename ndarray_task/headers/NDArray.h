#ifndef NDARRAY_H_INCLUDED
#define NDARRAY_H_INCLUDED

#include <iostream>
#include <vector>
#include <time.h>
#include <cassert>
#include <climits>

template <typename T> class NDArray {

    /**Private fields initialization*/
    private:
       
        vector<int> shapes;
        int dimsAmount;
        vector<T> data;

    public:

        /**Creates single-dimensional array filled with preset value*/
        NDArray(int sh, T val = INT_MAX) {
            shapes.push_back(sh);
            dimsAmount = sh;
            for (int i = 0; i < dimsAmount; i++) data.push_back(val);
        };


        /**Creates multidimensional array filled with preset value*/
        NDArray(vector<int>* sh, T val = INT_MAX) { 
            int prod = 1;
            for (auto value : *sh) {
                prod *= value;
                shapes.push_back(value);
            }
            dimsAmount = prod;
            for (int i = 0; i < dimsAmount; i++) data.push_back(val);
        };

        /**Fills created NDArray with random values*/
        void fillRandom() {
            srand(time(NULL));
            data.clear();
            for (int i = 0; i < dimsAmount; i++) {
                T num = rand();
                int sign = rand() % 2;
                if (sign == 0) num *= -1;
                data.push_back(num);
            }
        }

        /**Lets us to get elements by id*/
        T& operator() (int i) {
            assert((i < dimsAmount));
            return data[i];
        };


        /***/
        T& operator() (int i, int j) {
            assert(i < shapes[0]);
            assert(j < shapes[1]);
            return data[i * shapes[1] + j];
        };

        /**Assignment operator overload*/
        NDArray<T>& operator = (const NDArray<T>& v) {
            data.clear();
            dimsAmount = v.dimsAmount;
            shapes = v.shapes;
            if (v.data.empty()) return *this;
            for (int i = 0; i < dimsAmount; i++)
                data.push_back(v.data[i]);
            return *this;
    };

        /**Element-wise adding*/
        NDArray<T>& operator += (const NDArray<T>& v) {
            assert(shapes == v.shapes);
            for (int i = 0; i < dimsAmount; i++) data[i] += v.data[i];
            return *this;
        }


        NDArray<T> operator + (const NDArray<T>& v) const{
            NDArray<T> res(*this);
            return res += v;
        }

        /**Add some value to all the elements*/
        NDArray<T>& operator += (T n) {
            for (int i = 0; i < dimsAmount; i++)
                data[i] += n;
            return *this;
        }


        NDArray<T> operator + (T n) const {
            NDArray<T> res(*this);
            return res += n;
        }

        /**Element-wise multiply*/
        NDArray<T>& operator *= (const NDArray<T>& v) {
            assert(shapes == v.shapes);
            for (int i = 0; i < dimsAmount; i++)
                data[i] *= v.data[i];
            return *this;
    }

        NDArray<T> operator * (const NDArray<T>& v) const {
            NDArray<T> res(*this);
            return res *= v;
        }

        /**Multiply all the elements by a number*/
        NDArray<T>& operator *= (T n) {
            for (int i = 0; i < dimsAmount; i++)
                data[i] *= n;
            return *this;
        }


        NDArray<T> operator * (T n) const {
            NDArray<T> res(*this);
            return res *= n;
        }

        /**Turns values into negative ones*/
        NDArray<T> operator -() const {
            NDArray<T> res(*this);
            return res *= -1;
        }

        /**Element-wise substraction*/
        NDArray<T>& operator -= (const NDArray<T>& v) {
            return (*this += (-v));
        }

        NDArray<T> operator - (const NDArray<T>& v) const {
            NDArray<T> res(*this);
            return res -= v;
        }

        /**Sub some value from all elements */
        NDArray<T>& operator -= (T n) {
            return (*this += (-n));
        }


        NDArray<T> operator - (T n) const {
            NDArray<T> res(*this);
            return res -= n;
        }

        /**Element-wise divide*/
        NDArray<T>& operator /= (const NDArray<T>& v) {
            assert(shapes == v.shapes);
            for (int i = 0; i < dimsAmount; i++)
                data[i] /= v.data[i];
            return *this;
        }

        NDArray<T> operator / (const NDArray<T>& v) const {
            NDArray<T> res(*this);
            return res /= v;
        }
	
		/**Divide all the elements by a number*/
        NDArray<T>& operator /= (T n) {//деление на число всех элементов
            for (int i = 0; i < dimsAmount; i++)
                data[i] /= n;
            return *this;
        }

        NDArray<T> operator / (T n) const {
            NDArray<T> res(*this);
            return res /= n;
        }

        /**Replaces all the values with an other value*/
        NDArray<T>& replaceAllValues(T val) {
            data.clear();
            for (int i = 0; i < dimsAmount; i++)
                data.push_back(val);
            return *this;
        }

        /**Transposes NDArray*/
        NDArray<T>& transpose() {
            vector<int> s{ shapes[1], shapes[0] };
            NDArray<T> res(&s, 0);
            for (int i = 0; i < shapes[0]; i++) {
                for (int j = 0; j < shapes[1]; j++) {
                    res(j, i) = (*this)(i, j);
                }
            }
            *this = res;
            return *this;
        }

        /**Matrix multiplying*/
        NDArray<T> matMul(NDArray<T>& v1) {
            assert(shapes[1] == v1.shapes[0]);
            vector<int> s{ shapes[0], v1.shapes[1] };
            NDArray<T> res(&s, 0);
            NDArray<T> v(1);
            v = v1;
            v.transpose();
            for (int k = 0; k < shapes[0]; k++) {
                for (int i = 0; i < v.shapes[0]; i++) {
                    int n = 0;
                    for (int j = 0; j < shapes[1]; j++) {
                        n += (*this)(k, j) * v(i, j);
                    }
                    res(k, i) = n;
                }
            }
            return res;
        }

        /**Find minimal value*/
        T min(T n = -1) {
            assert(n < shapes[0]);
            T minVal = data[0];
            if (n == -1) {
                for (auto val : data)
                    if (val < minVal)
                        minVal = val;
            } else {
                minVal = data[n * shapes[1]];
                for (int i = n * shapes[1]; i < n * shapes[1] + shapes[1]; i++) {
                    if (data[i] < minVal)
                        minVal = data[i];
                }
            }
            return minVal;
        }

        /**Find maximal value*/
        T max(T n = -1) {
            assert(n < shapes[0]);
            T maxVal = data[0];
            if (n == -1) {
                for (auto val : data)
                    if (val > maxVal)
                        maxVal = val;
            } else {
                maxVal = data[n * shapes[1]];
                for (int i = n * shapes[1]; i < n * shapes[1] + shapes[1]; i++) {
                    if (data[i] > maxVal)
                        maxVal = data[i];
                }
            }
            return maxVal;
        }

        /**Find the mean value*/
        T mean(T n = -1) {
            assert(n < shapes[0]);
            T meanVal = 0;
            if (n == -1) {
                for (auto val : data){
                    meanVal += val;
                }
                meanVal = meanVal / dimsAmount;
            } else {
                for (int i = n * shapes[1]; i < n * shapes[1] + shapes[1]; i++) {
                    meanVal += data[i];
                }
                meanVal = meanVal / shapes[1];
            }
            return meanVal;
        }

        /**Prints NDArray into the console*/
        void ndaPrint() {
            if (shapes.dimsAmount() == 1) {
                if (!data.empty())
                    for (int i = 0; i < dimsAmount; i++)
                        std::cout << data[i] << " ";
                std::cout << std::endl;
            } else {
                for (int i = 0; i < shapes[0]; i++) {
                    for (int j = 0; j < shapes[1]; j++)
                        std::cout << (*this)(i, j) << " ";
                    std::cout << std::endl;
                }
            }
       };

};
#endif // NDARRAY_H_INCLUDED
