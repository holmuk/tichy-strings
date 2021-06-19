# tichy-strings
An implementation of Tichy's string-to-string correction algorithm

Compile
```bash
mkdir build && cd $_
cmake .. && make -j4
```

Run unit-tests
```bash
./tests/test_tichy
```

Check memory leaks
```bash
CK_FORK=no valgrind --leak-check=full ./tests/test_tichy 
```
