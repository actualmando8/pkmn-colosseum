stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r27, 0x1c(r1)
mr r28, r3
mr r29, r4
mr r30, r5
mr r31, r6
li r27, 0x1
bl fn_8025DA88
cmpwi r31, 0x2
bne @80061820
cmpwi r3, 0x2
beq @8006182C
li r27, 0x0
b @8006182C
@80061820
cmpwi r3, 0x2
bne @8006182C
li r27, 0x0
@8006182C
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @80061A18
mulli r4, r30, 0xc
lis r3, lbl_803A9A60@ha
lha r6, 0x6(r29)
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
sth r0, 0x50(r29)
lha r5, 0x84(r28)
lha r3, 0x50(r29)
lha r4, 0x86(r28)
lha r0, 0x52(r29)
add r3, r5, r3
extsh r3, r3
add r0, r4, r0
extsh r4, r0
bl fn_800FE6D0
bl fn_800FE4D4
mr r3, r30
bl fn_8025D914
bl fn_8012AC54
mr r27, r3
cmplwi r27, 0x0
bne @800618C8
li r3, 0x1
bl fn_800FA280
mr r27, r3
@800618C8
bl fn_8025D9CC
cmpwi r3, 0x4
bne @800618F0
mr r4, r27
li r3, 0x37
bl fn_80132A38
mr r4, r27
li r3, 0x4d
bl fn_80132A38
b @80061938
@800618F0
cmpwi r30, 0x0
bne @80061914
mr r4, r27
li r3, 0x37
bl fn_80132A38
mr r4, r27
li r3, 0x4d
bl fn_80132A38
b @80061938
@80061914
lis r4, lbl_803A9A60@ha
li r3, 0x37
addi r4, r4, lbl_803A9A60@l
addi r27, r4, 0x3c4
mr r4, r27
bl fn_80132A38
mr r4, r27
li r3, 0x4d
bl fn_80132A38
@80061938
cmpwi r31, 0x0
bne @80061990
cmpwi r30, 0x0
bne @80061970
lbz r4, 0x8b(r28)
li r0, -0x100
lha r5, 0x54(r29)
li r3, 0x0
lha r6, 0x56(r29)
or r7, r4, r0
li r4, 0x0
li r8, 0x30e2
bl fn_800FBB34
b @80061A18
@80061970
lbz r5, 0x8b(r28)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0xce
bl fn_800FB680
b @80061A18
@80061990
mr r3, r30
bl fn_8025DA18
clrlwi r4, r3, 16
li r3, 0x34
addi r4, r4, 0x1
bl fn_80132A38
cmpwi r30, 0x2
bge @800619FC
lbz r4, 0x8b(r28)
li r0, -0x100
lha r5, 0x54(r29)
li r3, 0x0
lha r6, 0x56(r29)
or r7, r4, r0
li r4, 0x0
li r8, 0x30e9
bl fn_800FBB34
lbz r4, 0x8b(r28)
li r0, -0x100
lha r5, 0x54(r29)
li r3, 0x0
lha r6, 0x56(r29)
or r7, r4, r0
li r4, 0x16
li r8, 0x30e8
bl fn_800FBB34
b @80061A18
@800619FC
lbz r5, 0x8b(r28)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x30e7
bl fn_800FB680
@80061A18
lmw r27, 0x1c(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
