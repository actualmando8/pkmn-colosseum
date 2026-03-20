stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
li r3, 0x2
li r4, 0x1
bl fn_8010264C
mr r0, r3
li r3, 0x2
mr r31, r0
bl fn_80102510
cmpwi r31, 0x0
blt @8005D118
mr r3, r31
bl fn_800347E8
@8005D118
li r3, 0x0
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
