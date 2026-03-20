stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r23, 0x1c(r1)
mr r26, r3
mr r28, r4
addi r27, r26, 0x1
li r4, 0x2
mr r3, r27
bl fn_8008ABE4
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r30, r0, 0x5
bl OSGetTick
mr r25, r3
@80073420
bl OSGetTick
subf r4, r25, r3
mr r3, r26
xor r0, r4, r30
cntlzw r0, r0
slw r0, r4, r0
srwi r29, r0, 31
bl fn_80073C38
cmpwi r3, 0x1
bne @80073450
cmpwi r29, 0x0
beq @80073420
@80073450
cmpwi r3, 0x0
beq @80073460
mr r25, r3
b @8007366C
@80073460
li r0, 0x88
mr r3, r26
stw r0, 0x10(r1)
addi r4, r1, 0x10
addi r5, r1, 0xa
bl fn_8025F648
cmpwi r3, 0x0
beq @80073488
li r25, 0xb
b @8007357C
@80073488
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r24, r0, 0x64
bl OSGetTick
lis r5, lbl_803B6E18@ha
lis r4, lbl_803B6E08@ha
slwi r6, r26, 3
mr r30, r3
addi r0, r5, lbl_803B6E18@l
slwi r29, r26, 2
add r23, r0, r6
addi r25, r4, lbl_803B6E08@l
addi r31, r23, 0x4
@800734D0
bl OSGetTick
subf r0, r30, r3
cmplw r0, r24
ble @800734E8
li r3, 0x1
b @80073568
@800734E8
mr r3, r26
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80073504
li r3, 0x2
b @80073568
@80073504
lbz r0, 0x8(r1)
andi. r0, r0, 0xa
cmpwi r0, 0x8
beq @80073544
lwz r12, 0x0(r23)
cmplwi r12, 0x0
beq @80073530
mr r3, r26
lwz r4, 0x0(r31)
mtctr r12
bctrl
@80073530
lwzx r0, r25, r29
cmpwi r0, 0x0
beq @800734D0
li r3, 0x3e8
b @80073568
@80073544
mr r3, r26
addi r4, r1, 0x14
addi r5, r1, 0xa
bl fn_8025F584
cmpwi r3, 0x0
beq @80073564
li r3, 0x3
b @80073568
@80073564
li r3, 0x0
@80073568
cmpwi r3, 0x0
beq @80073578
addi r25, r3, 0xb
b @8007357C
@80073578
li r25, 0x0
@8007357C
cmpwi r25, 0x0
beq @80073588
b @8007366C
@80073588
lwz r0, 0x14(r1)
srwi r0, r0, 24
cmplwi r0, 0x88
beq @800735A0
li r25, 0xf
b @8007366C
@800735A0
li r29, 0x0
lis r3, 0x1062
lis r31, 0x8000
addi r30, r3, 0x4dd3
@800735B0
lwz r0, 0x0(r28)
mr r3, r26
addi r4, r1, 0xc
addi r5, r1, 0x9
stw r0, 0xc(r1)
bl fn_8025F648
cmpwi r3, 0x0
beq @800735D8
li r25, 0x10
b @8007366C
@800735D8
lwz r0, 0xf8(r31)
srwi r0, r0, 2
mulhwu r0, r30, r0
srwi r0, r0, 6
mulli r24, r0, 0x64
bl OSGetTick
mr r25, r3
@800735F4
bl OSGetTick
subf r0, r25, r3
cmplw r0, r24
ble @8007360C
li r25, 0x11
b @8007366C
@8007360C
mr r3, r26
addi r4, r1, 0x9
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80073628
li r25, 0x12
b @8007366C
@80073628
lbz r0, 0x9(r1)
rlwinm r0, r0, 0, 30, 30
cmpwi r0, 0x0
bne @800735F4
slwi r0, r29, 26
srwi r3, r29, 31
subf r0, r3, r0
rotlwi r0, r0, 6
add r0, r0, r3
cmpwi r0, 0x0
bne @80073658
bl fn_800F0308
@80073658
addi r28, r28, 0x4
addi r29, r29, 0x4
cmpwi r29, 0x780
blt @800735B0
li r25, 0x0
@8007366C
mr r3, r27
li r4, 0x1
bl fn_8008ABE4
mr r3, r25
lmw r23, 0x1c(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
