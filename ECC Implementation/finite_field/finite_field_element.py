class FiniteFieldElement:
    def __init__(self, value, field):
        if not field.is_valid_element(value):
            raise ValueError("Value is not a valid element for the field")
        self.value = field.normalize_value(value)
        self.field = field

    def __repr__(self):
        return str(self.value)

    def __eq__(self, other):
        if other is None:
            return False
        return self.value == other.value and self.field == other.field

    def __ne__(self, other):
        return not (self == other)

    def __add__(self, other):
        if self.field != other.field:
            raise TypeError("Elements must be from the same field")
        return self.field.add(self, other)

    def __sub__(self, other):
        if self.field != other.field:
            raise TypeError("Elements must be from the same field")
        return self.field.subtract(self, other)

    def __mul__(self, other):
        if self.field != other.field:
            raise TypeError("Elements must be from the same field")
        return self.field.multiply(self, other)

    def __truediv__(self, other):
        if self.field != other.field:
            raise TypeError("Elements must be from the same field")
        return self.field.divide(self, other)

    def __pow__(self, exponent):
        return self.field.power(self, exponent)

    def inverse(self):
        return self.field.inverse(self)

    def sqrt(self):
        return self.field.sqrt(self)