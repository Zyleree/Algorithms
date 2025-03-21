# ECC Implementation

*A component of the [Algorithms Collection](../README.md)*

ECC Implementation is a Python library that provides comprehensive functionality for Elliptic Curve Cryptography including finite field arithmetic, curve operations, and cryptographic protocols.

## What's included

This library implements various components essential for ECC:

- Finite Field operations:
  * Prime field arithmetic
  * Binary field operations
  * Polynomial basis representation

- Elliptic Curve functionality:
  * Curve point arithmetic (addition, doubling, scalar multiplication)
  * Various curve forms (Weierstrass, Montgomery, Edwards)
  * Standard curves (NIST, Curve25519, secp256k1)

- Cryptographic applications:
  * ECDSA (Elliptic Curve Digital Signature Algorithm)
  * ECDH (Elliptic Curve Diffie-Hellman)
  * Key generation and management

- Utilities:
  * Big integer handling
  * Random number generation
  * Testing and validation tools

## Getting started

You'll need:
- Python 3.6+
- pip (Python package manager)

Installation is straightforward:
```bash
pip install -e .
```

Or install with requirements:
```bash
pip install -r requirements.txt
pip install -e .
```

## How to use it

### Basic elliptic curve operations
```python
from elliptic_curve import WeierstrassCurve
from finite_field import PrimeField

# Create a prime field
p = 23
field = PrimeField(p)

# Define curve parameters (y^2 = x^3 + ax + b)
a = field(1)
b = field(0)

# Create a curve
curve = WeierstrassCurve(a, b, field)

# Create points and perform operations
P = curve.point(3, 10)
Q = curve.point(9, 7)
R = P + Q  # Point addition
```

### Cryptographic usage
```python
from crypto import ECDSA
from elliptic_curve import NIST_P256

# Create ECDSA instance with NIST P-256 curve
ecdsa = ECDSA(NIST_P256)

# Generate key pair
private_key, public_key = ecdsa.generate_keypair()

# Sign a message
message = b"Hello, Elliptic Curve Cryptography!"
signature = ecdsa.sign(message, private_key)

# Verify the signature
is_valid = ecdsa.verify(message, signature, public_key)
```

## For developers

If you want to contribute to the codebase, check out these main components:

- `finite_field/`: Finite field arithmetic implementations
- `elliptic_curve/`: Curve definitions and point operations
- `crypto/`: Cryptographic protocols using ECC
- `utils/`: Helper functions and utilities
- `examples/`: Sample usage and demonstrations
- `tests/`: Unit and integration tests

To run the tests:
```bash
python -m unittest discover tests
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgements

- Cryptographic libraries that provided inspiration
- Academic papers on elliptic curve optimization
- Open standards for cryptographic implementations 