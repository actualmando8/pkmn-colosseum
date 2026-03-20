stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
li r3, 0xab2
bl fn_801906A0
addi r4, r3, 0x1
li r31, 0x1
cmplwi r4, 0x30
ble @80075BA4
li r4, 0x30
li r31, 0x0
@80075BA4
li r3, 0xab2
bl fn_8019075C
mr r3, r31
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
