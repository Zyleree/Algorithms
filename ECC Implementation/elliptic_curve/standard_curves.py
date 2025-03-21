from .weierstrass_curve import WeierstrassCurve
from .montgomery_curve import MontgomeryCurve
from .edwards_curve import EdwardsCurve
from ..finite_field import PrimeField

# NIST P-256 curve (also known as secp256r1)
def create_nist_p256():
    """
    Create the NIST P-256 curve (secp256r1)
    
    y^2 = x^3 - 3x + b over a prime field
    """
    p = 0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF
    
    a = -3
    b = 0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B
    
    gx = 0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296
    gy = 0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5
    
    n = 0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551
    
    field = PrimeField(p)
    curve = WeierstrassCurve(a, b, field)
    
    return {
        'name': 'NIST_P256',
        'curve': curve,
        'field': field,
        'generator_x': field(gx),
        'generator_y': field(gy),
        'order': n,
        'cofactor': 1
    }

def create_curve25519():
    """
    Create the Curve25519 (Montgomery form)
    
    v^2 = u^3 + Au^2 + u (mod p)
    
    Curve25519 is designed for use with the Montgomery ladder,
    which is ideal for constant-time implementation.
    """
    p = 2**255 - 19
    
    # (Montgomery form)
    A = 486662
    B = 1
    
    u = 9
    
    n = 2**252 + 27742317777372353535851937790883648493
    
    field = PrimeField(p)
    curve = MontgomeryCurve(A, B, field)
    
    return {
        'name': 'Curve25519',
        'curve': curve,
        'field': field,
        'base_u': field(u),
        'order': n,
        'cofactor': 8
    }

def create_secp256k1():
    """
    Create the secp256k1 curve used in Bitcoin
    
    y^2 = x^3 + 7 over a prime field
    """
    p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
    
    a = 0
    b = 7
    
    gx = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
    gy = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
    
    n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
    
    field = PrimeField(p)
    curve = WeierstrassCurve(a, b, field)
    
    return {
        'name': 'secp256k1',
        'curve': curve,
        'field': field,
        'generator_x': field(gx),
        'generator_y': field(gy),
        'order': n,
        'cofactor': 1
    }

NIST_P256 = create_nist_p256()
Curve25519 = create_curve25519()
secp256k1 = create_secp256k1()

STANDARD_CURVES = {
    'NIST_P256': NIST_P256,
    'Curve25519': Curve25519,
    'secp256k1': secp256k1
} 