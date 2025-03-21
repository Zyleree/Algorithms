import unittest
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from finite_field.prime_field import PrimeField
from finite_field.finite_field_element import FiniteFieldElement
from elliptic_curve.weierstrass_curve import WeierstrassCurve
from elliptic_curve.point import Point
from crypto.key_generation import generate_key_pair
from crypto.ecdh import ecdh_key_exchange
from crypto.ecdsa import ecdsa_sign, ecdsa_verify

class TestCrypto(unittest.TestCase):

    def test_key_generation(self):
        prime = 23
        field = PrimeField(prime)
        curve = WeierstrassCurve(1, 1, field)
        curve.generator_point = Point(FiniteFieldElement(3, field), FiniteFieldElement(10, field), curve)
        curve.order = 29 
        private_key, public_key = generate_key_pair(curve)
        self.assertIsNotNone(private_key)
        self.assertIsNotNone(public_key)
        self.assertTrue(curve.is_on_curve(public_key))

    def test_ecdh_key_exchange(self):
        prime = 23
        field = PrimeField(prime)
        curve = WeierstrassCurve(1, 1, field)
        curve.generator_point = Point(FiniteFieldElement(3, field), FiniteFieldElement(10, field), curve) 
        curve.order = 29 

        private_key_a, public_key_a = generate_key_pair(curve)
        private_key_b, public_key_b = generate_key_pair(curve)

        shared_secret_a = ecdh_key_exchange(private_key_a, public_key_b)
        shared_secret_b = ecdh_key_exchange(private_key_b, public_key_a)

        self.assertEqual(shared_secret_a, shared_secret_b) 

    def test_ecdsa_sign_verify(self):
        prime = 23
        field = PrimeField(prime)
        curve = WeierstrassCurve(1, 1, field)
        curve.generator_point = Point(FiniteFieldElement(3, field), FiniteFieldElement(10, field), curve) 
        curve.order = 29 

        private_key, public_key = generate_key_pair(curve)
        message = "Test message for ECDSA"
        signature = ecdsa_sign(message, private_key, curve)
        is_valid = ecdsa_verify(message, signature, public_key, curve)
        self.assertTrue(is_valid)

        invalid_signature = (signature[0], signature[1] + 1)
        is_invalid = ecdsa_verify(message, invalid_signature, public_key, curve)
        self.assertFalse(is_invalid)

if __name__ == '__main__':
    unittest.main()