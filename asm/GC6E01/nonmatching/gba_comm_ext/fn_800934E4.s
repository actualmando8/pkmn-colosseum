stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
cmpwi r3, 0x0
blt @80093508
cmpwi r3, 0x3
ble @80093510
@80093508
li r3, 0x0
b @8009355C
@80093510
lis r4, lbl_803FB328@ha
slwi r0, r3, 2
addi r3, r4, lbl_803FB328@l
lwzx r30, r3, r0
cmplwi r30, 0x0
beq @80093554
mr r3, r30
bl fn_8009F7B4
lwz r0, 0x4340(r30)
mr r3, r30
cntlzw r0, r0
srwi r31, r0, 5
bl fn_8009F890
addi r3, r30, 0x20
li r4, 0x8
bl fn_800A257C
b @80093558
@80093554
li r31, 0x1
@80093558
mr r3, r31
@8009355C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
