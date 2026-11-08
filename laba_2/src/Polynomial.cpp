#include "Polynomial.hpp"
#include <algorithm>
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace {
bool isNearZero(double value) {
    return std::abs(value) <=  1e-9;
}

bool isNear(double left, double right) {
    return std::abs(left - right) <=  1e-9;
}
}

Polynomial::Polynomial(const double* coeff_s, int degree){
    if (degree < -1){
        throw std::invalid_argument("Polynomial: degree must be >= -1");
    }
    if (degree >= 0 && coeff_s == nullptr){
        throw std::invalid_argument("Polynomial: coeffs is null");
    }
    degree_ = degree;
    coeffs_ = new double[degree_ + 1];  
    for (int i = 0; i < degree_ + 1; i++){
        coeffs_[i] = coeff_s[i];
    }
    trim();
}

Polynomial::~Polynomial(){
    delete[] coeffs_;
    coeffs_ = nullptr;
}

void Polynomial::trim(){
    int top = degree_;
    while (top >= 0 && isNearZero(coeffs_[top])){
        top--;
    }
    if (top != degree_){
        double* newCoeff = new double[top+1];
        for (int i = 0; i < top+1; i++){
            newCoeff[i] = coeffs_[i];
        }
        delete[] coeffs_;
        coeffs_ = newCoeff;
        degree_ = top;
    }
}

Polynomial::Polynomial() : coeffs_(nullptr), degree_(-1){}

Polynomial::Polynomial(const Polynomial& origin){
    degree_ = origin.degree_;
    coeffs_ = new double[degree_+1];
    for (int i = 0; i < degree_+1; i++){
        coeffs_[i] = origin.coeffs_[i];
    }
}

Polynomial::Polynomial(Polynomial&& origin) noexcept : coeffs_(origin.coeffs_), degree_(origin.degree_){
    origin.coeffs_ = nullptr;
    origin.degree_ = -1;
}

void Polynomial::swap(Polynomial& other) noexcept {
    std::swap(coeffs_, other.coeffs_);
    std::swap(degree_, other.degree_);
}

Polynomial& Polynomial::operator=(Polynomial other) {
    swap(other);
    return *this;
}

int Polynomial::degree() const {
    return degree_;
}

double Polynomial::coeff(int i) const {
    if (i < 0 || i > degree_) {
        return 0.0;
    }
    return coeffs_[i];
}

double Polynomial::evaluate(double x) const{
    double result = 0.0;
    for (int i = degree_; i >= 0; --i) {
        result = result * x + coeffs_[i];
    }
    return result;
}

void Polynomial::add(const Polynomial& other){
    int newDegree = std::max(degree_, other.degree_);
    double* newCoeff = new double[newDegree + 1];
    for (int i = 0; i < newDegree+1; i++){
        newCoeff[i] = coeff(i) + other.coeff(i);
    }
    delete[] coeffs_;
    coeffs_ = newCoeff;
    degree_ = newDegree;
    trim();
}

void Polynomial::mul(const Polynomial& other){
    if (degree_ < 0 || other.degree_ < 0){
        delete[] coeffs_;          
        coeffs_ = nullptr;
        degree_ = -1;
        return;
    }
    int newDegree = degree_ + other.degree_;            
    double* newCoeff = new double[newDegree + 1]();     
    for (int i = 0; i <= degree_; i++){
        for (int j = 0; j <= other.degree_; j++){       
            newCoeff[i + j] += coeffs_[i] * other.coeffs_[j];
        }
    }
    delete[] coeffs_;
    coeffs_ = newCoeff;
    degree_ = newDegree;
    trim();
}

void Polynomial::print(std::ostream& os) const {
    if (degree_ < 0){
        os << "0";
        return;
    }
    bool first = true;
    for (int i = 0; i <= degree_; i++){
        double c = coeffs_[i];
        if (isNearZero(c)){
            continue;
        }
        if (first){
            if (c < 0) os << "-";
        } else {
            os << (c < 0 ? " - " : " + ");
        }
        double a = std::fabs(c);
        if (i == 0 || !isNear(a, 1.0)){
            os << a;
        }
        if (i >= 1){
            os << "x";
            if (i >= 2){
                os << "^" << i;
            }
        }
        first = false;
    }
}

Polynomial Polynomial::derivative() const {
    if (degree_ <= 0){
        return Polynomial();      
    }
    double* tmp = new double[degree_];
    for (int i = 1; i <= degree_; i++){
        tmp[i - 1] = coeffs_[i] * i;
    }
    Polynomial result(tmp, degree_ - 1);
    delete[] tmp;
    return result;
}

Polynomial Polynomial::integral() const {
    if (degree_ < 0){
        return Polynomial();
    }
    double* tmp = new double[degree_ + 2];
    tmp[0] = 0.0;               
    for (int i = 0; i <= degree_; i++){
        tmp[i + 1] = coeffs_[i] / (i + 1);
    }
    Polynomial result(tmp, degree_ + 1);
    delete[] tmp;
    return result;
}