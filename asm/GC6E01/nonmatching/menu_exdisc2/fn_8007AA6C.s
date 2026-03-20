stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r0, 0x1
stw r0, lbl_8047A638@sda21(r0)
bl fn_80113F48
lis r4, 0x1094
li r5, 0x0
addi r4, r4, 0x1800
li r6, 0x0
bl fn_80176E0C
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
