stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
bl fn_80105624
lhz r3, 0x4(r3)
rlwinm r0, r3, 0, 27, 27
cmpwi r0, 0x0
beq @80096F10
lis r3, lbl_803FB380@ha
addi r31, r3, lbl_803FB380@l
lbz r0, 0x1(r31)
cmplwi r0, 0x8
bgt @80096F88
lis r3, jumptable_802EF05C@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EF05C@l
lwzx r0, r3, r0
mtctr r0
bctr
li r3, 0x0
li r0, 0x3
stb r3, 0x2(r31)
stb r0, 0x1(r31)
b @80096F88
lwz r3, 0xc(r31)
cmplwi r3, 0x0
beq @80096F88
li r4, 0x0
li r5, 0xc2
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
beq @80096DF0
li r3, 0x26
bl fn_80166A28
b @80096F88
@80096DF0
lis r3, lbl_803FB380@ha
addi r4, r3, lbl_803FB380@l
lbz r0, 0x0(r4)
rlwinm r0, r0, 0, 30, 30
cmpwi r0, 0x0
beq @80096F88
lbz r0, 0x2(r4)
li r3, 0x4
stb r3, 0x1(r31)
stb r0, 0x3(r4)
b @80096F88
lwz r3, 0xc(r31)
cmplwi r3, 0x0
beq @80096E3C
lbz r4, 0x3(r31)
lbz r5, 0x2(r31)
extsb r4, r4
extsb r5, r5
bl fn_80123C54
@80096E3C
lis r3, lbl_803FB380@ha
li r0, 0x3
addi r3, r3, lbl_803FB380@l
li r4, -0x1
stb r4, 0x3(r3)
stb r0, 0x1(r31)
b @80096F88
lwz r0, 0x1c(r31)
cmpwi r0, 0x0
ble @80096F88
li r8, 0x0
lis r3, 0x38e4
subi r6, r3, 0x71c7
b @80096EC4
@80096E74
extsb r7, r8
mulhw r4, r6, r7
slwi r0, r7, 30
srwi r3, r7, 31
subf r0, r3, r0
srawi r4, r4, 1
srwi r5, r4, 31
rotlwi r0, r0, 2
add r4, r4, r5
mulli r4, r4, 0x9
add r0, r0, r3
subf r3, r4, r7
slwi r3, r3, 2
add r3, r31, r3
add r3, r3, r0
lbz r0, 0x20(r3)
extsb r0, r0
cmpwi r0, 0x0
bge @80096ED0
addi r8, r8, 0x1
@80096EC4
extsb r0, r8
cmpwi r0, 0x24
blt @80096E74
@80096ED0
lis r3, lbl_803FB380@ha
li r0, 0x6
addi r3, r3, lbl_803FB380@l
stb r0, 0x1(r31)
stb r8, 0x1a(r3)
b @80096F88
lbz r0, 0x0(r31)
rlwinm r0, r0, 0, 27, 27
cmpwi r0, 0x0
beq @80096F88
li r0, 0x1
stb r0, 0x98(r30)
lbz r0, 0x2(r31)
extsb r0, r0
stw r0, 0x4(r31)
b @80096F88
@80096F10
rlwinm r0, r3, 0, 26, 26
cmpwi r0, 0x0
beq @80096F88
lis r3, lbl_803FB380@ha
addi r4, r3, lbl_803FB380@l
lbz r0, 0x1(r4)
cmplwi r0, 0x8
bgt @80096F88
lis r3, jumptable_802EF038@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EF038@l
lwzx r0, r3, r0
mtctr r0
bctr
li r3, 0x1
li r0, 0x8
stb r3, 0x98(r30)
stb r3, 0x99(r30)
stb r0, 0x1(r4)
b @80096F88
li r0, 0x2
stb r0, 0x1(r4)
b @80096F88
li r0, 0x3
stb r0, 0x1(r4)
b @80096F88
li r3, -0x1
li r0, 0x5
stb r3, 0x1a(r4)
stb r0, 0x1(r4)
@80096F88
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
