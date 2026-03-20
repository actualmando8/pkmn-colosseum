stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lis r4, lbl_80268184@ha
li r5, 0x6
addi r4, r4, lbl_80268184@l
bl fn_80070D84
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
