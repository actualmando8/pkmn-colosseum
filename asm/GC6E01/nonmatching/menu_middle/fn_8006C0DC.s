stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
lbz r0, 0x1(r30)
extsb r0, r0
cmpwi r0, 0x2
beq @8006C108
b @8006C14C
@8006C108
bl fn_80105624
mr r31, r3
lwz r3, 0x4(r30)
bl fn_801022B8
cmpwi r3, 0x9d2
bge @8006C144
cmpwi r3, 0x9ca
bge @8006C12C
b @8006C144
@8006C12C
lhz r0, 0x4(r31)
rlwinm r0, r0, 0, 27, 27
cmpwi r0, 0x0
beq @8006C144
b @8006C14C
b @8006C14C
@8006C144
mr r3, r30
bl fn_80102ED4
@8006C14C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
