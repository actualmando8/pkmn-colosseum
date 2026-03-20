stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
li r3, 0xba
li r4, 0x0
li r5, 0x1
bl fn_80102568
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
stw r31, 0x4(r3)
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
