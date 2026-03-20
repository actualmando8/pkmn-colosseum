stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
mr r31, r3
mr r30, r4
lis r0, 0x200
subi r3, r31, 0x1
stw r0, 0x0(r30)
addi r4, r1, 0x8
bl fn_80073034
cmpwi r3, 0x0
bge @8008AA28
lis r0, 0x200
stw r0, 0x8(r1)
b @8008AB04
@8008AA28
beq @8008AA80
lis r0, 0x300
lis r5, lbl_803FB318@ha
lis r4, lbl_803FB308@ha
stw r0, 0x0(r30)
addi r6, r5, lbl_803FB318@l
slwi r7, r31, 2
addi r5, r4, lbl_803FB308@l
slwi r10, r31, 1
li r4, lbl_8047A684@sda21
li r0, lbl_8047A67C@sda21
add r8, r6, r7
li r9, 0x1
add r6, r5, r7
li r7, 0x0
add r5, r4, r10
add r4, r0, r10
stw r9, -0x4(r8)
stw r7, -0x4(r6)
sth r7, -0x2(r5)
sth r7, -0x2(r4)
b @8008AB08
@8008AA80
lwz r5, 0x8(r1)
rlwinm r0, r5, 0, 16, 23
rlwinm r4, r5, 0, 8, 15
slwi r3, r5, 24
srwi r5, r5, 24
slwi r0, r0, 8
srwi r4, r4, 8
or r0, r3, r0
or r0, r4, r0
or r0, r5, r0
stw r0, 0x0(r30)
lwz r0, 0x0(r30)
srwi r0, r0, 24
cmplwi r0, 0x0
bne @8008AB04
lis r4, lbl_803FB318@ha
lis r3, lbl_803FB308@ha
addi r5, r4, lbl_803FB318@l
slwi r6, r31, 2
addi r4, r3, lbl_803FB308@l
slwi r9, r31, 1
li r3, lbl_8047A684@sda21
li r0, lbl_8047A67C@sda21
add r7, r5, r6
li r8, 0x1
add r5, r4, r6
li r6, 0x0
add r4, r3, r9
add r3, r0, r9
stw r8, -0x4(r7)
stw r6, -0x4(r5)
sth r6, -0x2(r4)
sth r6, -0x2(r3)
@8008AB04
li r3, 0x0
@8008AB08
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
mtlr r0
addi r1, r1, 0x20
blr
