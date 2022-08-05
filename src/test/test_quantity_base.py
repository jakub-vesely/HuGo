import sys
import os

sys.path.append("./devel") #from src

from ___basal.___quantity_base import QuantityBase

def main():
    current = QuantityBase("I", {"kA": 1e3, "A": 1, "mA": 1e-3, "uA": 1e-6}, 4)

    def print_both(value, fit_size):
        print((current.get_value_str(value, fit_size), current.get_value_str(value, 0)))

    print_both(12345678, 8)
    print_both(1234567, 8)
    print_both(123456, 8)
    print_both(12345, 8)
    print_both(1234, 8)
    print_both(123, 8)
    print_both(12, 8)
    print_both(1, 8)
    print_both(0, 8)
    print_both(0.1, 8)
    print_both(0.12, 8)
    print_both(0.123, 8)
    print_both(0.1234, 8)
    print_both(0.12345, 8)
    print_both(0.012345, 8)
    print_both(0.0012345, 8)
    print_both(0.00012345, 8)
    print_both(0.000012345, 8)
    print_both(0.0000012345, 8)
    print_both(0.00000012345, 8)
    print_both(0.000000012345, 8)
    print_both(0.0000000012345, 8)

    print(current.get_full_str(1))
    print_both(True, 8)
    print_both(-0.2144, 8)

main()
