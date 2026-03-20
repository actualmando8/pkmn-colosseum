stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
li r0, 0x0
li r3, lbl_8047A694@sda21
stw r0, lbl_8047A694@sda21(r0)
stw r0, 0x4(r3)
stw r0, lbl_8047A690@sda21(r0)
bl fn_80113F48
mr r0, r3
li r3, 0xb5d
mr r30, r0
bl fn_801906A0
mr r31, r3
lis r4, lbl_802EEC70@ha
slwi r0, r31, 2
mr r3, r30
addi r4, r4, lbl_802EEC70@l
lwzx r12, r4, r0
mtctr r12
bctrl
addi r4, r31, 0x1
cmplwi r4, 0x1f
blt @8008C76C
li r4, 0x0
@8008C76C
li r3, 0xb5d
bl fn_8019075C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
