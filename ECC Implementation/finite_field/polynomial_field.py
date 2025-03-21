from .finite_field_element import FiniteFieldElement

class PolynomialField:
    """
    Polynomial basis representation for finite fields.
    
    This class implements a finite field GF(p^m) as polynomials with coefficients in GF(p)
    and operations modulo an irreducible polynomial.
    """
    
    def __init__(self, prime, irreducible_poly, extension_degree):
        """
        Initialize a polynomial field.
        
        Args:
            prime: The characteristic of the field (p in GF(p^m))
            irreducible_poly: Coefficients of the irreducible polynomial (in decreasing order of degree)
            extension_degree: The extension degree (m in GF(p^m))
        """
        self.prime = prime
        self.extension_degree = extension_degree
        self.irreducible_poly = irreducible_poly
        
        if len(irreducible_poly) != extension_degree + 1:
            raise ValueError(f"Irreducible polynomial must have degree {extension_degree}")
        
        if irreducible_poly[0] != 1:
            raise ValueError("Irreducible polynomial must be monic (leading coefficient must be 1)")
    
    def __call__(self, coefficients):
        """Create a field element from the given coefficients."""
        if isinstance(coefficients, int):
            coeffs = [0] * self.extension_degree
            temp = coefficients
            for i in range(self.extension_degree - 1, -1, -1):
                coeffs[i] = temp % self.prime
                temp //= self.prime
            return PolynomialFieldElement(coeffs, self)
        else:
            coeffs = [c % self.prime for c in coefficients]
            if len(coeffs) > self.extension_degree:
                raise ValueError(f"Polynomial degree must be less than {self.extension_degree}")
            
            if len(coeffs) < self.extension_degree:
                coeffs = coeffs + [0] * (self.extension_degree - len(coeffs))
                
            return PolynomialFieldElement(coeffs, self)
    
    def zero(self):
        """Return the zero element of the field."""
        return self([0] * self.extension_degree)
    
    def one(self):
        """Return the multiplicative identity of the field."""
        coeffs = [0] * self.extension_degree
        coeffs[0] = 1
        return self(coeffs)
    
    def order(self):
        """Return the order of the field (p^m)."""
        return self.prime ** self.extension_degree
    
    def _add_poly(self, a, b):
        """Add two polynomials coefficient-wise."""
        return [(a_i + b_i) % self.prime for a_i, b_i in zip(a, b)]
    
    def _sub_poly(self, a, b):
        """Subtract two polynomials coefficient-wise."""
        return [(a_i - b_i) % self.prime for a_i, b_i in zip(a, b)]
    
    def _mul_poly(self, a, b):
        """Multiply two polynomials."""
        degree_a = len(a)
        degree_b = len(b)
        result = [0] * (degree_a + degree_b - 1)
        
        for i in range(degree_a):
            for j in range(degree_b):
                result[i + j] = (result[i + j] + a[i] * b[j]) % self.prime
                
        return result
    
    def _div_poly(self, a, b):
        """Divide polynomial a by b, returning quotient and remainder."""
        a = list(a)
        degree_a = len(a)
        degree_b = len(b)
        
        if degree_b == 0 or b[0] == 0:
            raise ZeroDivisionError("Division by zero polynomial")
            
        if degree_a < degree_b:
            return [0], a  
            
        quotient = [0] * (degree_a - degree_b + 1)
        
        for i in range(degree_a - degree_b, -1, -1):
            quotient[i] = a[i] * pow(b[0], -1, self.prime) % self.prime
            for j in range(degree_b):
                a[i + j] = (a[i + j] - quotient[i] * b[j]) % self.prime
                
        remainder = a[-(degree_b-1):] if degree_b > 1 else []
        
        return quotient, remainder
    
    def _mod_poly(self, a, m):
        """Compute a modulo m."""
        _, remainder = self._div_poly(a, m)
        return remainder
    
    def _extended_gcd(self, a, b):
        """Extended Euclidean algorithm for polynomials."""
        if len(b) == 0 or all(c == 0 for c in b):
            return a, [1], [0]
            
        quotient, remainder = self._div_poly(a, b)
        
        gcd, x, y = self._extended_gcd(b, remainder)
        
        new_x = y
        new_y = self._sub_poly(x, self._mul_poly(quotient, y))
        
        return gcd, new_x, new_y
        
    def _inverse_poly(self, a, m):
        """Compute the modular multiplicative inverse of a polynomial."""
        gcd, x, _ = self._extended_gcd(a, m)
        
        if len(gcd) != 1 or gcd[0] != 1:
            raise ValueError("Polynomial is not invertible")
            
        return self._mod_poly(x, m)

class PolynomialFieldElement(FiniteFieldElement):
    """Element of a polynomial field."""
    
    def __init__(self, coefficients, field):
        """
        Initialize a polynomial field element.
        
        Args:
            coefficients: List of coefficients (in order of increasing degree)
            field: The PolynomialField this element belongs to
        """
        super().__init__(field)
        self.coefficients = list(coefficients)  
        
    def __add__(self, other):
        """Add two field elements."""
        if isinstance(other, PolynomialFieldElement) and self.field == other.field:
            result = self.field._add_poly(self.coefficients, other.coefficients)
            return PolynomialFieldElement(result, self.field)
        elif isinstance(other, int):
            result = list(self.coefficients)
            result[0] = (result[0] + other) % self.field.prime
            return PolynomialFieldElement(result, self.field)
        else:
            return NotImplemented
            
    def __sub__(self, other):
        """Subtract two field elements."""
        if isinstance(other, PolynomialFieldElement) and self.field == other.field:
            result = self.field._sub_poly(self.coefficients, other.coefficients)
            return PolynomialFieldElement(result, self.field)
        elif isinstance(other, int):
            result = list(self.coefficients)
            result[0] = (result[0] - other) % self.field.prime
            return PolynomialFieldElement(result, self.field)
        else:
            return NotImplemented
            
    def __mul__(self, other):
        """Multiply two field elements."""
        if isinstance(other, PolynomialFieldElement) and self.field == other.field:
            product = self.field._mul_poly(self.coefficients, other.coefficients)
            result = self.field._mod_poly(product, self.field.irreducible_poly)
            return PolynomialFieldElement(result, self.field)
        elif isinstance(other, int):
            result = [(c * other) % self.field.prime for c in self.coefficients]
            return PolynomialFieldElement(result, self.field)
        else:
            return NotImplemented
            
    def __truediv__(self, other):
        """Divide two field elements."""
        if isinstance(other, PolynomialFieldElement) and self.field == other.field:
            inverse = other.inverse()
            return self * inverse
        elif isinstance(other, int):
            inv = pow(other, -1, self.field.prime)
            return self * inv
        else:
            return NotImplemented
            
    def __pow__(self, exponent):
        """Exponentiate a field element."""
        if exponent == 0:
            return self.field.one()
        elif exponent < 0:
            return self.inverse() ** (-exponent)
        else:
            result = self.field.one()
            base = self
            while exponent > 0:
                if exponent & 1:
                    result = result * base
                base = base * base
                exponent >>= 1
            return result
            
    def inverse(self):
        """Compute the multiplicative inverse."""
        inverse_coeffs = self.field._inverse_poly(self.coefficients, self.field.irreducible_poly)
        return PolynomialFieldElement(inverse_coeffs, self.field)
        
    def __eq__(self, other):
        """Check equality of field elements."""
        if isinstance(other, PolynomialFieldElement) and self.field == other.field:
            return self.coefficients == other.coefficients
        elif isinstance(other, int):
            temp_coeffs = [other] + [0] * (len(self.coefficients) - 1)
            return self.coefficients == temp_coeffs
        else:
            return False
            
    def __repr__(self):
        """String representation of the element."""
        terms = []
        for i, coef in enumerate(self.coefficients):
            if coef == 0:
                continue
            if i == 0:
                terms.append(str(coef))
            elif i == 1:
                if coef == 1:
                    terms.append("x")
                else:
                    terms.append(f"{coef}x")
            else:
                if coef == 1:
                    terms.append(f"x^{i}")
                else:
                    terms.append(f"{coef}x^{i}")
        
        if not terms:
            return "0"
        return " + ".join(terms) 