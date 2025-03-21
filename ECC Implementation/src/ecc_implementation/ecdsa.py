import hashlib
from .random_utils import generate_random_scalar

def ecdsa_sign(message, private_key, curve):
    n = curve.order 
    if n is None:
        raise ValueError("Curve order 'n' is not defined for ECDSA signing")

    while True:
        k = generate_random_scalar(n) 
        R = k * curve.generator_point
        r = R.x.value % n
        if r == 0: continue 

        message_hash = hashlib.sha256(message.encode()).hexdigest() 
        e = int(message_hash, 16) % n

        try:
            s = (pow(k, -1, n) * (e + private_key * r)) % n 
        except ValueError: 
            continue 
        if s == 0: continue 

        return (r, s) 

def ecdsa_verify(message, signature, public_key, curve):
    r, s = signature
    n = curve.order
    if not (1 <= r < n and 1 <= s < n):
        return False 

    message_hash = hashlib.sha256(message.encode()).hexdigest()
    e = int(message_hash, 16) % n

    try:
        w = pow(s, -1, n) 
    except ValueError:
        return False 

    u1 = (e * w) % n
    u2 = (r * w) % n

    P = u1 * curve.generator_point + u2 * public_key 

    if P == curve.identity_point:
        return False 

    v = P.x.value % n

    return v == r 