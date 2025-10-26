# CS361 Homework 4 - Thread Race 2000

# Creator

- Name: Samii Shabuse
- Drexel ID: sus24@drexel.edu

# Instructions

## To Build:
Run the following command in the project directory:
```bash

make
```

## To Run:
After compiling, run the race program with with any positive integer argument to set the number of racers:
```bash
./bin/race <num_racers>

# Example

./bin/race 3

# This start the racer with 3 racer threads and 1 game master threads.
```

## To Test Automatically:
```bash

make run

# This runs 3 race with 2, 4, and 10 racers.
```

## To Clean Up:
```bash

make clean

# Removes all compiled binaries and documentation files.
```

## To Generate Documentation:
```bash

make doc

# Creates Doxygen documentation for the project.
```


# Short Essay Questions

## Question 1: What semaphores did you use and why? (If you didn’t use any, why not?)

For this assignment I did not use explicit semaphores for this assignment. Instead, I used a combination of std::mutex and std::condition_variable to synchronize access to shared resources. The condition variable provides the same coordination behavior as a counting semaphore but allows for more flexibility and direct integration with C++ threading. Doing it it allowed me to wake up racer threads when new dice rolls are added to the queue and ensures that the queue is accessed safely. I wanted to try to challenge myself while doing this assignment and it was definitely a learning experience.

## Question 2: What locks did you use and why? (if you didn’t use any, why not?)

I used three main locks:

io_mtx: To ensure only one thread prints to std::cout at a time, preventing jumbled or overlapping output.

rank_mtx: To protects updates to shared race state like finished_count, finish_order, and finished flags, so that no two threads modify these values simultaneously.

Internal queue mutex: Which is inside the ThreadSafeQueue, it protects all push/pop operations so the game master and racer threads can safely share dice rolls.

All these locks make all shared operations atomic and prevent race conditions.

## Question 3: Why are you confident the program can never deadlock? (Explain how you programming it to avoid this)

I am confindent that the program cannot deadlock because each mutex protects a single independent resource, and no thread ever holds more than one mutex at once. With that, there are no circular dependencies or nested locking patterns that could block progress. In addition, the queue uses a condition variable with a predicate (!q.empty() || stop()), so threads always wake up safely and never wait indefinitely. Lastly, when the last racer finishes, the all_finished flag is set and notify_all() is called to release any waiting threads. These ensure that every thread either finishes or exits its loop cleanly.

## Question 4: Why are you confident the program can never let any thread starve? (Explain how you programming it to avoid this)

I am confident that the program can never let any threads starve. No thread can starve because the dice queue distributes rolls fairly in FIFO order. Every racer thread waits for rolls in the same queue and wakes up as rolls become available. To add, the game master continuously generates rolls until all racers finish, so no thread is permanently blocked waiting for input. Sleep intervals are random from intervals of 0–2 seconds for both the game master and racers. So ensuring that no thread gets deterministic priority over others. Then, when all racers finish, the queue is notified so that all waiting threads wake up and terminate gracefully. With all these together, these guaranteed that every racer eventually receives enough rolls to reach the finish line.

## Question 5: What was the most challenging part of this assignment?

The most challenging part was getting proper synchronization between threads while avoiding missed wakeups and infinite waits. I had a really hard time with balancing timing, queue behavior. The output safety required careful locking and condition variables. On top of that, the debugging part was really had. Debugging concurrency issues is tricky since they often appear randomly with the thread number as well, so ensuring clean exits and no data corruption took the most time with mutex locking. Once the locking structure was solid, the rest of the logic became straightforward, just minor fixes.