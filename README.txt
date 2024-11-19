
fun f2(a: i32, b: i32): i32 {
  for (i in 0..100) {
    a += b + 10;
  }
  print("result:{}", a)
  return a
}

fun f1(a: i32, b: i32): i32 {
  a += b;
  return f2(a, b)
}

types: i - 32bit, l - 64bit, d - double, b - bool

f2:
  .args 3
  .locals 2
  .consts 10, "result:{}", 1000
  mov 1 0
f2.l1:
  addi 2 1 3 
  addi -2 -2 2
  inci 1
  jlei f2.l1 1 5
  reti -2

f1:
  .args 3
  .locals
  addrr -2 -2 -1




//////////////////



class C {
  constructor(int a, string s) {
    this.a = a;
    this.s = s;
  }

  a: int
  s: string
}

fun test(c: C) {
  c = C()
  c.a = 1;
  c.s = "tetew"
}


test(LC;):
  .references "C.constructor(IS)", "C.a", "C.b"
  .consts 1, "tetew"
  .locals 0
  .args 1
  new -1 0 # 1 - c, 0 -> ref "C.constructor(IS)"
  putfieldi -1 1 0 # -1 - c, 1 - "C.a", 0 - const 1
  putfieldi -1 2 1




  
  


  

