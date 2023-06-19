import sys


FSTRING = '    0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x},'

def iter_rows(data: bytes):
    length = len(data)
    rows = length // 16
    for i in range(rows):
        pos = i*16
        yield FSTRING.format(*data[pos:pos+16])


def get_rem(data: bytes):
    length = len(data)
    rem = length % 16

    if rem == 0:
        return None

    end = length - rem
    res = []
    for i in range(rem):
        res.append(f'0x{data[end+i]:02x}')
    return '    ' + ', '.join(res)


def print_shellcode(shellcode: str):
    data = bytes.fromhex(shellcode)
    print(f'length: {len(data)}')
    print('{')
    for row in iter_rows(data):
        print(row)
    rem = get_rem(data)
    if rem:
        print(rem)
    print('};')


if __name__ == '__main__':
    if len(sys.argv) == 1:
        print(f'usage: {__file__} shellcode bytes...')
    else:
        print_shellcode(''.join(sys.argv[1:]))
