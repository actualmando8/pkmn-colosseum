stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
cmpwi r3, 0x0
blt @80093634
cmpwi r3, 0x3
ble @8009363C
@80093634
lis r3, 0x1
b @80093680
@8009363C
lis r4, lbl_803FB328@ha
slwi r0, r3, 2
addi r3, r4, lbl_803FB328@l
lwzx r30, r3, r0
cmplwi r30, 0x0
bne @8009365C
li r3, 0x0
b @80093680
@8009365C
mr r3, r30
bl fn_8009F7B4
lwz r31, 0x433c(r30)
mr r3, r30
bl fn_8009F890
addi r3, r30, 0x20
li r4, 0x8
bl fn_800A257C
mr r3, r31
@80093680
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
