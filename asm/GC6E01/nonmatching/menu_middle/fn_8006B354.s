stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
cmpwi r30, 0x0
blt @8006B37C
cmpwi r30, 0x7
blt @8006B394
@8006B37C
lis r3, lbl_80267DE8@ha
li r4, 0xe4
addi r3, r3, lbl_80267DE8@l
li r5, lbl_8047C040@sda21
bl fn_80196E10
b @8006B3B0
@8006B394
li r31, 0x1
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r0, r30, 0x1
add r3, r0, r3
stb r31, -0x342c(r3)
@8006B3B0
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
