# tichy-strings
An implementation of Tichy's string-to-string correction algorithm[1]

Compile
```bash
mkdir build && cd $_
cmake .. && make -j4
```

Encode the difference between English and Scots versions of [the UDHR](https://www.ohchr.org/EN/UDHR/Pages/SearchByLang.aspx) as a Vcdiff[2] delta file:
```bash
./tichydiff/tichydiff --source ../example/udhr_english.txt --template ../example/udhr_scots.txt --output udhr.diff
```

This file can be decoded, for example, with [open-vcdiff](https://github.com/google/open-vcdiff/tree/master)
```bash
vcdiff decode -delta udhr.diff -dictionary ../example/udhr_english.txt -target scots.txt
diff scots.txt ../example/udhr_scots.txt
echo $?
0
```

Run unit-tests
```bash
./tests/test_tichy
```

Check memory leaks
```bash
CK_FORK=no valgrind --leak-check=full ./tests/test_tichy 
```

#### References

* [1] W. Tichy, The String-to-String Correction Problem with Block
        Moves, ACM Transactions on Computer Systems, 2(4):309-321,
        November 1984.

* [2] Korn, D., MacDonald, J., Mogul, J., and K. Vo, "The VCDIFF Generic Differencing and Compression Data Format", RFC 3284, DOI 10.17487/RFC3284, June 2002, <https://www.rfc-editor.org/info/rfc3284>.
