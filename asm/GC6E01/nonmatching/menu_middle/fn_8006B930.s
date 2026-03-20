stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
bl fn_80071160
cmpwi r3, 0x0
beq @8006B960
li r0, 0x1
stb r0, 0x98(r31)
stb r0, 0x99(r31)
b @8006B9A4
@8006B960
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r3, 0x59cc(r3)
bl fn_80071208
rlwinm r0, r3, 0, 19, 19
cmplwi r0, 0x0
beq @8006B98C
li r0, 0x1
stb r0, 0x98(r31)
b @8006B9A4
@8006B98C
rlwinm r0, r3, 0, 22, 22
cmplwi r0, 0x0
beq @8006B9A4
li r0, 0x1
stb r0, 0x98(r31)
stb r0, 0x99(r31)
@8006B9A4
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
