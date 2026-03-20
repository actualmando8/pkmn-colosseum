stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r24, 0x10(r1)
mr r30, r3
mr r31, r4
li r0, -0x1
li r3, 0x0
cmpwi r3, 0x9
bge @80093FB0
stb r0, 0x4(r31)
stb r0, 0x8(r31)
stb r0, 0xc(r31)
stb r0, 0x10(r31)
stb r0, 0x14(r31)
stb r0, 0x18(r31)
stb r0, 0x1c(r31)
stb r0, 0x20(r31)
stb r0, 0x24(r31)
@80093FB0
li r3, 0x0
cmpwi r3, 0x9
bge @80093FE0
stb r0, 0x5(r31)
stb r0, 0x9(r31)
stb r0, 0xd(r31)
stb r0, 0x11(r31)
stb r0, 0x15(r31)
stb r0, 0x19(r31)
stb r0, 0x1d(r31)
stb r0, 0x21(r31)
stb r0, 0x25(r31)
@80093FE0
li r3, 0x0
cmpwi r3, 0x9
bge @80094010
stb r0, 0x6(r31)
stb r0, 0xa(r31)
stb r0, 0xe(r31)
stb r0, 0x12(r31)
stb r0, 0x16(r31)
stb r0, 0x1a(r31)
stb r0, 0x1e(r31)
stb r0, 0x22(r31)
stb r0, 0x26(r31)
@80094010
li r3, 0x0
cmpwi r3, 0x9
bge @80094040
stb r0, 0x7(r31)
stb r0, 0xb(r31)
stb r0, 0xf(r31)
stb r0, 0x13(r31)
stb r0, 0x17(r31)
stb r0, 0x1b(r31)
stb r0, 0x1f(r31)
stb r0, 0x23(r31)
stb r0, 0x27(r31)
@80094040
lis r3, lbl_802EEFD8@ha
li r28, 0x0
addi r29, r3, lbl_802EEFD8@l
li r27, 0x0
@80094050
lhz r5, 0x0(r29)
mr r3, r30
li r4, 0x0
li r6, 0x0
bl fn_8012640C
lbz r0, 0x3(r29)
cmpw r3, r0
ble @80094074
mr r3, r0
@80094074
cmpwi r3, 0x0
li r5, 0x0
ble @8009437C
cmpwi r3, 0x8
subi r8, r3, 0x8
ble @80094310
addi r4, r29, 0x2
addi r6, r8, 0x7
lis r7, 0x38e4
srwi r6, r6, 3
subi r0, r7, 0x71c7
mtctr r6
cmpwi r8, 0x0
ble @80094310
@800940AC
mulhw r7, r0, r28
addi r12, r28, 0x1
lbz r6, 0x0(r4)
addi r11, r28, 0x2
addi r26, r28, 0x3
extsb r6, r6
srawi r8, r7, 1
add r6, r5, r6
srwi r9, r8, 31
srawi r7, r7, 1
add r9, r8, r9
addi r8, r28, 0x4
mulli r10, r9, 0x9
srwi r9, r7, 31
extsb r25, r6
add r24, r7, r9
mulhw r7, r0, r12
subf r6, r10, r28
slwi r9, r6, 2
srawi r6, r7, 1
add r9, r9, r24
addi r10, r9, 0x4
srawi r7, r7, 1
srwi r9, r6, 31
stbx r25, r31, r10
add r6, r6, r9
srwi r9, r7, 31
mulli r10, r6, 0x9
lbz r6, 0x0(r4)
add r25, r7, r9
extsb r7, r6
mulhw r6, r0, r11
subf r10, r10, r12
add r9, r5, r7
slwi r7, r10, 2
addi r12, r9, 0x1
srawi r9, r6, 1
add r7, r7, r25
srawi r6, r6, 1
srwi r10, r9, 31
extsb r25, r12
addi r12, r7, 0x4
srwi r7, r6, 31
stbx r25, r31, r12
add r9, r9, r10
mulli r10, r9, 0x9
add r12, r6, r7
lbz r6, 0x0(r4)
mulhw r9, r0, r26
extsb r6, r6
subf r7, r10, r11
add r6, r5, r6
slwi r7, r7, 2
addi r11, r6, 0x2
srawi r6, r9, 1
add r10, r7, r12
srwi r7, r6, 31
srawi r12, r9, 1
mulhw r9, r0, r8
extsb r11, r11
addi r10, r10, 0x4
stbx r11, r31, r10
add r6, r6, r7
srwi r25, r12, 31
mulli r11, r6, 0x9
lbz r7, 0x0(r4)
srawi r6, r9, 1
extsb r10, r7
subf r24, r11, r26
srwi r7, r6, 31
add r11, r5, r10
add r12, r12, r25
slwi r10, r24, 2
add r6, r6, r7
addi r11, r11, 0x3
add r7, r10, r12
mulli r6, r6, 0x9
extsb r10, r11
addi r7, r7, 0x4
stbx r10, r31, r7
subf r11, r6, r8
addi r25, r28, 0x5
lbz r6, 0x0(r4)
mulhw r8, r0, r25
srawi r7, r9, 1
extsb r6, r6
srwi r10, r7, 31
add r9, r5, r6
add r12, r7, r10
srawi r6, r8, 1
slwi r7, r11, 2
addi r10, r9, 0x4
addi r11, r28, 0x6
add r9, r7, r12
srwi r7, r6, 31
add r6, r6, r7
extsb r10, r10
addi r7, r9, 0x4
srawi r8, r8, 1
stbx r10, r31, r7
mulli r12, r6, 0x9
srwi r10, r8, 31
lbz r7, 0x0(r4)
addi r9, r28, 0x7
mulhw r6, r0, r11
add r26, r8, r10
subf r10, r12, r25
extsb r8, r7
slwi r7, r10, 2
add r10, r5, r8
srawi r8, r6, 1
addi r12, r10, 0x5
add r7, r7, r26
srawi r6, r6, 1
srwi r10, r8, 31
extsb r25, r12
addi r12, r7, 0x4
srwi r7, r6, 31
stbx r25, r31, r12
add r8, r8, r10
mulli r8, r8, 0x9
add r10, r6, r7
lbz r7, 0x0(r4)
addi r28, r28, 0x8
mulhw r6, r0, r9
extsb r7, r7
subf r8, r8, r11
add r7, r5, r7
slwi r8, r8, 2
addi r11, r7, 0x6
srawi r7, r6, 1
add r10, r8, r10
srwi r8, r7, 31
srawi r6, r6, 1
add r8, r7, r8
extsb r11, r11
addi r10, r10, 0x4
srwi r7, r6, 31
stbx r11, r31, r10
mulli r8, r8, 0x9
add r6, r6, r7
lbz r10, 0x0(r4)
subf r7, r8, r9
extsb r8, r10
add r8, r5, r8
slwi r7, r7, 2
addi r8, r8, 0x7
addi r5, r5, 0x8
add r6, r7, r6
extsb r7, r8
addi r6, r6, 0x4
stbx r7, r31, r6
bdnz @800940AC
@80094310
addi r8, r29, 0x2
lis r4, 0x38e4
subf r0, r5, r3
subi r6, r4, 0x71c7
mtctr r0
cmpw r5, r3
bge @8009437C
@8009432C
mulhw r0, r6, r28
lbz r3, 0x0(r8)
extsb r3, r3
add r7, r5, r3
srawi r3, r0, 1
addi r5, r5, 0x1
srwi r4, r3, 31
srawi r0, r0, 1
add r3, r3, r4
extsb r7, r7
mulli r4, r3, 0x9
srwi r3, r0, 31
add r0, r0, r3
subf r3, r4, r28
addi r28, r28, 0x1
slwi r3, r3, 2
add r3, r3, r0
addi r0, r3, 0x4
stbx r7, r31, r0
bdnz @8009432C
@8009437C
addi r29, r29, 0x4
addi r27, r27, 0x1
cmplwi r27, 0xa
blt @80094050
lis r3, lbl_802EF000@ha
li r28, 0x0
addi r29, r3, lbl_802EF000@l
mr r27, r28
@8009439C
lhz r5, 0x0(r29)
mr r3, r30
li r4, 0x0
li r6, 0x0
bl fn_8012640C
lbz r0, 0x3(r29)
cmpw r3, r0
ble @800943C0
mr r3, r0
@800943C0
cmpwi r3, 0x0
li r5, 0x0
ble @80094558
cmpwi r3, 0x8
subi r7, r3, 0x8
ble @80094518
addi r4, r29, 0x2
addi r0, r7, 0x7
slwi r6, r28, 2
srwi r0, r0, 3
mtctr r0
cmpwi r7, 0x0
ble @80094518
@800943F4
lbz r0, 0x0(r4)
addi r9, r6, 0x7
addi r8, r28, 0x1
addi r7, r28, 0x2
extsb r10, r0
addi r0, r28, 0x3
add r10, r5, r10
slwi r26, r8, 2
extsb r8, r10
slwi r12, r7, 2
stbx r8, r31, r9
slwi r11, r0, 2
addi r7, r28, 0x4
addi r0, r28, 0x5
lbz r8, 0x0(r4)
slwi r10, r7, 2
slwi r9, r0, 2
addi r7, r28, 0x6
extsb r8, r8
addi r0, r28, 0x7
add r25, r5, r8
slwi r8, r7, 2
addi r25, r25, 0x1
slwi r7, r0, 2
extsb r25, r25
addi r0, r26, 0x7
stbx r25, r31, r0
addi r12, r12, 0x7
addi r11, r11, 0x7
addi r10, r10, 0x7
lbz r26, 0x0(r4)
addi r9, r9, 0x7
addi r8, r8, 0x7
addi r0, r7, 0x7
extsb r7, r26
addi r28, r28, 0x8
add r7, r5, r7
addi r6, r6, 0x20
addi r7, r7, 0x2
extsb r7, r7
stbx r7, r31, r12
lbz r7, 0x0(r4)
extsb r7, r7
add r7, r5, r7
addi r7, r7, 0x3
extsb r7, r7
stbx r7, r31, r11
lbz r7, 0x0(r4)
extsb r7, r7
add r7, r5, r7
addi r7, r7, 0x4
extsb r7, r7
stbx r7, r31, r10
lbz r7, 0x0(r4)
extsb r7, r7
add r7, r5, r7
addi r7, r7, 0x5
extsb r7, r7
stbx r7, r31, r9
lbz r7, 0x0(r4)
extsb r7, r7
add r7, r5, r7
addi r7, r7, 0x6
extsb r7, r7
stbx r7, r31, r8
lbz r7, 0x0(r4)
extsb r7, r7
add r7, r5, r7
addi r5, r5, 0x8
addi r7, r7, 0x7
extsb r7, r7
stbx r7, r31, r0
bdnz @800943F4
@80094518
slwi r0, r28, 2
addi r6, r29, 0x2
add r4, r31, r0
subf r0, r5, r3
mtctr r0
cmpw r5, r3
bge @80094558
@80094534
lbz r0, 0x0(r6)
addi r28, r28, 0x1
extsb r0, r0
add r0, r5, r0
addi r5, r5, 0x1
extsb r0, r0
stb r0, 0x7(r4)
addi r4, r4, 0x4
bdnz @80094534
@80094558
addi r29, r29, 0x4
addi r27, r27, 0x1
cmplwi r27, 0x7
blt @8009439C
li r5, 0x0
mr r4, r5
li r0, 0x4
mtctr r0
@80094578
add r3, r31, r4
lbz r0, 0x4(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @80094590
addi r5, r5, 0x1
@80094590
lbz r0, 0x8(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @800945A4
addi r5, r5, 0x1
@800945A4
lbz r0, 0xc(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @800945B8
addi r5, r5, 0x1
@800945B8
lbz r0, 0x10(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @800945CC
addi r5, r5, 0x1
@800945CC
lbz r0, 0x14(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @800945E0
addi r5, r5, 0x1
@800945E0
lbz r0, 0x18(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @800945F4
addi r5, r5, 0x1
@800945F4
lbz r0, 0x1c(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @80094608
addi r5, r5, 0x1
@80094608
lbz r0, 0x20(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @8009461C
addi r5, r5, 0x1
@8009461C
lbz r0, 0x24(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @80094630
addi r5, r5, 0x1
@80094630
addi r4, r4, 0x1
bdnz @80094578
stw r5, 0x0(r31)
lmw r24, 0x10(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
