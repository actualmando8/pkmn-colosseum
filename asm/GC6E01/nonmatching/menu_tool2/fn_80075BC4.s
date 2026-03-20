stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0xab2
bl fn_801906A0
cmplwi r3, 0x30
ble @80075BE8
li r3, 0x0
b @80075BEC
@80075BE8
subfic r3, r3, 0x30
@80075BEC
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
