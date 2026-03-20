stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
subi r3, r31, 0x1
bl fn_80072548
cmpwi r3, 0x0
bge @80089DD4
slwi r0, r31, 1
li r4, lbl_8047A684@sda21
add r4, r4, r0
li r0, 0x0
sth r0, -0x2(r4)
b @80089E0C
@80089DD4
cmpwi r3, 0x1
beq @80089DE4
cmpwi r3, 0x2
bne @80089E0C
@80089DE4
slwi r0, r31, 1
li r4, lbl_8047A684@sda21
add r5, r4, r0
lhz r4, -0x2(r5)
addi r4, r4, 0x1
clrlwi r0, r4, 16
sth r4, -0x2(r5)
cmplwi r0, 0xa
bgt @80089E0C
li r3, -0x1
@80089E0C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
