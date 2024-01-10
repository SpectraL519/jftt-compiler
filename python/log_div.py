def logarithmic_divide(a, b):
    quotient = 0
    if b == 0:
        return quotient
    if a == 0:
        return quotient

    while True:
        if a < b:
            break

        tmp_b = b
        multiple = 1
        while True:
            if a < (tmp_b << 1):
                break
            tmp_b <<= 1
            multiple <<= 1

        a -= tmp_b
        quotient += multiple

    return quotient


print(logarithmic_divide(6, 0)) # 0
print(logarithmic_divide(0, 2)) # 0
print(logarithmic_divide(6, 2)) # 3
print(logarithmic_divide(2, 6)) # 0
print(logarithmic_divide(3, 2)) # 1
