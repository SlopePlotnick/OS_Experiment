def produce():
    print("PRODUCE")

    print("produce 0")
    yield 0

    print("produce 1")
    yield 1

    print("produce 2")
    yield 2

    print("produce 3")
    yield 3

    print("produce 4")
    yield 4

    print("produce 5")
    yield 5

    print("produce 6")
    yield 6

    print("produce 7")
    yield 7

def consume():
    print("\tCONSUME")

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    print("END")

coro_p = produce()
consume()
