stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lwz r0, 0x0(r4)
lis r4, lbl_80268DC0@ha
addi r4, r4, lbl_80268DC0@l
cmplwi r0, 0x4a
bgt @80082624
lis r9, jumptable_802EEA4C@ha
slwi r0, r0, 2
addi r9, r9, jumptable_802EEA4C@l
lwzx r0, r9, r0
mtctr r0
bctr
lwz r4, 0x0(r3)
stw r6, 0x0(r4)
lwz r3, 0x0(r3)
lwz r0, 0x0(r3)
cmpwi r0, 0x2
bge @80081084
cmpwi r0, 0x0
bge @8008263C
@80081084
li r3, 0x0
b @80082640
lwz r4, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x4(r4)
lwz r3, 0x0(r3)
lbz r0, 0x4(r3)
cmpwi r0, 0x0
beq @800810B8
blt @800810B8
cmpwi r0, 0x6
bge @800810B8
b @8008263C
@800810B8
li r3, 0x0
b @80082640
lwz r4, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x5(r4)
lwz r3, 0x0(r3)
lbz r0, 0x5(r3)
cmpwi r0, 0x0
beq @800810EC
blt @800810EC
cmpwi r0, 0x4
bge @800810EC
b @8008263C
@800810EC
li r3, 0x0
b @80082640
lwz r4, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x6(r4)
lwz r3, 0x0(r3)
lbz r0, 0x6(r3)
cmpwi r0, 0x0
beq @80081120
blt @80081120
cmpwi r0, 0xa
bge @80081120
b @8008263C
@80081120
li r3, 0x0
b @80082640
lwz r4, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x7(r4)
lwz r3, 0x0(r3)
lbz r0, 0x7(r3)
cmpwi r0, 0x0
beq @80081150
blt @8008263C
cmpwi r0, 0xc
b @8008263C
@80081150
li r3, 0x0
b @80082640
lwz r3, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x8(r3)
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0xa
bl fn_800F9E70
b @8008263C
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x24(r3)
blt @8008119C
cmpwi r6, 0x6
blt @8008263C
@8008119C
li r3, 0x0
b @80082640
lwz r3, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x25(r3)
b @8008263C
subi r0, r6, 0x1
lwz r4, 0x0(r3)
extsb r0, r0
stb r0, 0x26(r4)
lwz r3, 0x0(r3)
lbz r0, 0x26(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @800811E0
cmpwi r0, 0x5
blt @8008263C
@800811E0
li r3, 0x0
b @80082640
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x28
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x38
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x48
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
extsb r0, r6
cmpwi r6, 0x1
stb r0, 0x58(r3)
blt @80081240
cmpwi r6, 0x3
ble @8008263C
@80081240
li r3, 0x0
b @80082640
lwz r3, 0x0(r3)
extsb r0, r6
cmpwi r6, 0x1
stb r0, 0x59(r3)
blt @80081264
cmpwi r6, 0x6
ble @8008263C
@80081264
li r3, 0x0
b @80082640
lwz r3, 0x0(r3)
extsb r0, r6
cmpwi r6, 0x1
stb r0, 0x5a(r3)
blt @80081288
cmpwi r6, 0x5
ble @8008263C
@80081288
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x5b(r3)
blt @800812B0
cmpwi r6, 0x9
ble @8008263C
@800812B0
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x5c(r3)
blt @800812D8
cmpwi r6, 0x9
ble @8008263C
@800812D8
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x5d(r3)
blt @80081300
cmpwi r6, 0x9
ble @8008263C
@80081300
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x5e(r3)
blt @80081328
cmpwi r6, 0x9
ble @8008263C
@80081328
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x5f(r3)
blt @80081350
cmpwi r6, 0x9
ble @8008263C
@80081350
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x60(r3)
blt @80081378
cmpwi r6, 0x9
ble @8008263C
@80081378
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x61(r3)
blt @800813A0
cmpwi r6, 0x9
ble @8008263C
@800813A0
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x62(r3)
blt @800813C8
cmpwi r6, 0x9
ble @8008263C
@800813C8
li r3, 0x0
b @80082640
subi r0, r6, 0x1
lwz r3, 0x0(r3)
extsb r0, r0
cmpwi r6, 0x0
stb r0, 0x63(r3)
blt @800813F0
cmpwi r6, 0x9
ble @8008263C
@800813F0
li r3, 0x0
b @80082640
lwz r5, 0x0(r3)
clrlwi r0, r6, 16
addi r6, r4, 0x384
li r4, 0x0
sth r0, 0x64(r5)
lwz r3, 0x0(r3)
lhz r3, 0x64(r3)
li r0, 0x2f
mtctr r0
@8008141C
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081430
li r0, 0x1
b @800814D0
@80081430
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081448
li r0, 0x1
b @800814D0
@80081448
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081460
li r0, 0x1
b @800814D0
@80081460
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081478
li r0, 0x1
b @800814D0
@80081478
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081490
li r0, 0x1
b @800814D0
@80081490
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @800814A8
li r0, 0x1
b @800814D0
@800814A8
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @800814C0
li r0, 0x1
b @800814D0
@800814C0
addi r6, r6, 0x2
addi r4, r4, 0x6
bdnz @8008141C
li r0, 0x0
@800814D0
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
lwz r5, 0x0(r3)
clrlwi r0, r6, 16
addi r6, r4, 0x384
li r4, 0x0
sth r0, 0x66(r5)
lwz r3, 0x0(r3)
lhz r3, 0x66(r3)
li r0, 0x2f
mtctr r0
@80081508
lhz r0, 0x0(r6)
cmplw r3, r0
bne @8008151C
li r0, 0x1
b @800815BC
@8008151C
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081534
li r0, 0x1
b @800815BC
@80081534
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @8008154C
li r0, 0x1
b @800815BC
@8008154C
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081564
li r0, 0x1
b @800815BC
@80081564
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @8008157C
li r0, 0x1
b @800815BC
@8008157C
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081594
li r0, 0x1
b @800815BC
@80081594
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @800815AC
li r0, 0x1
b @800815BC
@800815AC
addi r6, r6, 0x2
addi r4, r4, 0x6
bdnz @80081508
li r0, 0x0
@800815BC
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
lwz r5, 0x0(r3)
clrlwi r0, r6, 16
addi r6, r4, 0x384
li r4, 0x0
sth r0, 0x68(r5)
lwz r3, 0x0(r3)
lhz r3, 0x68(r3)
li r0, 0x2f
mtctr r0
@800815F4
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081608
li r0, 0x1
b @800816A8
@80081608
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081620
li r0, 0x1
b @800816A8
@80081620
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081638
li r0, 0x1
b @800816A8
@80081638
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081650
li r0, 0x1
b @800816A8
@80081650
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081668
li r0, 0x1
b @800816A8
@80081668
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081680
li r0, 0x1
b @800816A8
@80081680
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r3, r0
bne @80081698
li r0, 0x1
b @800816A8
@80081698
addi r6, r6, 0x2
addi r4, r4, 0x6
bdnz @800815F4
li r0, 0x0
@800816A8
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
lwz r4, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x6a(r4)
lwz r3, 0x0(r3)
lbz r0, 0x6a(r3)
cmplwi r0, 0x24
bgt @800816F8
lis r3, jumptable_802EE9B8@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EE9B8@l
lwzx r0, r3, r0
mtctr r0
bctr
li r0, 0x1
b @800816FC
@800816F8
li r0, 0x0
@800816FC
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
lwz r4, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x6b(r4)
lwz r3, 0x0(r3)
lbz r0, 0x6b(r3)
cmplwi r0, 0x24
bgt @8008174C
lis r3, jumptable_802EE924@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EE924@l
lwzx r0, r3, r0
mtctr r0
bctr
li r0, 0x1
b @80081750
@8008174C
li r0, 0x0
@80081750
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
lwz r4, 0x0(r3)
clrlwi r0, r6, 24
stb r0, 0x6c(r4)
lwz r3, 0x0(r3)
lbz r0, 0x6c(r3)
cmplwi r0, 0x24
bgt @800817A0
lis r3, jumptable_802EE890@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EE890@l
lwzx r0, r3, r0
mtctr r0
bctr
li r0, 0x1
b @800817A4
@800817A0
li r0, 0x0
@800817A4
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x6e
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x182
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x296
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0xca
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x1de
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x2f2
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x126
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x23a
bl fn_800F9E70
b @8008263C
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0x34e
bl fn_800F9E70
b @8008263C
mulli r5, r5, 0x28
lwz r0, 0x0(r3)
mr r4, r7
addi r3, r5, 0x3ac
add r3, r0, r3
bl fn_800F9E70
b @8008263C
cmpwi r6, 0x1
beq @800818B8
bge @800818D0
cmpwi r6, 0x0
bge @800818A0
b @800818D0
@800818A0
mulli r0, r5, 0x28
lwz r3, 0x0(r3)
li r4, 0x1
add r3, r3, r0
stb r4, 0x3b8(r3)
b @8008263C
@800818B8
mulli r0, r5, 0x28
lwz r3, 0x0(r3)
li r4, 0x0
add r3, r3, r0
stb r4, 0x3b8(r3)
b @8008263C
@800818D0
mulli r0, r5, 0x28
lwz r4, 0x0(r3)
li r5, 0x0
li r3, 0x0
add r4, r4, r0
stb r5, 0x3b8(r4)
b @80082640
mulli r0, r5, 0x28
lwz r3, 0x0(r3)
subi r4, r6, 0x1
cmpwi r6, 0x0
add r0, r3, r0
extsb r4, r4
add r3, r0, r8
stb r4, 0x3b9(r3)
blt @80081918
cmpwi r6, 0x24
ble @8008263C
@80081918
li r3, 0x0
b @80082640
mulli r5, r5, 0x28
lwz r3, 0x0(r3)
slwi r0, r8, 1
clrlwi r6, r6, 16
add r3, r3, r5
addi r5, r4, 0x384
add r3, r3, r0
li r4, 0x0
sth r6, 0x3be(r3)
li r0, 0x2f
mtctr r0
@8008194C
lhz r0, 0x0(r5)
cmplw r6, r0
bne @80081960
li r0, 0x1
b @80081A00
@80081960
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r6, r0
bne @80081978
li r0, 0x1
b @80081A00
@80081978
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r6, r0
bne @80081990
li r0, 0x1
b @80081A00
@80081990
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r6, r0
bne @800819A8
li r0, 0x1
b @80081A00
@800819A8
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r6, r0
bne @800819C0
li r0, 0x1
b @80081A00
@800819C0
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r6, r0
bne @800819D8
li r0, 0x1
b @80081A00
@800819D8
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r6, r0
bne @800819F0
li r0, 0x1
b @80081A00
@800819F0
addi r5, r5, 0x2
addi r4, r4, 0x6
bdnz @8008194C
li r0, 0x0
@80081A00
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
mulli r0, r5, 0x28
lwz r3, 0x0(r3)
add r3, r3, r0
stw r6, 0x3c8(r3)
b @8008263C
mulli r0, r5, 0x28
lwz r3, 0x0(r3)
clrlwi r5, r6, 16
addi r6, r4, 0x618
add r3, r3, r0
li r4, 0x0
sth r5, 0x3cc(r3)
li r0, 0x13
mtctr r0
@80081A4C
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081A60
li r0, 0x1
b @80081AB8
@80081A60
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081A78
li r0, 0x1
b @80081AB8
@80081A78
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081A90
li r0, 0x1
b @80081AB8
@80081A90
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081AA8
li r0, 0x1
b @80081AB8
@80081AA8
addi r6, r6, 0x2
addi r4, r4, 0x3
bdnz @80081A4C
li r0, 0x0
@80081AB8
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
mulli r0, r5, 0x28
lwz r4, 0x0(r3)
clrlwi r7, r6, 16
add r4, r4, r0
sth r7, 0x3ce(r4)
lwz r3, 0x0(r3)
lbz r0, 0x5b(r3)
extsb r0, r0
cmpw r0, r5
bne @80081AFC
li r0, 0x1
b @80081B30
@80081AFC
lbz r0, 0x5c(r3)
extsb r0, r0
cmpw r0, r5
bne @80081B14
li r0, 0x1
b @80081B30
@80081B14
lbz r0, 0x5d(r3)
extsb r0, r0
cmpw r0, r5
bne @80081B2C
li r0, 0x1
b @80081B30
@80081B2C
li r0, 0x0
@80081B30
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @8008263C
clrlwi r0, r6, 16
cmplwi r0, 0x3e7
ble @8008263C
li r3, 0x0
b @80082640
mulli r0, r5, 0x28
lwz r3, 0x0(r3)
clrlwi r5, r6, 24
addi r6, r4, 0x6b0
add r3, r3, r0
li r4, 0x0
stb r5, 0x3d0(r3)
li r0, 0x4
mtctr r0
@80081B74
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081B88
li r0, 0x1
b @80081C40
@80081B88
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081BA0
li r0, 0x1
b @80081C40
@80081BA0
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081BB8
li r0, 0x1
b @80081C40
@80081BB8
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081BD0
li r0, 0x1
b @80081C40
@80081BD0
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081BE8
li r0, 0x1
b @80081C40
@80081BE8
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081C00
li r0, 0x1
b @80081C40
@80081C00
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081C18
li r0, 0x1
b @80081C40
@80081C18
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081C30
li r0, 0x1
b @80081C40
@80081C30
addi r6, r6, 0x2
addi r4, r4, 0x7
bdnz @80081B74
li r0, 0x0
@80081C40
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
clrlwi r5, r6, 16
addi r7, r4, 0x6f0
add r4, r3, r0
clrlwi r3, r6, 24
sth r5, 0x514(r4)
li r4, 0x0
li r0, 0x2b
mtctr r0
@80081C7C
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081C90
li r0, 0x1
b @80081D60
@80081C90
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081CA8
li r0, 0x1
b @80081D60
@80081CA8
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081CC0
li r0, 0x1
b @80081D60
@80081CC0
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081CD8
li r0, 0x1
b @80081D60
@80081CD8
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081CF0
li r0, 0x1
b @80081D60
@80081CF0
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081D08
li r0, 0x1
b @80081D60
@80081D08
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081D20
li r0, 0x1
b @80081D60
@80081D20
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081D38
li r0, 0x1
b @80081D60
@80081D38
addi r7, r7, 0x2
lhz r0, 0x0(r7)
cmplw r3, r0
bne @80081D50
li r0, 0x1
b @80081D60
@80081D50
addi r7, r7, 0x2
addi r4, r4, 0x8
bdnz @80081C7C
li r0, 0x0
@80081D60
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
clrlwi r5, r6, 24
addi r6, r4, 0x9f8
add r3, r3, r0
li r4, 0x0
stb r5, 0x516(r3)
li r0, 0x3
mtctr r0
@80081D98
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081DAC
li r0, 0x1
b @80081E94
@80081DAC
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081DC4
li r0, 0x1
b @80081E94
@80081DC4
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081DDC
li r0, 0x1
b @80081E94
@80081DDC
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081DF4
li r0, 0x1
b @80081E94
@80081DF4
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081E0C
li r0, 0x1
b @80081E94
@80081E0C
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081E24
li r0, 0x1
b @80081E94
@80081E24
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081E3C
li r0, 0x1
b @80081E94
@80081E3C
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081E54
li r0, 0x1
b @80081E94
@80081E54
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081E6C
li r0, 0x1
b @80081E94
@80081E6C
addi r6, r6, 0x1
lbz r0, 0x0(r6)
cmplw r0, r5
bne @80081E84
li r0, 0x1
b @80081E94
@80081E84
addi r6, r6, 0x1
addi r4, r4, 0x9
bdnz @80081D98
li r0, 0x0
@80081E94
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
clrlwi r4, r6, 24
add r3, r3, r0
stb r4, 0x517(r3)
b @8008263C
mulli r5, r5, 0x2a
lwz r3, 0x0(r3)
slwi r0, r8, 1
clrlwi r7, r6, 16
add r3, r3, r5
addi r5, r4, 0xa18
add r4, r3, r0
clrlwi r3, r6, 24
sth r7, 0x518(r4)
li r4, 0x0
li r0, 0x47
mtctr r0
@80081EF0
lhz r0, 0x0(r5)
cmplw r3, r0
bne @80081F04
li r0, 0x1
b @80081F74
@80081F04
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r3, r0
bne @80081F1C
li r0, 0x1
b @80081F74
@80081F1C
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r3, r0
bne @80081F34
li r0, 0x1
b @80081F74
@80081F34
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r3, r0
bne @80081F4C
li r0, 0x1
b @80081F74
@80081F4C
addi r5, r5, 0x2
lhz r0, 0x0(r5)
cmplw r3, r0
bne @80081F64
li r0, 0x1
b @80081F74
@80081F64
addi r5, r5, 0x2
addi r4, r4, 0x4
bdnz @80081EF0
li r0, 0x0
@80081F74
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
clrlwi r5, r6, 16
addi r6, r4, 0x384
add r3, r3, r0
li r4, 0x0
sth r5, 0x520(r3)
li r0, 0x2f
mtctr r0
@80081FAC
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081FC0
li r0, 0x1
b @80082060
@80081FC0
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081FD8
li r0, 0x1
b @80082060
@80081FD8
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80081FF0
li r0, 0x1
b @80082060
@80081FF0
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80082008
li r0, 0x1
b @80082060
@80082008
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80082020
li r0, 0x1
b @80082060
@80082020
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80082038
li r0, 0x1
b @80082060
@80082038
addi r6, r6, 0x2
lhz r0, 0x0(r6)
cmplw r5, r0
bne @80082050
li r0, 0x1
b @80082060
@80082050
addi r6, r6, 0x2
addi r4, r4, 0x6
bdnz @80081FAC
li r0, 0x0
@80082060
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
cmpwi r6, 0x2
bge @800820A0
cmpwi r6, 0x0
bge @80082088
b @800820A0
@80082088
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
extsb r4, r6
add r3, r3, r0
stb r4, 0x522(r3)
b @8008263C
@800820A0
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
li r4, -0x1
add r3, r3, r0
stb r4, 0x522(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @800820D0
cmpwi r6, 0x1f
bgt @800820D0
li r0, 0x1
@800820D0
cmpwi r0, 0x0
beq @800820E0
extsb r4, r6
b @800820E4
@800820E0
li r4, -0x1
@800820E4
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
stb r4, 0x523(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082110
cmpwi r6, 0x1f
bgt @80082110
li r0, 0x1
@80082110
cmpwi r0, 0x0
beq @80082120
extsb r4, r6
b @80082124
@80082120
li r4, -0x1
@80082124
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
stb r4, 0x524(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082150
cmpwi r6, 0x1f
bgt @80082150
li r0, 0x1
@80082150
cmpwi r0, 0x0
beq @80082160
extsb r4, r6
b @80082164
@80082160
li r4, -0x1
@80082164
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
stb r4, 0x525(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082190
cmpwi r6, 0x1f
bgt @80082190
li r0, 0x1
@80082190
cmpwi r0, 0x0
beq @800821A0
extsb r4, r6
b @800821A4
@800821A0
li r4, -0x1
@800821A4
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
stb r4, 0x526(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @800821D0
cmpwi r6, 0x1f
bgt @800821D0
li r0, 0x1
@800821D0
cmpwi r0, 0x0
beq @800821E0
extsb r4, r6
b @800821E4
@800821E0
li r4, -0x1
@800821E4
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
stb r4, 0x527(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082210
cmpwi r6, 0x1f
bgt @80082210
li r0, 0x1
@80082210
cmpwi r0, 0x0
beq @80082220
extsb r4, r6
b @80082224
@80082220
li r4, -0x1
@80082224
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
stb r4, 0x528(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082250
cmpwi r6, 0xff
bgt @80082250
li r0, 0x1
@80082250
cmpwi r0, 0x0
beq @80082260
extsh r4, r6
b @80082264
@80082260
li r4, -0x1
@80082264
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
sth r4, 0x52a(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082290
cmpwi r6, 0xff
bgt @80082290
li r0, 0x1
@80082290
cmpwi r0, 0x0
beq @800822A0
extsh r4, r6
b @800822A4
@800822A0
li r4, -0x1
@800822A4
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
sth r4, 0x52c(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @800822D0
cmpwi r6, 0xff
bgt @800822D0
li r0, 0x1
@800822D0
cmpwi r0, 0x0
beq @800822E0
extsh r4, r6
b @800822E4
@800822E0
li r4, -0x1
@800822E4
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
sth r4, 0x52e(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082310
cmpwi r6, 0xff
bgt @80082310
li r0, 0x1
@80082310
cmpwi r0, 0x0
beq @80082320
extsh r4, r6
b @80082324
@80082320
li r4, -0x1
@80082324
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
sth r4, 0x530(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082350
cmpwi r6, 0xff
bgt @80082350
li r0, 0x1
@80082350
cmpwi r0, 0x0
beq @80082360
extsh r4, r6
b @80082364
@80082360
li r4, -0x1
@80082364
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
sth r4, 0x532(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @80082390
cmpwi r6, 0xff
bgt @80082390
li r0, 0x1
@80082390
cmpwi r0, 0x0
beq @800823A0
extsh r4, r6
b @800823A4
@800823A0
li r4, -0x1
@800823A4
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
sth r4, 0x534(r3)
b @8008263C
cmpwi r6, 0x0
li r0, 0x0
blt @800823D0
cmpwi r6, 0xff
bgt @800823D0
li r0, 0x1
@800823D0
cmpwi r0, 0x0
beq @800823E0
extsh r4, r6
b @800823E4
@800823E0
li r4, -0x1
@800823E4
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
add r3, r3, r0
sth r4, 0x536(r3)
b @8008263C
cmpwi r6, 0x2
beq @80082450
bge @80082414
cmpwi r6, 0x0
beq @80082420
bge @80082438
b @80082420
@80082414
cmpwi r6, 0x4
bge @80082420
b @80082468
@80082420
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
li r4, 0x0
add r3, r3, r0
stb r4, 0x538(r3)
b @8008263C
@80082438
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
li r4, 0x0
add r3, r3, r0
stb r4, 0x538(r3)
b @8008263C
@80082450
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
li r4, 0x1
add r3, r3, r0
stb r4, 0x538(r3)
b @8008263C
@80082468
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
li r4, 0x2
add r3, r3, r0
stb r4, 0x538(r3)
b @8008263C
rlwinm r0, r6, 0, 26, 26
cmpwi r0, 0x0
beq @800824A4
mulli r0, r5, 0x2a
lwz r6, 0x0(r3)
li r7, -0x1
add r6, r6, r0
stb r7, 0x539(r6)
b @800824BC
@800824A4
mulli r0, r5, 0x2a
lwz r7, 0x0(r3)
subi r6, r6, 0x1
extsb r8, r6
add r6, r7, r0
stb r8, 0x539(r6)
@800824BC
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
addi r5, r4, 0xce0
li r4, 0x0
add r3, r3, r0
lbz r3, 0x539(r3)
li r0, 0xd
extsb r3, r3
mtctr r0
@800824E0
lbz r0, 0x0(r5)
extsb r0, r0
cmpw r0, r3
bne @800824F8
li r0, 0x1
b @80082524
@800824F8
addi r5, r5, 0x1
lbz r0, 0x0(r5)
extsb r0, r0
cmpw r0, r3
bne @80082514
li r0, 0x1
b @80082524
@80082514
addi r5, r5, 0x1
addi r4, r4, 0x1
bdnz @800824E0
li r0, 0x0
@80082524
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
mulli r7, r5, 0x2a
lwz r0, 0x0(r3)
clrlwi r5, r6, 24
add r4, r0, r7
stb r5, 0x53a(r4)
lwz r0, 0x0(r3)
add r3, r0, r7
lbz r0, 0x53a(r3)
cmpwi r0, 0x4
bge @80082568
cmpwi r0, 0x0
bge @8008263C
@80082568
li r3, 0x0
b @80082640
mulli r7, r5, 0x2a
lwz r0, 0x0(r3)
clrlwi r5, r6, 24
add r4, r0, r7
stb r5, 0x53b(r4)
lwz r0, 0x0(r3)
add r4, r0, r7
lbz r0, 0x53b(r4)
cmpwi r0, 0x5
bge @800825A0
cmpwi r0, 0x0
bge @8008263C
@800825A0
li r0, 0x0
li r3, 0x0
stb r0, 0x53b(r4)
b @80082640
mulli r0, r5, 0x2a
lwz r3, 0x0(r3)
clrlwi r4, r6, 24
add r3, r3, r0
stb r4, 0x53c(r3)
b @8008263C
lwz r3, 0x0(r3)
addi r4, r4, 0xcfc
stw r6, 0xafc(r3)
li r0, 0x2b
mtctr r0
@800825DC
lwz r0, 0x0(r4)
cmplw r6, r0
bne @800825F0
li r0, 0x1
b @800825FC
@800825F0
addi r4, r4, 0x4
bdnz @800825DC
li r0, 0x0
@800825FC
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8008263C
li r3, 0x0
b @80082640
lwz r3, 0x0(r3)
mr r4, r7
addi r3, r3, 0xb00
bl fn_800F9E70
b @8008263C
@80082624
addi r3, r4, 0x63e8
li r4, 0x8c5
li r5, lbl_8047C178@sda21
bl fn_80196E10
li r3, 0x0
b @80082640
@8008263C
li r3, 0x1
@80082640
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
