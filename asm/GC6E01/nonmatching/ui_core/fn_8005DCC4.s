stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
bl fn_80105624
lhz r0, 0x6(r3)
clrlwi r0, r0, 31
cmpwi r0, 0x0
beq @8005DD0C
lbz r4, 0x95(r31)
subi r4, r4, 0x1
extsb r0, r4
stb r4, 0x95(r31)
cmpwi r0, 0x0
bge @8005DD0C
li r0, 0x0
stb r0, 0x95(r31)
@8005DD0C
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 30, 30
cmpwi r0, 0x0
beq @8005DD3C
lbz r3, 0x95(r31)
addi r3, r3, 0x1
extsb r0, r3
stb r3, 0x95(r31)
cmpwi r0, 0x1
ble @8005DD3C
li r0, 0x1
stb r0, 0x95(r31)
@8005DD3C
lbz r0, 0x95(r31)
extsb r0, r0
cmpwi r0, 0x1
beq @8005DE1C
bge @8005DED0
cmpwi r0, 0x0
bge @8005DD5C
b @8005DED0
@8005DD5C
bl fn_80105624
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
beq @8005DD90
lwz r4, lbl_8047A5C8@sda21(r0)
lwz r5, lbl_80478BD8@sda21(r0)
addi r0, r4, 0x1
cmpw r0, r5
stw r0, lbl_8047A5C8@sda21(r0)
blt @8005DD90
subi r0, r5, 0x1
stw r0, lbl_8047A5C8@sda21(r0)
@8005DD90
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 21, 21
cmpwi r0, 0x0
beq @8005DDC0
lwz r4, lbl_8047A5C8@sda21(r0)
lwz r5, lbl_80478BD8@sda21(r0)
addi r0, r4, 0xa
cmpw r0, r5
stw r0, lbl_8047A5C8@sda21(r0)
blt @8005DDC0
subi r0, r5, 0x1
stw r0, lbl_8047A5C8@sda21(r0)
@8005DDC0
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 29, 29
cmpwi r0, 0x0
beq @8005DDEC
lwz r4, lbl_8047A5C8@sda21(r0)
subi r0, r4, 0x1
cmpwi r0, 0x0
stw r0, lbl_8047A5C8@sda21(r0)
bge @8005DDEC
li r0, 0x0
stw r0, lbl_8047A5C8@sda21(r0)
@8005DDEC
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 22, 22
cmpwi r0, 0x0
beq @8005DED0
lwz r3, lbl_8047A5C8@sda21(r0)
subi r0, r3, 0xa
cmpwi r0, 0x0
stw r0, lbl_8047A5C8@sda21(r0)
bge @8005DED0
li r0, 0x0
stw r0, lbl_8047A5C8@sda21(r0)
b @8005DED0
@8005DE1C
bl fn_80105624
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
beq @8005DE4C
lwz r4, lbl_8047A5C4@sda21(r0)
addi r0, r4, 0x1
cmpwi r0, 0x3e7
stw r0, lbl_8047A5C4@sda21(r0)
ble @8005DE4C
li r0, 0x3e7
stw r0, lbl_8047A5C4@sda21(r0)
@8005DE4C
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 21, 21
cmpwi r0, 0x0
beq @8005DE78
lwz r4, lbl_8047A5C4@sda21(r0)
addi r0, r4, 0xa
cmpwi r0, 0x3e7
stw r0, lbl_8047A5C4@sda21(r0)
ble @8005DE78
li r0, 0x3e7
stw r0, lbl_8047A5C4@sda21(r0)
@8005DE78
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 29, 29
cmpwi r0, 0x0
beq @8005DEA4
lwz r4, lbl_8047A5C4@sda21(r0)
subi r0, r4, 0x1
cmpwi r0, 0x0
stw r0, lbl_8047A5C4@sda21(r0)
bge @8005DEA4
li r0, 0x0
stw r0, lbl_8047A5C4@sda21(r0)
@8005DEA4
lhz r0, 0x6(r3)
rlwinm r0, r0, 0, 22, 22
cmpwi r0, 0x0
beq @8005DED0
lwz r3, lbl_8047A5C4@sda21(r0)
subi r0, r3, 0xa
cmpwi r0, 0x0
stw r0, lbl_8047A5C4@sda21(r0)
bge @8005DED0
li r0, 0x0
stw r0, lbl_8047A5C4@sda21(r0)
@8005DED0
li r3, 0x0
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
