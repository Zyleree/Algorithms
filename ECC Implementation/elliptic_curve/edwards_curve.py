from .elliptic_curve import EllipticCurve
from .point import Point

class EdwardsCurve(EllipticCurve):
    """
    Edwards form of an elliptic curve: x^2 + y^2 = 1 + d*x^2*y^2
    
    This form has complete addition formulas that work for all points,
    which helps protect against certain side-channel attacks.
    """
    
    def __init__(self, d, field):
        """
        Initialize an Edwards curve with parameter d over the given field.
        
        Args:
            d: The parameter d in the curve equation
            field: The finite field over which the curve is defined
        """
        super().__init__(field)
        self.d = field(d)
        
        # Ensure d is not a square in the field for twisted Edwards curve
        if self.d == 0:
            raise ValueError("Invalid Edwards curve parameter: d must be non-zero")
            
    def is_on_curve(self, point):
        """Check if a point is on the curve"""
        if point.is_infinity():
            return True
            
        x, y = point.x, point.y
        left = x**2 + y**2
        right = 1 + self.d * (x**2) * (y**2)
        
        return left == right
        
    def add(self, p1, p2):
        """
        Add two points on the curve using Edwards addition formula.
        The neutral element is (0, 1).
        """
        if p1.is_infinity():
            return p2
        if p2.is_infinity():
            return p1
            
        x1, y1 = p1.x, p1.y
        x2, y2 = p2.x, p2.y
        
        # Edwards addition formula
        x_num = x1 * y2 + y1 * x2
        x_den = 1 + self.d * x1 * x2 * y1 * y2
        y_num = y1 * y2 - x1 * x2
        y_den = 1 - self.d * x1 * x2 * y1 * y2
        
        # Handle division by zero
        if x_den == 0 or y_den == 0:
            return self.identity_point()
            
        x3 = x_num / x_den
        y3 = y_num / y_den
        
        return Point(x3, y3)
        
    def double(self, point):
        """Double a point on the curve (same as addition for Edwards curves)"""
        return self.add(point, point)
    
    def identity_point(self):
        """Return the identity point (0, 1) for Edwards curves"""
        return Point(self.field(0), self.field(1), True)
    
    def scalar_multiply(self, k, point):
        """
        Scalar multiplication for Edwards curves
        Unlike other curve forms, Edwards curves don't require special handling for doubling
        """
        if k == 0 or point.is_infinity():
            return self.identity_point()
            
        # Double-and-add method
        result = self.identity_point()
        addend = point
        
        while k:
            if k & 1:
                result = self.add(result, addend)
            addend = self.add(addend, addend)
            k >>= 1
            
        return result 