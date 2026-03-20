stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lwz r3, lbl_8047A5D0@sda21(r0)
bl fn_801CB9D8
li r0, 0x0
stw r0, lbl_8047A5D0@sda21(r0)
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
