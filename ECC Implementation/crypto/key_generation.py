import os
import secrets
from ..elliptic_curve import Point

def generate_private_key(curve_info):
    """
    Generate a secure private key for use with the specified curve
    
    Args:
        curve_info: Information about the curve
        
    Returns:
        A randomly generated integer to be used as private key
    """
    order = curve_info.get('order')
    
    if not order:
        raise ValueError("Curve information must include 'order'")
    
    private_key = secrets.randbelow(order - 1) + 1
    
    return private_key

def derive_public_key(private_key, curve_info):
    """
    Derive the public key from a private key
    
    Args:
        private_key: The private key
        curve_info: Information about the curve
        
    Returns:
        The corresponding public key (a point on the curve)
    """
    curve = curve_info['curve']
    
    # Check if we're dealing with a Montgomery curve like Curve25519
    if curve_info.get('name') == 'Curve25519':
        u = curve_info.get('base_u')
        if not u:
            raise ValueError("Curve25519 information must include 'base_u'")
            
        raise NotImplementedError("X25519 key derivation not yet implemented")
    
    generator_x = curve_info.get('generator_x')
    generator_y = curve_info.get('generator_y')
    
    if not generator_x or not generator_y:
        raise ValueError("Curve information must include 'generator_x' and 'generator_y'")
    
    generator = Point(generator_x, generator_y)
    
    public_key = curve.scalar_multiply(private_key, generator)
    
    return public_key

def generate_key_pair(curve_info):
    """
    Generate a private/public key pair for elliptic curve cryptography
    
    Args:
        curve_info: Information about the curve
        
    Returns:
        (private_key, public_key) tuple
    """
    private_key = generate_private_key(curve_info)
    public_key = derive_public_key(private_key, curve_info)
    
    return private_key, public_key

def serialize_public_key(public_key, curve_info, compressed=True):
    """
    Serialize a public key to bytes
    
    Args:
        public_key: The public key to serialize
        curve_info: Information about the curve
        compressed: Whether to use compressed representation
        
    Returns:
        Byte representation of the public key
    """
    if public_key.is_infinity():
        return b'\x00'
    
    x_bytes = int(public_key.x).to_bytes(32, byteorder='big')
    
    if compressed:
        prefix = 2 + (int(public_key.y) & 1)
        return bytes([prefix]) + x_bytes
    else:
        y_bytes = int(public_key.y).to_bytes(32, byteorder='big')
        return b'\x04' + x_bytes + y_bytes

def deserialize_public_key(serialized_key, curve_info):
    """
    Deserialize a public key from bytes
    
    Args:
        serialized_key: Byte representation of the public key
        curve_info: Information about the curve
        
    Returns:
        The deserialized public key
    """
    curve = curve_info['curve']
    field = curve_info['field']
    
    if serialized_key[0] == 0:
        return curve.identity_point()
    
    if serialized_key[0] == 4: 
        if len(serialized_key) < 65:
            raise ValueError("Invalid uncompressed public key length")
        
        x_bytes = serialized_key[1:33]
        y_bytes = serialized_key[33:65]
        
        x = int.from_bytes(x_bytes, byteorder='big')
        y = int.from_bytes(y_bytes, byteorder='big')
        
        return Point(field(x), field(y))
        
    elif serialized_key[0] in (2, 3): 
        if len(serialized_key) < 33:
            raise ValueError("Invalid compressed public key length")
            
        x_bytes = serialized_key[1:33]
        x = field(int.from_bytes(x_bytes, byteorder='big'))
        
        y_is_odd = (serialized_key[0] == 3)
        
        # For Weierstrass curve y^2 = x^3 + ax + b, solve for y
        if isinstance(curve, curve_info.get('WeierstraSsCurve')):
            y_squared = x**3 + curve.a * x + curve.b
            y = y_squared.sqrt()
            
            if bool(int(y) & 1) != y_is_odd:
                y = -y
            
            return Point(x, y)
        else:
            raise ValueError("Compressed point format only supported for Weierstrass curves")
    else:
        raise ValueError("Invalid public key format")