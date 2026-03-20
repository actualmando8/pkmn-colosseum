stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
li r3, 0x2
li r4, 0x1
bl fn_8010264C
mr r31, r3
li r3, 0x2
bl fn_80102510
cmpwi r31, 0x0
blt @80088FE0
lwz r0, lbl_8047A660@sda21(r0)
add r0, r0, r31
stw r0, lbl_8047A660@sda21(r0)
@80088FE0
li r3, 0x0
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
