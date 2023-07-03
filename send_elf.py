#!/usr/bin/env python3
import argparse
import asyncio
import re
from contextlib import asynccontextmanager
from pathlib import Path
import aiofiles

SEM = asyncio.Semaphore(60)
ORIGINAL_ELF_PORT = 9020
LOGGER_PORT = 9021
KLOGGER_PORT = 9081
COMMAND_PORT = 9028
ELF_PORT = 9027

MAX_PROCESS_NAME_LENGTH = 32
DAEMON_TYPE = b'\x01'
GAME_TYPE = b'\x02'

RESPONSE_OK = b'\x00'
RESPONSE_ERROR = b'\xff'


class LineBuffer(bytearray):

    # hack to deal with StreamReader not allowing a regex pattern
    SEP = re.compile(b'(?:\r\n)|\r|\n')

    def find(self, _, offset):
        match = self.SEP.search(self, offset)
        return match.start() if match else -1


class DummyLogger:

    async def __aenter__(self):
        await asyncio.sleep(0)
        return self

    async def __aexit__(self, *args):
        await asyncio.sleep(0)

    async def write(self, _):
        await asyncio.sleep(0)


@asynccontextmanager
async def get_logger(log: Path | None):
    if log is None:
        async with DummyLogger() as fp:
           yield fp
    else:
        async with aiofiles.open(log, 'w+', encoding='utf-8') as fp:
            yield fp


@asynccontextmanager
async def open_connection(host: str, port: int):
    while True:
        try:
            reader, writer = await asyncio.open_connection(host, port)
        except OSError:
            await asyncio.sleep(1)
            continue
        try:
            yield reader, writer
            break
        finally:
            await writer.drain()
            writer.close()


async def send_elf(host: str, elf: Path, processName: str, isGame: bool):
    nameLength = len(processName)
    if nameLength > MAX_PROCESS_NAME_LENGTH:
        print(f'process name will be truncated to {processName[:MAX_PROCESS_NAME_LENGTH]}')
    elif nameLength < MAX_PROCESS_NAME_LENGTH:
        processName += '\0'*(MAX_PROCESS_NAME_LENGTH - nameLength)

    async with open_connection(host, ELF_PORT) as (reader, writer):
        data = elf.read_bytes()
        if isGame:
            writer.write(GAME_TYPE)
        else:
            writer.write(DAEMON_TYPE)
        writer.write(processName.encode('latin-1'))
        writer.write(len(data).to_bytes(8, byteorder='little'))
        writer.write(data)
        writer.write_eof()
        await writer.drain()
        response = await reader.readexactly(1)
        print(f'response {response}')
        if response == RESPONSE_OK:
            print('elf sent successfully')
            return True
        if response == RESPONSE_ERROR:
            print(f'failed to send {elf}')
        else:
            print('unexpected response ' + response.hex())
        return False


async def log_task(reader: asyncio.StreamReader, log: Path | None, silent = False):
    async with get_logger(log) as fp:
        line = b''
        while True:
            if reader.at_eof():
                if '\r' in line:
                    await fp.write(line.replace('\r', '\n'))
                break
            try:
                line = await reader.readline()
            except OSError:
                await asyncio.sleep(1)
                continue

            line = line.decode('latin-1')
            if silent:
                continue
            if '\r' not in line:
                await fp.write(line)
            print(line, end='')


async def has_homebrew_daemon(host: str) -> bool:
    # checks if the homebrew daemon is already running on the PS5
    async with SEM:
        try:
            reader, writer = await asyncio.open_connection(host, COMMAND_PORT)
            try:
                writer.write(b'\x00')
                response = await reader.read(1)
                return response == b'\x00'
            finally:
                writer.close()
        except OSError:
            return False


async def logger_client(host: str, spawner: Path, log: Path | None, silent: bool):
    async with SEM:
        async with open_connection(host, LOGGER_PORT) as (reader, writer):
            reader._buffer = LineBuffer(reader._buffer)
            writer.write(spawner.read_bytes())
            writer.write_eof()
            await writer.drain()
            await log_task(reader, log, silent)


async def send_spawner(host: str, spawner: Path):
    async with SEM:
        async with open_connection(host, ORIGINAL_ELF_PORT) as (reader, writer):
            reader._buffer = LineBuffer(reader._buffer)
            writer.write(spawner.read_bytes())
            writer.write_eof()
            await writer.drain()


async def klog_client(host, log: Path | None, silent: bool):
        async with open_connection(host, KLOGGER_PORT) as (reader, _):
            reader._buffer = LineBuffer(reader._buffer)
            await log_task(reader, log, silent)


async def run_loggers(args):
    has_daemon = await has_homebrew_daemon(args.host)
    if not has_daemon:
        spawner = asyncio.create_task(send_spawner(args.host, args.spawner))
        logger = asyncio.create_task(logger_client(args.host, args.spawner, args.log, args.silent))
        await asyncio.wait((spawner, logger), return_when=asyncio.ALL_COMPLETED)

    if args.name is None:
        args.name = args.elf.stem

    klogger = asyncio.create_task(klog_client(args.host, args.klog, args.silent))
    sender = asyncio.create_task(send_elf(args.host, args.elf, args.name, args.game))
    await asyncio.wait((klogger, sender), return_when=asyncio.ALL_COMPLETED)


def main():
    parser = argparse.ArgumentParser(
        description='Helper script for sending the spawner, an elf to load into spawned process and for logging',
    )
    parser.add_argument('ip', help='PS5 ip address')
    parser.add_argument('elf', help='Path to the elf to load into the spawned process')
    parser.add_argument(
        '--spawner',
        default='bin/spawner.elf',
        help='Path to the elf used to spawn the new process. (default: bin/spawner.elf)'
    )
    parser.add_argument(
        '--log',
        default='log.txt',
        help='Path to output log file if enabled. (default: log.txt)'
    )
    parser.add_argument(
        '--klog',
        default='klog.txt',
        help='Path to output log file if enabled. (default: log.txt)'
    )
    parser.add_argument(
        '--nolog',
        default=False,
        action='store_true',
        help='Switch to disable logging to an output file. (default False)'
    )
    parser.add_argument(
        '--silent',
        default=False,
        action='store_true',
        help='Switch to disable all logging. (default: False)'
    )
    parser.add_argument(
        '--game',
        default=False,
        action='store_true',
        help='Set process type to game. (default: False)'
    )
    parser.add_argument(
        '--name',
        default=None,
        help='The name of the new process. (default: the elf file name without the extension)'
    )
    args = parser.parse_args()
    try:
        args.host = args.ip
        args.elf = Path(args.elf)
        args.spawner = Path(args.spawner)
        args.log = Path(args.log)
        args.klog = Path(args.klog)
        if args.nolog:
            args.log = None
        if not args.spawner.exists():
            print(f'{args.spawner} does not exist')
            return
        if not args.elf.exists():
            print(f'{args.elf} does not exist')
            return
        asyncio.run(run_loggers(args))
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
