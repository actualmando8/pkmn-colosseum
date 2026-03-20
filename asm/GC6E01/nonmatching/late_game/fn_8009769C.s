stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r28, r5
lis r3, lbl_803FB338@ha
li r4, 0xc8
addi r3, r3, lbl_803FB338@l
li r5, 0xb4
bl fn_8010A5BC
lis r3, lbl_803FB338@ha
mr r4, r28
addi r3, r3, lbl_803FB338@l
bl fn_80109C88
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
beq @80097734
li r3, 0x1
bl fn_801C40F0
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 24, 24
cmpwi r0, 0x0
beq @80097728
lfs f1, lbl_8047C234@sda21(r0)
li r3, 0x2
bl fn_801C41C8
b @80097734
@80097728
lfs f1, lbl_8047C238@sda21(r0)
li r3, 0x2
bl fn_801C41C8
@80097734
li r0, 0x0
lis r3, lbl_803FB380@ha
stw r0, 0x8(r1)
addi r31, r3, lbl_803FB380@l
@80097744
addi r5, r1, 0x8
li r3, 0x53
li r4, 0x0
li r6, 0x0
li r7, 0x1
li r8, 0x0
crclr 6
bl fn_801026A4
cmpwi r3, -0x1
bne @80097774
stw r3, 0x4(r31)
b @80097818
@80097774
lbz r0, 0x0(r31)
lbz r3, 0x2(r31)
rlwinm r0, r0, 0, 25, 25
extsb r28, r3
cmpwi r0, 0x0
stw r28, 0x4(r31)
beq @80097818
clrlwi r0, r28, 16
lwz r29, 0xc(r31)
cmplwi r0, 0x4
bne @800977A8
lhz r30, 0x18(r31)
b @800977DC
@800977A8
mr r3, r29
mr r6, r28
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r30, r3, 16
mr r3, r29
mr r4, r28
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800977DC
li r30, 0x0
@800977DC
mr r4, r30
li r3, 0x0
li r5, 0x19
li r6, 0x0
bl fn_8011BEB4
cmpwi r3, 0x0
beq @80097818
li r3, 0x2
li r4, 0x2be9
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @80097744
@80097818
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
beq @80097874
li r3, 0x1
bl fn_801C40F0
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 24, 24
cmpwi r0, 0x0
beq @80097860
lfs f1, lbl_8047C234@sda21(r0)
li r3, 0x3
bl fn_801C41C8
b @8009786C
@80097860
lfs f1, lbl_8047C238@sda21(r0)
li r3, 0x3
bl fn_801C41C8
@8009786C
li r3, 0x1
bl fn_801C40F0
@80097874
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r3, 0x0(r3)
clrlwi r0, r3, 31
cmpwi r0, 0x0
beq @800978B4
rlwinm r0, r3, 0, 28, 28
cmpwi r0, 0x0
bne @800978B4
li r3, 0x1
bl fn_801C40F0
lfs f1, lbl_8047C238@sda21(r0)
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
@800978B4
li r3, 0x54
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800978D8
li r3, 0x54
li r4, 0x0
li r5, 0x0
bl fn_80102568
@800978D8
li r3, 0x55
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800978FC
li r3, 0x55
li r4, 0x0
li r5, 0x0
bl fn_80102568
@800978FC
li r3, 0x57
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80097920
li r3, 0x57
li r4, 0x0
li r5, 0x0
bl fn_80102568
@80097920
li r3, 0x56
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80097944
li r3, 0x56
li r4, 0x0
li r5, 0x0
bl fn_80102568
@80097944
li r3, 0x58
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80097968
li r3, 0x58
li r4, 0x0
li r5, 0x0
bl fn_80102568
@80097968
li r3, 0x53
li r4, 0x0
li r5, 0x1
bl fn_80102568
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
clrlwi r0, r0, 31
cmpwi r0, 0x0
beq @800979B8
bl fn_800FF660
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
beq @800979B8
li r3, 0x0
li r4, 0x0
bl fn_8011288C
@800979B8
lis r3, lbl_803FB338@ha
addi r3, r3, lbl_803FB338@l
bl fn_8010A420
bl fn_800F0308
lwz r3, 0x4(r31)
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
