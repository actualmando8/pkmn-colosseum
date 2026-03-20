stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r27, 0xc(r1)
mr r31, r3
li r4, 0x0
bl fn_801040D0
clrlwi r3, r3, 24
cmplwi r3, 0x0
beq @8006D9BC
li r28, 0x6
b @8006D9C0
@8006D9BC
li r28, 0x3
@8006D9C0
lbz r0, 0x1(r31)
extsb r0, r0
cmpwi r0, 0x3
beq @8006DA74
bge @8006DAC0
cmpwi r0, 0x0
beq @8006D9E0
b @8006DAC0
@8006D9E0
lbz r0, 0x2(r31)
extsb r0, r0
cmpwi r0, 0x0
bne @8006DAC0
cmplwi r3, 0x0
beq @8006DA00
li r0, 0x152
b @8006DA04
@8006DA00
li r0, 0x0
@8006DA04
extsh r0, r0
lis r3, lbl_8026864C@ha
sth r0, 0x84(r31)
addi r30, r3, lbl_8026864C@l
li r27, 0x0
@8006DA18
lhz r4, 0x0(r30)
mr r3, r31
lwz r29, 0x4(r30)
bl fn_801046C8
stw r29, 0x4c(r3)
addi r30, r30, 0x8
addi r27, r27, 0x1
cmplwi r27, 0x5
blt @8006DA18
lis r3, lbl_80267EA8@ha
lwz r27, 0x1c(r31)
slwi r29, r28, 2
addi r30, r3, lbl_80267EA8@l
b @8006DA68
@8006DA50
lha r0, 0x6(r27)
mr r3, r31
lhzx r5, r30, r29
clrlwi r4, r0, 16
bl fn_801081F8
lwz r27, 0x0(r27)
@8006DA68
cmplwi r27, 0x0
bne @8006DA50
b @8006DAC0
@8006DA74
lbz r0, 0x2(r31)
extsb r0, r0
cmpwi r0, 0x0
bne @8006DAC0
lis r3, lbl_80267EA8@ha
slwi r4, r28, 2
addi r0, r3, lbl_80267EA8@l
lwz r27, 0x1c(r31)
add r3, r0, r4
addi r29, r3, 0x2
b @8006DAB8
@8006DAA0
lha r0, 0x6(r27)
mr r3, r31
lhz r5, 0x0(r29)
clrlwi r4, r0, 16
bl fn_801081F8
lwz r27, 0x0(r27)
@8006DAB8
cmplwi r27, 0x0
bne @8006DAA0
@8006DAC0
mr r3, r31
li r4, 0x0
li r5, 0x0
bl fn_80070D84
lmw r27, 0xc(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
