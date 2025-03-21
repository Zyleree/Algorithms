from .elliptic_curve import EllipticCurve
from .point import Point

import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from finite_field import PrimeField, BinaryField, FiniteFieldElement

class WeierstrassCurve(EllipticCurve):
    def __init__(self, a, b, field):
        super().__init__(field)
        self.a = FiniteFieldElement(a, field)
        self.b = FiniteFieldElement(b, field)
        self.field = field 

        if isinstance(field, PrimeField):
            discriminant_val = (4 * (self.a**3).value + 27 * (self.b**2).value) % field.prime
            if discriminant_val == 0:
                raise ValueError("Singular curve (discriminant is zero)")
        elif isinstance(field, BinaryField):
            if self.a.value == 0:
                raise ValueError("For Binary Field Weierstrass curve, 'a' cannot be 0")
        else:
            raise TypeError("Field must be PrimeField or BinaryField for WeierstrassCurve")


    def is_on_curve(self, point):
        if point == self.identity_point:
            return True
        y_squared = point.y**2
        x_cubed_ax_b = point.x**3 + self.a * point.x + self.b
        return y_squared == x_cubed_ax_b

    def point_addition(self, point1, point2):
        if point1 == self.identity_point:
            return point2
        if point2 == self.identity_point:
            return point1
        if point1.x == point2.x:
            if point1.y != point2.y:
                return self.identity_point
            else:
                return self.point_doubling(point1)

        slope = (point2.y - point1.y) / (point2.x - point1.x)
        x3 = slope**2 - point1.x - point2.x
        y3 = slope * (point1.x - x3) - point1.y
        return Point(x3, y3, self)

    def point_doubling(self, point):
        if point == self.identity_point:
            return self.identity_point

        if isinstance(self.field, PrimeField):
            three = FiniteFieldElement(3, self.field)
            two = FiniteFieldElement(2, self.field)
            slope = (three * point.x**2 + self.a) / (two * point.y)
        elif isinstance(self.field, BinaryField):
            if point.y.value == 0: 
                return self.identity_point # Vertical tangent at points where y=0
            slope = (point.x**2 + self.a) / point.y # Different slope formula in char 2
        else:
            raise TypeError("Unsupported field type for point doubling")

        two = FiniteFieldElement(2, self.field)
        x3 = slope**2 - two * point.x
        y3 = slope * (point.x - x3) - point.y
        return Point(x3, y3, self)

    def point_negation(self, point):
        if point == self.identity_point:
            return self.identity_point
        return Point(point.x, FiniteFieldElement(-point.y.value % self.field.prime, self.field) if isinstance(self.field, PrimeField) else FiniteFieldElement(point.y.value, self.field), self) # Negation in PrimeField, same y in BinaryField (char 2)