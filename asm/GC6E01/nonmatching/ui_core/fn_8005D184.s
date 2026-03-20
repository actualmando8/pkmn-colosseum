stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
lis r5, lbl_80267A80@ha
addi r11, r1, 0x8
addi r10, r5, lbl_80267A80@l
li r12, 0x0
lwz r9, 0x0(r10)
lwz r8, 0x4(r10)
lwz r7, 0x8(r10)
lwz r6, 0xc(r10)
lwz r5, 0x10(r10)
lwz r0, 0x14(r10)
stw r9, 0x8(r1)
stw r8, 0xc(r1)
stw r7, 0x10(r1)
stw r6, 0x14(r1)
stw r5, 0x18(r1)
stw r0, 0x1c(r1)
li r0, 0x2
mtctr r0
@8005D1D8
li r6, 0x0
lha r5, 0x6(r4)
lwz r0, 0x0(r11)
cmpw r5, r0
beq @8005D210
li r6, 0x1
lwz r0, 0x4(r11)
cmpw r5, r0
beq @8005D210
li r6, 0x2
lwz r0, 0x8(r11)
cmpw r5, r0
beq @8005D210
li r6, 0x3
@8005D210
cmpwi r6, 0x3
blt @8005D224
addi r11, r11, 0xc
addi r12, r12, 0x1
bdnz @8005D1D8
@8005D224
cmpwi r12, 0x2
blt @8005D234
li r3, 0x0
b @8005D25C
@8005D234
lbz r0, 0x95(r3)
mr r3, r4
extsb r0, r0
cmpw r0, r12
bne @8005D250
li r4, 0x1
b @8005D254
@8005D250
li r4, 0x0
@8005D254
bl fn_80109220
li r3, 0x0
@8005D25C
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
