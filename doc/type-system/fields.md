# Fields

In ++C, structs may have only field members. Fields contain data of arbitrary type (except a type that references the struct itself). A field can be const (the memory behind the field might still be settable, but the field itself cannot be assigned to). ++C makes no guarantees for the order of fields or their alignment.

An example structure with fields:

```c++
export struct my_struct_t {
    number: float;
    integer: int;
    array: char[];
}
```
