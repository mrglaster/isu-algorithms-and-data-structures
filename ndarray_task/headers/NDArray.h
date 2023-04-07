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
        int size;
        std::vector<int> shapes;
        std::vector<T> data;

    public:

        /**Creates single-dimensional array filled with preset value*/
        NDArray(int sh, T val = INT_MAX) {
            shapes.push_back(sh);
            size = sh;
            for (int i = 0; i < size; i++) data.push_back(val);
        };


        /**Creates multidimensional array filled with preset value*/
        NDArray(std::vector<int>* sh, T val = INT_MAX) {
            int prod = 1;
            for (auto value : *sh) {
                prod *= value;
                shapes.push_back(value);
            }
            size = prod;
            for (int i = 0; i < size; i++) data.push_back(val);
        };

        /**Fills created NDArray with random values*/
        void fillRandom() {
            srand(time(NULL));
            data.clear();
            for (int i = 0; i < size; i++) {
                T num = rand();
                int sign = rand() % 2;
                if (sign == 0) num *= -1;
                data.push_back(num);
            }
        }

        /**Lets us to get elements by id*/
        T& operator() (int i) {
            assert((i < size));
            return data[i];
        };

        T& operator() (int i, int j) {
            assert(i < shapes[0]);
            assert(j < shapes[1]);
            return data[i * shapes[1] + j];
        };

        /**Assignment operator overload*/
        NDArray<T>& operator = (const NDArray<T>& v) {
            data.clear();
            size = v.size;
            shapes = v.shapes;
            if (v.data.empty()) return *this;
            for (int i = 0; i < size; i++)
                data.push_back(v.data[i]);
            return *this;
    };

        /**Element-wise adding*/
        NDArray<T>& operator += (const NDArray<T>& v) {
            assert(shapes == v.shapes);
            for (int i = 0; i < size; i++) data[i] += v.data[i];
            return *this;
        }


        NDArray<T> operator + (const NDArray<T>& v) const{
            NDArray<T> res(*this);
            return res += v;
        }

        /**Add some value to all the elements*/
        NDArray<T>& operator += (T n) {
            for (int i = 0; i < size; i++)
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
            for (int i = 0; i < size; i++)
                data[i] *= v.data[i];
            return *this;
    }

        NDArray<T> operator * (const NDArray<T>& v) const {
            NDArray<T> res(*this);
            return res *= v;
        }

        /**Multiply all the elements by a number*/
        NDArray<T>& operator *= (T n) {
            for (int i = 0; i < size; i++)
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
            for (int i = 0; i < size; i++)
                data[i] /= v.data[i];
            return *this;
        }

        NDArray<T> operator / (const NDArray<T>& v) const {
            NDArray<T> res(*this);
            return res /= v;
        }


        NDArray<T>& operator /= (T n) {
            for (int i = 0; i < size; i++)
                data[i] /= n;
            return *this;
        }

        NDArray<T> operator / (T n) const {
            NDArray<T> res(*this);
            return res /= n;
        }

        /**Replaces all the values with an other value*/
        NDArray<T>& filling(T val) {
            data.clear();
            for (int i = 0; i < size; i++)
                data.push_back(val);
            return *this;
        }

        /**Transposes NDArray*/
        NDArray<T>& transpose() {
            std::vector<int> s{ shapes[1], shapes[0] };
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
            std::vector<int> s{ shapes[0], v1.shapes[1] };
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
            T imin = data[0];
            const int row_size = shapes[1];
            const int start = (n == -1) ? 0 : n * row_size;
            const int end = (n == -1) ? size : start + row_size;

            for (int i = start; i < end; i++) {
                imin = (data[i] < imin) ? data[i] : imin;
            }
            return imin;
        }


        /**Finds maximum*/
        T max(T n = -1) {
            assert(n < shapes[0]);
            T imax = data[0];
            const int row_size = shapes[1];
            const int start = (n == -1) ? 0 : n * row_size;
            const int end = (n == -1) ? size : start + row_size;
            for (int i = start; i < end; i++) {
                imax = (data[i] > imax) ? data[i] : imax;
            }
            return imax;
        }


        /**Find the mean value*/
       T mean(T n = -1) {
            assert(n < shapes[0]);
            T imean = 0;
            const int row_size = shapes[1];
            const int start = (n == -1) ? 0 : n * row_size;
            const int end = (n == -1) ? size : start + row_size;
            for (int i = start; i < end; i++) {
                imean += data[i];
            }
            imean /= row_size;
            return imean;
        }

        /**Prints NDArray into the console*/
        void ndaPrint() {
            if (shapes.size() == 1) {
                if (!data.empty())
                    for (int i = 0; i < size; i++)
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

void functionalityDemonstration(){
            //Empty array demo
            std::cout<<"Empty array creation demo" << std::endl;
            NDArray<int> emptyOne(16); //One dimension, 16 elements
            emptyOne.ndaPrint();

            //Random fill demo
            std::cout<<std::endl<<std::endl<<"Let's fill it with random values!"<<std::endl<<std::endl;
            emptyOne.fillRandom();
            emptyOne.ndaPrint();

            //Arrays filled with preset values
            std::vector<int> dimensionsOne{ 4, 5 }; // 4 Rows & 5 Columns
            std::vector<int> dimensionsTwo{ 3, 5 }; // 3 Rows & 5 columns
            std::vector<int> dimensionsThree{ 2, 2 }; // 2 Rows & 2 columns
            NDArray<int> ndaDemoOne(&dimensionsOne, 0); //Filled with 0
            NDArray<int> ndaDemoTwo(&dimensionsTwo, 1); //Filled with 1
            NDArray<float> ndaDemoThree(&dimensionsThree, 42.5); //Filled with 42.5
            std::cout<<"\nFilled with 0s"<<std::endl<<std::endl;
            ndaDemoOne.ndaPrint();
            std::cout<<"\nFilled with 1s"<<std::endl<<std::endl;
            ndaDemoTwo.ndaPrint();
            std::cout<<"\nFilled with 42.5s"<<std::endl<<std::endl;
            ndaDemoThree.ndaPrint();
            std::cout<<std::endl<<std::endl;


            //Element-wise adding demo
            std::cout<<"Element-wise adding demo: "<<std::endl<<std::endl;
            NDArray<float> ewaDemo1(&dimensionsThree, 6.12);
            std::cout<<"First: "<<std::endl;
            ewaDemo1.ndaPrint();
            std::cout<<std::endl<<"Second: "<<std::endl<<std::endl;
            NDArray<float> ewaDemo2(&dimensionsThree, 5.425);
            ewaDemo2.ndaPrint();
            std::cout<<"\nAdding with + "<<std::endl<<std::endl;
            (ewaDemo1 + ewaDemo2).ndaPrint();
            std::cout<<std::endl<<std::endl;
            ewaDemo1+=ewaDemo2;
            std::cout<<"Adding with += "<<std::endl<<std::endl;
            ewaDemo1.ndaPrint();
            std::cout<<std::endl<<std::endl;

            //Element-wise Subtraction demo
            std::cout<<"Element-wise Subtraction demo: "<<std::endl<<std::endl;
            NDArray<int> ewaDemo3(&dimensionsThree, 2);
            std::cout<<"\nFirst: "<<std::endl;
            ewaDemo3.ndaPrint();
            NDArray<int> ewaDemo4(&dimensionsThree, 1);
            std::cout<<"\nSecond: "<<std::endl;
            ewaDemo4.ndaPrint();
            std::cout<<"Substract with - "<<std::endl<<std::endl;
            (ewaDemo3 - ewaDemo4).ndaPrint();
            std::cout<<std::endl<<std::endl;
            ewaDemo3-=ewaDemo4;
            std::cout<<"Substract with -= "<<std::endl<<std::endl;
            ewaDemo3.ndaPrint();
            std::cout<<std::endl<<std::endl;

            //Element-wise multiply demo
            std::cout<<"Element-wise Multiply demo: "<<std::endl<<std::endl;
            NDArray<int> ewaDemo5(&dimensionsThree, 4);
            std::cout<<"\nFirst: "<<std::endl;
            ewaDemo5.ndaPrint();
            NDArray<int> ewaDemo6(&dimensionsThree, 8);
            std::cout<<"\nSecond: "<<std::endl;
            ewaDemo6.ndaPrint();
            std::cout<<"\nMultiply  with * "<<std::endl<<std::endl;
            (ewaDemo5 * ewaDemo6).ndaPrint();
            std::cout<<std::endl<<std::endl;
            ewaDemo5*=ewaDemo6;
            std::cout<<"Multiply with *= "<<std::endl<<std::endl;
            ewaDemo5.ndaPrint();
            std::cout<<std::endl<<std::endl;

            //Element-wise divide demo
            std::cout<<"Element-wise Multiply demo: "<<std::endl<<std::endl;
            NDArray<float> ewaDemo7(&dimensionsThree, 8.26);
            std::cout<<"\nFirst: "<<std::endl;
            ewaDemo7.ndaPrint();
            NDArray<float> ewaDemo8(&dimensionsThree, 4.1);
            std::cout<<"\nSecond: "<<std::endl;
            ewaDemo8.ndaPrint();
            std::cout<<"\nDivide  with / "<<std::endl<<std::endl;
            (ewaDemo7 / ewaDemo8).ndaPrint();
            std::cout<<std::endl<<std::endl;
            ewaDemo7/=ewaDemo8;
            std::cout<<"Divide with /= "<<std::endl<<std::endl;
            ewaDemo7.ndaPrint();
            std::cout<<std::endl<<std::endl;

            //Operators of add, substract, divide, multiply by a number
            std::cout<<"Demonstration for Operators of add, substract, divide, multiply by a number"<<std::endl;
            NDArray<int> testRabbit(8, 4); //1 dimension, 8 elements, filled with 4
            std::cout<<"\nLab rabbit:\n";
            testRabbit.ndaPrint();
            std::cout<<"\nAdd 1: \n";
            testRabbit+=1;
            testRabbit.ndaPrint();
            std::cout<<"\nSubstract 1: \n";
            testRabbit-=1;
            testRabbit.ndaPrint();
            std::cout<<"\nMultiply by 2\n";
            testRabbit*=2;
            testRabbit.ndaPrint();
            std::cout<<"\nDivide by 2\n";
            testRabbit/=2;
            testRabbit.ndaPrint();

            //Matrix multiply demo
            std::cout<<"\nMatrix Multiply Demo:\n";
            std::vector<int> shapeOne{ 2, 3 };
            std::vector<int> shapeTwo{ 3, 2 };
            NDArray<int> ndaMatrixOne(&shapeOne, 4);
            std::cout<<"\nFirst:\n";
            ndaMatrixOne.ndaPrint();
            std::cout<<"\nSecond:\n";
            NDArray<int> ndaMatrixTwo(&shapeTwo, 13);
            ndaMatrixTwo.ndaPrint();
            std::cout<<"\nMultiply result: \n";
            (ndaMatrixOne.matMul(ndaMatrixTwo)).ndaPrint();

            //Transpose Matrix
            std::cout<<"\nTranspose Matrix demo:\n";
            std::cout<<"\nSource matrix: \n";
            (ndaDemoTwo).ndaPrint();
            std::cout<<"\nTransposed one:\n";
            ndaDemoTwo.transpose().ndaPrint();
}
#endif // NDARRAY_H_INCLUDED
