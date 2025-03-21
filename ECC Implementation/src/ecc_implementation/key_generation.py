import random
from .random_utils import generate_random_scalar
from .prime_field import PrimeField

def generate_key_pair(curve):
    private_key = generate_random_scalar(curve.field.prime if isinstance(curve.field, PrimeField) else (1 << curve.field.m)) 
    public_key = private_key * curve.generator_point 
    return private_key, public_key