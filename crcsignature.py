def crc16_EN13757(data):
    POLY = 0x3D65
    crc = 0x0000
    for byte in data:
        for _ in range(8):
            if ((crc & 0x8000) >> 8) ^ (byte & 0x80):
                crc = ((crc << 1) ^ POLY) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
            byte = (byte << 1) & 0xFF
    return (~crc) & 0xFFFF

def sig(hex_str):
    b = bytes.fromhex(hex_str)
    s = crc16_EN13757(b)
    return f"sig={s:04x}"

# Original format (verify)
orig = "04FF234413523B06FF1B426C61675167023B04138101E7FF0F"
print(f"Original:  {orig} → {sig(orig)}")

# Replace 8101E7FF0F with 06FF1C
with_1c = "04FF234413523B06FF1B426C61675167023B041306FF1C"
print(f"With FF1C: {with_1c} → {sig(with_1c)}")

# Replace 8101E7FF0F with 06FF1D
with_1d = "04FF234413523B06FF1B426C61675167023B041306FF1D"
print(f"With FF1D: {with_1d} → {sig(with_1d)}")

# With both 06FF1C and 06FF1D
with_both = "04FF234413523B06FF1B426C61675167023B041306FF1C06FF1D"
print(f"With both: {with_both} → {sig(with_both)}")

# With both 06FF1D and 06FF1C
with_both = "04FF234413523B06FF1B426C61675167023B041306FF1D06FF1C"
print(f"With both: {with_both} → {sig(with_both)}")