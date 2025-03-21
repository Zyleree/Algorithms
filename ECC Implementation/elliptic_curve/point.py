
class Point:
    def __init__(self, x, y, curve):
        self.x = x
        self.y = y
        self.curve = curve

        if x is not None and y is not None:
            if hasattr(curve, 'identity_point') and curve.identity_point is not None:
                if not curve.is_on_curve(self):
                    raise ValueError("Point is not on the elliptic curve")

    def __repr__(self):
        if self.x is None and self.y is None:
            return "Identity Point"
        return f"Point({self.x}, {self.y})"

    def __eq__(self, other):
        if other is None:
            return False
        if self.x is None and self.y is None:
            return other.x is None and other.y is None
        return self.x == other.x and self.y == other.y and self.curve == other.curve

    def __ne__(self, other):
        return not (self == other)

    def __add__(self, other):
        return self.curve.point_addition(self, other)

    def __rmul__(self, scalar):
        return self.curve.scalar_multiplication(self, scalar)