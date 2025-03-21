import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from finite_field.prime_field import PrimeField
from finite_field.finite_field_element import FiniteFieldElement
from elliptic_curve.weierstrass_curve import WeierstrassCurve
from elliptic_curve.point import Point
from crypto.key_generation import generate_key_pair
from crypto.ecdsa import ecdsa_sign, ecdsa_verify

if __name__ == "__main__":
    prime = 23
    field = PrimeField(prime)
    curve = WeierstrassCurve(1, 1, field)
    curve.generator_point = Point(FiniteFieldElement(3, field), FiniteFieldElement(10, field), curve)
    curve.order = 29 

    private_key, public_key = generate_key_pair(curve)
    message = "Sign this important document!"

    signature = ecdsa_sign(message, private_key, curve)
    print("Signature (r, s):", signature)

    is_valid = ecdsa_verify(message, signature, public_key, curve)
    if is_valid:
        print("ECDSA Signature Verification Successful! Signature is valid.")
    else:
        print("ECDSA Signature Verification Failed! Signature is invalid.")

    tampered_message = "Sign this important document tampered!"
    is_valid_tampered = ecdsa_verify(tampered_message, signature, public_key, curve)
    if not is_valid_tampered:
        print("ECDSA correctly detects tampering: Tampered message signature is invalid (as expected).")
    else:
        print("ECDSA verification failed to detect tampering (Error!)")