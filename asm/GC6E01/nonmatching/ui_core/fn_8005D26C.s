stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
lwz r5, lbl_8047BF30@sda21(r0)
li r3, 0x9e
lwz r0, lbl_8047BF34@sda21(r0)
li r4, 0x1
stw r5, 0x8(r1)
stw r0, 0xc(r1)
bl fn_8010264C
mr r31, r3
li r3, 0x9e
bl fn_80102510
li r3, 0x9e
li r4, 0x1
bl fn_80102428
cmpwi r31, -0x1
blt @8005D2C0
cmpwi r31, 0x2
blt @8005D2C8
@8005D2C0
li r3, 0x1
b @8005D2D4
@8005D2C8
slwi r0, r31, 2
addi r3, r1, 0x8
lwzx r3, r3, r0
@8005D2D4
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
mtlr r0
addi r1, r1, 0x20
blr
