import random


def hash_crc(data: str):
    """CRC Hash Function"""
    h = 0
    for ki in data:
        high_order = h & 0xf8000000
        h = h << 5
        h = h ^ (high_order >> 27)
        h = h ^ ki
    return h


def hash_pjw(data: str) -> int:
    """PJW Hash Function"""
    h = 0
    for ki in data:
        h = (h << 4) + ki
        g = h & 0xf0000000
        if g != 0:
            h = h ^ (g >> 24)
            h = h ^ g
    return h


def hash_buz(data: str) -> int:
    """BUZ Hash Function"""
    h = 0
    R = dict()
    for ki in data:
        high_order = h & 0x80000000
        h = h << 1
        h = h ^ (high_order >> 31)
        if not (ki in R):
            R[ki] = random.randint(0, 0xFFFFFFFF)
        h = h ^ R[ki]
    return h


def hash_default(data: str) -> int:
    return hash(data)
