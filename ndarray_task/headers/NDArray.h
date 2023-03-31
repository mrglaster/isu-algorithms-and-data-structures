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
        vector<int> shape;
        int size;
        vector<T> vec;

    public:

        /**Creates single-dimensional array filled with preset value*/
        NDArray(int sh, T val = INT_MAX) {
            shape.push_back(sh);
            size = sh;
            for (int i = 0; i < size; i++) vec.push_back(val);
        };


        /**Creates multidimensional array filled with preset value*/
        NDArray(vector<int>* sh, T val = INT_MAX) { //создание многомерного массива заполненого каким-либо значением
            int prod = 1;
            for (auto value : *sh) {
                prod *= value;
                shape.push_back(value);
            }
            size = prod;
            for (int i = 0; i < size; i++) vec.push_back(val);
        };

        /**Fills created NDArray with random values*/
        void fillRandom() {
            srand(time(NULL));
            vec.clear();
            for (int i = 0; i < size; i++) {
                T num = rand();
                int sign = rand() % 2;
                if (sign == 0) num *= -1;
                vec.push_back(num);
            }
        }

        /**Lets us to get elements by id*/
        T& operator() (int i) {
            assert((i < size));
            return vec[i];
        };


        /***/
        T& operator() (int i, int j) {
        assert(i < shape[0]);
        assert(j < shape[1]);
        return vec[i * shape[1] + j];
    };

        /**Assignment operator overload*/
        NDArray<T>& operator = (const NDArray<T>& v) {
            vec.clear();
            size = v.size;
            shape = v.shape;
            if (v.vec.empty()) return *this;
            for (int i = 0; i < size; i++)
                vec.push_back(v.vec[i]);
            return *this;
    };

        /**Element-wise adding*/
        NDArray<T>& operator += (const NDArray<T>& v) {//поэлементное сложение
            assert(shape == v.shape);
            for (int i = 0; i < size; i++) vec[i] += v.vec[i];
            return *this;
        }


        NDArray<T> operator + (const NDArray<T>& v) const{
            NDArray<T> res(*this);
            return res += v;
        }

        /**Add some value to all the elements*/
        NDArray<T>& operator += (T n) {
            for (int i = 0; i < size; i++)
                vec[i] += n;
            return *this;
        }


        NDArray<T> operator + (T n) const {
            NDArray<T> res(*this);
            return res += n;
        }

        /**Element-wise multiply*/
        NDArray<T>& operator *= (const NDArray<T>& v) {
            assert(shape == v.shape);
            for (int i = 0; i < size; i++)
                vec[i] *= v.vec[i];
            return *this;
    }

        NDArray<T> operator * (const NDArray<T>& v) const {
            NDArray<T> res(*this);
            return res *= v;
        }

        /**Multiply all the elements by a number*/
        NDArray<T>& operator *= (T n) {
            for (int i = 0; i < size; i++)
                vec[i] *= n;
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
        NDArray<T>& operator -= (const NDArray<T>& v) {//поэлементное вычитание
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
        NDArray<T>& operator /= (const NDArray<T>& v) {//поэлементное деление
            assert(shape == v.shape);
            for (int i = 0; i < size; i++)
                vec[i] /= v.vec[i];
            return *this;
        }

        NDArray<T> operator / (const NDArray<T>& v) const {
            NDArray<T> res(*this);
            return res /= v;
        }


        NDArray<T>& operator /= (T n) {//деление на число всех элементов
            for (int i = 0; i < size; i++)
                vec[i] /= n;
            return *this;
        }

        NDArray<T> operator / (T n) const {
            NDArray<T> res(*this);
            return res /= n;
        }

        /**Replaces all the values with an other value*/
        NDArray<T>& filling(T val) {
            vec.clear();
            for (int i = 0; i < size; i++)
                vec.push_back(val);
            return *this;
        }

        /**Transposes NDArray*/
        NDArray<T>& transpose() {
            vector<int> s{ shape[1], shape[0] };
            NDArray<T> res(&s, 0);
            for (int i = 0; i < shape[0]; i++) {
                for (int j = 0; j < shape[1]; j++) {
                    res(j, i) = (*this)(i, j);
                }
            }
            *this = res;
            return *this;
    }

        /**Matrix multiplying*/
        NDArray<T> matMul(NDArray<T>& v1) {
            assert(shape[1] == v1.shape[0]);
            vector<int> s{ shape[0], v1.shape[1] };
            NDArray<T> res(&s, 0);
            NDArray<T> v(1);
            v = v1;
            v.transpose();
            for (int k = 0; k < shape[0]; k++) {
                for (int i = 0; i < v.shape[0]; i++) {
                    int n = 0;
                    for (int j = 0; j < shape[1]; j++) {
                        n += (*this)(k, j) * v(i, j);
                    }
                    res(k, i) = n;
                }
            }
            return res;
        }

        /**Find minimal value*/
        T min(T n = -1) {
            assert(n < shape[0]);
            T imin = vec[0];
            if (n == -1) {
                for (auto val : vec)
                    if (val < imin)
                        imin = val;
            } else {
                imin = vec[n * shape[1]];
                for (int i = n * shape[1]; i < n * shape[1] + shape[1]; i++) {
                    if (vec[i] < imin)
                        imin = vec[i];
                }
            }
            return imin;
        }

        /**Find maximal value*/
        T max(T n = -1) {
            assert(n < shape[0]);
            T imax = vec[0];
            if (n == -1) {
                for (auto val : vec)
                    if (val > imax)
                        imax = val;
            } else {
                imax = vec[n * shape[1]];
                for (int i = n * shape[1]; i < n * shape[1] + shape[1]; i++) {
                    if (vec[i] > imax)
                        imax = vec[i];
                }
            }
            return imax;
        }

        /**Find the mean value*/
        T mean(T n = -1) {
            assert(n < shape[0]);
            T imean = 0;
            if (n == -1) {
                for (auto val : vec){
                    imean += val;
                }
                imean = imean / size;
            } else {
                for (int i = n * shape[1]; i < n * shape[1] + shape[1]; i++) {
                    imean += vec[i];
                }
                imean = imean / shape[1];
            }
            return imean;
        }

        /**Prints NDArray into the console*/
        void ndaPrint() {
            if (shape.size() == 1) {
                if (!vec.empty())
                    for (int i = 0; i < size; i++)
                        std::cout << vec[i] << " ";
                std::cout << std::endl;
            } else {
                for (int i = 0; i < shape[0]; i++) {
                    for (int j = 0; j < shape[1]; j++)
                        std::cout << (*this)(i, j) << " ";
                    std::cout << std::endl;
                }
            }
        };
};
#endif // NDARRAY_H_INCLUDED
