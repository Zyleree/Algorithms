import random
from .finite_field_element import FiniteFieldElement

class PrimeField:
    def __init__(self, prime):
        if not self.is_prime(prime):
            raise ValueError("Modulus must be a prime number")
        self.prime = prime

    def __repr__(self):
        return f"PrimeField(p={self.prime})"

    def is_prime(self, n, k=5):
        if n <= 1: return False
        if n <= 3: return True
        if n % 2 == 0: return False

        r, s = 0, n - 1
        while s % 2 == 0:
            r += 1
            s //= 2

        for _ in range(k):
            a = random.randint(2, n - 2)
            x = pow(a, s, n)
            if x == 1 or x == n - 1:
                continue
            for _ in range(r - 1):
                x = pow(x, 2, n)
                if x == n - 1:
                    break
            else:
                return False
        return True

    def is_valid_element(self, value):
        return isinstance(value, int) and 0 <= value < self.prime

    def normalize_value(self, value):
        return value % self.prime

    def add(self, elem1, elem2):
        return FiniteFieldElement((elem1.value + elem2.value) % self.prime, self)

    def subtract(self, elem1, elem2):
        return FiniteFieldElement((elem1.value - elem2.value) % self.prime, self)

    def multiply(self, elem1, elem2):
        return FiniteFieldElement((elem1.value * elem2.value) % self.prime, self)

    def divide(self, elem1, elem2):
        inverse = pow(elem2.value, self.prime - 2, self.prime)
        return FiniteFieldElement((elem1.value * inverse) % self.prime, self)

    def power(self, elem, exponent):
        return FiniteFieldElement(pow(elem.value, exponent, self.prime), self)

    def inverse(self, elem):
        if elem.value == 0:
            raise ZeroDivisionError("Cannot find inverse of zero")
        inverse = pow(elem.value, elem.prime.prime - 2, elem.prime.prime) 
        return FiniteFieldElement(inverse, self)

    def sqrt(self, elem):
        if self.legendre_symbol(elem) != 1:
            return None

        if self.prime % 4 == 3:
            r = pow(elem.value, (self.prime + 1) // 4, self.prime)
            return FiniteFieldElement(r, self)
        else:
            q = self.prime - 1
            s = 0
            while q % 2 == 0:
                q //= 2
                s += 1
            if s == 1: 
                r = pow(elem.value, (self.prime + 1) // 4, self.prime)
                if (r * r) % self.prime == elem.value % self.prime:
                    return FiniteFieldElement(r, self)
                else:
                    r = (r * pow(2, (self.prime - 1) // 4, self.prime)) % self.prime 
                    return FiniteFieldElement(r, self)

            raise NotImplementedError("General Tonelli-Shanks algorithm for sqrt is not fully implemented")


    def legendre_symbol(self, elem):
        return pow(elem.value, (self.prime - 1) // 2, self.prime)