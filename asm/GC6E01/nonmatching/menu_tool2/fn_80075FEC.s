stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
bl fn_8011F5C8
clrlwi r0, r3, 16
cmpwi r0, 0x19a
beq @80076020
bge @8007603C
cmpwi r0, 0x97
beq @80076020
b @8007603C
@80076020
mr r3, r31
bl fn_8011E7A4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8007603C
li r3, 0x0
b @80076040
@8007603C
li r3, 0x1
@80076040
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
