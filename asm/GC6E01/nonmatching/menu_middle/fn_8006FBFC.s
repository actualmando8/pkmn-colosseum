stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
li r4, 0x9bb
bl fn_801046C8
lbz r0, 0x95(r29)
extsb r0, r0
cmpwi r0, 0x3
bge @8006FC38
li r0, 0x3dc0
b @8006FC3C
@8006FC38
li r0, 0x3dc1
@8006FC3C
stw r0, 0x4c(r3)
lbz r0, 0x1(r29)
extsb r0, r0
cmpwi r0, 0x2
beq @8006FC9C
bge @8006FC60
cmpwi r0, 0x0
beq @8006FC68
b @8006FCC8
@8006FC60
cmpwi r0, 0x4
b @8006FCC8
@8006FC68
lis r3, lbl_80268234@ha
li r30, 0x0
addi r31, r3, lbl_80268234@l
@8006FC74
lhz r4, 0x0(r31)
mr r3, r29
bl fn_801046C8
lwz r0, 0x4(r31)
addi r31, r31, 0x8
addi r30, r30, 0x1
stw r0, 0x4c(r3)
cmplwi r30, 0x8
blt @8006FC74
b @8006FCC8
@8006FC9C
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 21, 21
cmpwi r0, 0x0
beq @8006FCC8
lbz r0, 0x95(r29)
extsb r0, r0
cmpwi r0, 0x6
bge @8006FCC8
li r0, 0x1
stb r0, 0x98(r29)
@8006FCC8
lis r4, lbl_802681B4@ha
mr r3, r29
addi r4, r4, lbl_802681B4@l
li r5, 0x10
bl fn_80070D84
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
