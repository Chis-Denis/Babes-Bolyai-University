# Problem 2 — Correct Proposed Solutions (All Subjects)

For each exam subject, this file gives the **corrected code** for Problem 2 (the one with issues and fixes), based on the teacher’s original code with only the necessary modifications applied (the fixes identified in the solutions).

---

## 1. 2022_2 — Producer-Consumer Queue

**Fix applied:** H — remove the unlock in the middle of `dequeue()` and insert an unlock on each return path (place 3 and place 4).

**Original bug:** The lock was released (statement 2) before checking `if(!items.empty())` and accessing `items`, causing a data race. The lock must be held for the whole critical section and released only before each return.

**Corrected `dequeue()` (C++-style):**

```cpp
std::optional<T> dequeue() {
    std::unique_lock<std::mutex> lck(mtx);   // statement 1 — at start (unchanged)
    while (items.empty() && !isClosed) {
        cv.wait(lck);
    }
    if (items.empty() && isClosed) {
        lck.unlock();                         // unlock before return (place 4)
        return std::optional<T>();            // EOF
    }
    std::optional<T> ret(items.front());
    items.pop_front();
    lck.unlock();                             // unlock before return (place 3)
    return ret;
}
```

**Full class code (corrected — fix H applied):**

```cpp
#include <optional>
#include <list>
#include <mutex>
#include <condition_variable>

template<typename T>
class ProducerConsumerQueue {
public:
    void enqueue(T v) {
        std::unique_lock<std::mutex> lck(mtx);
        items.push_back(v);
        cv.notify_one();
    }

    std::optional<T> dequeue() {
        std::unique_lock<std::mutex> lck(mtx);
        while (items.empty() && !isClosed) {
            cv.wait(lck);
        }
        if (items.empty() && isClosed) {
            lck.unlock();
            return std::optional<T>();
        }
        std::optional<T> ret(items.front());
        items.pop_front();
        lck.unlock();
        return ret;
    }

    void close() {
        std::unique_lock<std::mutex> lck(mtx);
        isClosed = true;
        cv.notify_all();
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    std::list<T> items;
    bool isClosed = false;
};
```

**Summary of changes:** Remove the single `lck.unlock();` that was in the middle of `dequeue()`. Add `lck.unlock();` immediately before each of the two returns (before `return std::optional<T>();` and before `return ret;`). The lock is now held for the entire critical section (predicate check, wait, and access to `items`) and released only on each exit path.

---

## 2. 2025_1 — Future with `set()` and `addContinuation()`

**Fix applied:** G — move the content of line 12 (`unique_lock<mutex> lck(mtx);`) to between lines 9 and 10 in `set()` (i.e. acquire the mutex at the start of `set()`).

**Original bug:** In `set()`, `hasValue` and `val` were updated (lines 10–11) without holding the mutex, so a thread in `addContinuation()` could see `hasValue == true` but `val` not yet written and call `f(val)` with an uninitialized value.

**Corrected `set(T v)` (C++-style):**

```cpp
void set(T v) {
    std::unique_lock<std::mutex> lck(mtx);   // moved here (was line 12; now first)
    hasValue = true;
    val = v;
    for (auto& f : continuations) {
        f(v);
    }
    continuations.clear();
}
```

**`addContinuation()` — unchanged** (keep the unlock before `f(val)` so that if `f` calls back into the Future, we avoid deadlock):

```cpp
void addContinuation(std::function<void(T)> f) {
    std::unique_lock<std::mutex> lck(mtx);
    if (hasValue) {
        lck.unlock();
        f(val);
    } else {
        continuations.push_back(f);
    }
}
```

**Full class code (corrected — fix G applied):**

```cpp
#include <list>
#include <functional>
#include <mutex>

template<typename T>
class Future {
public:
    Future() : hasValue(false) {}

    void set(T v) {
        std::unique_lock<std::mutex> lck(mtx);
        hasValue = true;
        val = v;
        for (auto& f : continuations) {
            f(v);
        }
        continuations.clear();
    }

    void addContinuation(std::function<void(T)> f) {
        std::unique_lock<std::mutex> lck(mtx);
        if (hasValue) {
            lck.unlock();
            f(val);
        } else {
            continuations.push_back(f);
        }
    }

private:
    std::list<std::function<void(T)>> continuations;
    T val;
    bool hasValue;
    std::mutex mtx;
};
```

**Summary of changes:** In `set()`, acquire the mutex at the very beginning, then set `hasValue` and `val`, then run and clear continuations. No other change. Any thread that later sees `hasValue == true` in `addContinuation()` does so only after `set()` has released the lock, so `val` has been written.

---

## 3. 2025_3 — Future with `set()` and `get()`

**Fix applied:** H — reorder lines 10–13 in `set()` to: 11, 13, 12, 10 (lock, then `val = v`, then `hasValue = true`, then `cv.notify_all()`).

**Original bug:** (1) Notify was called before acquiring the lock and before writing the value. (2) `hasValue = true` was written before `val = v`, so a woken `get()` could see `hasValue == true` and return an uninitialized or stale `val`.

**Corrected `set(T v)` (C++-style):**

```cpp
void set(T v) {
    unique_lock<mutex> lck(mtx);   // 11 — first
    val = v;                       // 13 — write value before "ready"
    hasValue = true;               // 12 — then set flag
    cv.notify_all();               // 10 — then notify
}
```

**`get()` — unchanged:**

```cpp
T get() {
    unique_lock<mutex> lck(mtx);
    while (!hasValue) {
        cv.wait(lck);
    }
    return val;
}
```

**Summary of changes:** In `set()`: (1) acquire the lock first, (2) write `val` before `hasValue`, (3) then notify. So shared state is updated under the lock, and any thread that sees `hasValue == true` is guaranteed to see the updated `val`.

---

## 4. 2024_3 — CountdownEvent

**Fixes applied:** G, H, and I together.

- **G:** The mutex must be locked just before line 10 and unlocked after line 13 in `done()` (protect the decrement and the notify).
- **H:** Line 17 (`unique_lock<mutex> lck(mtx);`) must be moved to just before line 16 in `wait()` (check `count` under the lock).
- **I:** Replace `notify_one()` with `notify_all()` in `done()` so all waiters wake when count reaches 0.

**Original bugs:** (1) Only one waiter was woken when count reached 0, so other waiters could block forever. (2) The first check `if (count == 0) return` in `wait()` was done without the lock, so `wait()` could return without ever waiting. (3) The decrement and notify in `done()` were not protected by the mutex.

**Corrected `done()` (C++-style):**

```cpp
void done() {
    unique_lock<mutex> lck(mtx);   // G — lock before decrement
    unsigned old = count.fetch_sub(1);
    if (old == 1) {
        cv.notify_all();           // I — wake all waiters (was notify_one())
    }
}                                  // G — unlock when lck goes out of scope
```

**Corrected `wait()` (C++-style):**

```cpp
T wait() {
    unique_lock<mutex> lck(mtx);   // H — lock first (was after "if (count==0)")
    if (count == 0) return;
    while (count > 0) {
        cv.wait(lck);
    }
}
```

**Summary of changes:**

1. **done():** Wrap the whole body in `unique_lock<mutex> lck(mtx);` so the decrement and the notify run under the lock; replace `cv.notify_one()` with `cv.notify_all()`.
2. **wait():** Move the lock acquisition to the very beginning, before `if (count == 0) return`, so the decision to wait or return is always made under the lock.

---

## 5. 2023_7 — ThreadPool

**Fix applied:** H — add `lck.unlock()` just before line 13 (before executing the task) and `lck.lock()` just after line 13. Optionally G — replace `cv.notify_one()` with `cv.notify_all()` in enqueue.

**Original bugs:** (1) The lock is held during task execution (`wi()`), so only one thread can execute work at a time — work is never done in parallel, and a task that calls back into the pool (e.g. `enqueue()` or `close()`) can deadlock. (2) With `notify_one()` in enqueue, only one waiting thread is woken per enqueue, so work can sit in the queue longer than necessary when multiple threads are sleeping.

**Corrected `run()` (C++-style):**

```cpp
void run() {
    std::unique_lock<std::mutex> lck(mtx);
    while (true) {
        if (!work.empty()) {
            std::function<void()> wi = work.front();
            work.pop_front();
            lck.unlock();    // H — release before executing task
            wi();
            lck.lock();      // H — re-acquire before next iteration
        } else if (closed) {
            return;
        } else {
            cv.wait(lck);
        }
    }
}
```

**Full class code (corrected — fix H applied):**

```cpp
#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(int n) {
        threads.reserve(n);
        for (int i = 0; i < n; ++i) {
            threads.emplace_back([this]() { run(); });
        }
    }

    void enqueue(std::function<void()> f) {
        std::unique_lock<std::mutex> lck(mtx);
        work.push_back(f);
        cv.notify_one();   // optionally: cv.notify_all(); (fix G)
    }

    void close() {
        std::unique_lock<std::mutex> lck(mtx);
        closed = true;
        cv.notify_all();
        lck.unlock();
        for (std::thread& th : threads) {
            th.join();
        }
    }

private:
    void run() {
        std::unique_lock<std::mutex> lck(mtx);
        while (true) {
            if (!work.empty()) {
                std::function<void()> wi = work.front();
                work.pop_front();
                lck.unlock();
                wi();
                lck.lock();
            } else if (closed) {
                return;
            } else {
                cv.wait(lck);
            }
        }
    }

    std::deque<std::function<void()>> work;
    std::vector<std::thread> threads;
    std::mutex mtx;
    std::condition_variable cv;
    bool closed = false;
};
```

**Optional (G):** In `enqueue()`, use `cv.notify_all();` instead of `cv.notify_one();` so all waiting workers wake when new work is added.

**Summary of changes:** Unlock before calling `wi()` and lock again after it returns. Other threads can then take work and run tasks in parallel, and a task that calls back into the pool does not deadlock. Optionally notify all waiters on enqueue to reduce latency when multiple threads are idle.

---

*End of Problem 2 corrected solutions.*
