import unittest
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from finite_field.prime_field import PrimeField
from finite_field.binary_field import BinaryField
from finite_field.finite_field_element import FiniteFieldElement

class TestFiniteField(unittest.TestCase):

    def test_prime_field_arithmetic(self):
        prime = 23
        field = PrimeField(prime)
        a = FiniteFieldElement(5, field)
        b = FiniteFieldElement(17, field)

        self.assertEqual(a + b, FiniteFieldElement(22, field))
        self.assertEqual(a - b, FiniteFieldElement(11, field))
        self.assertEqual(a * b, FiniteFieldElement(16, field))
        self.assertEqual(a / b, FiniteFieldElement(4, field)) 
        self.assertEqual(a**3, FiniteFieldElement(125 % prime, field))
        self.assertEqual(a.inverse() * a, FiniteFieldElement(1, field))

    def test_binary_field_arithmetic(self):
        m = 4
        irreducible_poly = [1, 1, 0, 0, 1] 
        field = BinaryField(m, irreducible_poly)
        elem1_val = 0b1011 
        elem2_val = 0b0110 

        elem1 = FiniteFieldElement(elem1_val, field)
        elem2 = FiniteFieldElement(elem2_val, field)

        self.assertEqual(elem1 + elem2, FiniteFieldElement(elem1_val ^ elem2_val, field)) # Addition (XOR)
        self.assertEqual(elem1 - elem2, FiniteFieldElement(elem1_val ^ elem2_val, field)) # Subtraction (XOR)

        product = elem1 * elem2 
        expected_product_val = 0b1001
        self.assertEqual(product, FiniteFieldElement(expected_product_val, field))

        inverse_elem2 = elem2.inverse()
        identity = elem2 * inverse_elem2
        self.assertEqual(identity, FiniteFieldElement(1, field)) 

    def test_prime_field_sqrt(self):
        prime = 23
        field = PrimeField(prime)
        elem_qr = FiniteFieldElement(9, field) 
        sqrt_elem = elem_qr.sqrt()
        self.assertIsNotNone(sqrt_elem)
        self.assertEqual(sqrt_elem**2, elem_qr)

        elem_non_qr = FiniteFieldElement(3, field)
        sqrt_non_qr = elem_non_qr.sqrt()
        self.assertIsNone(sqrt_non_qr) 

    def test_binary_field_sqrt(self):
        m = 4
        irreducible_poly = [1, 1, 0, 0, 1] # x^4 + x + 1 (GF(2^4))
        field = BinaryField(m, irreducible_poly)
        elem = FiniteFieldElement(0b1011, field) 
        sqrt_elem = elem.sqrt()
        self.assertEqual(sqrt_elem**2, elem)

if __name__ == '__main__':
    unittest.main()