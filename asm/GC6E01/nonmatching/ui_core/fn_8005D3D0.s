stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
lwz r5, lbl_8047BF38@sda21(r0)
li r0, 0x0
lwz r4, lbl_8047BF3C@sda21(r0)
addi r6, r1, 0xc
stw r5, 0xc(r1)
stw r4, 0x10(r1)
stw r0, 0x8(r1)
b @8005D41C
@8005D400
lwz r0, 0x0(r6)
cmpw r3, r0
beq @8005D428
lwz r4, 0x8(r1)
addi r6, r6, 0x4
addi r0, r4, 0x1
stw r0, 0x8(r1)
@8005D41C
lwz r4, 0x8(r1)
cmpwi r4, 0x2
blt @8005D400
@8005D428
cmpwi r4, 0x2
blt @8005D438
li r0, 0x0
stw r0, 0x8(r1)
@8005D438
bl fn_801046B8
mr r4, r3
addi r5, r1, 0x8
li r3, 0xa7
li r6, 0x0
li r7, 0x1
li r8, 0x0
crclr 6
bl fn_801026A4
mr r31, r3
li r3, 0xa7
bl fn_80102510
li r3, 0xa7
li r4, 0x1
bl fn_80102428
cmpwi r31, -0x1
ble @8005D484
cmpwi r31, 0x2
blt @8005D48C
@8005D484
li r3, 0x1
b @8005D498
@8005D48C
slwi r0, r31, 2
addi r3, r1, 0xc
lwzx r3, r3, r0
@8005D498
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
mtlr r0
addi r1, r1, 0x20
blr
