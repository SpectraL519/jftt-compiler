def logarithmic_divide(a, b):
    quotient = 0
    if b == 0:
        return quotient
    if a == quotient:
        return quotient

    while True:
        if a < b:
            break

        temp_b = b
        multiple = 1
        while True:
            if a < (temp_b << 1):
                break
            temp_b <<= 1
            multiple <<= 1

        a -= temp_b
        quotient += multiple

    return quotient


print(logarithmic_divide(6, 2))
print(logarithmic_divide(6, 0))
print(logarithmic_divide(0, 2))
print(logarithmic_divide(2, 6))
print(logarithmic_divide(3, 2))
