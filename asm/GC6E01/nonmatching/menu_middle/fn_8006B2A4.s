stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
mr r30, r4
cmpwi r29, 0x0
blt @8006B2D4
cmpwi r29, 0x7
blt @8006B2DC
@8006B2D4
li r0, 0x0
b @8006B2F4
@8006B2DC
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r0, r29, 0x1
add r3, r0, r3
lbz r0, -0x342c(r3)
@8006B2F4
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @8006B338
cmpwi r30, 0x0
blt @8006B338
cmplwi r30, 0x2
blt @8006B314
b @8006B338
@8006B314
li r31, 0x1
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r4, r30, 0x1
slwi r0, r29, 1
add r3, r4, r3
add r3, r3, r0
stb r31, -0x3425(r3)
@8006B338
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
