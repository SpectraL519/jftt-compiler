def logarithmic_modulo(a, b):
    remainder = 0
    if a == 0:
        return remainder
    if b == 0:
        return remainder

    remainder = a

    while True:
        if remainder < b:
            break

        tmp_b = b
        while True:
            if remainder < (tmp_b << 1):
                break
            tmp_b <<= 1

        remainder -= tmp_b

    return remainder



print(logarithmic_modulo(0, 4)) # 0
print(logarithmic_modulo(7, 0)) # 0
print(logarithmic_modulo(6, 2)) # 0
print(logarithmic_modulo(8, 3)) # 2
print(logarithmic_modulo(5, 7)) # 5
