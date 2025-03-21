def ecdh_key_exchange(private_key_a, public_key_b):
    shared_secret_point = private_key_a * public_key_b
    return shared_secret_point.x