stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
lis r4, lbl_803A9A60@ha
addi r31, r4, lbl_803A9A60@l
lwz r0, 0x4(r31)
cmpwi r0, 0x1
beq @8005E6F4
bge @8005E718
cmpwi r0, 0x0
bge @8005E6CC
b @8005E718
@8005E6CC
lwz r0, 0x38(r31)
cmpwi r0, 0x3
blt @8005E6DC
bl fn_80102ED4
@8005E6DC
lwz r0, 0x38(r31)
cmpwi r0, 0x64
bne @8005E718
li r0, 0x1
stb r0, 0x98(r30)
b @8005E718
@8005E6F4
lwz r0, 0x38(r31)
cmpwi r0, 0x7
blt @8005E704
bl fn_80102ED4
@8005E704
lwz r0, 0x38(r31)
cmpwi r0, 0x9
bne @8005E718
li r0, 0x1
stb r0, 0x98(r30)
@8005E718
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
