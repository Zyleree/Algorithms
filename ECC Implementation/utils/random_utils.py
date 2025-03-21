import secrets
import os
import hashlib

def generate_random_bytes(length):
    """
    Generate cryptographically secure random bytes
    
    Args:
        length: Number of bytes to generate
        
    Returns:
        Random bytes of specified length
    """
    return secrets.token_bytes(length)

def generate_random_scalar(order):
    """
    Generate a random scalar suitable for ECC operations
    
    Args:
        order: Upper bound for the scalar (typically the curve order)
        
    Returns:
        A random integer in the range [1, order-1]
    """
    return secrets.randbelow(order - 1) + 1

def generate_deterministic_scalar(seed, order):
    """
    Generate a deterministic scalar from a seed (for testing or key derivation)
    
    Args:
        seed: The seed value (bytes or string)
        order: Upper bound for the scalar
        
    Returns:
        A deterministic scalar in the range [1, order-1]
    """
    if isinstance(seed, str):
        seed = seed.encode('utf-8')
        
    digest = hashlib.sha512(seed).digest()
    value = int.from_bytes(digest, byteorder='big')
    
    return (value % (order - 1)) + 1

def generate_seed():
    """
    Generate a high-entropy seed that can be used for key derivation
    
    Returns:
        A 32-byte high-entropy seed
    """
    return os.urandom(32)

def generate_blinding_factor(order):
    """
    Generate a random blinding factor used for side-channel countermeasures
    
    Args:
        order: Upper bound for the factor (typically the curve order)
        
    Returns:
        A random integer in the range [1, order-1]
    """
    return generate_random_scalar(order)