stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r24, 0x20(r1)
mr r24, r3
lbz r0, 0xa(r24)
cmplwi r0, 0x0
bne @8006C5C4
bl fn_80105624
lhz r0, 0x6(r3)
clrlwi r3, r0, 31
neg r0, r3
or r0, r0, r3
srwi r27, r0, 31
bl fn_80105624
lhz r0, 0x6(r3)
rlwinm r3, r0, 0, 30, 30
neg r0, r3
or r0, r0, r3
srwi r28, r0, 31
bl fn_80105624
lhz r0, 0x6(r3)
rlwinm r3, r0, 0, 29, 29
neg r0, r3
or r0, r0, r3
srwi r29, r0, 31
bl fn_80105624
lhz r4, 0x6(r3)
clrlwi r0, r29, 24
cmplwi r0, 0x0
li r3, 0x0
rlwinm r4, r4, 0, 28, 28
neg r0, r4
or r0, r0, r4
srwi r30, r0, 31
bne @8006C200
clrlwi r0, r30, 24
cmplwi r0, 0x0
beq @8006C204
@8006C200
li r3, 0x1
@8006C204
clrlwi r0, r29, 24
clrlwi r26, r3, 24
cmplwi r0, 0x0
li r3, 0x0
bne @8006C224
clrlwi r0, r30, 24
cmplwi r0, 0x0
bne @8006C228
@8006C224
li r3, 0x1
@8006C228
clrlwi r25, r3, 24
bl fn_80077BD0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006C27C
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @8006C5C4
lwz r3, 0x4(r24)
li r4, 0x9d2
bl fn_80102138
extsb r3, r3
li r0, 0x0
stb r3, 0x11(r1)
addi r4, r1, 0xc
stb r0, 0x10(r1)
lhz r0, 0x10(r1)
sth r0, 0xc(r1)
lwz r3, 0x4(r24)
bl fn_801044D0
b @8006C5C4
@8006C27C
mr r3, r24
li r4, 0x0
bl fn_801040D0
mr r31, r3
lwz r3, 0x4(r24)
bl fn_801022B8
clrlwi r0, r28, 24
li r5, 0x0
cmplwi r0, 0x0
beq @8006C2AC
li r5, -0x1
b @8006C2BC
@8006C2AC
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @8006C2BC
li r5, 0x1
@8006C2BC
subi r0, r3, 0x9e2
cmplwi r0, 0x8
bgt @8006C35C
lis r4, jumptable_802EDEFC@ha
slwi r0, r0, 2
addi r4, r4, jumptable_802EDEFC@l
lwzx r0, r4, r0
mtctr r0
bctr
mulli r5, r5, 0xa
mulli r5, r5, 0xa
lha r0, 0x0(r31)
add r0, r0, r5
extsh r0, r0
sth r0, 0x0(r31)
lha r0, 0x2(r31)
lha r4, 0x0(r31)
cmpw r0, r4
bge @8006C360
sth r4, 0x2(r31)
b @8006C360
mulli r5, r5, 0xa
mulli r5, r5, 0xa
lha r0, 0x2(r31)
add r0, r0, r5
extsh r0, r0
sth r0, 0x2(r31)
lha r4, 0x2(r31)
lha r0, 0x0(r31)
cmpw r4, r0
bge @8006C360
sth r4, 0x0(r31)
b @8006C360
mulli r5, r5, 0xa
mulli r5, r5, 0xa
lha r0, 0x4(r31)
add r0, r0, r5
extsh r0, r0
sth r0, 0x4(r31)
b @8006C360
@8006C35C
li r5, 0x0
@8006C360
cmpwi r5, 0x0
beq @8006C3F4
lha r0, 0x0(r31)
cmpwi r0, 0x1
bge @8006C37C
li r0, 0x1
b @8006C388
@8006C37C
cmpwi r0, 0x64
ble @8006C388
li r0, 0x64
@8006C388
extsh r0, r0
sth r0, 0x0(r31)
lha r0, 0x2(r31)
cmpwi r0, 0x1
bge @8006C3A4
li r0, 0x1
b @8006C3B0
@8006C3A4
cmpwi r0, 0x64
ble @8006C3B0
li r0, 0x64
@8006C3B0
extsh r0, r0
sth r0, 0x2(r31)
lha r0, 0x0(r31)
lha r3, 0x4(r31)
mulli r0, r0, 0x6
cmpw r0, r3
ble @8006C3D0
b @8006C3E8
@8006C3D0
lha r0, 0x2(r31)
mulli r0, r0, 0x6
cmpw r0, r3
bge @8006C3E4
b @8006C3E8
@8006C3E4
mr r0, r3
@8006C3E8
extsh r0, r0
sth r0, 0x4(r31)
b @8006C5C4
@8006C3F4
subi r0, r3, 0x9ca
lhz r3, 0x94(r24)
cmplwi r0, 0x20
sth r3, 0x14(r1)
bgt @8006C5BC
lis r3, jumptable_802EDE78@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EDE78@l
lwzx r0, r3, r0
mtctr r0
bctr
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @8006C5BC
b @8006C5C4
clrlwi r0, r30, 24
cmplwi r0, 0x0
bne @8006C5C4
clrlwi r0, r29, 24
cmplwi r0, 0x0
bne @8006C5BC
clrlwi r0, r29, 24
cmplwi r0, 0x0
beq @8006C460
lbz r3, 0x15(r1)
subi r0, r3, 0x1
stb r0, 0x15(r1)
@8006C460
clrlwi r0, r30, 24
cmplwi r0, 0x0
beq @8006C478
lbz r3, 0x15(r1)
addi r0, r3, 0x1
stb r0, 0x15(r1)
@8006C478
lhz r0, 0x14(r1)
addi r4, r1, 0x8
sth r0, 0x8(r1)
lwz r3, 0x4(r24)
bl fn_801044D0
b @8006C5C4
cmplwi r26, 0x0
beq @8006C5BC
lbz r0, 0xc(r31)
cmplw r0, r25
beq @8006C4AC
li r3, 0x24
bl fn_80166A28
@8006C4AC
stb r25, 0xc(r31)
b @8006C5C4
clrlwi r0, r29, 24
cmplwi r0, 0x0
beq @8006C4F0
lwz r3, 0x8(r31)
subi r0, r3, 0x1
stw r0, 0x8(r31)
lwz r0, 0x8(r31)
cmpwi r0, 0x0
bge @8006C4E4
li r0, 0x0
stw r0, 0x8(r31)
b @8006C5C4
@8006C4E4
li r3, 0x24
bl fn_80166A28
b @8006C5C4
@8006C4F0
clrlwi r0, r30, 24
cmplwi r0, 0x0
beq @8006C5BC
lwz r3, 0x8(r31)
addi r0, r3, 0x1
stw r0, 0x8(r31)
lwz r0, 0x8(r31)
cmpwi r0, 0x3
blt @8006C520
li r0, 0x2
stw r0, 0x8(r31)
b @8006C5C4
@8006C520
li r3, 0x24
bl fn_80166A28
b @8006C5C4
cmplwi r26, 0x0
beq @8006C5BC
lbz r0, 0xd(r31)
cmplw r0, r25
beq @8006C548
li r3, 0x24
bl fn_80166A28
@8006C548
stb r25, 0xd(r31)
b @8006C5C4
cmplwi r26, 0x0
beq @8006C5BC
lbz r0, 0xe(r31)
cmplw r0, r25
beq @8006C56C
li r3, 0x24
bl fn_80166A28
@8006C56C
stb r25, 0xe(r31)
b @8006C5C4
cmplwi r26, 0x0
beq @8006C5BC
lbz r0, 0xf(r31)
cmplw r0, r25
beq @8006C590
li r3, 0x24
bl fn_80166A28
@8006C590
stb r25, 0xf(r31)
b @8006C5C4
cmplwi r26, 0x0
beq @8006C5BC
lbz r0, 0x10(r31)
cmplw r0, r25
beq @8006C5B4
li r3, 0x24
bl fn_80166A28
@8006C5B4
stb r25, 0x10(r31)
b @8006C5C4
@8006C5BC
mr r3, r24
bl fn_80102F38
@8006C5C4
lmw r24, 0x20(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
