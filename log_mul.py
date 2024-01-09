def logarithmic_multiply(a, b):
    product = 0
    if a == 0:
        return product
    if b == 0:
        return product

    while True:
        if b == 0:
            break

        if (b - (b >> 1 << 1)) > 0:
            product += a

        a <<= 1
        b >>= 1

    return product



print(logarithmic_multiply(6, 0)) # 0
print(logarithmic_multiply(0, 2)) # 0
print(logarithmic_multiply(6, 2)) # 12
print(logarithmic_multiply(4, 5)) # 20
print(logarithmic_multiply(3, 2)) # 6
