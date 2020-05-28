# Vale in Midas

## Building and Running Vale (OSX)

get llvm 7:

```
$ brew install --with-toolchain llvm@7
```

generate the build files and use it to build conec:

```
$ cmake .
$ make
```

foo.vale contents:

```
fn main() {
   3 + 4
}
```

run Driver3.jar and capture the Vale IR json:

```
$ java -cp Driver3.jar net.verdagon.vale.driver.Driver run foo.vale | sed -nE -e "/^\{/,$ p" > foo.vir
```

run conec on it to generate an object file:

```
$ ./conec foo.vir
```

run clang on the object file to compile it down to an executable:

```
$ clang foo.o
```

run a.out:

```
$ ./a.out
$ echo $?
7
```

