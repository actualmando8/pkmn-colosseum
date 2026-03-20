stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r27, 0x1c(r1)
mr r30, r3
cmplwi r30, 0x0
lis r3, lbl_8026F1C8@ha
addi r29, r3, lbl_8026F1C8@l
beq @800830CC
b @800830DC
@800830CC
li r3, 0x0
li r4, 0xd
bl fn_80129280
mr r30, r3
@800830DC
lbz r3, 0x4000(r30)
li r0, 0x0
cmplwi r3, 0x1
beq @800830F4
cmplwi r3, 0x2
bne @800830F8
@800830F4
li r0, 0x1
@800830F8
cmpwi r0, 0x0
bne @80083110
addi r3, r29, 0x0
addi r5, r29, 0xa0
li r4, 0x161
bl fn_80196E10
@80083110
li r0, 0x0
li r27, 0x0
stb r0, 0x4000(r30)
addi r31, r1, 0x8
@80083120
cmplwi r30, 0x0
beq @80083130
mr r3, r30
b @8008313C
@80083130
li r3, 0x0
li r4, 0xd
bl fn_80129280
@8008313C
cmplwi r31, 0x0
addi r6, r3, 0x4000
beq @80083150
li r0, 0x0
stw r0, 0x8(r1)
@80083150
li r7, 0x0
@80083154
addi r0, r3, 0x24
cmplw r6, r0
blt @800831EC
lhz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800831EC
lbz r0, 0x1b(r3)
extsb r0, r0
cmpwi r0, 0x3
bgt @8008319C
lbz r0, 0x1c(r3)
extsb r0, r0
cmpwi r0, 0x6
bgt @8008319C
lbz r0, 0x1d(r3)
extsb r0, r0
cmpwi r0, 0x5
ble @800831A8
@8008319C
li r0, 0x0
sth r0, 0x0(r3)
b @800831EC
@800831A8
cmpw r7, r27
bne @800831B4
stw r3, 0x8(r1)
@800831B4
lbz r4, 0x1c(r3)
addi r7, r7, 0x1
lbz r0, 0x1d(r3)
extsb r4, r4
lbz r5, 0x1b(r3)
extsb r0, r0
mullw r0, r4, r0
extsb r5, r5
slwi r4, r0, 4
addi r0, r4, 0x76
mullw r0, r5, r0
add r3, r0, r3
addi r3, r3, 0x24
b @80083154
@800831EC
cmpwi r27, 0x0
bge @800831F8
stw r3, 0x8(r1)
@800831F8
lwz r28, 0x8(r1)
cmplwi r28, 0x0
bne @80083214
addi r3, r29, 0x0
li r4, 0x169
li r5, lbl_8047C180@sda21
bl fn_80196E10
@80083214
lbz r3, 0x1a(r28)
lbz r0, 0x4001(r30)
cmplw r3, r0
beq @8008322C
addi r27, r27, 0x1
b @80083120
@8008322C
cmplwi r28, 0x0
lbz r27, 0x4002(r30)
bne @80083248
addi r3, r29, 0x0
li r4, 0x17f
li r5, lbl_8047C180@sda21
bl fn_80196E10
@80083248
extsb r3, r27
li r4, 0x0
cmpwi r3, 0x0
blt @8008326C
lbz r0, 0x1b(r28)
extsb r0, r0
cmpw r3, r0
bge @8008326C
li r4, 0x1
@8008326C
cmpwi r4, 0x0
bne @80083284
addi r3, r29, 0x0
addi r5, r29, 0x10
li r4, 0x180
bl fn_80196E10
@80083284
lbz r3, 0x1c(r28)
extsb r4, r27
lbz r0, 0x1d(r28)
li r5, 0x1
extsb r3, r3
extsb r0, r0
mullw r0, r3, r0
slwi r3, r0, 4
addi r0, r3, 0x76
mullw r0, r4, r0
add r3, r28, r0
stb r5, 0x95(r3)
lmw r27, 0x1c(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
