stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r24, 0x10(r1)
mr r30, r4
mr r24, r5
mr r25, r6
mr r26, r7
bl fn_8025DA88
lis r4, lbl_803A9A60@ha
mr r28, r3
addi r29, r4, lbl_803A9A60@l
li r31, 0x1
lwz r0, 0x4(r29)
cmpwi r0, 0x1
beq @80061D94
bge @80061DA0
cmpwi r0, 0x0
bge @80061D84
b @80061DA0
@80061D84
mr r3, r24
bl fn_8025D89C
clrlwi r27, r3, 16
b @80061DA0
@80061D94
mr r3, r24
bl fn_8025D808
clrlwi r27, r3, 16
@80061DA0
lwz r0, 0x4(r29)
cmpwi r0, 0x0
bne @80061E5C
cmpwi r26, 0x2
bne @80061DD4
cmpwi r28, 0x2
beq @80061F54
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061DD4
cmpwi r26, 0x0
bne @80061E1C
cmpwi r27, 0x4
blt @80061DFC
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061DFC
cmpwi r28, 0x2
bne @80061F54
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061E1C
cmpwi r27, 0x4
bge @80061E3C
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061E3C
cmpwi r28, 0x2
bne @80061F54
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061E5C
cmpwi r26, 0x2
bne @80061E90
cmpwi r28, 0x2
beq @80061E80
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
@80061E80
cmpw r27, r25
bgt @80061F54
li r31, 0x0
b @80061F54
@80061E90
cmpwi r26, 0x0
bne @80061EF8
cmpwi r27, 0x4
blt @80061EB8
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061EB8
cmpwi r28, 0x2
bne @80061ED8
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061ED8
cmpw r27, r25
bgt @80061F54
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061EF8
cmpwi r27, 0x4
bge @80061F18
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061F18
cmpwi r28, 0x2
bne @80061F38
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80061F54
@80061F38
cmpw r27, r25
bgt @80061F54
lbz r0, 0x4(r30)
li r31, 0x0
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
@80061F54
mr r3, r31
lmw r24, 0x10(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
