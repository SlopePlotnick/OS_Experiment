def produce():
    print("PRODUCE")

    print("produce 1")
    yield 1

    print("produce 2")
    yield 2

    print("produce 3")
    yield 3

def consume():
    print("\tCONSUME")

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    x = next(coro_p)
    print("\tconsume %d" % x)

    print("END")

coro_p = produce()
consume()
