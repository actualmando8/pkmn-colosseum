stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stw r31, 0x3c(r1)
stw r30, 0x38(r1)
mr r31, r3
mr r30, r4
mr r3, r30
bl fn_8025DA18
mr r3, r31
addi r4, r1, 0x8
bl fn_8008A9E4
lwz r3, 0x8(r1)
lis r0, 0x100
clrrwi r3, r3, 24
cmpw r3, r0
beq @80068228
bge @80068218
cmpwi r3, 0x0
beq @800682E0
bge @80068400
lis r0, 0xff00
cmpw r3, r0
beq @800682B0
b @80068400
@80068218
lis r0, 0x300
cmpw r3, r0
beq @800683DC
b @80068400
@80068228
mr r3, r30
bl fn_8025D560
mr r0, r3
mr r3, r30
mr r4, r0
bl fn_8025D644
mr r31, r3
cmpwi r31, 0x0
blt @80068400
li r3, 0x3c3
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
subfic r3, r31, 0x5
lis r0, 0x4330
mulli r3, r3, 0x18
lis r4, lbl_803A9F08@ha
stw r0, 0x30(r1)
slwi r5, r31, 2
lfd f2, lbl_8047BFF0@sda21(r0)
addi r4, r4, lbl_803A9F08@l
xoris r0, r3, 0x8000
lfs f0, lbl_8047BFE8@sda21(r0)
stw r0, 0x34(r1)
mulli r0, r30, 0x30
lfd f1, 0x30(r1)
add r3, r4, r0
addis r4, r3, 0x1
fsubs f1, f1, f2
subi r4, r4, 0x3274
add r3, r4, r5
stfsx f1, r4, r5
stfs f0, 0x18(r3)
b @80068400
@800682B0
mr r3, r30
bl fn_8025D560
mr r31, r3
mr r3, r30
bl fn_8025D584
cmpw r31, r3
beq @80068400
li r3, 0x25
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
b @80068400
@800682E0
mr r3, r30
bl fn_8025D808
clrlwi r31, r3, 16
lwz r3, 0x8(r1)
addi r4, r1, 0xc
bl fn_8008A9AC
li r6, 0x0
cmpw r6, r31
bge @800683B4
cmpwi r31, 0x8
subi r3, r31, 0x8
ble @8006837C
addi r4, r1, 0xc
addi r5, r1, 0x14
addi r0, r3, 0x7
srwi r0, r0, 3
mtctr r0
cmpwi r3, 0x0
ble @8006837C
@8006832C
lbz r0, 0x0(r4)
addi r6, r6, 0x8
stw r0, 0x0(r5)
lbz r0, 0x1(r4)
stw r0, 0x4(r5)
lbz r0, 0x2(r4)
stw r0, 0x8(r5)
lbz r0, 0x3(r4)
stw r0, 0xc(r5)
lbz r0, 0x4(r4)
stw r0, 0x10(r5)
lbz r0, 0x5(r4)
stw r0, 0x14(r5)
lbz r0, 0x6(r4)
stw r0, 0x18(r5)
lbz r0, 0x7(r4)
addi r4, r4, 0x8
stw r0, 0x1c(r5)
addi r5, r5, 0x20
bdnz @8006832C
@8006837C
addi r3, r1, 0xc
slwi r0, r6, 2
addi r4, r1, 0x14
add r3, r3, r6
add r4, r4, r0
subf r0, r6, r31
mtctr r0
cmpw r6, r31
bge @800683B4
@800683A0
lbz r0, 0x0(r3)
addi r3, r3, 0x1
stw r0, 0x0(r4)
addi r4, r4, 0x4
bdnz @800683A0
@800683B4
mr r3, r30
mr r4, r31
addi r5, r1, 0x14
bl fn_8025D5E0
lis r3, lbl_803A9F08@ha
li r4, 0x1
addi r0, r3, lbl_803A9F08@l
add r3, r0, r30
stb r4, 0x4(r3)
b @80068400
@800683DC
lis r3, lbl_803A9F08@ha
li r0, 0x0
addi r3, r3, lbl_803A9F08@l
addis r3, r3, 0x1
stb r0, -0x31a8(r3)
lwz r0, -0x31a4(r3)
cmpwi r0, 0x0
bge @80068400
stw r31, -0x31a4(r3)
@80068400
lwz r0, 0x44(r1)
lwz r31, 0x3c(r1)
lwz r30, 0x38(r1)
mtlr r0
addi r1, r1, 0x40
blr
