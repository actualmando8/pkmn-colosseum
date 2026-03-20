stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r26, 0x18(r1)
mr r27, r3
mr r28, r4
mr r29, r5
mr r30, r6
bl fn_8025D9CC
mr r31, r3
li r26, 0x1
bl fn_8025DA88
cmpwi r30, 0x2
bne @8006163C
cmpwi r3, 0x2
beq @80061648
li r26, 0x0
b @80061648
@8006163C
cmpwi r3, 0x2
bne @80061648
li r26, 0x0
@80061648
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800617CC
mulli r4, r29, 0xc
lis r3, lbl_803A9A60@ha
lha r6, 0x6(r28)
lis r5, lbl_802EF0A8@ha
addi r0, r3, lbl_803A9A60@l
add r3, r0, r4
lfs f0, 0x32c(r3)
mulli r3, r6, 0x1c
addi r0, r5, lbl_802EF0A8@l
fctiwz f0, f0
add r3, r0, r3
lha r3, 0x2(r3)
stfd f0, 0x8(r1)
lwz r0, 0xc(r1)
add r0, r3, r0
extsh r0, r0
sth r0, 0x50(r28)
lha r5, 0x84(r27)
lha r3, 0x50(r28)
lha r4, 0x86(r27)
lha r0, 0x52(r28)
add r3, r5, r3
extsh r3, r3
add r0, r4, r0
extsh r4, r0
bl fn_800FE6D0
bl fn_800FE4D4
cmpwi r31, 0x4
bne @80061738
cmpwi r30, 0x0
bne @800617CC
mr r3, r29
bl fn_8025DA18
clrlwi r4, r3, 16
li r3, 0x34
addi r4, r4, 0x1
bl fn_80132A38
cmpwi r29, 0x0
bne @80061718
lbz r4, 0x8b(r27)
li r0, -0x100
lha r5, 0x54(r28)
li r3, 0x0
lha r6, 0x56(r28)
or r7, r4, r0
li r4, 0x0
li r8, 0x30e9
bl fn_800FBB34
b @800617CC
@80061718
lbz r5, 0x8b(r27)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x30e5
bl fn_800FB680
b @800617CC
@80061738
mr r3, r29
bl fn_8025D28C
mr r28, r3
clrlwi r3, r28, 16
bl fn_801FCCC4
bl fn_801FCC64
bl fn_801FBD58
bl fn_801FBD28
clrlwi r0, r28, 16
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
cmplwi r0, 0x0
lwz r3, 0x3dc(r3)
bne @80061780
li r3, 0x1
bl fn_800FA280
mr r28, r3
b @80061788
@80061780
bl fn_800FA280
mr r28, r3
@80061788
mr r4, r28
li r3, 0x37
bl fn_80132A38
mr r4, r28
li r3, 0x4d
bl fn_80132A38
cmpwi r30, 0x0
bne @800617CC
cmpwi r29, 0x0
beq @800617CC
lbz r5, 0x8b(r27)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0xcf
bl fn_800FB680
@800617CC
lmw r26, 0x18(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
