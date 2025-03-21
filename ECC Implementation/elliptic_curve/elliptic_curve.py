from .point import Point

class EllipticCurve:
    def __init__(self, field):
        self.field = field
        self.identity_point = Point(None, None, self)

    def is_on_curve(self, point):
        raise NotImplementedError("Subclasses must implement is_on_curve")

    def point_addition(self, point1, point2):
        raise NotImplementedError("Subclasses must implement point_addition")

    def point_doubling(self, point):
        raise NotImplementedError("Subclasses must implement point_doubling")

    def scalar_multiplication(self, point, scalar):
        if scalar == 0:
            return self.identity_point
        if scalar < 0:
            return self.scalar_multiplication(self.point_negation(point), -scalar)
        result = self.identity_point
        power_of_2 = point
        while scalar > 0:
            if scalar % 2 == 1:
                result = result + power_of_2
            power_of_2 = self.point_doubling(power_of_2)
            scalar //= 2
        return result

    def point_negation(self, point):
        if point == self.identity_point:
            return self.identity_point
        raise NotImplementedError("Subclasses must implement point_negation")