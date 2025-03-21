from .finite_field_element import FiniteFieldElement
from .number_theory import polynomial_add, polynomial_multiply, polynomial_mod, polynomial_inverse, polynomial_power

class BinaryField:
    def __init__(self, m, irreducible_polynomial_coeffs):
        self.m = m
        self.irreducible_polynomial_coeffs = tuple(irreducible_polynomial_coeffs) 
        self.mod_poly = irreducible_polynomial_coeffs

        if len(irreducible_polynomial_coeffs) != m + 1:
            raise ValueError(f"Irreducible polynomial must be of degree m={m}")
        if irreducible_polynomial_coeffs[-1] != 1: 
            raise ValueError("Irreducible polynomial must be monic (leading coefficient 1)")
        if irreducible_polynomial_coeffs[0] != 1: 
            pass 

        if m > 1 and not self._is_irreducible_basic_check():
            raise ValueError("Basic irreducibility check failed. Polynomial might not be irreducible.")


    def _is_irreducible_basic_check(self):
        if self.mod_poly[0] == 0: return False 
        sum_coeffs = sum(self.mod_poly) % 2
        if sum_coeffs == 0 and self.m > 1: return False # Divisible by (x+1) for m > 1

        return True

    def __repr__(self):
        return f"BinaryField(GF(2^{self.m}), poly={self.irreducible_polynomial_coeffs})"

    def is_valid_element(self, value):
        return isinstance(value, int) and 0 <= value < (1 << self.m)

    def normalize_value(self, value):
        return value & ((1 << self.m) - 1) 

    def add(self, elem1, elem2):
        return FiniteFieldElement(elem1.value ^ elem2.value, self) 

    def subtract(self, elem1, elem2):
        return self.add(elem1, elem2) 

    def multiply(self, elem1, elem2):
        product_poly = polynomial_multiply(self._int_to_poly(elem1.value), self._int_to_poly(elem2.value))
        reduced_poly = polynomial_mod(product_poly, self.mod_poly)
        return FiniteFieldElement(self._poly_to_int(reduced_poly), self)

    def divide(self, elem1, elem2):
        inverse_elem2 = self.inverse(elem2)
        return self.multiply(elem1, inverse_elem2)

    def power(self, elem, exponent):
        poly_base = self._int_to_poly(elem.value)
        poly_result = polynomial_power(poly_base, exponent, self.mod_poly)
        return FiniteFieldElement(self._poly_to_int(poly_result), self)

    def inverse(self, elem):
        if elem.value == 0:
            raise ZeroDivisionError("Cannot find inverse of zero")
        poly_elem = self._int_to_poly(elem.value)
        inverse_poly = polynomial_inverse(poly_elem, self.mod_poly, x_degree=self.m) 
        return FiniteFieldElement(self._poly_to_int(inverse_poly), self)

    def sqrt(self, elem): 
        return self.power(elem, (1 << (self.m - 1))) if self.m > 0 else elem 

    def _int_to_poly(self, val_int):
        poly_coeffs = []
        for i in range(self.m + 1): 
            if (val_int >> i) & 1:
                poly_coeffs.append(1)
            else:
                poly_coeffs.append(0)
        return poly_coeffs

    def _poly_to_int(self, poly_coeffs):
        val_int = 0
        for i, coeff in enumerate(poly_coeffs):
            if coeff:
                val_int |= (coeff << i) 
        return val_int