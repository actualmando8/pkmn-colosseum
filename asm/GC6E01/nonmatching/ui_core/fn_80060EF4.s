stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r31, r4
mr r29, r5
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
lwz r3, 0x0(r3)
lwz r30, 0xc(r3)
bl fn_8025D9A8
cmpwi r29, 0x0
bge @80060F7C
cmpwi r3, 0x1
bne @80060F4C
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @80060FFC
@80060F4C
cmpw r29, r30
bne @80060F68
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @80060FFC
@80060F68
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @80060FFC
@80060F7C
cmpwi r3, 0x1
bne @80060F98
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @80060FFC
@80060F98
cmpwi r30, 0x5
bne @80060FD0
cmpwi r29, 0x3
bne @80060FBC
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @80060FFC
@80060FBC
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @80060FFC
@80060FD0
cmpw r29, r30
bne @80060FEC
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @80060FFC
@80060FEC
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
@80060FFC
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
