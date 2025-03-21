# examples/ecdh_example.py
import sys
import os

# Add the parent directory to sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

# Import modules directly from their locations
from finite_field.prime_field import PrimeField
from finite_field.finite_field_element import FiniteFieldElement
from elliptic_curve.weierstrass_curve import WeierstrassCurve
from elliptic_curve.point import Point
from crypto.key_generation import generate_key_pair
from crypto.ecdh import ecdh_key_exchange

if __name__ == "__main__":
    prime = 23
    field = PrimeField(prime)
    curve = WeierstrassCurve(1, 1, field)
    curve.generator_point = Point(FiniteFieldElement(3, field), FiniteFieldElement(10, field), curve) # Example generator point
    curve.order = 29 # Example order

    private_key_a, public_key_a = generate_key_pair(curve)
    private_key_b, public_key_b = generate_key_pair(curve)

    print("Public Key A:", public_key_a)
    print("Public Key B:", public_key_b)

    shared_secret_a = ecdh_key_exchange(private_key_a, public_key_b)
    shared_secret_b = ecdh_key_exchange(private_key_b, public_key_a)

    print("Shared Secret A:", shared_secret_a)
    print("Shared Secret B:", shared_secret_b)

    if shared_secret_a == shared_secret_b:
        print("ECDH Key Exchange Successful! Shared secrets match.")
    else:
        print("ECDH Key Exchange Failed! Shared secrets do not match.")