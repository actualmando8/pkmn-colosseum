stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r31, r3
li r3, 0x1
bl fn_800F7BC4
rlwinm r0, r3, 0, 26, 26
cmplwi r0, 0x0
beq @800678E4
li r3, 0x0
bl fn_8025D2B0
cmpwi r3, 0x1
bne @80067948
lis r3, lbl_803A9F08@ha
addi r3, r3, lbl_803A9F08@l
lwz r0, 0x0(r3)
cmpwi r0, 0x2
beq @80067848
bge @80067754
cmpwi r0, 0x0
beq @8006775C
bge @80067838
b @80067948
@80067754
cmpwi r0, 0x4
b @80067948
@8006775C
lbz r0, 0x4(r3)
cmplwi r0, 0x0
bne @80067828
li r3, 0x1
bl fn_800F7BC4
mr r30, r3
li r29, -0x1
li r3, 0x0
bl fn_8025D89C
clrlwi r0, r30, 31
clrlwi r3, r3, 16
cmplwi r0, 0x0
beq @80067794
li r29, 0x0
@80067794
rlwinm r0, r30, 0, 28, 28
cmplwi r0, 0x0
beq @800677A4
li r29, 0x1
@800677A4
rlwinm r0, r30, 0, 20, 20
cmplwi r0, 0x0
beq @800677B4
li r29, 0x2
@800677B4
rlwinm r0, r30, 0, 29, 29
cmplwi r0, 0x0
beq @800677C4
li r29, 0x3
@800677C4
rlwinm r0, r30, 0, 30, 30
cmplwi r0, 0x0
beq @800677D4
li r29, 0x4
@800677D4
rlwinm r0, r30, 0, 21, 21
cmplwi r0, 0x0
beq @800677E4
li r29, 0x5
@800677E4
cmpw r3, r29
bgt @800677F0
li r29, -0x1
@800677F0
cmpwi r29, 0x0
blt @80067828
li r3, 0x0
bl fn_8025D89C
clrlwi r0, r3, 16
cmpw r29, r0
bge @80067828
li r0, 0x0
lis r3, lbl_803A9F08@ha
stb r0, 0x95(r31)
li r0, 0x1
addi r3, r3, lbl_803A9F08@l
stb r0, 0x98(r31)
stw r29, 0xc(r3)
@80067828
mr r3, r31
li r4, 0x1
bl fn_800679C0
b @80067948
@80067838
mr r3, r31
li r4, 0x1
bl fn_800679C0
b @80067948
@80067848
li r3, 0x1
bl fn_800F7BC4
lis r4, lbl_803A9F08@ha
li r5, 0x0
addi r4, r4, lbl_803A9F08@l
lwz r0, 0xc(r4)
cmpwi r0, 0x3
beq @800678AC
bge @80067884
cmpwi r0, 0x1
beq @8006789C
bge @800678A4
cmpwi r0, 0x0
bge @80067894
b @800678C0
@80067884
cmpwi r0, 0x5
beq @800678BC
bge @800678C0
b @800678B4
@80067894
li r5, 0x1
b @800678C0
@8006789C
li r5, 0x8
b @800678C0
@800678A4
li r5, 0x800
b @800678C0
@800678AC
li r5, 0x4
b @800678C0
@800678B4
li r5, 0x2
b @800678C0
@800678BC
li r5, 0x400
@800678C0
and r0, r3, r5
cmplwi r0, 0x0
bne @800678D4
li r0, 0x1
stb r0, 0x98(r31)
@800678D4
mr r3, r31
li r4, 0x1
bl fn_800679C0
b @80067948
@800678E4
lis r3, lbl_803A9F08@ha
addi r3, r3, lbl_803A9F08@l
lwz r0, 0x0(r3)
cmpwi r0, 0x2
beq @80067934
bge @8006790C
cmpwi r0, 0x0
beq @80067914
bge @80067924
b @80067948
@8006790C
cmpwi r0, 0x4
b @80067948
@80067914
mr r3, r31
li r4, 0x0
bl fn_800679C0
b @80067948
@80067924
mr r3, r31
li r4, 0x1
bl fn_800679C0
b @80067948
@80067934
mr r3, r31
li r4, 0x1
bl fn_800679C0
li r0, 0x1
stb r0, 0x98(r31)
@80067948
bl fn_8006905C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80067964
li r0, 0x1
stb r0, 0x98(r31)
stb r0, 0x99(r31)
@80067964
lis r3, lbl_803A9F08@ha
addi r3, r3, lbl_803A9F08@l
addis r3, r3, 0x1
lbz r0, -0x31a8(r3)
cmplwi r0, 0x0
bne @80067988
li r0, 0x1
stb r0, 0x98(r31)
stb r0, 0x99(r31)
@80067988
lis r3, lbl_803A9F08@ha
addi r3, r3, lbl_803A9F08@l
lwz r0, 0x0(r3)
cmpwi r0, 0x1
bne @800679A4
mr r3, r31
bl fn_80102ED4
@800679A4
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
