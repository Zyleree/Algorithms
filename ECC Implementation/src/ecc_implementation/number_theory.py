def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    gcd, x1, y1 = extended_gcd(b % a, a)
    x = y1 - (b // a) * x1
    y = x1
    return gcd, x, y

def modular_inverse(a, m):
    gcd, x, y = extended_gcd(a, m)
    if gcd != 1:
        return None 
    return x % m

def polynomial_add(poly1_coeffs, poly2_coeffs):
    max_len = max(len(poly1_coeffs), len(poly2_coeffs))
    poly1_coeffs = poly1_coeffs + [0] * (max_len - len(poly1_coeffs))
    poly2_coeffs = poly2_coeffs + [0] * (max_len - len(poly2_coeffs))
    result_coeffs = [(c1 ^ c2) for c1, c2 in zip(poly1_coeffs, poly2_coeffs)] # XOR for addition
    return result_coeffs

def polynomial_multiply(poly1_coeffs, poly2_coeffs):
    degree1 = len(poly1_coeffs) - 1
    degree2 = len(poly2_coeffs) - 1
    result_degree = degree1 + degree2
    result_coeffs = [0] * (result_degree + 1)

    for i in range(len(poly1_coeffs)):
        for j in range(len(poly2_coeffs)):
            result_coeffs[i+j] ^= (poly1_coeffs[i] * poly2_coeffs[j]) 
    return result_coeffs

def polynomial_mod(poly_coeffs, mod_poly_coeffs):
    if not mod_poly_coeffs or mod_poly_coeffs[-1] == 0: 
        raise ValueError("Modulo polynomial cannot be zero or have zero leading coefficient")

    poly_degree = len(poly_coeffs) - 1
    mod_degree = len(mod_poly_coeffs) - 1

    if poly_degree < mod_degree:
        return poly_coeffs

    remainder_coeffs = list(poly_coeffs) 

    for i in range(poly_degree, mod_degree - 1, -1): 
        if remainder_coeffs[i] != 0:
            coeff_to_subtract = remainder_coeffs[i]

            for j in range(mod_degree + 1):
                remainder_coeffs[i - mod_degree + j] ^= (coeff_to_subtract * mod_poly_coeffs[j]) 

    while remainder_coeffs and remainder_coeffs[-1] == 0 and len(remainder_coeffs) > 1:
        remainder_coeffs.pop()
    return remainder_coeffs

def polynomial_inverse(poly_coeffs, mod_poly_coeffs, x_degree): 
    if not poly_coeffs or all(c == 0 for c in poly_coeffs):
        raise ValueError("Cannot find inverse of zero polynomial")

    a = polynomial_mod(poly_coeffs, mod_poly_coeffs) 
    b = mod_poly_coeffs
    x0 = [1] + [0] * x_degree 
    y0 = [0] * (x_degree + 1) 
    x1 = [0] * (x_degree + 1) 
    y1 = [1] + [0] * x_degree 

    while True:
        if not b or all(c == 0 for c in b): 
            return x0 

        quotient_poly, remainder_poly = polynomial_long_division(a, b) 

        a = b
        b = remainder_poly

        x_temp = polynomial_subtract(x0, polynomial_multiply(quotient_poly, x1))
        x0 = x1
        x1 = x_temp

        y_temp = polynomial_subtract(y0, polynomial_multiply(quotient_poly, y1))
        y0 = y1
        y1 = y_temp

def polynomial_long_division(numerator_coeffs, denominator_coeffs): 
    if not denominator_coeffs or all(c == 0 for c in denominator_coeffs):
        raise ValueError("Division by zero polynomial")
    if len(denominator_coeffs) > len(numerator_coeffs):
        return [0], numerator_coeffs

    quotient_coeffs = [0] * (len(numerator_coeffs) - len(denominator_coeffs) + 1)
    remainder_coeffs = list(numerator_coeffs) 

    denominator_degree = len(denominator_coeffs) - 1

    for i in range(len(quotient_coeffs) - 1, -1, -1): 
        leading_coeff_num = remainder_coeffs[len(remainder_coeffs) - 1 - i] 
        if leading_coeff_num != 0:
            quotient_coeffs[i] = leading_coeff_num 
            term_to_subtract = [0] * i + denominator_coeffs 
            term_to_subtract = [c * quotient_coeffs[i] for c in term_to_subtract] 
            remainder_coeffs = polynomial_subtract(remainder_coeffs, term_to_subtract)

    while remainder_coeffs and remainder_coeffs[-1] == 0 and len(remainder_coeffs) > 1:
        remainder_coeffs.pop()

    return quotient_coeffs, remainder_coeffs


def polynomial_subtract(poly1_coeffs, poly2_coeffs):
    return polynomial_add(poly1_coeffs, poly2_coeffs)

def polynomial_power(poly_base_coeffs, exponent, mod_poly_coeffs): 
    if exponent < 0:
        raise ValueError("Exponent must be non-negative")
    if exponent == 0:
        return [1] 

    result_poly = [1]
    base_poly = list(poly_base_coeffs) 

    while exponent > 0:
        if exponent % 2 == 1:
            result_poly = polynomial_multiply(result_poly, base_poly)
            result_poly = polynomial_mod(result_poly, mod_poly_coeffs) 

        base_poly = polynomial_multiply(base_poly, base_poly)
        base_poly = polynomial_mod(base_poly, mod_poly_coeffs) 
        exponent //= 2
    return result_poly