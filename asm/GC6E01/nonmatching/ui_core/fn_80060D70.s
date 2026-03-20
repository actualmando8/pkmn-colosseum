stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stw r31, 0x2c(r1)
stw r30, 0x28(r1)
stw r29, 0x24(r1)
mr r29, r4
mr r30, r5
mr r31, r6
lis r3, lbl_803A9A60@ha
lwz r5, lbl_8047BF50@sda21(r0)
addi r3, r3, lbl_803A9A60@l
lwz r4, lbl_8047BF54@sda21(r0)
lwz r0, 0x4(r3)
stw r5, 0x8(r1)
cmpwi r0, 0x1
stw r4, 0xc(r1)
bne @80060EC8
bl fn_801EF634
clrlwi r0, r3, 16
cmpwi r0, 0x5
beq @80060DE8
bge @80060DDC
cmpwi r0, 0x2
beq @80060DE8
bge @80060DFC
b @80060E20
@80060DDC
cmpwi r0, 0x8
bge @80060E20
b @80060E10
@80060DE8
li r3, 0x0
li r0, 0x1
stw r3, 0x8(r1)
stw r0, 0xc(r1)
b @80060E2C
@80060DFC
li r3, 0x1
li r0, 0x0
stw r3, 0x8(r1)
stw r0, 0xc(r1)
b @80060E2C
@80060E10
li r0, 0x2
stw r0, 0x8(r1)
stw r0, 0xc(r1)
b @80060E2C
@80060E20
li r0, 0x2
stw r0, 0x8(r1)
stw r0, 0xc(r1)
@80060E2C
lis r3, lbl_803A9A60@ha
addi r4, r3, lbl_803A9A60@l
lwz r0, 0x38(r4)
cmpwi r0, 0x6
blt @80060EB4
slwi r0, r30, 2
addi r3, r1, 0x8
lwzx r0, r3, r0
cmpw r31, r0
bne @80060EA0
slwi r0, r30, 3
lfs f2, lbl_8047BF90@sda21(r0)
add r3, r4, r0
lfs f0, lbl_8047BFA0@sda21(r0)
lfs f3, 0x358(r3)
fsubs f1, f3, f2
fsubs f1, f2, f1
fmuls f0, f0, f1
fctiwz f0, f0
stfd f0, 0x10(r1)
lwz r0, 0x14(r1)
stb r0, 0x67(r29)
stfs f3, 0x68(r29)
stfs f3, 0x6c(r29)
lbz r0, 0x4(r29)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r29)
b @80060ED8
@80060EA0
lbz r0, 0x4(r29)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r29)
b @80060ED8
@80060EB4
lbz r0, 0x4(r29)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r29)
b @80060ED8
@80060EC8
lbz r0, 0x4(r29)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r29)
@80060ED8
lwz r0, 0x34(r1)
lwz r31, 0x2c(r1)
lwz r30, 0x28(r1)
lwz r29, 0x24(r1)
mtlr r0
addi r1, r1, 0x30
blr
