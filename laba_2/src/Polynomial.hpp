#pragma once
#include <iosfwd>
 
class Polynomial
{
    double* coeffs_;
    int degree_ = -1;
public:
    Polynomial();
    Polynomial(const double* coeff_s, int degree);
    ~Polynomial();
    Polynomial(const Polynomial& origin);
    Polynomial(Polynomial&& origin) noexcept;
    Polynomial& operator=(Polynomial other);
    void swap(Polynomial& other) noexcept;
 
    int degree() const;
    double coeff(int i) const;
    double evaluate(double x) const;
    void add(const Polynomial& other);
    void mul(const Polynomial& other);
    void print(std::ostream& os) const;
    Polynomial derivative() const;
    Polynomial integral() const;
private:
    void trim();
};
 