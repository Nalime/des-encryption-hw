# DES Encryption Homework

This program expects for each line a pair of
key string and plaintext string as input.
Both strings have to have 8 ASCII characters.
See des.in for examples.
Though it should be easy for it to support longer strings.

The implementation of the core function of the DES encryption is mostly
composed of bitwise operations.

The encryption time and (especially) IO time for the first case
seems to be very long compared to subsequent cases.
I don't know why :/

Please note that although it's correct for the 6 cases in des.in,
general correctness isn't guaranteed.
