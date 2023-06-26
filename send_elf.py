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
ELF_PORT = 9027


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


async def send_elf(host: str, elf: Path):
    async with open_connection(host, ELF_PORT) as (_, writer):
        data = elf.read_bytes()
        writer.write(len(data).to_bytes(8, byteorder='little'))
        writer.write(data)
        writer.write_eof()
        await writer.drain()


async def log_task(host: str, reader: asyncio.StreamReader, elf: Path, log: Path | None, silent = False):
    sent_elf = False
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
            if not sent_elf and line.startswith('waiting for connection'):
                if not silent:
                    if '\r' not in line:
                        await fp.write(line)
                    print(line, end='')
                await send_elf(host, elf)
                sent_elf = True
            if silent:
                continue
            if '\r' not in line:
                await fp.write(line)
            print(line, end='')


async def logger_client(host: str, elf: Path, spawner: Path, log: Path | None, silent: bool):
    # waiting for connection
    async with SEM:
        async with open_connection(host, LOGGER_PORT) as (reader, writer):
            reader._buffer = LineBuffer(reader._buffer)
            writer.write(spawner.read_bytes())
            writer.write_eof()
            await writer.drain()
            await log_task(host, reader, elf, log, silent)


async def send_spawner(host: str, spawner: Path):
    # waiting for connection
    async with SEM:
        async with open_connection(host, ORIGINAL_ELF_PORT) as (reader, writer):
            reader._buffer = LineBuffer(reader._buffer)
            writer.write(spawner.read_bytes())
            writer.write_eof()
            await writer.drain()


async def run_loggers(host: str, elf: Path, spawner: Path, log: Path | None, silent: bool):
    await send_spawner(host, spawner)

    # klogger code was left incase it's helpful for someone in the future
    logger = asyncio.create_task(logger_client(host, elf, spawner, log, silent))
    #klogger = asyncio.create_task(klog_client(host))
    tasks = (logger, ) #klogger)
    await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
    if not logger.done():
        await asyncio.wait_for(logger, timeout=None)
    #if not klogger.done():
    #    klogger.cancel()


def main():
    parser = argparse.ArgumentParser(
        description='Helper script for sending the spawner, elf to load into spawned process and for logging',
        epilog='Text at the bottom of help'
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
    args = parser.parse_args()
    try:
        elf = Path(args.elf)
        spawner = Path(args.spawner)
        log = Path(args.log)
        if args.nolog:
            log = None
        if not spawner.exists():
            print(f'{spawner} does not exist')
            return
        if not elf.exists():
            print(f'{elf} does not exist')
            return
        asyncio.run(run_loggers(args.ip, elf, spawner, log, args.silent))
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
