stwu r1, -0x90(r1)
mflr r0
stw r0, 0x94(r1)
stw r31, 0x8c(r1)
stw r30, 0x88(r1)
stw r29, 0x84(r1)
mr r30, r3
mr r29, r4
lis r3, lbl_80268780@ha
addi r31, r3, lbl_80268780@l
bl fn_800A7BCC
mr r0, r3
li r3, lbl_8047A60C@sda21
mr r4, r0
li r5, 0x4
bl memcpy
lwz r0, lbl_8047A60C@sda21(r0)
oris r0, r0, 0x20
ori r0, r0, 0x2020
stw r0, lbl_8047A60C@sda21(r0)
bl fn_8025F350
mr r3, r30
addi r4, r1, 0x44
bl fn_800A501C
cmpwi r3, 0x0
bne @80073F08
addi r3, r31, 0x0
addi r5, r31, 0x10
li r4, 0x1d6
crclr 6
bl fn_800060F0
@80073F08
lwz r3, 0x78(r1)
addi r0, r3, 0x7
clrrwi r3, r0, 3
cmplwi r3, 0x0
stw r3, lbl_8047A608@sda21(r0)
beq @80073F2C
lis r0, 0x2
cmplw r3, r0
ble @80073F40
@80073F2C
addi r3, r31, 0x0
addi r5, r31, 0x28
li r4, 0x1dc
crclr 6
bl fn_800060F0
@80073F40
lwz r5, lbl_8047A608@sda21(r0)
lis r3, lbl_803D6E40@ha
addi r4, r3, lbl_803D6E40@l
li r6, 0x0
addi r0, r5, 0x1f
addi r3, r1, 0x44
clrrwi r5, r0, 5
li r7, 0x2
stw r5, lbl_8047A608@sda21(r0)
bl fn_800A541C
cmpwi r3, 0x0
bge @80073F84
addi r3, r31, 0x0
addi r5, r31, 0x4c
li r4, 0x1e1
crclr 6
bl fn_800060F0
@80073F84
addi r3, r1, 0x44
bl fn_800A50E4
li r30, lbl_8047A60C@sda21
lis r3, lbl_803D6E40@ha
li r8, 0xa0
lbz r6, 0x0(r30)
addi r5, r3, lbl_803D6E40@l
lbz r4, 0x1(r30)
lbz r3, 0x2(r30)
cmpwi r8, 0xbd
lbz r0, 0x3(r30)
li r7, 0xe7
stb r6, 0xac(r5)
stb r4, 0xad(r5)
stb r3, 0xae(r5)
stb r0, 0xaf(r5)
bge @800740E0
lbz r4, 0xa0(r5)
addi r6, r5, 0xa7
lbz r3, 0xa1(r5)
li r8, 0xbc
subfic r7, r4, 0xe7
lbz r0, 0xa2(r5)
subf r7, r3, r7
lbz r3, 0xa3(r5)
subf r7, r0, r7
lbz r0, 0xa4(r5)
subf r7, r3, r7
lbz r3, 0xa5(r5)
subf r7, r0, r7
lbz r0, 0xa6(r5)
subf r7, r3, r7
lbz r4, 0x0(r6)
subf r7, r0, r7
lbz r3, 0x1(r6)
subf r7, r4, r7
lbz r0, 0x2(r6)
subf r7, r3, r7
lbz r3, 0x3(r6)
subf r7, r0, r7
lbz r0, 0x4(r6)
subf r7, r3, r7
lbz r3, 0x5(r6)
subf r7, r0, r7
lbz r0, 0x6(r6)
addi r6, r6, 0x7
subf r7, r3, r7
lbz r4, 0x0(r6)
subf r7, r0, r7
lbz r3, 0x1(r6)
subf r7, r4, r7
lbz r0, 0x2(r6)
subf r7, r3, r7
lbz r3, 0x3(r6)
subf r7, r0, r7
lbz r0, 0x4(r6)
subf r7, r3, r7
lbz r3, 0x5(r6)
subf r7, r0, r7
lbz r0, 0x6(r6)
addi r6, r6, 0x7
subf r7, r3, r7
lbz r4, 0x0(r6)
subf r7, r0, r7
lbz r3, 0x1(r6)
subf r7, r4, r7
lbz r0, 0x2(r6)
subf r7, r3, r7
lbz r3, 0x3(r6)
subf r7, r0, r7
lbz r0, 0x4(r6)
subf r7, r3, r7
lbz r3, 0x5(r6)
subf r7, r0, r7
lbz r0, 0x6(r6)
subf r7, r3, r7
subf r7, r0, r7
addi r3, r5, 0xbc
subfic r0, r8, 0xbd
mtctr r0
cmpwi r8, 0xbd
bge @800740E0
@800740CC
lbz r0, 0x0(r3)
addi r3, r3, 0x1
addi r8, r8, 0x1
subf r7, r0, r7
bdnz @800740CC
@800740E0
lis r3, lbl_803D6E40@ha
cmplwi r29, 0x0
clrlwi r0, r7, 24
addi r3, r3, lbl_803D6E40@l
stbx r0, r3, r8
beq @80074304
mr r3, r29
addi r4, r1, 0x8
bl fn_800A501C
cmpwi r3, 0x0
bne @80074120
addi r3, r31, 0x0
addi r5, r31, 0x10
li r4, 0x1d6
crclr 6
bl fn_800060F0
@80074120
lwz r3, 0x3c(r1)
addi r0, r3, 0x7
clrrwi r3, r0, 3
cmplwi r3, 0x0
stw r3, lbl_8047A604@sda21(r0)
beq @80074144
lis r0, 0x2
cmplw r3, r0
ble @80074158
@80074144
addi r3, r31, 0x0
addi r5, r31, 0x28
li r4, 0x1dc
crclr 6
bl fn_800060F0
@80074158
lwz r5, lbl_8047A604@sda21(r0)
lis r3, lbl_803B6E40@ha
addi r4, r3, lbl_803B6E40@l
li r6, 0x0
addi r0, r5, 0x1f
addi r3, r1, 0x8
clrrwi r5, r0, 5
li r7, 0x2
stw r5, lbl_8047A604@sda21(r0)
bl fn_800A541C
cmpwi r3, 0x0
bge @8007419C
addi r3, r31, 0x0
addi r5, r31, 0x4c
li r4, 0x1e1
crclr 6
bl fn_800060F0
@8007419C
addi r3, r1, 0x8
bl fn_800A50E4
lis r3, lbl_803B6E40@ha
li r8, 0xa0
lbz r6, 0x0(r30)
addi r5, r3, lbl_803B6E40@l
lbz r4, 0x1(r30)
cmpwi r8, 0xbd
lbz r3, 0x2(r30)
li r7, 0xe7
lbz r0, 0x3(r30)
stb r6, 0xac(r5)
stb r4, 0xad(r5)
stb r3, 0xae(r5)
stb r0, 0xaf(r5)
bge @800742F4
lbz r4, 0xa0(r5)
addi r6, r5, 0xa7
lbz r3, 0xa1(r5)
li r8, 0xbc
subfic r7, r4, 0xe7
lbz r0, 0xa2(r5)
subf r7, r3, r7
lbz r3, 0xa3(r5)
subf r7, r0, r7
lbz r0, 0xa4(r5)
subf r7, r3, r7
lbz r3, 0xa5(r5)
subf r7, r0, r7
lbz r0, 0xa6(r5)
subf r7, r3, r7
lbz r4, 0x0(r6)
subf r7, r0, r7
lbz r3, 0x1(r6)
subf r7, r4, r7
lbz r0, 0x2(r6)
subf r7, r3, r7
lbz r3, 0x3(r6)
subf r7, r0, r7
lbz r0, 0x4(r6)
subf r7, r3, r7
lbz r3, 0x5(r6)
subf r7, r0, r7
lbz r0, 0x6(r6)
addi r6, r6, 0x7
subf r7, r3, r7
lbz r4, 0x0(r6)
subf r7, r0, r7
lbz r3, 0x1(r6)
subf r7, r4, r7
lbz r0, 0x2(r6)
subf r7, r3, r7
lbz r3, 0x3(r6)
subf r7, r0, r7
lbz r0, 0x4(r6)
subf r7, r3, r7
lbz r3, 0x5(r6)
subf r7, r0, r7
lbz r0, 0x6(r6)
addi r6, r6, 0x7
subf r7, r3, r7
lbz r4, 0x0(r6)
subf r7, r0, r7
lbz r3, 0x1(r6)
subf r7, r4, r7
lbz r0, 0x2(r6)
subf r7, r3, r7
lbz r3, 0x3(r6)
subf r7, r0, r7
lbz r0, 0x4(r6)
subf r7, r3, r7
lbz r3, 0x5(r6)
subf r7, r0, r7
lbz r0, 0x6(r6)
subf r7, r3, r7
subf r7, r0, r7
addi r3, r5, 0xbc
subfic r0, r8, 0xbd
mtctr r0
cmpwi r8, 0xbd
bge @800742F4
@800742E0
lbz r0, 0x0(r3)
addi r3, r3, 0x1
addi r8, r8, 0x1
subf r7, r0, r7
bdnz @800742E0
@800742F4
lis r3, lbl_803B6E40@ha
clrlwi r0, r7, 24
addi r3, r3, lbl_803B6E40@l
stbx r0, r3, r8
@80074304
li r3, 0x0
lwz r0, 0x94(r1)
lwz r31, 0x8c(r1)
lwz r30, 0x88(r1)
lwz r29, 0x84(r1)
mtlr r0
addi r1, r1, 0x90
blr
