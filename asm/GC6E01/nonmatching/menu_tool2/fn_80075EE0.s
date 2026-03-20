stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lwz r0, lbl_8047A618@sda21(r0)
cmpwi r0, 0x0
bne @80075F20
bl fn_800FF560
lis r5, fn_80075F4C@ha
mr r4, r3
addi r8, r5, fn_80075F4C@l
li r3, 0x1
li r5, 0x4000
li r6, 0x1
li r7, 0x1
bl fn_800F07A8
b @80075F30
@80075F20
li r3, 0x46a
li r4, 0x0
li r5, 0x7f
bl fn_80165A20
@80075F30
lfs f1, lbl_8047C0C8@sda21(r0)
li r3, 0x2
bl fn_801C41C8
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
