stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
beq @8006AD40
bge @8006AD40
cmpwi r0, 0x0
bge @8006AD10
b @8006AD40
@8006AD10
cmpwi r29, 0x0
blt @8006AD94
cmpwi r29, 0x1
bgt @8006AD94
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r29, 0x1660
mr r0, r3
addi r3, r4, 0x24
add r3, r0, r3
b @8006AD98
@8006AD40
li r30, 0x0
li r31, 0x0
@8006AD48
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r31, 0x4c
lwzx r0, r3, r0
cmpw r29, r0
bne @8006AD84
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r30, 0x1660
mr r0, r3
addi r3, r4, 0x24
add r3, r0, r3
b @8006AD98
@8006AD84
addi r31, r31, 0x1660
addi r30, r30, 0x1
cmpwi r30, 0x4
blt @8006AD48
@8006AD94
li r3, 0x0
@8006AD98
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
