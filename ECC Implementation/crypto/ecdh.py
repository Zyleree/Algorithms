"""
Implementation of Elliptic Curve Diffie-Hellman key exchange
"""
from .key_generation import generate_key_pair
import os
import hashlib

def ecdh_generate_keypair(curve_info):
    """
    Generate a key pair for ECDH key exchange
    
    Args:
        curve_info: Information about the curve to use
        
    Returns:
        (private_key, public_key) tuple
    """
    return generate_key_pair(curve_info)

def ecdh_compute_shared_secret(private_key, other_public_key, curve_info):
    """
    Compute the shared secret using ECDH
    
    Args:
        private_key: Your private key
        other_public_key: The other party's public key
        curve_info: Information about the curve
        
    Returns:
        The computed shared point
    """
    curve = curve_info['curve']
    
    if hasattr(other_public_key, 'x') and hasattr(other_public_key, 'y'):
        shared_point = curve.scalar_multiply(private_key, other_public_key)
        return shared_point
    else:
        if curve_info['name'] == 'Curve25519':
            u_coordinate = other_public_key
            # In X25519, we work with the u-coordinate only
            raise NotImplementedError("X25519 implementation not available")
        else:
            raise ValueError("Unknown curve format for ECDH")

def ecdh_derive_key(shared_point, key_len=32):
    """
    Derive a symmetric key from the shared point
    
    Args:
        shared_point: The computed shared point
        key_len: Desired key length in bytes
        
    Returns:
        A byte string to use as a symmetric key
    """
    if hasattr(shared_point, 'x') and hasattr(shared_point, 'y'):
        x_bytes = str(shared_point.x).encode('utf-8')
    else:
        x_bytes = str(shared_point).encode('utf-8')
    
    key_material = hashlib.sha256(x_bytes).digest()
    
    if key_len > 32:
        expanded = key_material
        while len(expanded) < key_len:
            expanded += hashlib.sha256(expanded + x_bytes).digest()
        key_material = expanded
    
    return key_material[:key_len]

def ecdh_key_exchange(curve_info, peer_public_key=None):
    """
    Complete ECDH key exchange process
    
    Args:
        curve_info: Information about the curve to use
        peer_public_key: The peer's public key (if already known)
        
    Returns:
        Dictionary with private_key, public_key, peer_public_key (if provided),
        and shared_secret (if peer_public_key was provided)
    """
    private_key, public_key = ecdh_generate_keypair(curve_info)
    
    result = {
        'private_key': private_key,
        'public_key': public_key
    }
    
    if peer_public_key is not None:
        shared_point = ecdh_compute_shared_secret(private_key, peer_public_key, curve_info)
        shared_secret = ecdh_derive_key(shared_point)
        
        result['peer_public_key'] = peer_public_key
        result['shared_secret'] = shared_secret
    
    return result