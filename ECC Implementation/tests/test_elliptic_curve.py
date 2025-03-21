import unittest
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from finite_field.prime_field import PrimeField
from finite_field.binary_field import BinaryField
from finite_field.finite_field_element import FiniteFieldElement
from elliptic_curve.weierstrass_curve import WeierstrassCurve
from elliptic_curve.point import Point

class TestEllipticCurve(unittest.TestCase):

    def test_weierstrass_prime_field_curve(self):
        prime = 23
        field = PrimeField(prime)
        curve = WeierstrassCurve(1, 1, field) # y^2 = x^3 + x + 1 mod 23

        x1 = FiniteFieldElement(3, field)
        y1 = FiniteFieldElement(10, field)
        point1 = Point(x1, y1, curve)
        self.assertTrue(curve.is_on_curve(point1))

        x2 = FiniteFieldElement(9, field)
        y2 = FiniteFieldElement(7, field)
        point2 = Point(x2, y2, curve)
        self.assertTrue(curve.is_on_curve(point2))

        point_sum = point1 + point2
        point_double = point1 + point1

        scalar_mult_point = 5 * point1
        self.assertTrue(curve.is_on_curve(scalar_mult_point))

    def test_weierstrass_binary_field_curve(self):
        m = 4
        irreducible_poly = [1, 1, 0, 0, 1] # x^4 + x + 1 (GF(2^4))
        field = BinaryField(m, irreducible_poly)
        curve = WeierstrassCurve(FiniteFieldElement(1, field), FiniteFieldElement(1, field), field) 

        x1_val = 0b1010 
        y1_val = 0b0101 
        x1 = FiniteFieldElement(x1_val, field)
        y1 = FiniteFieldElement(y1_val, field)
        point1 = Point(x1, y1, curve)
        self.assertTrue(curve.is_on_curve(point1)) 

        x2_val = 0b0111 
        y2_val = 0b1100 
        x2 = FiniteFieldElement(x2_val, field)
        y2 = FiniteFieldElement(y2_val, field)
        point2 = Point(x2, y2, curve)
        self.assertTrue(curve.is_on_curve(point2)) 

        point_sum = point1 + point2
        point_double = point1 + point1

        scalar_mult_point = 3 * point1
        self.assertTrue(curve.is_on_curve(scalar_mult_point))


if __name__ == '__main__':
    unittest.main()