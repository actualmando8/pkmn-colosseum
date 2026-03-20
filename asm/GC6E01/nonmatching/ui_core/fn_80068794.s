stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r27, 0x1c(r1)
mr r31, r4
mr r27, r5
mr r28, r6
mr r3, r27
bl fn_8025D560
mr r30, r3
li r29, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800687E8
bge @800687E8
cmpwi r3, 0x0
bge @800687DC
b @800687E8
@800687DC
cmpwi r27, 0x2
blt @800687E8
li r29, 0x0
@800687E8
clrlwi r0, r29, 24
cmplwi r0, 0x0
beq @800688B0
cmpw r30, r28
ble @800688A0
mulli r6, r27, 0x30
lis r5, lbl_803A9F08@ha
lha r4, 0x6(r31)
lis r3, lbl_802EF0A8@ha
addi r0, r5, lbl_803A9F08@l
lfs f0, lbl_8047BFE8@sda21(r0)
add r5, r0, r6
slwi r6, r28, 2
addis r5, r5, 0x1
addi r0, r3, lbl_802EF0A8@l
subi r5, r5, 0x3274
add r5, r5, r6
lfs f1, 0x0(r5)
mulli r3, r4, 0x1c
fctiwz f1, f1
add r3, r0, r3
lha r3, 0x2(r3)
stfd f1, 0x8(r1)
lwz r0, 0xc(r1)
add r0, r3, r0
extsh r0, r0
sth r0, 0x50(r31)
lfs f2, 0x18(r5)
lfs f1, 0x0(r5)
fsubs f2, f2, f1
fcmpo cr0, f2, f0
ble @8006886C
b @80068870
@8006886C
fneg f2, f2
@80068870
lfs f1, lbl_8047C00C@sda21(r0)
lfs f0, lbl_8047C008@sda21(r0)
fnmsubs f0, f1, f2, f0
fctiwz f0, f0
stfd f0, 0x10(r1)
lwz r0, 0x14(r1)
stb r0, 0x67(r31)
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800688B0
@800688A0
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
@800688B0
lmw r27, 0x1c(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
