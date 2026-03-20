stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
li r0, 0x1
lis r3, 0x10bd
stw r0, lbl_8047A620@sda21(r0)
addi r3, r3, 0x1000
bl fn_801CBA0C
li r4, 0x1
mr r31, r3
bl fn_801CB954
lis r4, 0x104f
mr r3, r31
addi r4, r4, 0x1000
li r5, 0x207
bl fn_801CB61C
mr r3, r31
li r4, 0x0
li r5, 0x0
li r6, 0x1
bl fn_801CB834
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
