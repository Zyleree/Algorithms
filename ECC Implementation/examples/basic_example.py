import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from finite_field.prime_field import PrimeField
from finite_field.finite_field_element import FiniteFieldElement
from elliptic_curve.weierstrass_curve import WeierstrassCurve
from elliptic_curve.point import Point

if __name__ == "__main__":
    print("Starting example...")
    
    prime_modulus = 23
    field = PrimeField(prime_modulus)
    curve = WeierstrassCurve(1, 1, field)  
    curve.generator_point = Point(FiniteFieldElement(3, field), FiniteFieldElement(10, field), curve) 
    curve.order = 29 

    x1 = FiniteFieldElement(3, field)
    y1 = FiniteFieldElement(10, field)
    point1 = Point(x1, y1, curve)

    x2 = FiniteFieldElement(9, field)
    y2 = FiniteFieldElement(7, field)
    point2 = Point(x2, y2, curve)

    print("Point 1:", point1)
    print("Point 2:", point2)
    print("Point 1 on curve:", curve.is_on_curve(point1))
    print("Point 2 on curve:", curve.is_on_curve(point2))

    point_sum = point1 + point2
    print("Point 1 + Point 2:", point_sum)

    scalar = 5
    scalar_multiplied_point = scalar * point1
    print(f"{scalar} * Point 1:", scalar_multiplied_point)