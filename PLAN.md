# redis_from_scratch

A Redis-compatible server written from scratch: real clients (`redis-cli`, client
libraries, `redis-benchmark`) connect to it and can't tell the difference for the
commands it supports.

Target Linux; the event loop
is the heart of the project, so write it against `epoll` directly rather than hiding it
behind an async runtime.

## Milestones

### 1. RESP and a blocking server
- Parser and encoder for RESP2: simple strings, errors, integers, bulk strings, arrays.
  Handle partial reads (a command split across TCP packets) and pipelined commands
  (several in one read).
- One-connection TCP server on port 6379 answering `PING`, `ECHO`, `COMMAND` (enough for
  `redis-cli` to be happy).

### 2. Event loop
- Single-threaded, non-blocking sockets, `epoll` (optionally `kqueue` on macOS).
- Per-connection read and write buffers; handle slow clients and partial writes.
- Thousands of concurrent connections without a thread per client.

### 3. Data and commands
- Strings: `GET`, `SET` (with `EX`, `PX`, `NX`, `XX`), `DEL`, `EXISTS`, `INCR`, `DECR`,
  `MGET`, `MSET`.
- Expiry: `EXPIRE`, `TTL`, `PERSIST`; lazy deletion on access plus periodic active
  sampling, like Redis.
- One collection type to start, e.g. lists (`LPUSH`, `RPUSH`, `LPOP`, `LRANGE`), then
  hashes (`HSET`, `HGET`, `HGETALL`).
- Correct error replies for wrong types and wrong argument counts.

### 4. Persistence
- Append-only file: log every write command; `appendfsync` modes `always`, `everysec`, `no`.
- Replay the AOF on startup.
- `BGREWRITEAOF` to compact the log (fork, or write a snapshot then swap).

### 5. Stretch
- RDB-style snapshots, pub/sub (`SUBSCRIBE`/`PUBLISH`), or primary/replica replication.

## Measuring it

- `redis-benchmark -t set,get -n 1000000 -P 16` against this server and against real
  Redis on the same machine; report ops/sec and p50/p99 latency in the README.
- Compatibility: run a real client library's basic test against it.
- Crash test: write keys with `appendfsync always`, `kill -9` the server mid-load,
  restart, and check nothing acknowledged was lost.
- Fuzz the RESP parser (malformed input must never crash the server).

## Language

- **C**: closest to real Redis. You hand-build the hash table, dynamic
  strings, and buffers, and call `epoll` directly. Strongest "systems" signal, most time
  spent on memory bugs.

## Done means

- Milestones 1–4 working, `redis-cli` and `redis-benchmark` run against it.
- README with the benchmark table, the event-loop design, and the persistence trade-offs.
