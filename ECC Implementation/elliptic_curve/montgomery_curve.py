from .elliptic_curve import EllipticCurve
from .point import Point

class MontgomeryCurve(EllipticCurve):
    """
    Montgomery form of an elliptic curve: By^2 = x^3 + Ax^2 + x
    
    This form is often used for efficient scalar multiplication
    """
    
    def __init__(self, A, B, field):
        """
        Initialize a Montgomery curve with parameters A and B over the given field.
        
        Args:
            A: Coefficient A in the curve equation
            B: Coefficient B in the curve equation
            field: The finite field over which the curve is defined
        """
        super().__init__(field)
        self.A = field(A)
        self.B = field(B)
        
        if self.B * (self.A**2 - 4) == 0:
            raise ValueError("Invalid Montgomery curve parameters: B(A^2-4) must not be 0")
            
    def is_on_curve(self, point):
        """Check if a point is on the curve"""
        if point.is_infinity():
            return True
            
        x, y = point.x, point.y
        left = self.B * (y**2)
        right = x**3 + self.A * (x**2) + x
        
        return left == right
        
    def add(self, p1, p2):
        """Add two points on the curve"""
        if p1.is_infinity():
            return p2
        if p2.is_infinity():
            return p1
        if p1 == p2:
            return self.double(p1)
        if p1.x == p2.x:
            return self.identity_point()
            
        x1, y1 = p1.x, p1.y
        x2, y2 = p2.x, p2.y
        
        lambda_val = (y2 - y1) / (x2 - x1)
        x3 = self.B * (lambda_val**2) - self.A - x1 - x2
        y3 = lambda_val * (x1 - x3) - y1
        
        return Point(x3, y3)
        
    def double(self, point):
        """Double a point on the curve"""
        if point.is_infinity():
            return point
            
        x, y = point.x, point.y
        
        if y == 0:
            return self.identity_point()
            
        lambda_val = (3 * x**2 + 2 * self.A * x + 1) / (2 * self.B * y)
        x3 = self.B * (lambda_val**2) - self.A - 2 * x
        y3 = lambda_val * (x - x3) - y
        
        return Point(x3, y3)
    
    def scalar_multiply(self, k, point):
        """
        Scalar multiplication using Montgomery ladder - more efficient and constant-time
        """
        if k == 0 or point.is_infinity():
            return self.identity_point()
            
        r0 = self.identity_point()
        r1 = point
        
        for i in range(k.bit_length(), -1, -1):
            bit = (k >> i) & 1
            if bit == 0:
                r1 = self.add(r0, r1)
                r0 = self.double(r0)
            else:
                r0 = self.add(r0, r1)
                r1 = self.double(r1)
                
        return r0 