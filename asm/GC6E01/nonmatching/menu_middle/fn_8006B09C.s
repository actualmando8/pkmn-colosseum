stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
cmpwi r31, 0x0
blt @8006B0C0
cmpwi r31, 0x4
blt @8006B0C8
@8006B0C0
li r3, 0x0
b @8006B0E4
@8006B0C8
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r31, 0x1660
mr r0, r3
addi r3, r4, 0x24
add r3, r0, r3
@8006B0E4
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
