
/// Tests template instantiation with dependency on template class declared later

template <typename T>
void foo(T x) {
}

template <typename T>
struct str {};

void bar() {
    foo<str<int>>(str<int>{});
}
