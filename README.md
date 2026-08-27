*This project has been created as part of the 42 curriculum by dosorio-.*

# Codexion

## Description

Codexion is a multithreaded simulation based on the classic **Dining
Philosophers** problem. Coders (threads) sit in a circle, each sharing a
left and right **dongle** with its neighbors. A coder needs both dongles to
**compile**; without them it **debugs/refactors**. If a coder goes too long
without compiling, it **burns out** and the simulation stops.

Goal: every coder reaches the required number of compiles before anyone
burns out, while safely handling deadlock, starvation, and race conditions.

## Instructions

### Compilation

```bash
make
```

Other Makefile commands:

| Command       | What it does                                                   |
|---------------|------------------------------------------------------------------|
| `make`        | Compiles the sources and produces the `codexion` binary.        |
| `make clean`  | Removes the object files (`.o`), keeps the binary.               |
| `make fclean` | Removes the object files and the binary, then clears the screen. |
| `make re`     | Runs `fclean` followed by `all` (full rebuild).                  |
| `make r`      | Runs `all` followed by `clean`, then clears the screen.          |
| `make help`   | Prints a list of all available commands.                         |

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument                      | Meaning                                                                                             |
|-------------------------------|-----------------------------------------------------------------------------------------------------|
| `number_of_coders`            | How many coders (threads) join the simulation. (cannot be 0)                                        |
| `time_to_burnout`             | Max time (ms) a coder can go without compiling before it burns out and the sim stops.  (cannot be 0)|
| `time_to_compile`             | Time (ms) a compile takes.                                                                          |
| `time_to_debug`               | Time (ms) spent debugging.                                                                          |
| `time_to_refactor`            | Time (ms) spent refactoring.                                                                        |
| `number_of_compiles_required` | How many compiles each coder must reach for the simulation to succeed. (cannot be 0)                |
| `dongle_cooldown`             | Time (ms) a released dongle must stay idle before it can be picked up again.                        |
| `scheduler`                   | Policy used to resolve contention on a dongle: `fifo` or `edf` (see below).                         |

All numeric arguments must be positive integers.

**Example:**
```bash
./codexion 5 800 200 100 100 7 50 fifo
```
5 coders, 800 ms to burn out, 200 ms compiles, 100 ms debug/refactor, 7
compiles required per coder, 50 ms cooldown, FIFO scheduling.

### `fifo` vs `edf`

Both policies decide, when two coders are waiting for the same dongle, who
gets it next.

- **`fifo`** — first come, first served: whoever asked for the dongle first
  gets it first. Simple and predictable, but it doesn't take burnout risk
  into account — a coder that is about to burn out waits in line just like
  anyone else.
- **`edf`** (Earliest Deadline First) — the coder that has gone the
  **longest without compiling** (i.e. is closest to burning out) is served
  first, regardless of arrival order.

In most runs the two behave very similarly, since coders tend to compile at
a fairly even pace. `edf` exists for the **specific, tighter cases**: tight
`time_to_burnout` values, uneven timings, or many coders competing for few
dongles, where a coder could realistically starve and burn out while
waiting its turn in a plain FIFO queue. In those edge cases, `edf` actively
protects whoever is closest to burnout instead of letting them wait behind
less urgent requests.

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions):** dongles are always
  acquired in ascending `dongle_id` order, so no circular-wait chain can
  form between coders.
- **Starvation prevention:** each dongle keeps a waiting queue; the
  scheduler picks the next owner by `fifo` order or `edf` (coder that
  waited longest since its last compile gets priority).
- **Cooldown handling:** a released dongle can't be re-taken until
  `dongle_cooldown` ms have passed, forcing fair hand-off between neighbors.
- **Precise burnout detection:** a supervisor thread checks each coder's
  time since last compile against `time_to_burnout` and stops the
  simulation the instant the limit is reached.
- **Log serialization:** all output goes through a locked print function so
  messages from different threads never interleave.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle** — protects `owner`, `waiting_queue`, and
  `released_ms`; every check/update of a dongle's state is atomic.
- **`pthread_cond_t` per dongle** — coders block with `pthread_cond_wait`
  when a dongle is busy, and are woken with `pthread_cond_broadcast` when it
  is released or when the simulation stops.
- **`pthread_mutex_t` per coder** — protects `last_compile` /
  `total_compiles`, read both by the coder itself and by the supervisor.
- **Global `pthread_mutex_t`** — guards the `running` flag and completion
  counter, giving every thread a consistent view of simulation state.
- **Logging mutex** — serializes all `printf` calls.
- **Custom waiting-queue "event" structure** — each dongle keeps its own
  2-slot queue, letting `scheduler()` decide exactly which waiting coder
  gets served next (FIFO or EDF), instead of relying on unspecified pthread
  wake order.

**Race condition prevention example:** in `try_get_dongle`, the dongle's
mutex is locked before checking ownership/queue and stays locked until the
coder either takes the dongle or queues — making "check → take" atomic.

**Thread-safe communication example:** before compiling, `verify_compile`
re-locks both dongles and re-checks that the coder is still the owner of
both, protecting against another thread reclaiming a dongle in between.

## Resources

- https://dev.to/pliniohr/threads-na-linguagem-c-598k
- https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html
- POSIX Threads documentation (`pthread_mutex_t`, `pthread_cond_t`,
  `pthread_create`, `pthread_join`).
- https://www.youtube.com/watch?v=y73bgQNluBk
- https://www.youtube.com/watch?v=jHRUxUMT4dc

**AI usage:** an AI assistant (Copilot) was used to help draft and organize
this README from the existing source code and design, to the documentation and to help with debugging. All concurrency
design and implementation (dongle ordering, scheduler, cooldown, burnout
detection, synchronization) was done by the author.
