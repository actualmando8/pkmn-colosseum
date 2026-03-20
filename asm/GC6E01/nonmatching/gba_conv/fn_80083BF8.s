stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
cmplwi r3, 0x0
beq @80083C10
b @80083C1C
@80083C10
li r3, 0x0
li r4, 0xd
bl fn_80129280
@80083C1C
addi r6, r3, 0x4000
li r7, 0x0
@80083C24
addi r0, r3, 0x24
cmplw r6, r0
blt @80083C98
lhz r0, 0x0(r3)
cmplwi r0, 0x0
beq @80083C98
lbz r0, 0x1b(r3)
extsb r5, r0
cmpwi r5, 0x3
bgt @80083C6C
lbz r0, 0x1c(r3)
extsb r4, r0
cmpwi r4, 0x6
bgt @80083C6C
lbz r0, 0x1d(r3)
extsb r0, r0
cmpwi r0, 0x5
ble @80083C78
@80083C6C
li r0, 0x0
sth r0, 0x0(r3)
b @80083C98
@80083C78
mullw r0, r4, r0
addi r7, r7, 0x1
slwi r4, r0, 4
addi r0, r4, 0x76
mullw r0, r5, r0
add r3, r0, r3
addi r3, r3, 0x24
b @80083C24
@80083C98
addi r0, r1, 0x8
cmplwi r0, 0x0
beq @80083CA8
stw r7, 0x8(r1)
@80083CA8
lwz r3, 0x8(r1)
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
