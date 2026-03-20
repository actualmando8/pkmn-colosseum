stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stb r3, lbl_8047A5A8@sda21(r0)
li r3, 0x9d
li r4, 0x1
bl fn_8010264C
mr r31, r3
li r3, 0x9d
bl fn_80102510
li r3, 0x9d
li r4, 0x1
bl fn_80102428
cmpwi r31, 0x73d
bne @8005D780
lbz r3, lbl_8047A5A8@sda21(r0)
b @8005D784
@8005D780
li r3, 0xff
@8005D784
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
