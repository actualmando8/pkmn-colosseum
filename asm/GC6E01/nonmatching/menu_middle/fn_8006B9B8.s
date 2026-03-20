stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r27, 0xc(r1)
mr r31, r3
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
beq @8006B9FC
bge @8006B9FC
cmpwi r0, 0x0
bge @8006B9F4
b @8006B9FC
@8006B9F4
li r27, 0x2
b @8006BA00
@8006B9FC
li r27, 0x4
@8006BA00
lbz r0, 0x1(r31)
extsb r0, r0
cmpwi r0, 0x2
beq @8006BA24
bge @8006BA1C
cmpwi r0, 0x0
b @8006BB20
@8006BA1C
cmpwi r0, 0x6
b @8006BB20
@8006BA24
bl fn_80071160
cmpwi r3, 0x0
beq @8006BA40
li r0, 0x1
stb r0, 0x98(r31)
stb r0, 0x99(r31)
b @8006BB20
@8006BA40
li r28, 0x0
li r29, 0x0
b @8006BB18
@8006BA4C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r29, 0x59a8
add r3, r3, r0
bl fn_8006A814
bl fn_80071208
rlwinm r0, r3, 0, 23, 23
cmplwi r0, 0x0
beq @8006BAB4
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r29, 0x7005
lbzx r0, r3, r0
cmplwi r0, 0x0
bne @8006BB10
li r30, 0x1
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r29, 0x7005
stbx r30, r3, r0
li r3, 0x24
bl fn_80166A28
b @8006BB10
@8006BAB4
rlwinm r0, r3, 0, 22, 22
cmplwi r0, 0x0
beq @8006BB10
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r29, 0x7005
lbzx r0, r3, r0
cmplwi r0, 0x0
beq @8006BB00
li r30, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r29, 0x7005
stbx r30, r3, r0
li r3, 0x25
bl fn_80166A28
b @8006BB10
@8006BB00
li r0, 0x1
stb r0, 0x98(r31)
stb r0, 0x99(r31)
b @8006BB20
@8006BB10
addi r29, r29, 0x1660
addi r28, r28, 0x1
@8006BB18
cmpw r28, r27
blt @8006BA4C
@8006BB20
lmw r27, 0xc(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
