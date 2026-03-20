stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
cmpwi r3, 0x0
blt @80093598
cmpwi r3, 0x3
ble @800935A0
@80093598
lis r3, 0x1
b @800935F8
@800935A0
lis r4, lbl_803FB328@ha
slwi r0, r3, 2
addi r3, r4, lbl_803FB328@l
lwzx r30, r3, r0
cmplwi r30, 0x0
bne @800935C0
li r3, 0x0
b @800935F8
@800935C0
mr r3, r30
bl fn_8009F7B4
lwz r31, 0x433c(r30)
mr r3, r30
bl fn_8009F890
addi r3, r30, 0x20
li r4, 0x8
bl fn_800A257C
srwi r0, r31, 16
cmpwi r0, 0x3
bne @800935F4
bl fn_800F0308
b @800935C0
@800935F4
mr r3, r31
@800935F8
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
