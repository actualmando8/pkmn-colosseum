stwu r1, -0x140(r1)
mflr r0
stw r0, 0x144(r1)
stw r31, 0x13c(r1)
stw r30, 0x138(r1)
stw r29, 0x134(r1)
bl fn_8025DA88
lwz r5, lbl_8047BF58@sda21(r0)
lis r4, lbl_803A9E40@ha
lwz r0, lbl_8047BF5C@sda21(r0)
mr r31, r3
stw r5, 0x8(r1)
addi r7, r4, lbl_803A9E40@l
stw r0, 0xc(r1)
li r29, 0x0
li r6, 0x1
li r5, 0x2
li r4, 0x3
li r3, 0x4
li r0, 0x5
stw r29, 0x0(r7)
addi r8, r7, 0x18
stw r6, 0x4(r7)
stw r5, 0x8(r7)
stw r4, 0xc(r7)
stw r3, 0x10(r7)
stw r0, 0x14(r7)
stw r29, 0x0(r8)
stw r6, 0x4(r8)
stw r5, 0x8(r8)
stw r4, 0xc(r8)
stw r3, 0x10(r8)
stw r0, 0x14(r8)
addi r8, r8, 0x18
stw r29, 0x0(r8)
stw r6, 0x4(r8)
stw r5, 0x8(r8)
stw r4, 0xc(r8)
stw r3, 0x10(r8)
stw r0, 0x14(r8)
addi r8, r8, 0x18
stw r29, 0x0(r8)
stw r6, 0x4(r8)
stw r5, 0x8(r8)
stw r4, 0xc(r8)
stw r3, 0x10(r8)
stw r0, 0x14(r8)
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
lwz r0, 0x4(r3)
cmpwi r0, 0x1
beq @800600EC
bge @8006010C
cmpwi r0, 0x0
bge @800600C4
b @8006010C
@800600C4
addi r30, r1, 0x8
li r29, 0x0
@800600CC
mr r3, r29
bl fn_8025D89C
sth r3, 0x0(r30)
addi r30, r30, 0x2
addi r29, r29, 0x1
cmpwi r29, 0x4
blt @800600CC
b @8006010C
@800600EC
addi r30, r1, 0x8
@800600F0
mr r3, r29
bl fn_8025D808
sth r3, 0x0(r30)
addi r30, r30, 0x2
addi r29, r29, 0x1
cmpwi r29, 0x4
blt @800600F0
@8006010C
cmpwi r31, 0x1
beq @800601C8
bge @80060124
cmpwi r31, 0x0
bge @80060130
b @80060418
@80060124
cmpwi r31, 0x3
bge @80060418
b @80060260
@80060130
lis r3, lbl_803A9E40@ha
addi r8, r1, 0x8
addi r7, r3, lbl_803A9E40@l
li r9, 0x0
lis r3, lbl_80267AF8@ha
addi r4, r3, lbl_80267AF8@l
@80060148
li r0, 0x12
addi r6, r1, 0x9c
subi r5, r4, 0x4
mtctr r0
@80060158
lwz r3, 0x4(r5)
lwzu r0, 0x8(r5)
stw r3, 0x4(r6)
stwu r0, 0x8(r6)
bdnz @80060158
lhz r3, 0x0(r8)
addi r5, r1, 0xa0
addi r8, r8, 0x2
addi r9, r9, 0x1
subi r0, r3, 0x1
mulli r0, r0, 0x18
add r5, r5, r0
lwz r3, 0x0(r5)
lwz r0, 0x4(r5)
stw r3, 0x0(r7)
lwz r3, 0x8(r5)
stw r0, 0x4(r7)
lwz r0, 0xc(r5)
stw r3, 0x8(r7)
lwz r3, 0x10(r5)
stw r0, 0xc(r7)
lwz r0, 0x14(r5)
stw r3, 0x10(r7)
stw r0, 0x14(r7)
addi r7, r7, 0x18
cmpwi r9, 0x2
blt @80060148
b @80060418
@800601C8
lis r3, lbl_803A9E40@ha
addi r8, r1, 0x8
addi r7, r3, lbl_803A9E40@l
li r9, 0x0
lis r3, lbl_80267B88@ha
addi r4, r3, lbl_80267B88@l
@800601E0
li r0, 0x12
addi r6, r1, 0xc
subi r5, r4, 0x4
mtctr r0
@800601F0
lwz r3, 0x4(r5)
lwzu r0, 0x8(r5)
stw r3, 0x4(r6)
stwu r0, 0x8(r6)
bdnz @800601F0
lhz r3, 0x0(r8)
addi r5, r1, 0x10
addi r8, r8, 0x2
addi r9, r9, 0x1
subi r0, r3, 0x1
mulli r0, r0, 0x18
add r5, r5, r0
lwz r3, 0x0(r5)
lwz r0, 0x4(r5)
stw r3, 0x0(r7)
lwz r3, 0x8(r5)
stw r0, 0x4(r7)
lwz r0, 0xc(r5)
stw r3, 0x8(r7)
lwz r3, 0x10(r5)
stw r0, 0xc(r7)
lwz r0, 0x14(r5)
stw r3, 0x10(r7)
stw r0, 0x14(r7)
addi r7, r7, 0x18
cmpwi r9, 0x2
blt @800601E0
b @80060418
@80060260
lis r3, lbl_803A9E40@ha
addi r4, r1, 0x8
addi r5, r3, lbl_803A9E40@l
li r0, 0x4
mtctr r0
@80060274
lhz r3, 0x0(r4)
li r12, 0x0
cmplwi r3, 0x0
ble @8006032C
cmplwi r3, 0x8
subi r0, r3, 0x8
ble @80060320
clrlwi r0, r0, 16
b @80060300
@80060298
clrlwi r9, r12, 16
addi r7, r12, 0x1
slwi r8, r9, 2
addi r6, r12, 0x2
add r30, r5, r8
addi r10, r12, 0x3
stw r9, 0x0(r30)
clrlwi r7, r7, 16
addi r9, r12, 0x4
addi r8, r12, 0x5
stw r7, 0x4(r30)
clrlwi r11, r6, 16
addi r7, r12, 0x6
addi r6, r12, 0x7
stw r11, 0x8(r30)
clrlwi r10, r10, 16
clrlwi r9, r9, 16
clrlwi r8, r8, 16
stw r10, 0xc(r30)
clrlwi r7, r7, 16
clrlwi r6, r6, 16
addi r12, r12, 0x8
stw r9, 0x10(r30)
stw r8, 0x14(r30)
stw r7, 0x18(r30)
stw r6, 0x1c(r30)
@80060300
clrlwi r6, r12, 16
cmplw r6, r0
blt @80060298
b @80060320
@80060310
clrlwi r6, r12, 16
addi r12, r12, 0x1
slwi r0, r6, 2
stwx r6, r5, r0
@80060320
clrlwi r0, r12, 16
cmplw r0, r3
blt @80060310
@8006032C
cmplwi r3, 0x6
mr r6, r3
bge @8006040C
subfic r0, r3, 0x6
clrlwi r0, r0, 16
cmplwi r0, 0x8
ble @80060400
b @800603D8
@8006034C
clrlwi r0, r6, 16
mr r7, r3
slwi r0, r0, 2
addi r3, r3, 0x1
clrlwi r8, r7, 16
addi r6, r6, 0x8
mr r7, r3
add r9, r5, r0
addi r3, r3, 0x1
stw r8, 0x0(r9)
mr r0, r3
clrlwi r7, r7, 16
stw r7, 0x4(r9)
addi r3, r3, 0x1
mr r7, r3
clrlwi r0, r0, 16
stw r0, 0x8(r9)
addi r3, r3, 0x1
mr r0, r3
clrlwi r7, r7, 16
stw r7, 0xc(r9)
addi r3, r3, 0x1
mr r7, r3
clrlwi r0, r0, 16
stw r0, 0x10(r9)
addi r3, r3, 0x1
mr r0, r3
clrlwi r7, r7, 16
stw r7, 0x14(r9)
clrlwi r0, r0, 16
addi r3, r3, 0x1
stw r0, 0x18(r9)
clrlwi r0, r3, 16
addi r3, r3, 0x1
stw r0, 0x1c(r9)
@800603D8
clrlwi r0, r6, 16
cmplwi r0, 0xfffe
blt @8006034C
b @80060400
@800603E8
clrlwi r0, r6, 16
clrlwi r7, r3, 16
slwi r0, r0, 2
addi r3, r3, 0x1
stwx r7, r5, r0
addi r6, r6, 0x1
@80060400
clrlwi r0, r6, 16
cmplwi r0, 0x6
blt @800603E8
@8006040C
addi r4, r4, 0x2
addi r5, r5, 0x18
bdnz @80060274
@80060418
lwz r0, 0x144(r1)
lwz r31, 0x13c(r1)
lwz r30, 0x138(r1)
lwz r29, 0x134(r1)
mtlr r0
addi r1, r1, 0x140
blr
