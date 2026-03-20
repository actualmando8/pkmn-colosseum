stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
mr r31, r3
bl fn_80105624
lbz r4, 0x95(r31)
lwz r0, lbl_8047BF40@sda21(r0)
extsb r4, r4
cmpwi r4, 0x0
stw r0, 0x8(r1)
blt @8005D71C
cmpwi r4, 0x3
bgt @8005D71C
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 27, 27
cmpwi r0, 0x0
beq @8005D71C
addi r3, r1, 0x8
lbz r6, lbl_8047A5A8@sda21(r0)
lbzx r0, r3, r4
li r3, 0x3c6
li r4, 0x0
li r5, 0xff
xor r0, r6, r0
li r6, 0x0
stb r0, lbl_8047A5A8@sda21(r0)
bl fn_80166A50
b @8005D724
@8005D71C
mr r3, r31
bl fn_80102ED4
@8005D724
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
mtlr r0
addi r1, r1, 0x20
blr
