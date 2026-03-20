stwu r1, -0x160(r1)
mflr r0
stw r0, 0x164(r1)
stw r31, 0x15c(r1)
stw r30, 0x158(r1)
mr r31, r3
mr r3, r4
bl fn_8011CA34
mr r0, r3
mr r3, r31
mr r30, r0
li r4, 0x0
li r5, 0x50
bl memset
cmplwi r30, 0x0
beq @80084018
mr r3, r30
bl fn_8011C7C0
bl fn_800FA280
li r4, 0x0
mr r30, r3
addi r3, r1, 0xac
b @80083F44
@80083F28
cmpwi r4, 0x50
bge @80083F40
mr r0, r4
addi r4, r4, 0x1
slwi r0, r0, 1
sthx r5, r3, r0
@80083F40
addi r30, r30, 0x2
@80083F44
lhz r5, 0x0(r30)
cmplwi r5, 0x0
beq @80083F58
cmplwi r5, 0xffff
bne @80083F28
@80083F58
slwi r0, r4, 1
addi r4, r1, 0xac
li r5, 0x0
li r3, 0x0
sthx r5, r4, r0
li r4, 0x5
bl fn_80135938
mr r5, r3
mr r3, r31
addi r4, r1, 0xac
bl fn_800F9AEC
lhz r0, 0x0(r30)
add r31, r31, r3
cmplwi r0, 0xffff
bne @80084010
li r0, 0xfe
addi r6, r30, 0x3
stb r0, 0x0(r31)
addi r31, r31, 0x1
li r4, 0x0
addi r3, r1, 0x8
b @80083FCC
@80083FB0
cmpwi r4, 0x50
bge @80083FC8
mr r0, r4
addi r4, r4, 0x1
slwi r0, r0, 1
sthx r5, r3, r0
@80083FC8
addi r6, r6, 0x2
@80083FCC
lhz r5, 0x0(r6)
cmplwi r5, 0x0
beq @80083FE0
cmplwi r5, 0xffff
bne @80083FB0
@80083FE0
slwi r0, r4, 1
addi r4, r1, 0x8
li r5, 0x0
li r3, 0x0
sthx r5, r4, r0
li r4, 0x5
bl fn_80135938
mr r5, r3
mr r3, r31
addi r4, r1, 0x8
bl fn_800F9AEC
add r31, r31, r3
@80084010
li r0, 0xff
stb r0, 0x0(r31)
@80084018
li r3, 0x0
lwz r0, 0x164(r1)
lwz r31, 0x15c(r1)
lwz r30, 0x158(r1)
mtlr r0
addi r1, r1, 0x160
blr
